#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include <util/log.h>

#include <dev/i2c.h>

LOG_DECLARE_MODULE("i2c");

static int _desc;

static struct i2c_msg * _msg = NULL;
static uint32_t _count = 0;

static void _conv_buf_to_str (char ** str, const uint8_t * buf, size_t len) {
    *str = (char *)malloc(3 * len + 5);

    sprintf(*str, "(0x) ");
    for (size_t i = 0; i < len; i++) {
        sprintf(*str + 3 * i + 5, "%02" PRIx8 " ", buf[i]);
    }

    (*str)[3 * len + 4] = '\0';
}

int i2c_init (const char * path) {
    LOG_INF("Opening device file: %s", path);

    _desc = open(path, O_RDWR);
    if (_desc < 0) {
        LOG_ERR(
            "Failed to open device file (%s)",
            strerror(errno)
        );
        return -1;
    }

    return 0;
}

int i2c_deinit (void) {
    int ret;

    LOG_INF("Closing device file");

    ret = close(_desc);
    if (ret < 0) {
        LOG_ERR("Failed to close device file (%s)", strerror(errno));
        return -1;
    }

    return 0;
}

void i2c_send (uint8_t addr, const uint8_t * buf, size_t len) {
    char * buf_str;

    _conv_buf_to_str(&buf_str, buf, len);
    LOG_INF(
        "Queueing transmission: Addr: 0x%02" PRIx8 ", Len: %zu, Data: %s",
        addr, len, buf_str
    );
    free(buf_str);

    _msg = (struct i2c_msg *)realloc(
        _msg, (_count + 1) * sizeof(struct i2c_msg)
    );

    _msg[_count].addr = addr;
    _msg[_count].flags = 0;
    _msg[_count].len = len;
    _msg[_count].buf = (uint8_t *)malloc(len);

    memcpy(_msg[_count].buf, buf, len);

    _count++;
}

void i2c_recv (uint8_t addr, uint8_t * buf, size_t len) {
    LOG_INF(
        "Queueing reception: Addr: 0x%02" PRIx8 ", Len: %zu",
        addr, len
    );

    _msg = (struct i2c_msg *)realloc(
        _msg, (_count + 1) * sizeof(struct i2c_msg)
    );

    _msg[_count].addr = addr;
    _msg[_count].flags = I2C_M_RD;
    _msg[_count].len = len;
    _msg[_count].buf = buf;

    _count++;
}

int i2c_exec (void) {
    int ret;
    char * buf_str;
    struct i2c_rdwr_ioctl_data oper;

    oper.msgs = _msg;
    oper.nmsgs = _count;

    LOG_INF("Executing operations");

    ret = ioctl(_desc, I2C_RDWR, &oper);
    if (ret < 0) {
        LOG_ERR("Failed to execute operations (%s)", strerror(errno));

        for (uint32_t i = 0; i < _count; i++) {
            if (_msg[i].flags == 0) {
                free(_msg[i].buf);
            }
        }
        free(_msg);

        _msg = NULL;
        _count = 0;

        return -1;
    }

    for (uint32_t i = 0; i < _count; i++) {
        if (_msg[i].flags == I2C_M_RD) {
            _conv_buf_to_str(&buf_str, _msg[i].buf, _msg[i].len);
            LOG_INF(
                "Received data: Addr: 0x%02" PRIx8 ", Len: %zu, Data: %s",
                _msg[i].addr, _msg[i].len, buf_str
            );
            free(buf_str);
        }
    }

    for (uint32_t i = 0; i < _count; i++) {
        if (_msg[i].flags == 0) {
            free(_msg[i].buf);
        }
    }
    free(_msg);

    _msg = NULL;
    _count = 0;

    return 0;
}

#include <stdint.h>

#include <util/log.h>

#include <dev/i2c.h>
#include <dev/servo.h>

LOG_DECLARE_MODULE("servo");

int servo_set_pos (double pos) {
    int ret;
    uint8_t cmd [] = {(uint8_t)(100 * pos)};

    LOG_INF("Setting servo position: %.3e", pos);

    i2c_send(SERVO_ADDR, cmd, 1);

    ret = i2c_exec();
    if (ret < 0) {
        LOG_ERR("Failed to set servo position");
        return -1;
    }

    return 0;
}

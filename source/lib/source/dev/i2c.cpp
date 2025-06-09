#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cerrno>

#include <string>
#include <vector>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include <sys/logging.hpp>
#include <dev/i2c.hpp>

namespace i2c {

bool tracker::exist = false;

bool tracker::instantiate (void) {
    if (tracker::exist) {
        return false;
    } else {
        tracker::exist = true;
        return true;
    }
}

instance::instance (const std::string & path) : logging("i2c"), init(false) {
    this->logging.inf("Initializing instance: Path: ", path);

    if (!instance::instantiate()) {
        this->logging.err("Failed to initialize instance (Instance already exists)");
        throw except_ctor::fail;
    }

    this->logging.inf("Opening device node");
    this->desc = open(path.c_str(), O_RDWR);
    if (this->desc < 0) {
        this->logging.err("Failed to open device node (", std::strerror(errno), ")");
        this->logging.err("Failed to initialize instance");
        throw except_ctor::fail;
    }

    this->init = true;
}

instance::~instance (void) {
    int ret;

    if (this->init) {
        this->logging.inf("Closing device node");
        ret = close(this->desc);
        if (ret < 0) {
            this->logging.err("Failed to close device node (", std::strerror(errno), ")");
        }
    }
}

void instance::send (std::uint8_t addr, const std::vector<std::uint8_t> & data) {
    this->logging.inf("Queueing transmission: Addr: ", addr, ", Len: ", int(data.size()), ", Data: ", data);

    if (!this->init) {
        this->logging.err("Failed to queue transmission (Instance not initialized)");
        throw except_send::fail;
    }

    if (addr >= 0x80) {
        this->logging.err("Failed to queue transmission (Invalid address)");
        throw except_send::fail;
    }

    if (data.empty()) {
        this->logging.err("Failed to queue transmission (Empty transaction)");
        throw except_send::fail;
    }

    this->addr.push_back(addr);
    this->send_flag.push_back(true);
    this->recv_flag.push_back(false);
    this->send_data.push_back(data);
    this->recv_data.push_back(NULL);
}

void instance::recv (std::uint8_t addr, std::vector<std::uint8_t> & data) {
    this->logging.inf("Queueing reception: Addr: ", addr, ", Len: ", int(data.size()));

    if (!this->init) {
        this->logging.err("Failed to queue reception (Instance not initialized)");
        throw except_recv::fail;
    }

    if (addr >= 0x80) {
        this->logging.err("Failed to queue reception (Invalid address)");
        throw except_recv::fail;
    }

    if (data.empty()) {
        this->logging.err("Failed to queue reception (Empty transaction)");
        throw except_recv::fail;
    }

    this->addr.push_back(addr);
    this->send_flag.push_back(false);
    this->recv_flag.push_back(true);
    this->send_data.push_back(std::vector<std::uint8_t>());
    this->recv_data.push_back(&data);
}

void instance::exec (void) {
    int ret;
    struct i2c_msg * msg;
    struct i2c_rdwr_ioctl_data oper;

    this->logging.inf("Executing transactions");

    if (!this->init) {
        this->logging.err("Failed to execute transactions (Instance not initialized)");
        throw except_exec::fail;
    }

    if (this->addr.empty()) {
        this->logging.err("Failed to execute transactions (No queued transactions)");
        throw except_exec::fail;
    }

    msg = new struct i2c_msg [this->addr.size()];
    for (int i = 0; i < int(this->addr.size()); i++) {
        if (this->send_flag[i]) {
            msg[i].addr = this->addr[i];
            msg[i].flags = 0;
            msg[i].len = this->send_data[i].size();
            msg[i].buf = new std::uint8_t [msg[i].len];
            for (int j = 0; j < int(msg[i].len); j++) {
                msg[i].buf[j] = this->send_data[i][j];
            }
        }
        if (this->recv_flag[i]) {
            msg[i].addr = this->addr[i];
            msg[i].flags = I2C_M_RD;
            msg[i].len = this->recv_data[i]->size();
            msg[i].buf = new std::uint8_t [msg[i].len];
        }
    }

    oper.msgs = msg;
    oper.nmsgs = this->addr.size();

    ret = ioctl(this->desc, I2C_RDWR, &oper);
    if (ret < 0) {
        this->logging.err("Failed to execute transactions (", std::strerror(errno), ")");

        for (int i = 0; i < int(this->addr.size()); i++) {
            delete [] msg[i].buf;
        }
        delete [] msg;

        this->addr.clear();
        this->send_flag.clear();
        this->recv_flag.clear();
        this->send_data.clear();
        this->recv_data.clear();

        throw except_exec::fail;
    }

    for (int i = 0; i < int(this->addr.size()); i++) {
        if (this->recv_flag[i]) {
            for (int j = 0; j < int(this->recv_data[i]->size()); j++) {
                (*(this->recv_data[i]))[j] = msg[i].buf[j];
            }
            this->logging.inf(
                "Received data: Addr: ", this->addr[i], ", Len: ", int(this->recv_data[i]->size()), ", Data: ", *(this->recv_data[i])
            );
        }
    }

    for (int i = 0; i < int(this->addr.size()); i++) {
        delete [] msg[i].buf;
    }
    delete [] msg;

    this->addr.clear();
    this->send_flag.clear();
    this->recv_flag.clear();
    this->send_data.clear();
    this->recv_data.clear();
}

}

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

namespace i2c :: intern {

std::string mod = "i2c";

bool fail;

int desc;

std::vector<std::uint8_t> addr;
std::vector<bool> send;
std::vector<bool> recv;
std::vector<std::vector<std::uint8_t>> send_buf;
std::vector<std::vector<std::uint8_t> *> recv_buf;

}

namespace i2c {

bool fail (void) {
    return intern::fail;
}

void init (std::string path) {
    logging::inf(intern::mod,
        "Opening device node: Path: ", path
    );

    intern::desc = open(path.c_str(), O_RDWR);
    if (intern::desc < 0) {
        logging::err(intern::mod,
            "Failed to open device node (", std::strerror(errno), ")"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

void deinit (void) {
    int ret;

    logging::inf(intern::mod,
        "Closing device node"
    );

    ret = close(intern::desc);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to close device node (", std::strerror(errno), ")"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

void send (std::uint8_t addr, std::vector<std::uint8_t> buf) {
    logging::inf(intern::mod,
        "Queueing transmission: Addr: ", addr, ", Len: ", int(buf.size()), ", Data: ", buf
    );

    intern::addr.push_back(addr);
    intern::send.push_back(true);
    intern::recv.push_back(false);
    intern::send_buf.push_back(buf);
    intern::recv_buf.push_back(NULL);
}

void recv (std::uint8_t addr, std::vector<std::uint8_t> & buf) {
    logging::inf(intern::mod,
        "Queueing reception: Addr: ", addr, ", Len: ", int(buf.size())
    );

    intern::addr.push_back(addr);
    intern::send.push_back(false);
    intern::recv.push_back(true);
    intern::send_buf.push_back(std::vector<std::uint8_t>());
    intern::recv_buf.push_back(&buf);
}

void exec (void) {
    int ret;
    struct i2c_msg * msg;
    struct i2c_rdwr_ioctl_data oper;

    logging::inf(intern::mod,
        "Executing operations"
    );

    msg = new struct i2c_msg [intern::addr.size()];
    for (int i = 0; i < intern::addr.size(); i++) {
        if (intern::send[i]) {
            msg[i].addr = intern::addr[i];
            msg[i].flags = 0;
            msg[i].len = intern::send_buf[i].size();
            msg[i].buf = new std::uint8_t [msg[i].len];
            for (int j = 0; j < msg[i].len; j++) {
                msg[i].buf[j] = intern::send_buf[i][j];
            }
        }
        if (intern::recv[i]) {
            msg[i].addr = intern::addr[i];
            msg[i].flags = I2C_M_RD;
            msg[i].len = intern::recv_buf[i]->size();
            msg[i].buf = new std::uint8_t [msg[i].len];
        }
    }

    oper.msgs = msg;
    oper.nmsgs = intern::addr.size();

    ret = ioctl(intern::desc, I2C_RDWR, &oper);
    if (ret < 0) {
        logging::err(intern::mod,
            "Failed to execute operations (", std::strerror(errno), ")"
        );

        for (int i = 0; i < intern::addr.size(); i++) {
            delete [] msg[i].buf;
        }
        delete [] msg;

        intern::addr.clear();
        intern::send.clear();
        intern::recv.clear();
        intern::send_buf.clear();
        intern::recv_buf.clear();

        intern::fail = true;
        return;
    }

    for (int i = 0; i < intern::addr.size(); i++) {
        if (intern::recv[i]) {
            for (int j = 0; j < intern::recv_buf[i]->size(); j++) {
                (*(intern::recv_buf[i]))[j] = msg[i].buf[j];
            }
            logging::inf(intern::mod,
                "Received data: Addr: ", intern::addr[i], ", Len: ", int(intern::recv_buf[i]->size()), ", Data: ", *(intern::recv_buf[i])
            );
        }
    }

    for (int i = 0; i < intern::addr.size(); i++) {
        delete [] msg[i].buf;
    }
    delete [] msg;

    intern::addr.clear();
    intern::send.clear();
    intern::recv.clear();
    intern::send_buf.clear();
    intern::recv_buf.clear();

    intern::fail = false;
}

}

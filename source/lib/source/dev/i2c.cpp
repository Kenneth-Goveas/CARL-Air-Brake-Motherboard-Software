#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cerrno>

#include <iomanip>
#include <iostream>
#include <sstream>
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

std::string conv_hex_to_oupt (std::uint8_t hex);
std::string conv_hex_to_oupt (std::vector<std::uint8_t> hex);

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
            "Failed to open device node ",
            "(", std::strerror(errno), ")"
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
            "Failed to close device node ",
            "(", std::strerror(errno), ")"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

void send (std::uint8_t addr, std::vector<std::uint8_t> buf) {
    std::string oupt_addr, oupt_buf;

    oupt_addr = intern::conv_hex_to_oupt(addr);
    oupt_buf = intern::conv_hex_to_oupt(buf);
    logging::inf(intern::mod,
        "Queueing transmission: ",
        "Addr: ", oupt_addr, ", Len: ", buf.size(), ", Data: ", oupt_buf
    );

    intern::addr.push_back(addr);
    intern::send.push_back(true);
    intern::recv.push_back(false);
    intern::send_buf.push_back(buf);
    intern::recv_buf.push_back(NULL);
}

void recv (std::uint8_t addr, std::vector<std::uint8_t> * buf) {
    std::string oupt_addr;

    oupt_addr = intern::conv_hex_to_oupt(addr);
    logging::inf(intern::mod,
        "Queueing reception: ",
        "Addr: ", oupt_addr, ", Len: ", buf->size()
    );

    intern::addr.push_back(addr);
    intern::send.push_back(false);
    intern::recv.push_back(true);
    intern::send_buf.push_back(std::vector<std::uint8_t>());
    intern::recv_buf.push_back(buf);
}

void exec (void) {
    int ret;
    struct i2c_msg * msg;
    struct i2c_rdwr_ioctl_data oper;
    std::string oupt_addr, oupt_buf;

    logging::inf(intern::mod,
        "Executing operations"
    );

    msg = new struct i2c_msg [intern::addr.size()];
    for (auto i = 0; i < intern::addr.size(); i++) {
        if (intern::send[i]) {
            msg[i].addr = intern::addr[i];
            msg[i].flags = 0;
            msg[i].len = intern::send_buf[i].size();
            msg[i].buf = new std::uint8_t [msg[i].len];
            for (auto j = 0; j < msg[i].len; j++) {
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
            "Failed to execute operations ",
            "(", std::strerror(errno), ")"
        );

        for (auto i = 0; i < intern::addr.size(); i++) {
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

    for (auto i = 0; i < intern::addr.size(); i++) {
        if (intern::recv[i]) {
            for (auto j = 0; j < msg[i].len; j++) {
                (*(intern::recv_buf[i]))[j] = msg[i].buf[j];
            }
            oupt_addr = intern::conv_hex_to_oupt(intern::addr[i]);
            oupt_buf = intern::conv_hex_to_oupt(*(intern::recv_buf[i]));
            logging::inf(intern::mod,
                "Received data: ",
                "Addr: ", oupt_addr, ", Len: ", msg[i].len, ", Data: ", oupt_buf
            );
        }
    }

    for (auto i = 0; i < intern::addr.size(); i++) {
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

namespace i2c :: intern {

std::string conv_hex_to_oupt (std::uint8_t hex) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "0x"
         << std::uppercase << std::hex << std::setfill('0') << std::setw(2)
         << (int)hex;
    oupt = ostr.str();

    return oupt;
}

std::string conv_hex_to_oupt (std::vector<std::uint8_t> hex) {
    std::ostringstream ostr;
    std::string oupt;

    for (auto i = 0; i < hex.size(); i++) {
        ostr << "0x"
             << std::uppercase << std::hex << std::setfill('0') << std::setw(2)
             << (int)(hex[i]);
        if (i < hex.size() - 1) {
            ostr << " ";
        }
    }
    oupt = ostr.str();

    return oupt;
}

}

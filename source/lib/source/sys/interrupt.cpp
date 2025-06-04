#include <cstring>
#include <cerrno>
#include <csignal>

#include <sys/logging.hpp>
#include <sys/interrupt.hpp>

namespace interrupt {

bool tracker::exist = false;
bool instance::flag = false;

bool tracker::instantiate (void) {
    if (tracker::exist) {
        return false;
    } else {
        tracker::exist = true;
        return true;
    }
}

instance::instance (void) : logging("interrupt"), init(false) {
    this->logging.inf("Initializing instance");

    if (!instance::instantiate()) {
        this->logging.err("Failed to initialize instance (Instance already exists)");
        throw except_ctor::fail;
    }

    this->logging.inf("Configuring interrupt signal handler");
    if (std::signal(SIGINT, instance::handler) == SIG_ERR) {
        this->logging.err("Failed to configure interrupt signal handler (", std::strerror(errno), ")");
        this->logging.err("Failed to initialize instance");
        throw except_ctor::fail;
    }

    this->init = true;
}

bool instance::caught (void) {
    bool flag;

    this->logging.inf("Checking if interrupt signal is caught");

    if (!this->init) {
        this->logging.err("Failed to check if interrupt signal is caught (Instance not initialized)");
        throw except_caught::fail;
    }

    flag = instance::flag;

    if (flag) {
        this->logging.inf("Interrupt signal is caught");
    } else {
        this->logging.inf("Interrupt signal is not caught");
    }

    return flag;
}

void instance::handler (int signum) {
    if (signum == SIGINT) {
        instance::flag = true;
    }
}

}

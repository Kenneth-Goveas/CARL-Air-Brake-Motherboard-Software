#include <cstdint>

#include <vector>

#include <sys/logging.hpp>
#include <dev/i2c.hpp>
#include <dev/servo.hpp>

namespace servo {

bool tracker::exist = false;

bool tracker::instantiate (void) {
    if (tracker::exist) {
        return false;
    } else {
        tracker::exist = true;
        return true;
    }
}

instance::instance (i2c::instance & i2c) : logging("servo"), init(false), i2c(&i2c) {
    this->logging.inf("Initializing instance");

    if (!instance::instantiate()) {
        this->logging.err("Failed to initialize instance (Instance already exists)");
        throw except_ctor::fail;
    }

    this->init = true;
}

void instance::put_pos (double pos) {
    std::vector<std::uint8_t> cmd;

    this->logging.inf("Setting servo position: ", pos);

    if (!this->init) {
        this->logging.err("Failed to set servo position (Instance not initialized)");
        throw except_put_pos::fail;
    }

    if (pos < 0 || pos > 1) {
        this->logging.err("Failed to set servo position (Invalid position)");
        throw except_put_pos::fail;
    }

    cmd = {std::uint8_t(0xFF * pos)};

    try {
        this->i2c->send(SERVO_ADDR, cmd);
        this->i2c->exec();
    } catch (...) {
        this->logging.err("Failed to set servo position");
        throw except_put_pos::fail;
    }
}

}

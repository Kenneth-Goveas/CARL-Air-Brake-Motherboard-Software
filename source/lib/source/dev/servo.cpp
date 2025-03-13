#include <cstdint>

#include <string>
#include <vector>

#include <sys/logging.hpp>

#include <dev/i2c.hpp>
#include <dev/servo.hpp>

namespace servo :: intern {

std::string mod = "servo";

bool fail;

}

namespace servo {

bool fail (void) {
    return intern::fail;
}

void put_pos (double pos) {
    std::vector<std::uint8_t> cmd;

    logging::inf(intern::mod,
        "Setting servo position: ", pos
    );

    if (pos < 0 || pos > 1) {
        logging::err(intern::mod,
            "Failed to set servo position (Invalid position)"
        );
        intern::fail = true;
        return;
    }

    cmd = {std::uint8_t(0xFF * pos)};

    i2c::send(SERVO_ADDR, cmd);
    i2c::exec();
    if (i2c::fail()) {
        logging::err(intern::mod,
            "Failed to set servo position"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

}

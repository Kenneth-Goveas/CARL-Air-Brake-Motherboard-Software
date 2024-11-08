#include <cstdint>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <sys/logging.hpp>

#include <dev/i2c.hpp>
#include <dev/servo.hpp>

namespace servo :: intern {

std::string mod = "servo";

bool fail;

std::string conv_val_to_oupt (double val);

}

namespace servo {

bool fail (void) {
    return intern::fail;
}

void put_pos (double pos) {
    std::vector<std::uint8_t> cmd(1);
    std::string oupt;

    oupt = intern::conv_val_to_oupt(pos);
    logging::inf(intern::mod,
        "Setting servo position: ", oupt
    );

    if (!(pos >= 0 && pos <= 1)) {
        logging::err(intern::mod,
            "Failed to set servo position (Invalid position)"
        );
        intern::fail = true;
        return;
    }

    cmd[0] = (std::uint8_t)(0xFF * pos);

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

namespace servo :: intern {

std::string conv_val_to_oupt (double val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << std::showpos << std::scientific << std::setprecision(2) << val;
    oupt = ostr.str();

    return oupt;
}

}

#include <string>

#include <sys/logging.hpp>
#include <sys/interrupt.hpp>
#include <sys/timing.hpp>
#include <sys/config.hpp>

#include <ipc/channel.hpp>

#include <dev/i2c.hpp>
#include <dev/servo.hpp>

namespace logging::local {
    std::string path = "log/system.log";
    std::string app = "actuator";
    std::string lib = "main";
}

namespace timing::local {
    std::string dur_name = "sleep_duration";
    double dur;
}

namespace config::local {
    std::string path = "config/actuator.conf";
}

namespace channel::local {
    bool acces = false, error;
    double ctrl;
}

namespace i2c::local {
    std::string path_name = "device_path";
    std::string path;
}

int main (void) {
    if (!logging::instance::start(logging::local::path, logging::local::app)) {
        return 1;
    }

    logging::instance logging(logging::local::lib);
    logging.inf("Starting");

    try {
        config::instance config(config::local::path);

        timing::local::dur = config.get<double>(timing::local::dur_name);

        i2c::local::path = config.get<std::string>(i2c::local::path_name);

        interrupt::instance interrupt;
        timing::instance timing;

        channel::instance channel(channel::mode::none);

        i2c::instance i2c(i2c::local::path);
        servo::instance servo(i2c);

        while (!interrupt.caught()) {
            timing.sleep(timing::local::dur);

            channel::local::error = false;

            try {
                if (!channel::local::acces) {
                    channel.acquire();
                    channel::local::acces = true;
                }
            } catch (...) {
                channel::local::error = true;
            }

            try {
                if (channel::local::acces) {
                    channel::local::ctrl = channel.get_ctrl();
                }
            } catch (...) {
                channel::local::error = true;
            }

            try {
                if (channel::local::acces) {
                    channel.release();
                    channel::local::acces = false;
                }
            } catch (...) {
                channel::local::error = true;
            }

            if (channel::local::error) {
                continue;
            }

            try {
                servo.put_pos(channel::local::ctrl);
            } catch (...) {}
        }
    } catch (...) {
        logging.wrn("Exiting");
        logging::instance::stop();
        return 1;
    }

    logging.inf("Exiting");
    logging::instance::stop();

    return 0;
}

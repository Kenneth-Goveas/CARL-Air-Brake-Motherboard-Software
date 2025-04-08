#include <string>

#include <sys/logging.hpp>
#include <sys/config.hpp>
#include <sys/interrupt.hpp>
#include <sys/timing.hpp>

#include <ipc/channel.hpp>

#include <dev/i2c.hpp>
#include <dev/servo.hpp>

namespace logging :: local {
    std::string path = "log/system.log";
    std::string unit = "actuator";
    std::string mod = "main";
}

namespace config :: local {
    std::string path = "config/actuator.conf";
}

namespace timing :: local {
    std::string name_dur = "sleep_duration";
    double dur;
}

namespace channel :: local {
    bool want_ctrl = true, want_stat = false, want_estm = false, want_sens = false;
    bool acces, error;
    double ctrl;
}

namespace i2c :: local {
    std::string name_path = "device_path";
    std::string path;
}

int main (void) {
    logging::init(logging::local::path, logging::local::unit);
    if (logging::fail()) {
        return 1;
    }

    logging::inf(logging::local::mod,
        "Starting"
    );

    interrupt::init();
    if (interrupt::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    config::init(config::local::path);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    timing::local::dur = config::get<double>(timing::local::name_dur);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    i2c::local::path = config::get<std::string>(i2c::local::name_path);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::init(channel::local::want_ctrl, channel::local::want_stat, channel::local::want_estm, channel::local::want_sens);

    channel::open();
    if (channel::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    i2c::init(i2c::local::path);
    if (i2c::fail()) {
        channel::close();
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::acces = false;

    while (!interrupt::caught()) {
        timing::sleep(timing::local::dur);

        channel::local::error = false;

        if (!channel::local::acces) {
            channel::acquire_acces();
            if (channel::fail()) {
                channel::local::error = true;
            } else {
                channel::local::acces = true;
            }
        }

        if (channel::local::acces) {
            channel::local::ctrl = channel::get_ctrl();
            if (channel::fail()) {
                channel::local::error = true;
            } else if (channel::blank()) {
                channel::local::error = true;
            }
        }

        if (channel::local::acces) {
            channel::release_acces();
            if (channel::fail()) {
                channel::local::error = true;
            } else {
                channel::local::acces = false;
            }
        }

        if (channel::local::error) {
            continue;
        }

        servo::put_pos(channel::local::ctrl);
    }

    i2c::deinit();
    if (i2c::fail()) {
        channel::close();
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::close();
    if (channel::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    logging::inf(logging::local::mod,
        "Exiting"
    );

    logging::deinit();

    return 0;
}

#include <algorithm>
#include <string>
#include <vector>

#include <math/linalg.hpp>

#include <sys/logging.hpp>
#include <sys/config.hpp>
#include <sys/interrupt.hpp>
#include <sys/timing.hpp>

#include <ipc/channel.hpp>

namespace logging :: local {
    std::string path = "log/system.log";
    std::string unit = "detector";
    std::string mod = "main";
}

namespace config :: local {
    std::string path = "config/detector.conf";
}

namespace timing :: local {
    std::string name_dur = "sleep_duration";
    double dur;
}

namespace channel :: local {
    bool want_ctrl = false, want_stat = false, want_estm = true, want_sens = false;
    bool acces, error;
    linalg::fvector estm;
}

namespace local {
    std::string name_enab = "enable_altitude", name_count = "reading_count";
    double enab;
    int count, index;
    std::vector<int> categ;
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

    local::enab = config::get<double>(local::name_enab);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    local::count = config::get<int>(local::name_count);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    if (local::enab <= 0) {
        logging::err(logging::local::mod,
            "Enable altitude must be positive"
        );
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    if (local::count <= 0) {
        logging::err(logging::local::mod,
            "Reading count must be positive"
        );
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

    channel::local::acces = false;

    local::index = 0;
    local::categ = std::vector<int>(local::count, 0);

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
            channel::local::estm = channel::get_estm();
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

        if (channel::local::estm[2] > 0) {
            local::categ[local::index] = 1;
        } else if (channel::local::estm[2] < 0) {
            local::categ[local::index] = -1;
        } else {
            local::categ[local::index] = 0;
        }

        local::index++;
        if (local::index == local::count) {
            local::index = 0;
        }

        if (channel::local::estm[0] < local::enab) {
            continue;
        }

        auto pred = [] (auto elem) {
            return (elem < 0);
        };

        if (std::all_of(local::categ.begin(), local::categ.end(), pred)) {
            break;
        }
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

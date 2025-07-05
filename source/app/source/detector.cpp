#include <algorithm>
#include <string>
#include <vector>

#include <math/linalg.hpp>

#include <sys/logging.hpp>
#include <sys/interrupt.hpp>
#include <sys/timing.hpp>
#include <sys/config.hpp>

#include <ipc/channel.hpp>

namespace logging::local {
    std::string path = "log/system.log";
    std::string app = "detector";
    std::string lib = "main";
}

namespace timing::local {
    std::string dur_name = "sleep_duration";
    double dur;
}

namespace config::local {
    std::string path = "config/detector.conf";
}

namespace channel::local {
    bool acces = false, error;
    linalg::fvector estm;
}

namespace local {
    std::string enab_name = "enable_altitude", count_name = "reading_count";
    double enab;
    int count, idx = 0;
    std::vector<int> categ;

    class except {};
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

        local::enab = config.get<double>(local::enab_name);
        local::count = config.get<int>(local::count_name);

        if (local::enab <= 0) {
            logging.err("Enable altitude must be positive");
            throw local::except{};
        }

        if (local::count <= 0) {
            logging.err("Reading count must be positive");
            throw local::except{};
        }

        interrupt::instance interrupt;
        timing::instance timing;

        channel::instance channel(channel::mode::none);

        local::categ = std::vector<int>(local::count, 0);

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
                    channel::local::estm = channel.get_estm();
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

            if (channel::local::estm[2] > 0) {
                local::categ[local::idx] = 1;
            } else if (channel::local::estm[2] < 0) {
                local::categ[local::idx] = -1;
            } else {
                local::categ[local::idx] = 0;
            }

            local::idx++;
            if (local::idx == local::count) {
                local::idx = 0;
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
    } catch (...) {
        logging.wrn("Exiting");
        logging::instance::stop();
        return 1;
    }

    logging.inf("Exiting");
    logging::instance::stop();

    return 0;
}

#include <string>

#include <math/linalg.hpp>

#include <sys/logging.hpp>
#include <sys/interrupt.hpp>
#include <sys/timing.hpp>
#include <sys/config.hpp>

#include <ipc/channel.hpp>

#include <dyn/estim.hpp>

namespace logging::local {
    std::string path = "log/system.log";
    std::string app = "estimator";
    std::string lib = "main";
}

namespace timing::local {
    std::string dur_name = "sleep_duration";
    double dur;
}

namespace config::local {
    std::string path = "config/estimator.conf";
}

namespace channel::local {
    bool acces = false, error;
    linalg::fvector estm, sens;
}

namespace estim::local {
    std::string regr_count_name = "reading_count";
    std::string kalm_pos_var_name = "altitude_variance", kalm_acc_var_name = "acceleration_variance";

    std::string atm_path = "input/atmospheric-data.csv";
    int regr_count;
    double kalm_pos_var, kalm_acc_var;
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

        estim::local::regr_count = config.get<int>(estim::local::regr_count_name);
        estim::local::kalm_pos_var = config.get<double>(estim::local::kalm_pos_var_name);
        estim::local::kalm_acc_var = config.get<double>(estim::local::kalm_acc_var_name);

        interrupt::instance interrupt;
        timing::instance timing;

        channel::instance channel(channel::mode::none);

        estim::instance estim(estim::local::atm_path, estim::local::regr_count, estim::local::kalm_pos_var, estim::local::kalm_acc_var);

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
                    channel::local::sens = channel.get_sens();
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
                channel::local::estm = estim.get_estm(channel::local::sens);
            } catch (...) {
                continue;
            }

            try {
                if (!channel::local::acces) {
                    channel.acquire();
                    channel::local::acces = true;
                }
            } catch (...) {}

            try {
                if (channel::local::acces) {
                    channel.put_estm(channel::local::estm);
                }
            } catch (...) {}

            try {
                if (channel::local::acces) {
                    channel.release();
                    channel::local::acces = false;
                }
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

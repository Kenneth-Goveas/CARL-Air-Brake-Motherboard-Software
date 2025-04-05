#include <string>

#include <math/linalg.hpp>

#include <sys/logging.hpp>
#include <sys/config.hpp>
#include <sys/interrupt.hpp>
#include <sys/timing.hpp>

#include <ipc/channel.hpp>

#include <dyn/estim.hpp>

namespace logging :: local {
    std::string path = "data/system.log";
    std::string unit = "estimator";
    std::string mod = "main";
}

namespace config :: local {
    std::string path = "config/estimator.conf";
}

namespace timing :: local {
    std::string name_dur = "sleep_duration";
    double dur;
}

namespace channel :: local {
    bool want_ctrl = false, want_stat = false, want_estm = true, want_sens = true;
    bool acces, error;
    linalg::fvector estm, sens;
}

namespace estim :: local {
    std::string atm_path = "data/atmospheric-data.csv";

    std::string name_regr_count = "reading_count";
    int regr_count;

    std::string name_kalm_pos_var = "altitude_variance", name_kalm_acc_var = "acceleration_variance";
    double kalm_pos_var, kalm_acc_var;
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

    estim::local::regr_count = config::get<int>(estim::local::name_regr_count);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    estim::local::kalm_pos_var = config::get<double>(estim::local::name_kalm_pos_var);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    estim::local::kalm_acc_var = config::get<double>(estim::local::name_kalm_acc_var);
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

    estim::init(estim::local::atm_path, estim::local::regr_count, estim::local::kalm_pos_var, estim::local::kalm_acc_var);
    if (estim::fail()) {
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
            channel::local::sens = channel::get_sens();
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

        channel::local::estm = estim::get_estm(channel::local::sens);
        if (estim::fail()) {
            continue;
        }

        if (!channel::local::acces) {
            channel::acquire_acces();
            if (channel::fail()) {
                channel::local::error = true;
            } else {
                channel::local::acces = true;
            }
        }

        if (channel::local::acces) {
            channel::put_estm(channel::local::estm);
            if (channel::fail()) {
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

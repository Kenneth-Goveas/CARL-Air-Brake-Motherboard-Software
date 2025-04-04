#include <string>

#include <math/linalg.hpp>

#include <sys/logging.hpp>
#include <sys/config.hpp>
#include <sys/interrupt.hpp>

#include <ipc/channel.hpp>

#include <dyn/cntrl.hpp>

namespace logging :: local {
    std::string path = "data/system.log";
    std::string unit = "controller";
    std::string mod = "main";
}

namespace config :: local {
    std::string path = "config/controller.conf";
}

namespace channel :: local {
    bool want_ctrl = true, want_stat = false, want_estm = true, want_sens = false;
    bool acces, error;
    double ctrl;
    linalg::fvector estm;
}

namespace cntrl :: local {
    std::string aer_path = "data/aerodynamic-data.csv", atm_path = "data/atmospheric-data.csv";

    std::string name_env_grvty = "gravitational_acceleration";
    std::string name_env_gcnst = "ideal_gas_constant";
    std::string name_env_molar = "molar_mass";
    std::string name_env_gamma = "gamma_factor";
    double env_grvty, env_gcnst, env_molar, env_gamma;

    std::string name_vhc_mass = "mass";
    std::string name_vhc_iner = "inertia";
    std::string name_vhc_cmas = "center_of_mass";
    std::string name_vhc_area = "cross_sectional_area";
    double vhc_mass, vhc_iner, vhc_cmas, vhc_area;

    std::string name_mpc_apog = "apogee_altitude";
    std::string name_mpc_enab = "enable_altitude";
    std::string name_mpc_step = "time_step";
    std::string name_mpc_horz = "prediction_horizon";
    std::string name_mpc_tol = "tolerance";
    double mpc_apog, mpc_enab, mpc_step, mpc_horz, mpc_tol;
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

    cntrl::local::env_grvty = config::get<double>(cntrl::local::name_env_grvty);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    cntrl::local::env_gcnst = config::get<double>(cntrl::local::name_env_gcnst);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    cntrl::local::env_molar = config::get<double>(cntrl::local::name_env_molar);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    cntrl::local::env_gamma = config::get<double>(cntrl::local::name_env_gamma);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    cntrl::local::vhc_mass = config::get<double>(cntrl::local::name_vhc_mass);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    cntrl::local::vhc_iner = config::get<double>(cntrl::local::name_vhc_iner);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    cntrl::local::vhc_cmas = config::get<double>(cntrl::local::name_vhc_cmas);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    cntrl::local::vhc_area = config::get<double>(cntrl::local::name_vhc_area);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    cntrl::local::mpc_apog = config::get<double>(cntrl::local::name_mpc_apog);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    cntrl::local::mpc_enab = config::get<double>(cntrl::local::name_mpc_enab);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    cntrl::local::mpc_step = config::get<double>(cntrl::local::name_mpc_step);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    cntrl::local::mpc_horz = config::get<double>(cntrl::local::name_mpc_horz);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    cntrl::local::mpc_tol = config::get<double>(cntrl::local::name_mpc_tol);
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

    cntrl::init(
        cntrl::local::aer_path, cntrl::local::atm_path,
        cntrl::local::env_grvty, cntrl::local::env_gcnst, cntrl::local::env_molar, cntrl::local::env_gamma,
        cntrl::local::vhc_mass, cntrl::local::vhc_iner, cntrl::local::vhc_cmas, cntrl::local::vhc_area,
        cntrl::local::mpc_apog, cntrl::local::mpc_enab, cntrl::local::mpc_step, cntrl::local::mpc_horz, cntrl::local::mpc_tol
    );

    if (cntrl::fail()) {
        channel::close();
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    channel::local::acces = false;

    while (!interrupt::caught()) {
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

        channel::local::ctrl = cntrl::get_ctrl(channel::local::estm);

        if (!channel::local::acces) {
            channel::acquire_acces();
            if (channel::fail()) {
                channel::local::error = true;
            } else {
                channel::local::acces = true;
            }
        }

        if (channel::local::acces) {
            channel::put_ctrl(channel::local::ctrl);
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

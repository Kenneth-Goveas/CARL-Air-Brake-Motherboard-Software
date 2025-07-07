#include <string>

#include <math/linalg.hpp>

#include <sys/logging.hpp>
#include <sys/interrupt.hpp>
#include <sys/timing.hpp>
#include <sys/config.hpp>

#include <ipc/channel.hpp>

#include <dyn/cntrl.hpp>

namespace logging::local {
    std::string path = "log/system.log";
    std::string app = "controller";
    std::string lib = "main";
}

namespace timing::local {
    std::string dur_name = "sleep_duration";
    double dur;
}

namespace config::local {
    std::string path = "config/controller.conf";
}

namespace channel::local {
    bool acces = false, error;
    double ctrl;
    linalg::fvector estm;
}

namespace cntrl::local {
    std::string env_grvty_name = "gravitational_acceleration",
                env_gcnst_name = "ideal_gas_constant",
                env_molar_name = "molar_mass",
                env_gamma_name = "gamma_factor";

    std::string vhc_mass_name = "mass",
                vhc_iner_name = "moment_of_inertia",
                vhc_cmas_name = "center_of_mass",
                vhc_area_name = "cross_sectional_area";

    std::string mpc_apog_name = "apogee_altitude",
                mpc_enab_name = "enable_altitude",
                mpc_step_name = "time_step",
                mpc_phrz_name = "prediction_horizon",
                mpc_thrz_name = "termination_horizon",
                mpc_tol_name = "tolerance";

    std::string aer_path = "input/aerodynamic-data.csv", atm_path = "input/atmospheric-data.csv";
    double env_grvty, env_gcnst, env_molar, env_gamma;
    double vhc_mass, vhc_iner, vhc_cmas, vhc_area;
    double mpc_apog, mpc_enab, mpc_step, mpc_phrz, mpc_thrz, mpc_tol;
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

        cntrl::local::env_grvty = config.get<double>(cntrl::local::env_grvty_name);
        cntrl::local::env_gcnst = config.get<double>(cntrl::local::env_gcnst_name);
        cntrl::local::env_molar = config.get<double>(cntrl::local::env_molar_name);
        cntrl::local::env_gamma = config.get<double>(cntrl::local::env_gamma_name);

        cntrl::local::vhc_mass = config.get<double>(cntrl::local::vhc_mass_name);
        cntrl::local::vhc_iner = config.get<double>(cntrl::local::vhc_iner_name);
        cntrl::local::vhc_cmas = config.get<double>(cntrl::local::vhc_cmas_name);
        cntrl::local::vhc_area = config.get<double>(cntrl::local::vhc_area_name);

        cntrl::local::mpc_apog = config.get<double>(cntrl::local::mpc_apog_name);
        cntrl::local::mpc_enab = config.get<double>(cntrl::local::mpc_enab_name);
        cntrl::local::mpc_step = config.get<double>(cntrl::local::mpc_step_name);
        cntrl::local::mpc_phrz = config.get<double>(cntrl::local::mpc_phrz_name);
        cntrl::local::mpc_thrz = config.get<double>(cntrl::local::mpc_thrz_name);
        cntrl::local::mpc_tol = config.get<double>(cntrl::local::mpc_tol_name);

        interrupt::instance interrupt;
        timing::instance timing;

        channel::instance channel(channel::mode::none);

        cntrl::instance cntrl(
                            cntrl::local::aer_path, cntrl::local::atm_path,
                            cntrl::local::env_grvty, cntrl::local::env_gcnst, cntrl::local::env_molar, cntrl::local::env_gamma,
                            cntrl::local::vhc_mass, cntrl::local::vhc_iner, cntrl::local::vhc_cmas, cntrl::local::vhc_area,
                            cntrl::local::mpc_apog, cntrl::local::mpc_enab,
                            cntrl::local::mpc_step, cntrl::local::mpc_phrz, cntrl::local::mpc_thrz, cntrl::local::mpc_tol
                        );

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

            try {
                channel::local::ctrl = cntrl.get_ctrl(channel::local::estm);
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
                    channel.put_ctrl(channel::local::ctrl);
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

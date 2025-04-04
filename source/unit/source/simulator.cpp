#include <string>

#include <math/linalg.hpp>

#include <sys/logging.hpp>
#include <sys/config.hpp>
#include <sys/interrupt.hpp>
#include <sys/timing.hpp>

#include <ipc/channel.hpp>

#include <dyn/simul.hpp>

namespace logging :: local {
    std::string path = "data/system.log";
    std::string unit = "simulator";
    std::string mod = "main";
}

namespace config :: local {
    std::string path = "config/simulator.conf";
}

namespace timing :: local {
    std::string name_dur = "sleep_duration";
    double time, dur;
}

namespace channel :: local {
    bool want_ctrl = true, want_stat = true, want_estm = false, want_sens = true;
    bool acces, error;
    double ctrl;
    linalg::fvector stat, sens;
}

namespace simul :: local {
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

    std::string name_imu_rot_var = "angular_velocity_variance";
    std::string name_imu_qua_var = "attitude_quaternion_variance";
    std::string name_imu_lia_var = "linear_acceleration_variance";
    linalg::fmatrix imu_rot_var, imu_qua_var, imu_lia_var;

    std::string name_imu_rot_rate = "angular_velocity_data_rate";
    std::string name_imu_qua_rate = "attitude_quaternion_data_rate";
    std::string name_imu_lia_rate = "linear_acceleration_data_rate";
    double imu_rot_rate, imu_qua_rate, imu_lia_rate;

    std::string name_bar_pres_var = "pressure_variance";
    std::string name_bar_temp_var = "temperature_variance";
    double bar_pres_var, bar_temp_var;

    std::string name_bar_pres_rate = "pressure_data_rate";
    std::string name_bar_temp_rate = "temperature_data_rate";
    double bar_pres_rate, bar_temp_rate;

    std::string name_act_turn = "actuator_turning_rate";
    double act_turn;

    std::string name_ctrl = "starting_control_signal";
    std::string name_stat = "starting_state_vector";
    double ctrl;
    linalg::fvector stat;
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

    simul::local::env_grvty = config::get<double>(simul::local::name_env_grvty);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::env_gcnst = config::get<double>(simul::local::name_env_gcnst);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::env_molar = config::get<double>(simul::local::name_env_molar);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::env_gamma = config::get<double>(simul::local::name_env_gamma);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::vhc_mass = config::get<double>(simul::local::name_vhc_mass);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::vhc_iner = config::get<double>(simul::local::name_vhc_iner);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::vhc_cmas = config::get<double>(simul::local::name_vhc_cmas);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::vhc_area = config::get<double>(simul::local::name_vhc_area);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::imu_rot_var = config::get<linalg::fmatrix>(simul::local::name_imu_rot_var);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::imu_qua_var = config::get<linalg::fmatrix>(simul::local::name_imu_qua_var);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::imu_lia_var = config::get<linalg::fmatrix>(simul::local::name_imu_lia_var);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::imu_rot_rate = config::get<double>(simul::local::name_imu_rot_rate);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::imu_qua_rate = config::get<double>(simul::local::name_imu_qua_rate);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::imu_lia_rate = config::get<double>(simul::local::name_imu_lia_rate);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::bar_pres_var = config::get<double>(simul::local::name_bar_pres_var);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::bar_temp_var = config::get<double>(simul::local::name_bar_temp_var);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::bar_pres_rate = config::get<double>(simul::local::name_bar_pres_rate);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::bar_temp_rate = config::get<double>(simul::local::name_bar_temp_rate);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::act_turn = config::get<double>(simul::local::name_act_turn);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::ctrl = config::get<double>(simul::local::name_ctrl);
    if (config::fail()) {
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::local::stat = config::get<linalg::fvector>(simul::local::name_stat);
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

    simul::init(
        simul::local::aer_path, simul::local::atm_path,
        simul::local::env_grvty, simul::local::env_gcnst, simul::local::env_molar, simul::local::env_gamma,
        simul::local::vhc_mass, simul::local::vhc_iner, simul::local::vhc_cmas, simul::local::vhc_area,
        simul::local::imu_rot_var, simul::local::imu_qua_var, simul::local::imu_lia_var,
        simul::local::imu_rot_rate, simul::local::imu_qua_rate, simul::local::imu_lia_rate,
        simul::local::bar_pres_var, simul::local::bar_temp_var, simul::local::bar_pres_rate, simul::local::bar_temp_rate,
        simul::local::act_turn
    );

    if (simul::fail()) {
        channel::close();
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    timing::local::time = timing::get_time();

    simul::put_time(timing::local::time);
    if (simul::fail()) {
        channel::close();
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::put_ctrl(simul::local::ctrl);
    if (simul::fail()) {
        channel::close();
        logging::wrn(logging::local::mod,
            "Exiting"
        );
        logging::deinit();
        return 1;
    }

    simul::put_stat(simul::local::stat);
    if (simul::fail()) {
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

        timing::local::time = timing::get_time();

        simul::put_time(timing::local::time);
        simul::update();

        channel::local::stat = simul::get_stat();
        channel::local::sens = simul::get_sens();

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
            channel::put_stat(channel::local::stat);
            if (channel::fail()) {
                channel::local::error = true;
            }
        }

        if (channel::local::acces) {
            channel::put_sens(channel::local::sens);
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

        if (channel::local::error) {
            continue;
        }

        simul::put_ctrl(channel::local::ctrl);
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

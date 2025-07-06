#include <string>

#include <math/linalg.hpp>

#include <sys/logging.hpp>
#include <sys/interrupt.hpp>
#include <sys/timing.hpp>
#include <sys/config.hpp>

#include <ipc/channel.hpp>

#include <dyn/simul.hpp>

namespace logging::local {
    std::string path = "log/system.log";
    std::string app = "simulator";
    std::string lib = "main";
}

namespace timing::local {
    std::string dur_name = "sleep_duration";
    double time, dur;
}

namespace config::local {
    std::string path = "config/simulator.conf";
}

namespace channel::local {
    bool acces = false;
    double ctrl;
    linalg::fvector stat, sens;
}

namespace simul::local {
    std::string env_grvty_name = "gravitational_acceleration",
                env_gcnst_name = "ideal_gas_constant",
                env_molar_name = "molar_mass",
                env_gamma_name = "gamma_factor";

    std::string vhc_mass_name = "mass",
                vhc_iner_name = "moment_of_inertia",
                vhc_cmas_name = "center_of_mass",
                vhc_area_name = "cross_sectional_area";

    std::string imu_rot_var_name = "angular_velocity_variance",
                imu_qua_var_name = "attitude_quaternion_variance",
                imu_lia_var_name = "linear_acceleration_variance";

    std::string imu_rot_rate_name = "angular_velocity_data_rate",
                imu_qua_rate_name = "attitude_quaternion_data_rate",
                imu_lia_rate_name = "linear_acceleration_data_rate";

    std::string bar_pres_var_name = "pressure_variance",
                bar_temp_var_name = "temperature_variance";

    std::string bar_pres_rate_name = "pressure_data_rate",
                bar_temp_rate_name = "temperature_data_rate";

    std::string act_turn_name = "actuator_turning_rate";

    std::string ctrl_name = "starting_control_signal",
                stat_name = "starting_state_vector";

    std::string aer_path = "input/aerodynamic-data.csv", atm_path = "input/atmospheric-data.csv";
    double env_grvty, env_gcnst, env_molar, env_gamma;
    double vhc_mass, vhc_iner, vhc_cmas, vhc_area;
    linalg::fmatrix imu_rot_var, imu_qua_var, imu_lia_var;
    double imu_rot_rate, imu_qua_rate, imu_lia_rate;
    double bar_pres_var, bar_temp_var;
    double bar_pres_rate, bar_temp_rate;
    double act_turn;

    double ctrl;
    linalg::fvector stat;
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

        simul::local::env_grvty = config.get<double>(simul::local::env_grvty_name);
        simul::local::env_gcnst = config.get<double>(simul::local::env_gcnst_name);
        simul::local::env_molar = config.get<double>(simul::local::env_molar_name);
        simul::local::env_gamma = config.get<double>(simul::local::env_gamma_name);

        simul::local::vhc_mass = config.get<double>(simul::local::vhc_mass_name);
        simul::local::vhc_iner = config.get<double>(simul::local::vhc_iner_name);
        simul::local::vhc_cmas = config.get<double>(simul::local::vhc_cmas_name);
        simul::local::vhc_area = config.get<double>(simul::local::vhc_area_name);

        simul::local::imu_rot_var = config.get<linalg::fmatrix>(simul::local::imu_rot_var_name);
        simul::local::imu_qua_var = config.get<linalg::fmatrix>(simul::local::imu_qua_var_name);
        simul::local::imu_lia_var = config.get<linalg::fmatrix>(simul::local::imu_lia_var_name);
        simul::local::imu_rot_rate = config.get<double>(simul::local::imu_rot_rate_name);
        simul::local::imu_qua_rate = config.get<double>(simul::local::imu_qua_rate_name);
        simul::local::imu_lia_rate = config.get<double>(simul::local::imu_lia_rate_name);

        simul::local::bar_pres_var = config.get<double>(simul::local::bar_pres_var_name);
        simul::local::bar_temp_var = config.get<double>(simul::local::bar_temp_var_name);
        simul::local::bar_pres_rate = config.get<double>(simul::local::bar_pres_rate_name);
        simul::local::bar_temp_rate = config.get<double>(simul::local::bar_temp_rate_name);

        simul::local::act_turn = config.get<double>(simul::local::act_turn_name);

        simul::local::ctrl = config.get<double>(simul::local::ctrl_name);
        simul::local::stat = config.get<linalg::fvector>(simul::local::stat_name);

        interrupt::instance interrupt;
        timing::instance timing;

        channel::instance channel(channel::mode::none);

        simul::instance simul(
                            simul::local::aer_path, simul::local::atm_path,
                            simul::local::env_grvty, simul::local::env_gcnst, simul::local::env_molar, simul::local::env_gamma,
                            simul::local::vhc_mass, simul::local::vhc_iner, simul::local::vhc_cmas, simul::local::vhc_area,
                            simul::local::imu_rot_var, simul::local::imu_qua_var, simul::local::imu_lia_var,
                            simul::local::imu_rot_rate, simul::local::imu_qua_rate, simul::local::imu_lia_rate,
                            simul::local::bar_pres_var, simul::local::bar_temp_var,
                            simul::local::bar_pres_rate, simul::local::bar_temp_rate,
                            simul::local::act_turn
                        );

        timing::local::time = timing.get_time();

        simul.put_time(timing::local::time);
        simul.put_ctrl(simul::local::ctrl);
        simul.put_stat(simul::local::stat);

        channel::local::ctrl = simul::local::ctrl;
        channel::local::stat = simul::local::stat;

        while (!interrupt.caught()) {
            timing.sleep(timing::local::dur);

            timing::local::time = timing.get_time();

            simul.put_time(timing::local::time);
            simul.update();

            channel::local::stat = simul.get_stat();
            channel::local::sens = simul.get_sens();

            try {
                if (!channel::local::acces) {
                    channel.acquire();
                    channel::local::acces = true;
                }
            } catch (...) {}

            try {
                if (channel::local::acces) {
                    channel.put_stat(channel::local::stat);
                    channel.put_sens(channel::local::sens);
                    channel::local::ctrl = channel.get_ctrl();
                }
            } catch (...) {}

            try {
                if (channel::local::acces) {
                    channel.release();
                    channel::local::acces = false;
                }
            } catch (...) {}

            simul.put_ctrl(channel::local::ctrl);
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

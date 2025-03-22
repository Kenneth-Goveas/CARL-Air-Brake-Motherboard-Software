#ifndef __DYN_SIMUL_HPP__
#define __DYN_SIMUL_HPP__

#include <string>

#include <math/linalg.hpp>

namespace simul {

bool fail (void);

void init (
    std::string aer_path, std::string atm_path,
    double env_grvty, double env_gcnst, double env_molar, double env_gamma,
    double vhc_mass, double vhc_iner, double vhc_cmas, double vhc_area,
    linalg::fmatrix imu_rot_var, linalg::fmatrix imu_qua_var, linalg::fmatrix imu_lia_var,
    double imu_rot_rate, double imu_qua_rate, double imu_lia_rate,
    double bar_pres_var, double bar_temp_var, double bar_pres_rate, double bar_temp_rate,
    double act_turn
);

void put_time (double time);
void put_ctrl (double ctrl);
void put_stat (linalg::fvector stat);

linalg::fvector get_stat (void);
linalg::fvector get_sens (void);

void update (void);

}

#endif

#ifndef __DYN_CNTRL_HPP__
#define __DYN_CNTRL_HPP__

#include <string>

#include <math/linalg.hpp>

namespace cntrl {

bool fail (void);

void init (
    std::string aer_path, std::string atm_path,
    double env_grvty, double env_gcnst, double env_molar, double env_gamma,
    double vhc_mass, double vhc_iner, double vhc_cmas, double vhc_area,
    double mpc_apog, double mpc_enab, double mpc_step, double mpc_horz, double mpc_tol
);

double get_ctrl (linalg::fvector estm);

}

#endif

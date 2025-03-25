#ifndef __DYN_ESTIM_HPP__
#define __DYN_ESTIM_HPP__

#include <string>

#include <math/linalg.hpp>

namespace estim {

bool fail (void);

void init (std::string atm_path, int regr_count, double kalm_pos_var, double kalm_acc_var);

linalg::fvector get_estm (linalg::fvector sens);

}

#endif

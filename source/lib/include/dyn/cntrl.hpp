#ifndef __DYN_CNTRL_HPP__
#define __DYN_CNTRL_HPP__

#include <string>
#include <vector>

#include <math/linalg.hpp>
#include <sys/logging.hpp>
#include <dat/csvread.hpp>

namespace cntrl {

class tracker {
    protected:
        static bool exist;
        static bool instantiate (void);
};

class instance : private tracker {
    public:
        instance (
            const std::string & aer_path, const std::string & atm_path,
            double env_grvty, double env_gcnst, double env_molar, double env_gamma,
            double vhc_mass, double vhc_iner, double vhc_cmas, double vhc_area,
            double mpc_apog, double mpc_enab, double mpc_step, double mpc_phrz, double mpc_thrz, double mpc_tol
        );

        double get_ctrl (const linalg::fvector & estm);

        enum class except_ctor {fail};
        enum class except_get_ctrl {fail};

    private:
        logging::instance logging;
        bool init;

        csvread::instance aer_dat, atm_dat;
        std::vector<double> aer_attk, aer_mach, aer_brak;
        std::vector<std::vector<std::vector<double>>> aer_lift, aer_drag, aer_cprs;
        std::vector<double> atm_altd, atm_pres, atm_temp, atm_dens;
        double env_grvty, env_gcnst, env_molar, env_gamma;
        double vhc_mass, vhc_iner, vhc_cmas, vhc_area;
        double mpc_apog, mpc_enab, mpc_step, mpc_phrz, mpc_thrz, mpc_tol;

        void aer_load (void);
        void aer_find (double attk, double mach, double brak, double & lift, double & drag, double & cprs);

        void atm_load (void);
        void atm_find (double altd, double & pres, double & temp, double & dens);

        void comp_stat (const linalg::fvector & estm, linalg::fvector & stat);
        void comp_derv (double ctrl, const linalg::fvector & stat, linalg::fvector & derv);
        void comp_pred (double ctrl, const linalg::fvector & stat, linalg::fvector & pred);
        void comp_term (double ctrl, const linalg::fvector & pred, linalg::fvector & term);
        void comp_categ (double ctrl, const linalg::fvector & estm, int & categ);

        enum class except_aer_load {fail};
        enum class except_atm_load {fail};
        enum class except_comp_term {fail};
        enum class except_comp_categ {fail};
};

}

#endif

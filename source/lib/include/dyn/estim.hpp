#ifndef __DYN_ESTIM_HPP__
#define __DYN_ESTIM_HPP__

#include <string>
#include <vector>

#include <math/linalg.hpp>
#include <sys/logging.hpp>
#include <dat/csvread.hpp>

namespace estim {

class tracker {
    protected:
        static bool exist;
        static bool instantiate (void);
};

class instance : private tracker {
    public:
        instance (const std::string & atm_path, int regr_count, double kalm_pos_var, double kalm_acc_var);

        linalg::fvector get_estm (const linalg::fvector & sens);

        enum class except_ctor {fail};
        enum class except_get_estm {fail};

    private:
        logging::instance logging;
        bool init;

        csvread::instance atm_dat;
        std::vector<double> atm_pres, atm_altd;
        int regr_count, regr_idx;
        double regr_time, regr_sum_pos, regr_sum_time, regr_sum_pos_time, regr_sum_time_time;
        double kalm_time, kalm_pos_var, kalm_acc_var, kalm_acc;
        linalg::fvector kalm_stat;
        linalg::fmatrix kalm_stat_var;
        bool mode_regr, mode_kalm;
        double estm_pos_ver, estm_pos_ang, estm_vel_ver, estm_vel_ang;

        void atm_load (void);
        void atm_find (double pres, double & altd);

        void regr_iter (double pos, double acc, double pos_time);

        void kalm_pred (double acc, double acc_time);
        void kalm_updt (double pos);
        void kalm_iter (double pos, double acc, double pos_time, double acc_time);

        void qua_conj (const linalg::fvector & qua, linalg::fvector & res);
        void qua_prod (const linalg::fvector & qua1, const linalg::fvector & qua2, linalg::fvector & res);
        void qua_tran (const linalg::fvector & vec, const linalg::fvector & qua, linalg::fvector & res);

        void comp_ang (const linalg::fvector & rot, const linalg::fvector & qua);
        void comp_lin (const linalg::fvector & qua, const linalg::fvector & lia, double pres, double lia_time, double pres_time);

        enum class except_atm_load {fail};
};

}

#endif

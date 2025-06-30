#ifndef __DYN_SIMUL_HPP__
#define __DYN_SIMUL_HPP__

#include <string>
#include <vector>

#include <math/linalg.hpp>
#include <sys/logging.hpp>
#include <dat/csvread.hpp>

namespace simul {

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
            const linalg::fmatrix & imu_rot_var, const linalg::fmatrix & imu_qua_var, const linalg::fmatrix & imu_lia_var,
            double imu_rot_rate, double imu_qua_rate, double imu_lia_rate,
            double bar_pres_var, double bar_temp_var, double bar_pres_rate, double bar_temp_rate,
            double act_turn
        );

        void put_time (double time);
        void put_ctrl (double ctrl);
        void put_stat (const linalg::fvector & stat);

        linalg::fvector get_stat (void);
        linalg::fvector get_sens (void);

        void update (void);

        enum class except_ctor {fail};
        enum class except_put_time {fail};
        enum class except_put_ctrl {fail};
        enum class except_put_stat {fail};
        enum class except_get_stat {fail};
        enum class except_get_sens {fail};
        enum class except_update {fail};

    private:
        logging::instance logging;
        bool init;

        csvread::instance aer_dat, atm_dat;
        std::vector<double> aer_attk, aer_mach, aer_brak;
        std::vector<std::vector<std::vector<double>>> aer_lift, aer_drag, aer_cprs;
        std::vector<double> atm_altd, atm_pres, atm_temp, atm_dens;
        double env_grvty, env_gcnst, env_molar, env_gamma;
        double vhc_mass, vhc_iner, vhc_cmas, vhc_area;
        linalg::fmatrix imu_rot_var, imu_qua_var, imu_lia_var;
        double imu_rot_rate, imu_qua_rate, imu_lia_rate;
        double bar_pres_var, bar_temp_var, bar_pres_rate, bar_temp_rate;
        double act_turn;
        double ang_aln, ang_ins;

        double time_prev, time_curr;
        double ctrl;
        linalg::fvector stat, sens;
        bool time_prev_valid, time_curr_valid, ctrl_valid, stat_valid, sens_valid;

        void aer_load (void);
        void aer_find (double attk, double mach, double brak, double & lift, double & drag, double & cprs);

        void atm_load (void);
        void atm_find (double altd, double & pres, double & temp, double & dens);

        void qua_conj (const linalg::fvector & qua, linalg::fvector & res);
        void qua_prod (const linalg::fvector & qua1, const linalg::fvector & qua2, linalg::fvector & res);
        void qua_tran (const linalg::fvector & vec, const linalg::fvector & qua, linalg::fvector & res);

        void comp_derv (double ctrl, const linalg::fvector & stat, linalg::fvector & derv);
        void comp_sens (double time, double ctrl, const linalg::fvector & stat, linalg::fvector & sens);

        enum class except_aer_load {fail};
        enum class except_atm_load {fail};
};

}

#endif

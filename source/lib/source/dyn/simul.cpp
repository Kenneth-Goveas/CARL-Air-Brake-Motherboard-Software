#include <cmath>

#include <iterator>
#include <algorithm>
#include <string>
#include <vector>

#include <math/linalg.hpp>
#include <math/randnum.hpp>
#include <sys/logging.hpp>
#include <dat/csvread.hpp>
#include <dyn/simul.hpp>

namespace simul {

bool tracker::exist = false;

bool tracker::instantiate (void) {
    if (tracker::exist) {
        return false;
    } else {
        tracker::exist = true;
        return true;
    }
}

instance::instance (
    const std::string & aer_path, const std::string & atm_path,
    double env_grvty, double env_gcnst, double env_molar, double env_gamma,
    double vhc_mass, double vhc_iner, double vhc_cmas, double vhc_area,
    const linalg::fmatrix & imu_rot_var, const linalg::fmatrix & imu_qua_var, const linalg::fmatrix & imu_lia_var,
    double imu_rot_rate, double imu_qua_rate, double imu_lia_rate,
    double bar_pres_var, double bar_temp_var, double bar_pres_rate, double bar_temp_rate,
    double act_turn
) try
  : logging("simul"), init(false),
    aer_dat(aer_path), atm_dat(atm_path),
    env_grvty(env_grvty), env_gcnst(env_gcnst), env_molar(env_molar), env_gamma(env_gamma),
    vhc_mass(vhc_mass), vhc_iner(vhc_iner), vhc_cmas(vhc_cmas), vhc_area(vhc_area),
    imu_rot_var(imu_rot_var), imu_qua_var(imu_qua_var), imu_lia_var(imu_lia_var),
    imu_rot_rate(imu_rot_rate), imu_qua_rate(imu_qua_rate), imu_lia_rate(imu_lia_rate),
    bar_pres_var(bar_pres_var), bar_temp_var(bar_temp_var), bar_pres_rate(bar_pres_rate), bar_temp_rate(bar_temp_rate),
    act_turn(act_turn),
    ang_aln(randnum::unif(-M_PI, M_PI)), ang_ins(randnum::unif(-M_PI, M_PI)),
    time_prev_valid(false), time_curr_valid(false), ctrl_valid(false), stat_valid(false), sens_valid(false) {

    this->logging.inf(
        "Initializing instance: ",
        "Aer path: ", aer_path, ", Atm path: ", atm_path, ", ",
        "Gravity: ", this->env_grvty, ", Gas const: ", this->env_gcnst, ", Mol mass: ", this->env_molar, ", Gamma: ", this->env_gamma, ", ",
        "Mass: ", this->vhc_mass, ", Iner: ", this->vhc_iner, ", Cent mass: ", this->vhc_cmas, ", Cross sect area: ", this->vhc_area, ", ",
        "Rot vel var: ", this->imu_rot_var, ", Att qua var: ", this->imu_qua_var, ", Lin acc var: ", this->imu_lia_var, ", ",
        "Rot vel rate: ", this->imu_rot_rate, ", Att qua rate: ", this->imu_qua_rate, ", Lin acc rate: ", this->imu_lia_rate, ", ",
        "Pres var: ", this->bar_pres_var, ", Temp var: ", this->bar_temp_var, ", ",
        "Pres rate: ", this->bar_pres_rate, ", Temp rate: ", this->bar_temp_rate, ", ",
        "Act turn rate: ", this->act_turn
    );

    if (!instance::instantiate()) {
        this->logging.err("Failed to initialize instance (Instance already exists)");
        throw except_ctor::fail;
    }

    if (this->env_grvty <= 0) {
        this->logging.err("Failed to initialize instance (Invalid gravitational acceleration)");
        throw except_ctor::fail;
    }

    if (this->env_gcnst <= 0) {
        this->logging.err("Failed to initialize instance (Invalid ideal gas constant)");
        throw except_ctor::fail;
    }

    if (this->env_molar <= 0) {
        this->logging.err("Failed to initialize instance (Invalid atmospheric molar mass)");
        throw except_ctor::fail;
    }

    if (this->env_gamma <= 0) {
        this->logging.err("Failed to initialize instance (Invalid atmospheric gamma factor)");
        throw except_ctor::fail;
    }

    if (this->vhc_mass <= 0) {
        this->logging.err("Failed to initialize instance (Invalid vehicular mass)");
        throw except_ctor::fail;
    }

    if (this->vhc_iner <= 0) {
        this->logging.err("Failed to initialize instance (Invalid vehicular moment of inertia)");
        throw except_ctor::fail;
    }

    if (this->vhc_cmas <= 0) {
        this->logging.err("Failed to initialize instance (Invalid vehicular center of mass)");
        throw except_ctor::fail;
    }

    if (this->vhc_area <= 0) {
        this->logging.err("Failed to initialize instance (Invalid vehicular cross sectional area)");
        throw except_ctor::fail;
    }

    if (linalg::rows(this->imu_rot_var) != 3 || linalg::cols(this->imu_rot_var) != 3 || !linalg::sym_pdf(this->imu_rot_var)) {
        this->logging.err("Failed to initialize instance (Invalid rotational velocity variance)");
        throw except_ctor::fail;
    }

    if (linalg::rows(this->imu_qua_var) != 3 || linalg::cols(this->imu_qua_var) != 3 || !linalg::sym_pdf(this->imu_qua_var)) {
        this->logging.err("Failed to initialize instance (Invalid attitude quaternion variance)");
        throw except_ctor::fail;
    }

    if (linalg::rows(this->imu_lia_var) != 3 || linalg::cols(this->imu_lia_var) != 3 || !linalg::sym_pdf(this->imu_lia_var)) {
        this->logging.err("Failed to initialize instance (Invalid linear acceleration variance)");
        throw except_ctor::fail;
    }

    if (this->imu_rot_rate <= 0) {
        this->logging.err("Failed to initialize instance (Invalid rotational velocity data rate)");
        throw except_ctor::fail;
    }

    if (this->imu_qua_rate <= 0) {
        this->logging.err("Failed to initialize instance (Invalid attitude quaternion data rate)");
        throw except_ctor::fail;
    }

    if (this->imu_lia_rate <= 0) {
        this->logging.err("Failed to initialize instance (Invalid linear acceleration data rate)");
        throw except_ctor::fail;
    }

    if (this->bar_pres_var <= 0) {
        this->logging.err("Failed to initialize instance (Invalid pressure variance)");
        throw except_ctor::fail;
    }

    if (this->bar_temp_var <= 0) {
        this->logging.err("Failed to initialize instance (Invalid temperature variance)");
        throw except_ctor::fail;
    }

    if (this->bar_pres_rate <= 0) {
        this->logging.err("Failed to initialize instance (Invalid pressure data rate)");
        throw except_ctor::fail;
    }

    if (this->bar_temp_rate <= 0) {
        this->logging.err("Failed to initialize instance (Invalid temperature data rate)");
        throw except_ctor::fail;
    }

    if (this->act_turn <= 0) {
        this->logging.err("Failed to initialize instance (Invalid actuator turning rate)");
        throw except_ctor::fail;
    }

    try {
        this->aer_load();
        this->atm_load();
    } catch (...) {
        this->logging.err("Failed to initialize instance");
        throw except_ctor::fail;
    }

    this->init = true;
} catch (csvread::instance::except_ctor) {
    throw except_ctor::fail;
}

void instance::put_time (double time) {
    this->logging.inf("Setting time stamp: ", time);

    if (!this->init) {
        this->logging.err("Failed to set time stamp (Instance not initialized)");
        throw except_put_time::fail;
    }

    if (this->time_prev_valid && this->time_curr_valid) {
        this->logging.err("Failed to set time stamp (Time stamp already set)");
        throw except_put_time::fail;
    }

    if (!this->time_prev_valid) {
        if (time <= 0) {
            this->logging.err("Failed to set time stamp (Invalid time stamp)");
            throw except_put_time::fail;
        }
        this->time_prev = time;
        this->time_prev_valid = true;
    } else if (!this->time_curr_valid) {
        if (time <= this->time_prev) {
            this->logging.err("Failed to set time stamp (Invalid time stamp)");
            throw except_put_time::fail;
        }
        this->time_curr = time;
        this->time_curr_valid = true;
    }
}

void instance::put_ctrl (double ctrl) {
    this->logging.inf("Setting control signal: ", ctrl);

    if (!this->init) {
        this->logging.err("Failed to set control signal (Instance not initialized)");
        throw except_put_ctrl::fail;
    }

    if (this->ctrl_valid) {
        this->logging.err("Failed to set control signal (Control signal already set)");
        throw except_put_ctrl::fail;
    }

    if (ctrl < 0 || ctrl > 1) {
        this->logging.err("Failed to set control signal (Invalid control signal)");
        throw except_put_ctrl::fail;
    }

    this->ctrl = ctrl;
    this->ctrl_valid = true;
}

void instance::put_stat (const linalg::fvector & stat) {
    this->logging.inf("Setting state vector: ", stat);

    if (!this->init) {
        this->logging.err("Failed to set state vector (Instance not initialized)");
        throw except_put_stat::fail;
    }

    if (this->stat_valid) {
        this->logging.err("Failed to set state vector (State vector already set)");
        throw except_put_stat::fail;
    }

    if (linalg::dim(stat) != 7) {
        this->logging.err("Failed to set state vector (Invalid state vector)");
        throw except_put_stat::fail;
    }

    if (stat[2] < -M_PI || stat[2] > M_PI) {
        this->logging.err("Failed to set state vector (Invalid state vector)");
        throw except_put_stat::fail;
    }

    if (stat[6] < 0 || stat[6] > 1) {
        this->logging.err("Failed to set state vector (Invalid state vector)");
        throw except_put_stat::fail;
    }

    this->stat = stat;
    this->stat_valid = true;
}

linalg::fvector instance::get_stat (void) {
    this->logging.inf("Getting state vector");

    if (!this->init) {
        this->logging.err("Failed to get state vector (Instance not initialized)");
        throw except_get_stat::fail;
    }

    if (!this->stat_valid) {
        this->logging.err("Failed to get state vector (State vector not set)");
        throw except_get_stat::fail;
    }

    this->logging.inf("Got state vector: ", this->stat);

    return this->stat;
}

linalg::fvector instance::get_sens (void) {
    this->logging.inf("Getting sensing vector");

    if (!this->init) {
        this->logging.err("Failed to get sensing vector (Instance not initialized)");
        throw except_get_sens::fail;
    }

    if (!this->sens_valid) {
        this->logging.err("Failed to get sensing vector (Sensing vector not set)");
        throw except_get_sens::fail;
    }

    this->logging.inf("Got sensing vector: ", this->sens);

    return this->sens;
}

void instance::update (void) {
    double time_diff;
    linalg::fvector runge_kutta_k1, runge_kutta_k2, runge_kutta_k3, runge_kutta_k4;

    this->logging.inf("Propagating system");

    if (!this->init) {
        this->logging.err("Failed to propagate system (Instance not initialized)");
        throw except_update::fail;
    }

    if (!this->time_prev_valid || !this->time_curr_valid) {
        this->logging.err("Failed to propagate system (Time stamp not set)");
        throw except_update::fail;
    }

    if (!this->ctrl_valid) {
        this->logging.err("Failed to propagate system (Control signal not set)");
        throw except_update::fail;
    }

    if (!this->stat_valid) {
        this->logging.err("Failed to propagate system (State vector not set)");
        throw except_update::fail;
    }

    time_diff = this->time_curr - this->time_prev;

    this->comp_derv(this->ctrl, this->stat, runge_kutta_k1);
    this->comp_derv(this->ctrl, this->stat + runge_kutta_k1 * time_diff / 2, runge_kutta_k2);
    this->comp_derv(this->ctrl, this->stat + runge_kutta_k2 * time_diff / 2, runge_kutta_k3);
    this->comp_derv(this->ctrl, this->stat + runge_kutta_k3 * time_diff, runge_kutta_k4);

    this->stat += (runge_kutta_k1 + 2 * runge_kutta_k2 + 2 * runge_kutta_k3 + runge_kutta_k4) * time_diff / 6;

    while (this->stat[2] < -M_PI) {
        this->stat[2] += 2 * M_PI;
    }

    while (this->stat[2] > M_PI) {
        this->stat[2] -= 2 * M_PI;
    }

    if (this->stat[6] < 0) {
        this->stat[6] = 0;
    }

    if (this->stat[6] > 1) {
        this->stat[6] = 1;
    }

    this->comp_sens(this->time_curr, this->ctrl, this->stat, this->sens);

    this->time_prev = this->time_curr;

    this->time_curr_valid = false;
    this->ctrl_valid = false;
    this->sens_valid = true;
}

void instance::aer_load (void) {
    std::vector<std::string> head;
    std::vector<double> body;
    std::vector<std::vector<double>> data;
    std::vector<std::vector<std::vector<bool>>> covered;
    double attk, mach, brak, lift, drag, cprs;
    int attk_col, mach_col, brak_col, lift_col, drag_col, cprs_col;
    int attk_idx, mach_idx, brak_idx;

    this->logging.inf("Creating aerodynamic lookup table");

    try {
        head = this->aer_dat.get_head();
    } catch (...) {
        this->logging.err("Failed to create aerodynamic lookup table");
        throw except_aer_load::fail;
    }

    attk_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Angle of attack (rad)"));
    mach_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Mach number"));
    brak_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Brake deployment"));
    lift_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Lift coefficient"));
    drag_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Drag coefficient"));
    cprs_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Center of pressure (m)"));

    if (
           attk_col == int(head.size()) || mach_col == int(head.size()) || brak_col == int(head.size())
        || lift_col == int(head.size()) || drag_col == int(head.size()) || cprs_col == int(head.size())
    ) {
        this->logging.err("Failed to create aerodynamic lookup table (Missing column)");
        throw except_aer_load::fail;
    }

    try {
        while (true) {
            body = this->aer_dat.get_body();

            attk = body[attk_col];
            mach = body[mach_col];
            brak = body[brak_col];
            lift = body[lift_col];
            drag = body[drag_col];
            cprs = body[cprs_col];

            data.push_back({attk, mach, brak, lift, drag, cprs});
        }
    } catch (csvread::instance::except_get_body except) {
        switch (except) {
            case csvread::instance::except_get_body::fail:
                this->logging.err("Failed to create aerodynamic lookup table");
                throw except_aer_load::fail;
            case csvread::instance::except_get_body::eof:
                break;
        }
    }

    for (auto itr = data.begin(); itr < data.end(); itr++) {
        if (!std::binary_search(this->aer_attk.begin(), this->aer_attk.end(), (*itr)[0])) {
            this->aer_attk.insert(std::lower_bound(this->aer_attk.begin(), this->aer_attk.end(), (*itr)[0]), (*itr)[0]);
        }
        if (!std::binary_search(this->aer_mach.begin(), this->aer_mach.end(), (*itr)[1])) {
            this->aer_mach.insert(std::lower_bound(this->aer_mach.begin(), this->aer_mach.end(), (*itr)[1]), (*itr)[1]);
        }
        if (!std::binary_search(this->aer_brak.begin(), this->aer_brak.end(), (*itr)[2])) {
            this->aer_brak.insert(std::lower_bound(this->aer_brak.begin(), this->aer_brak.end(), (*itr)[2]), (*itr)[2]);
        }
    }

    if (this->aer_attk.size() < 2 || this->aer_mach.size() < 2 || this->aer_brak.size() < 2) {
        this->logging.err("Failed to create aerodynamic lookup table (Insufficient data)");
        throw except_aer_load::fail;
    }

    this->aer_lift = std::vector<std::vector<std::vector<double>>>(
        this->aer_attk.size(), std::vector<std::vector<double>>(
            this->aer_mach.size(), std::vector<double>(
                this->aer_brak.size()
            )
        )
    );

    this->aer_drag = std::vector<std::vector<std::vector<double>>>(
        this->aer_attk.size(), std::vector<std::vector<double>>(
            this->aer_mach.size(), std::vector<double>(
                this->aer_brak.size()
            )
        )
    );

    this->aer_cprs = std::vector<std::vector<std::vector<double>>>(
        this->aer_attk.size(), std::vector<std::vector<double>>(
            this->aer_mach.size(), std::vector<double>(
                this->aer_brak.size()
            )
        )
    );

    covered = std::vector<std::vector<std::vector<bool>>>(
        this->aer_attk.size(), std::vector<std::vector<bool>>(
            this->aer_mach.size(), std::vector<bool>(
                this->aer_brak.size(), false
            )
        )
    );

    for (auto itr = data.begin(); itr < data.end(); itr++) {
        attk_idx = std::distance(this->aer_attk.begin(), std::find(this->aer_attk.begin(), this->aer_attk.end(), (*itr)[0]));
        mach_idx = std::distance(this->aer_mach.begin(), std::find(this->aer_mach.begin(), this->aer_mach.end(), (*itr)[1]));
        brak_idx = std::distance(this->aer_brak.begin(), std::find(this->aer_brak.begin(), this->aer_brak.end(), (*itr)[2]));

        if (covered[attk_idx][mach_idx][brak_idx]) {
            this->logging.err("Failed to create aerodynamic lookup table (Repeated data point)");
            throw except_aer_load::fail;
        }

        this->aer_lift[attk_idx][mach_idx][brak_idx] = (*itr)[3];
        this->aer_drag[attk_idx][mach_idx][brak_idx] = (*itr)[4];
        this->aer_cprs[attk_idx][mach_idx][brak_idx] = (*itr)[5];

        covered[attk_idx][mach_idx][brak_idx] = true;
    }

    for (auto elem1 : covered) {
        for (auto elem2 : elem1) {
            for (auto elem3 : elem2) {
                if (!elem3) {
                    this->logging.err("Failed to create aerodynamic lookup table (Missing data point)");
                    throw except_aer_load::fail;
                }
            }
        }
    }
}

void instance::aer_find (double attk, double mach, double brak, double & lift, double & drag, double & cprs) {
    int attk_idx1, attk_idx2, mach_idx1, mach_idx2, brak_idx1, brak_idx2;
    double attk_mean, mach_mean, brak_mean, attk_diff, mach_diff, brak_diff;
    double lift_mean, lift_grad_attk, lift_grad_mach, lift_grad_brak;
    double drag_mean, drag_grad_attk, drag_grad_mach, drag_grad_brak;
    double cprs_mean, cprs_grad_attk, cprs_grad_mach, cprs_grad_brak;

    if (attk < this->aer_attk.front()) {
        attk_idx1 = 0;
    } else if (attk >= this->aer_attk.back()) {
        attk_idx1 = this->aer_attk.size() - 2;
    } else {
        attk_idx1 = std::distance(this->aer_attk.begin(), std::upper_bound(this->aer_attk.begin(), this->aer_attk.end(), attk)) - 1;
    }
    attk_idx2 = attk_idx1 + 1;

    if (mach < this->aer_mach.front()) {
        mach_idx1 = 0;
    } else if (mach >= this->aer_mach.back()) {
        mach_idx1 = this->aer_mach.size() - 2;
    } else {
        mach_idx1 = std::distance(this->aer_mach.begin(), std::upper_bound(this->aer_mach.begin(), this->aer_mach.end(), mach)) - 1;
    }
    mach_idx2 = mach_idx1 + 1;

    if (brak < this->aer_brak.front()) {
        brak_idx1 = 0;
    } else if (brak >= this->aer_brak.back()) {
        brak_idx1 = this->aer_brak.size() - 2;
    } else {
        brak_idx1 = std::distance(this->aer_brak.begin(), std::upper_bound(this->aer_brak.begin(), this->aer_brak.end(), brak)) - 1;
    }
    brak_idx2 = brak_idx1 + 1;

    attk_mean = 0.5 * (this->aer_attk[attk_idx1] + this->aer_attk[attk_idx2]);
    mach_mean = 0.5 * (this->aer_mach[mach_idx1] + this->aer_mach[mach_idx2]);
    brak_mean = 0.5 * (this->aer_brak[brak_idx1] + this->aer_brak[brak_idx2]);

    attk_diff = attk - attk_mean;
    mach_diff = mach - mach_mean;
    brak_diff = brak - brak_mean;

    lift_mean = 0.125 * (
        + this->aer_lift[attk_idx1][mach_idx1][brak_idx1]
        + this->aer_lift[attk_idx1][mach_idx1][brak_idx2]
        + this->aer_lift[attk_idx1][mach_idx2][brak_idx1]
        + this->aer_lift[attk_idx1][mach_idx2][brak_idx2]
        + this->aer_lift[attk_idx2][mach_idx1][brak_idx1]
        + this->aer_lift[attk_idx2][mach_idx1][brak_idx2]
        + this->aer_lift[attk_idx2][mach_idx2][brak_idx1]
        + this->aer_lift[attk_idx2][mach_idx2][brak_idx2]
    );

    lift_grad_attk = 0.25 * (
        - this->aer_lift[attk_idx1][mach_idx1][brak_idx1]
        - this->aer_lift[attk_idx1][mach_idx1][brak_idx2]
        - this->aer_lift[attk_idx1][mach_idx2][brak_idx1]
        - this->aer_lift[attk_idx1][mach_idx2][brak_idx2]
        + this->aer_lift[attk_idx2][mach_idx1][brak_idx1]
        + this->aer_lift[attk_idx2][mach_idx1][brak_idx2]
        + this->aer_lift[attk_idx2][mach_idx2][brak_idx1]
        + this->aer_lift[attk_idx2][mach_idx2][brak_idx2]
    ) / (this->aer_attk[attk_idx2] - this->aer_attk[attk_idx1]);

    lift_grad_mach = 0.25 * (
        - this->aer_lift[attk_idx1][mach_idx1][brak_idx1]
        - this->aer_lift[attk_idx1][mach_idx1][brak_idx2]
        + this->aer_lift[attk_idx1][mach_idx2][brak_idx1]
        + this->aer_lift[attk_idx1][mach_idx2][brak_idx2]
        - this->aer_lift[attk_idx2][mach_idx1][brak_idx1]
        - this->aer_lift[attk_idx2][mach_idx1][brak_idx2]
        + this->aer_lift[attk_idx2][mach_idx2][brak_idx1]
        + this->aer_lift[attk_idx2][mach_idx2][brak_idx2]
    ) / (this->aer_mach[mach_idx2] - this->aer_mach[mach_idx1]);

    lift_grad_brak = 0.25 * (
        - this->aer_lift[attk_idx1][mach_idx1][brak_idx1]
        + this->aer_lift[attk_idx1][mach_idx1][brak_idx2]
        - this->aer_lift[attk_idx1][mach_idx2][brak_idx1]
        + this->aer_lift[attk_idx1][mach_idx2][brak_idx2]
        - this->aer_lift[attk_idx2][mach_idx1][brak_idx1]
        + this->aer_lift[attk_idx2][mach_idx1][brak_idx2]
        - this->aer_lift[attk_idx2][mach_idx2][brak_idx1]
        + this->aer_lift[attk_idx2][mach_idx2][brak_idx2]
    ) / (this->aer_brak[brak_idx2] - this->aer_brak[brak_idx1]);

    drag_mean = 0.125 * (
        + this->aer_drag[attk_idx1][mach_idx1][brak_idx1]
        + this->aer_drag[attk_idx1][mach_idx1][brak_idx2]
        + this->aer_drag[attk_idx1][mach_idx2][brak_idx1]
        + this->aer_drag[attk_idx1][mach_idx2][brak_idx2]
        + this->aer_drag[attk_idx2][mach_idx1][brak_idx1]
        + this->aer_drag[attk_idx2][mach_idx1][brak_idx2]
        + this->aer_drag[attk_idx2][mach_idx2][brak_idx1]
        + this->aer_drag[attk_idx2][mach_idx2][brak_idx2]
    );

    drag_grad_attk = 0.25 * (
        - this->aer_drag[attk_idx1][mach_idx1][brak_idx1]
        - this->aer_drag[attk_idx1][mach_idx1][brak_idx2]
        - this->aer_drag[attk_idx1][mach_idx2][brak_idx1]
        - this->aer_drag[attk_idx1][mach_idx2][brak_idx2]
        + this->aer_drag[attk_idx2][mach_idx1][brak_idx1]
        + this->aer_drag[attk_idx2][mach_idx1][brak_idx2]
        + this->aer_drag[attk_idx2][mach_idx2][brak_idx1]
        + this->aer_drag[attk_idx2][mach_idx2][brak_idx2]
    ) / (this->aer_attk[attk_idx2] - this->aer_attk[attk_idx1]);

    drag_grad_mach = 0.25 * (
        - this->aer_drag[attk_idx1][mach_idx1][brak_idx1]
        - this->aer_drag[attk_idx1][mach_idx1][brak_idx2]
        + this->aer_drag[attk_idx1][mach_idx2][brak_idx1]
        + this->aer_drag[attk_idx1][mach_idx2][brak_idx2]
        - this->aer_drag[attk_idx2][mach_idx1][brak_idx1]
        - this->aer_drag[attk_idx2][mach_idx1][brak_idx2]
        + this->aer_drag[attk_idx2][mach_idx2][brak_idx1]
        + this->aer_drag[attk_idx2][mach_idx2][brak_idx2]
    ) / (this->aer_mach[mach_idx2] - this->aer_mach[mach_idx1]);

    drag_grad_brak = 0.25 * (
        - this->aer_drag[attk_idx1][mach_idx1][brak_idx1]
        + this->aer_drag[attk_idx1][mach_idx1][brak_idx2]
        - this->aer_drag[attk_idx1][mach_idx2][brak_idx1]
        + this->aer_drag[attk_idx1][mach_idx2][brak_idx2]
        - this->aer_drag[attk_idx2][mach_idx1][brak_idx1]
        + this->aer_drag[attk_idx2][mach_idx1][brak_idx2]
        - this->aer_drag[attk_idx2][mach_idx2][brak_idx1]
        + this->aer_drag[attk_idx2][mach_idx2][brak_idx2]
    ) / (this->aer_brak[brak_idx2] - this->aer_brak[brak_idx1]);

    cprs_mean = 0.125 * (
        + this->aer_cprs[attk_idx1][mach_idx1][brak_idx1]
        + this->aer_cprs[attk_idx1][mach_idx1][brak_idx2]
        + this->aer_cprs[attk_idx1][mach_idx2][brak_idx1]
        + this->aer_cprs[attk_idx1][mach_idx2][brak_idx2]
        + this->aer_cprs[attk_idx2][mach_idx1][brak_idx1]
        + this->aer_cprs[attk_idx2][mach_idx1][brak_idx2]
        + this->aer_cprs[attk_idx2][mach_idx2][brak_idx1]
        + this->aer_cprs[attk_idx2][mach_idx2][brak_idx2]
    );

    cprs_grad_attk = 0.25 * (
        - this->aer_cprs[attk_idx1][mach_idx1][brak_idx1]
        - this->aer_cprs[attk_idx1][mach_idx1][brak_idx2]
        - this->aer_cprs[attk_idx1][mach_idx2][brak_idx1]
        - this->aer_cprs[attk_idx1][mach_idx2][brak_idx2]
        + this->aer_cprs[attk_idx2][mach_idx1][brak_idx1]
        + this->aer_cprs[attk_idx2][mach_idx1][brak_idx2]
        + this->aer_cprs[attk_idx2][mach_idx2][brak_idx1]
        + this->aer_cprs[attk_idx2][mach_idx2][brak_idx2]
    ) / (this->aer_attk[attk_idx2] - this->aer_attk[attk_idx1]);

    cprs_grad_mach = 0.25 * (
        - this->aer_cprs[attk_idx1][mach_idx1][brak_idx1]
        - this->aer_cprs[attk_idx1][mach_idx1][brak_idx2]
        + this->aer_cprs[attk_idx1][mach_idx2][brak_idx1]
        + this->aer_cprs[attk_idx1][mach_idx2][brak_idx2]
        - this->aer_cprs[attk_idx2][mach_idx1][brak_idx1]
        - this->aer_cprs[attk_idx2][mach_idx1][brak_idx2]
        + this->aer_cprs[attk_idx2][mach_idx2][brak_idx1]
        + this->aer_cprs[attk_idx2][mach_idx2][brak_idx2]
    ) / (this->aer_mach[mach_idx2] - this->aer_mach[mach_idx1]);

    cprs_grad_brak = 0.25 * (
        - this->aer_cprs[attk_idx1][mach_idx1][brak_idx1]
        + this->aer_cprs[attk_idx1][mach_idx1][brak_idx2]
        - this->aer_cprs[attk_idx1][mach_idx2][brak_idx1]
        + this->aer_cprs[attk_idx1][mach_idx2][brak_idx2]
        - this->aer_cprs[attk_idx2][mach_idx1][brak_idx1]
        + this->aer_cprs[attk_idx2][mach_idx1][brak_idx2]
        - this->aer_cprs[attk_idx2][mach_idx2][brak_idx1]
        + this->aer_cprs[attk_idx2][mach_idx2][brak_idx2]
    ) / (this->aer_brak[brak_idx2] - this->aer_brak[brak_idx1]);

    lift = lift_mean + lift_grad_attk * attk_diff + lift_grad_mach * mach_diff + lift_grad_brak * brak_diff;
    drag = drag_mean + drag_grad_attk * attk_diff + drag_grad_mach * mach_diff + drag_grad_brak * brak_diff;
    cprs = cprs_mean + cprs_grad_attk * attk_diff + cprs_grad_mach * mach_diff + cprs_grad_brak * brak_diff;
}

void instance::atm_load (void) {
    std::vector<std::string> head;
    std::vector<double> body;
    std::vector<std::vector<double>> data;
    std::vector<bool> covered;
    double altd, pres, temp, dens;
    int altd_col, pres_col, temp_col, dens_col;
    int altd_idx;

    this->logging.inf("Creating atmospheric lookup table");

    try {
        head = this->atm_dat.get_head();
    } catch (...) {
        this->logging.err("Failed to create atmospheric lookup table");
        throw except_atm_load::fail;
    }

    altd_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Altitude (m)"));
    pres_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Pressure (Pa)"));
    temp_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Temperature (K)"));
    dens_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Density (kg/m³)"));

    if (altd_col == int(head.size()) || pres_col == int(head.size()) || temp_col == int(head.size()) || dens_col == int(head.size())) {
        this->logging.err("Failed to create atmospheric lookup table (Missing column)");
        throw except_atm_load::fail;
    }

    try {
        while (true) {
            body = this->atm_dat.get_body();

            altd = body[altd_col];
            pres = body[pres_col];
            temp = body[temp_col];
            dens = body[dens_col];

            data.push_back({altd, pres, temp, dens});
        }
    } catch (csvread::instance::except_get_body except) {
        switch (except) {
            case csvread::instance::except_get_body::fail:
                this->logging.err("Failed to create atmospheric lookup table");
                throw except_atm_load::fail;
            case csvread::instance::except_get_body::eof:
                break;
        }
    }

    for (auto itr = data.begin(); itr < data.end(); itr++) {
        if (!std::binary_search(this->atm_altd.begin(), this->atm_altd.end(), (*itr)[0])) {
            this->atm_altd.insert(std::lower_bound(this->atm_altd.begin(), this->atm_altd.end(), (*itr)[0]), (*itr)[0]);
        }
    }

    if (this->atm_altd.size() < 2) {
        this->logging.err("Failed to create atmospheric lookup table (Insufficient data)");
        throw except_atm_load::fail;
    }

    this->atm_pres = std::vector<double>(this->atm_altd.size());
    this->atm_temp = std::vector<double>(this->atm_altd.size());
    this->atm_dens = std::vector<double>(this->atm_altd.size());

    covered = std::vector<bool>(this->atm_altd.size(), false);

    for (auto itr = data.begin(); itr < data.end(); itr++) {
        altd_idx = std::distance(this->atm_altd.begin(), std::find(this->atm_altd.begin(), this->atm_altd.end(), (*itr)[0]));

        if (covered[altd_idx]) {
            this->logging.err("Failed to create atmospheric lookup table (Repeated data point)");
            throw except_atm_load::fail;
        }

        this->atm_pres[altd_idx] = (*itr)[1];
        this->atm_temp[altd_idx] = (*itr)[2];
        this->atm_dens[altd_idx] = (*itr)[3];

        covered[altd_idx] = true;
    }

    for (auto elem : covered) {
        if (!elem) {
            this->logging.err("Failed to create atmospheric lookup table (Missing data point)");
            throw except_atm_load::fail;
        }
    }
}

void instance::atm_find (double altd, double & pres, double & temp, double & dens) {
    int altd_idx1, altd_idx2;
    double altd_mean, altd_diff;
    double pres_mean, pres_grad_altd;
    double temp_mean, temp_grad_altd;
    double dens_mean, dens_grad_altd;

    if (altd < this->atm_altd.front()) {
        altd_idx1 = 0;
    } else if (altd >= this->atm_altd.back()) {
        altd_idx1 = this->atm_altd.size() - 2;
    } else {
        altd_idx1 = std::distance(this->atm_altd.begin(), std::upper_bound(this->atm_altd.begin(), this->atm_altd.end(), altd)) - 1;
    }
    altd_idx2 = altd_idx1 + 1;

    altd_mean = 0.5 * (this->atm_altd[altd_idx1] + this->atm_altd[altd_idx2]);
    altd_diff = altd - altd_mean;

    pres_mean = 0.5 * (this->atm_pres[altd_idx1] + this->atm_pres[altd_idx2]);
    pres_grad_altd = (this->atm_pres[altd_idx2] - this->atm_pres[altd_idx1]) / (this->atm_altd[altd_idx2] - this->atm_altd[altd_idx1]);

    temp_mean = 0.5 * (this->atm_temp[altd_idx1] + this->atm_temp[altd_idx2]);
    temp_grad_altd = (this->atm_temp[altd_idx2] - this->atm_temp[altd_idx1]) / (this->atm_altd[altd_idx2] - this->atm_altd[altd_idx1]);

    dens_mean = 0.5 * (this->atm_dens[altd_idx1] + this->atm_dens[altd_idx2]);
    dens_grad_altd = (this->atm_dens[altd_idx2] - this->atm_dens[altd_idx1]) / (this->atm_altd[altd_idx2] - this->atm_altd[altd_idx1]);

    pres = pres_mean + pres_grad_altd * altd_diff;
    temp = temp_mean + temp_grad_altd * altd_diff;
    dens = dens_mean + dens_grad_altd * altd_diff;
}

void instance::qua_conj (const linalg::fvector & qua, linalg::fvector & res) {
    res = {qua[0], -qua[1], -qua[2], -qua[3]};
}

void instance::qua_prod (const linalg::fvector & qua1, const linalg::fvector & qua2, linalg::fvector & res) {
    res = {
        qua1[0] * qua2[0] - qua1[1] * qua2[1] - qua1[2] * qua2[2] - qua1[3] * qua2[3],
        qua1[0] * qua2[1] + qua1[1] * qua2[0] + qua1[2] * qua2[3] - qua1[3] * qua2[2],
        qua1[0] * qua2[2] - qua1[1] * qua2[3] + qua1[2] * qua2[0] + qua1[3] * qua2[1],
        qua1[0] * qua2[3] + qua1[1] * qua2[2] - qua1[2] * qua2[1] + qua1[3] * qua2[0]
    };
}

void instance::qua_tran (const linalg::fvector & vec, const linalg::fvector & qua, linalg::fvector & res) {
    linalg::fvector aux[3];
    this->qua_conj(qua, aux[0]);
    aux[1] = {0, vec[0], vec[1], vec[2]};
    this->qua_prod(aux[0], aux[1], aux[2]);
    this->qua_prod(aux[2], qua, res);
    res = {res[1], res[2], res[3]};
}

void instance::comp_derv (double ctrl, const linalg::fvector & stat, linalg::fvector & derv) {
    double pos_ver, pos_ang, vel_ver, vel_hor, vel_ang, acc_ver, acc_hor, acc_ang, brak, turn;
    double attk, mach, lift, drag, cprs, altd, pres, temp, dens;
    double vel_par, vel_per, vel_tot, vel_snd;
    double forc_wght, forc_lift, forc_drag, forc_ver, forc_hor, torq;
    double cos_pos_ang, sin_pos_ang;

    pos_ver = stat[0];
    pos_ang = stat[2];
    vel_ver = stat[3];
    vel_hor = stat[4];
    vel_ang = stat[5];
    brak = stat[6];

    if (vel_ver > 0) {
        altd = pos_ver;
        this->atm_find(altd, pres, temp, dens);

        cos_pos_ang = std::cos(pos_ang);
        sin_pos_ang = std::sin(pos_ang);

        vel_par = vel_ver * cos_pos_ang + vel_hor * sin_pos_ang;
        vel_per = vel_hor * cos_pos_ang - vel_ver * sin_pos_ang;
        vel_tot = std::sqrt(vel_ver * vel_ver + vel_hor * vel_hor);
        vel_snd = std::sqrt(this->env_gamma * this->env_gcnst * temp / this->env_molar);

        attk = std::atan2(vel_per, vel_par);
        mach = vel_tot / vel_snd;
        this->aer_find(attk, mach, brak, lift, drag, cprs);

        forc_wght = this->vhc_mass * this->env_grvty;
        forc_lift = 0.5 * dens * vel_tot * vel_tot * this->vhc_area * lift * attk;
        forc_drag = 0.5 * dens * vel_tot * vel_tot * this->vhc_area * drag;
        forc_ver = forc_lift * sin_pos_ang - forc_drag * cos_pos_ang - forc_wght;
        forc_hor = -(forc_lift * cos_pos_ang + forc_drag * sin_pos_ang);
        torq = forc_lift * (cprs - this->vhc_cmas);
    } else {
        forc_ver = -this->vhc_mass * this->env_grvty;
        forc_hor = 0;
        torq = 0;
    }

    acc_ver = forc_ver / this->vhc_mass;
    acc_hor = forc_hor / this->vhc_mass;
    acc_ang = torq / this->vhc_iner;

    if (ctrl > brak) {
        turn = this->act_turn;
    } else if (ctrl < brak) {
        turn = -this->act_turn;
    } else {
        turn = 0;
    }

    derv = {vel_ver, vel_hor, vel_ang, acc_ver, acc_hor, acc_ang, turn};
}

void instance::comp_sens (double time, double ctrl, const linalg::fvector & stat, linalg::fvector & sens) {
    linalg::fvector derv;
    double pos_ver, pos_ang, vel_ang, acc_ver, acc_hor;
    linalg::fvector rot, qua, lia;
    double pres, temp;
    double rot_time, qua_time, lia_time, pres_time, temp_time;
    double altd;
    linalg::fvector trn_eth_lch, trn_eth_aln, trn_eth_ins, trn_lch_aln, trn_aln_ins, trn_aln_fix, trn_fix_aln, trn_fix_ins;
    linalg::fvector nois_rot, nois_qua, nois_lia;
    double nois_pres, nois_temp;
    double nois_qua_ang;
    linalg::fvector nois_qua_dir;
    bool updt_rot, updt_qua, updt_lia, updt_pres, updt_temp;
    double ign;

    this->comp_derv(ctrl, stat, derv);

    pos_ver = stat[0];
    pos_ang = stat[2];
    vel_ang = stat[5];
    acc_ver = derv[3];
    acc_hor = derv[4];

    altd = pos_ver;

    trn_eth_lch = {0.5, -0.5, -0.5, -0.5};

    trn_lch_aln = {std::cos(0.5 * this->ang_aln), std::sin(0.5 * this->ang_aln), 0, 0};

    trn_aln_ins = {
        std::cos(0.5 * pos_ang), 0, std::sin(0.5 * pos_ang) * std::cos(this->ang_ins), std::sin(0.5 * pos_ang) * std::sin(this->ang_ins)
    };

    trn_aln_fix = {std::cos(0.5 * this->ang_ins - 0.25 * M_PI), std::sin(0.5 * this->ang_ins - 0.25 * M_PI), 0, 0};

    this->qua_prod(trn_eth_lch, trn_lch_aln, trn_eth_aln);
    trn_eth_aln /= linalg::norm(trn_eth_aln);

    this->qua_prod(trn_eth_aln, trn_aln_ins, trn_eth_ins);
    trn_eth_ins /= linalg::norm(trn_eth_ins);

    this->qua_conj(trn_aln_fix, trn_fix_aln);

    this->qua_prod(trn_fix_aln, trn_aln_ins, trn_fix_ins);
    trn_fix_ins /= linalg::norm(trn_fix_ins);

    if (linalg::null(sens)) {
        updt_rot = true;
        updt_qua = true;
        updt_lia = true;
        updt_pres = true;
        updt_temp = true;
    } else {
        rot = {sens[0], sens[1], sens[2]};
        qua = {sens[3], sens[4], sens[5], sens[6]};
        lia = {sens[7], sens[8], sens[9]};
        pres = sens[10];
        temp = sens[11];

        rot_time = sens[12];
        qua_time = sens[13];
        lia_time = sens[14];
        pres_time = sens[15];
        temp_time = sens[16];

        updt_rot = (time >= rot_time + 1.0 / this->imu_rot_rate);
        updt_qua = (time >= qua_time + 1.0 / this->imu_qua_rate);
        updt_lia = (time >= lia_time + 1.0 / this->imu_lia_rate);
        updt_pres = (time >= pres_time + 1.0 / this->bar_pres_rate);
        updt_temp = (time >= temp_time + 1.0 / this->bar_temp_rate);
    }

    if (updt_rot) {
        nois_rot = randnum::gauss({0, 0, 0}, this->imu_rot_var);

        this->qua_tran({0, 0, vel_ang}, trn_fix_ins, rot);
        rot += nois_rot;

        rot_time = time;
    }

    if (updt_qua) {
        nois_qua = randnum::gauss({0, 0, 0}, this->imu_qua_var);

        nois_qua_ang = linalg::norm(nois_qua);
        nois_qua_dir = nois_qua / linalg::norm(nois_qua);

        nois_qua = {0, nois_qua_dir[0], nois_qua_dir[1], nois_qua_dir[2]};
        nois_qua *= std::sin(0.5 * nois_qua_ang);
        nois_qua[0] = std::cos(0.5 * nois_qua_ang);

        this->qua_prod(trn_eth_ins, nois_qua, qua);
        qua /= linalg::norm(qua);

        qua_time = time;
    }

    if (updt_lia) {
        nois_lia = randnum::gauss({0, 0, 0}, this->imu_lia_var);

        this->qua_tran({acc_ver, acc_hor, 0}, trn_fix_ins, lia);
        lia += nois_lia;

        lia_time = time;
    }

    if (updt_pres) {
        nois_pres = randnum::gauss(0, this->bar_pres_var);

        this->atm_find(altd, pres, ign, ign);
        pres += nois_pres;

        pres_time = time;
    }

    if (updt_temp) {
        nois_temp = randnum::gauss(0, this->bar_temp_var);

        this->atm_find(altd, ign, temp, ign);
        temp += nois_temp;

        temp_time = time;
    }

    sens = {
        rot[0], rot[1], rot[2], qua[0], qua[1], qua[2], qua[3], lia[0], lia[1], lia[2], pres, temp,
        rot_time, qua_time, lia_time, pres_time, temp_time
    };
}

}

#include <cmath>

#include <iterator>
#include <algorithm>
#include <string>
#include <vector>

#include <math/linalg.hpp>
#include <sys/logging.hpp>
#include <dat/csvread.hpp>
#include <dyn/cntrl.hpp>

namespace cntrl {

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
    double mpc_apog, double mpc_enab, double mpc_step, double mpc_phrz, double mpc_thrz, double mpc_tol
) try
  : logging("cntrl"), init(false),
    aer_dat(aer_path), atm_dat(atm_path),
    env_grvty(env_grvty), env_gcnst(env_gcnst), env_molar(env_molar), env_gamma(env_gamma),
    vhc_mass(vhc_mass), vhc_iner(vhc_iner), vhc_cmas(vhc_cmas), vhc_area(vhc_area),
    mpc_apog(mpc_apog), mpc_enab(mpc_enab), mpc_step(mpc_step), mpc_phrz(mpc_phrz), mpc_thrz(mpc_thrz), mpc_tol(mpc_tol) {

    this->logging.inf(
        "Initializing instance: ",
        "Aer path: ", aer_path, ", Atm path: ", atm_path, ", ",
        "Gravity: ", this->env_grvty, ", Gas const: ", this->env_gcnst, ", Mol mass: ", this->env_molar, ", Gamma: ", this->env_gamma, ", ",
        "Mass: ", this->vhc_mass, ", Iner: ", this->vhc_iner, ", Cent mass: ", this->vhc_cmas, ", Cross sect area: ", this->vhc_area, ", ",
        "Apog altd: ", this->mpc_apog, ", Enab altd: ", this->mpc_enab, ", Time step: ", this->mpc_step, ", ",
        "Pred horz: ", this->mpc_phrz, ", Term horz: ", this->mpc_thrz, ", Tol: ", this->mpc_tol
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

    if (this->mpc_apog <= 0) {
        this->logging.err("Failed to initialize instance (Invalid apogee altitude)");
        throw except_ctor::fail;
    }

    if (this->mpc_enab <= 0) {
        this->logging.err("Failed to initialize instance (Invalid enable altitude)");
        throw except_ctor::fail;
    }

    if (this->mpc_step <= 0) {
        this->logging.err("Failed to initialize instance (Invalid time step)");
        throw except_ctor::fail;
    }

    if (this->mpc_phrz <= 0) {
        this->logging.err("Failed to initialize instance (Invalid prediction horizon)");
        throw except_ctor::fail;
    }

    if (this->mpc_thrz <= 0) {
        this->logging.err("Failed to initialize instance (Invalid termination horizon)");
        throw except_ctor::fail;
    }

    if (this->mpc_tol <= 0) {
        this->logging.err("Failed to initialize instance (Invalid tolerance)");
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

double instance::get_ctrl (const linalg::fvector & estm) {
    double ctrl;
    double ctrl_min = 0, ctrl_mid = 0.5, ctrl_max = 1;
    int categ_min, categ_mid, categ_max;

    this->logging.inf("Getting control signal: Estm: ", estm);

    if (!this->init) {
        this->logging.err("Failed to get control signal (Instance not initialized)");
        throw except_get_ctrl::fail;
    }

    if (linalg::dim(estm) != 4) {
        this->logging.err("Failed to get control signal (Invalid estimate vector)");
        throw except_get_ctrl::fail;
    }

    if (estm[1] < 0 || estm[1] > M_PI) {
        this->logging.err("Failed to get control signal (Invalid estimate vector)");
        throw except_get_ctrl::fail;
    }

    if (estm[0] <= this->mpc_enab) {
        ctrl = 0;
    } else if (estm[2] <= 0) {
        ctrl = 0;
    } else if (estm[0] >= this->mpc_apog) {
        ctrl = 1;
    } else {
        try {
            this->comp_categ(ctrl_min, estm, categ_min);
            this->comp_categ(ctrl_max, estm, categ_max);

            while (ctrl_max - ctrl_min > this->mpc_tol) {
                ctrl_mid = 0.5 * (ctrl_min + ctrl_max);
                this->comp_categ(ctrl_mid, estm, categ_mid);

                if (categ_min == 1 && categ_mid == 1 && categ_max == 1) {
                    ctrl_min = ctrl_max;
                    categ_min = categ_max;
                } else if (categ_min == 1 && categ_mid == 1 && categ_max == 0) {
                    ctrl_min = ctrl_mid;
                    categ_min = categ_mid;
                } else if (categ_min == 1 && categ_mid == 1 && categ_max == -1) {
                    ctrl_min = ctrl_mid;
                    categ_min = categ_mid;
                } else if (categ_min == 1 && categ_mid == 0 && categ_max == 0) {
                    ctrl_max = ctrl_mid;
                    categ_max = categ_mid;
                } else if (categ_min == 1 && categ_mid == 0 && categ_max == -1) {
                    ctrl_max = ctrl_mid;
                    categ_max = categ_mid;
                } else if (categ_min == 1 && categ_mid == -1 && categ_max == -1) {
                    ctrl_max = ctrl_mid;
                    categ_max = categ_mid;
                } else if (categ_min == 0 && categ_mid == 0 && categ_max == 0) {
                    ctrl_max = ctrl_min;
                    categ_max = categ_min;
                } else if (categ_min == 0 && categ_mid == 0 && categ_max == -1) {
                    ctrl_max = ctrl_min;
                    categ_max = categ_min;
                } else if (categ_min == 0 && categ_mid == -1 && categ_max == -1) {
                    ctrl_max = ctrl_min;
                    categ_max = categ_min;
                } else if (categ_min == -1 && categ_mid == -1 && categ_max == -1) {
                    ctrl_max = ctrl_min;
                    categ_max = categ_min;
                } else {
                    ctrl_min = 0;
                    ctrl_max = 0;
                }
            }

            ctrl = 0.5 * (ctrl_min + ctrl_max);
        } catch (...) {
            this->logging.err("Failed to get control signal (Computation limit exceeded)");
            throw except_get_ctrl::fail;
        }
    }

    this->logging.inf("Got control signal: ", ctrl);

    return ctrl;
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

void instance::comp_stat (const linalg::fvector & estm, linalg::fvector & stat) {
    double pos_ver, pos_hor, pos_ang, vel_ver, vel_hor, vel_ang;

    pos_ver = estm[0];
    pos_ang = estm[1];
    vel_ver = estm[2];
    vel_ang = estm[3];

    pos_hor = 0;
    vel_hor = vel_ver * std::tan(pos_ang);

    stat = {pos_ver, pos_hor, pos_ang, vel_ver, vel_hor, vel_ang};
}

void instance::comp_derv (double ctrl, const linalg::fvector & stat, linalg::fvector & derv) {
    double pos_ver, pos_ang, vel_ver, vel_hor, vel_ang, acc_ver, acc_hor, acc_ang;
    double attk, mach, brak, lift, drag, cprs, altd, pres, temp, dens;
    double vel_par, vel_per, vel_tot, vel_snd;
    double forc_wght, forc_lift, forc_drag, forc_ver, forc_hor, torq;
    double cos_pos_ang, sin_pos_ang;

    pos_ver = stat[0];
    pos_ang = stat[2];
    vel_ver = stat[3];
    vel_hor = stat[4];
    vel_ang = stat[5];

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
        brak = ctrl;
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

    derv = {vel_ver, vel_hor, vel_ang, acc_ver, acc_hor, acc_ang};
}

void instance::comp_pred (double ctrl, const linalg::fvector & stat, linalg::fvector & pred) {
    linalg::fvector runge_kutta_k1, runge_kutta_k2, runge_kutta_k3, runge_kutta_k4;
    int count = 0;

    pred = stat;

    while (this->mpc_phrz > count * this->mpc_step && pred[3] > 0) { 
        this->comp_derv(ctrl, pred, runge_kutta_k1);
        this->comp_derv(ctrl, pred + runge_kutta_k1 * this->mpc_step / 2, runge_kutta_k2);
        this->comp_derv(ctrl, pred + runge_kutta_k2 * this->mpc_step / 2, runge_kutta_k3);
        this->comp_derv(ctrl, pred + runge_kutta_k3 * this->mpc_step, runge_kutta_k4);

        pred += (runge_kutta_k1 + 2 * runge_kutta_k2 + 2 * runge_kutta_k3 + runge_kutta_k4) * this->mpc_step / 6; 

        while (pred[2] < -M_PI) {
            pred[2] += 2 * M_PI;
        }    

        while (pred[2] > M_PI) {
            pred[2] -= 2 * M_PI;
        }    

        count++;
    }
}

void instance::comp_term (double ctrl, const linalg::fvector & pred, linalg::fvector & term) {
    linalg::fvector runge_kutta_k1, runge_kutta_k2, runge_kutta_k3, runge_kutta_k4;
    int count = 0;

    term = pred;

    while (term[3] > 0) {
        this->comp_derv(ctrl, term, runge_kutta_k1);
        this->comp_derv(ctrl, term + runge_kutta_k1 * this->mpc_step / 2, runge_kutta_k2);
        this->comp_derv(ctrl, term + runge_kutta_k2 * this->mpc_step / 2, runge_kutta_k3);
        this->comp_derv(ctrl, term + runge_kutta_k3 * this->mpc_step, runge_kutta_k4);

        term += (runge_kutta_k1 + 2 * runge_kutta_k2 + 2 * runge_kutta_k3 + runge_kutta_k4) * this->mpc_step / 6;

        while (term[2] < -M_PI) {
            term[2] += 2 * M_PI;
        }

        while (term[2] > M_PI) {
            term[2] -= 2 * M_PI;
        }

        count++;

        if (this->mpc_thrz <= count * this->mpc_step) {
            throw except_comp_term::fail;
        }
    }
}

void instance::comp_categ (double ctrl, const linalg::fvector & estm, int & categ) {
    linalg::fvector stat, pred, term;

    this->comp_stat(estm, stat);
    this->comp_pred(ctrl, stat, pred);

    try {
        this->comp_term(0, pred, term);
    } catch (...) {
        throw except_comp_categ::fail;
    }

    if (term[0] < this->mpc_apog) {
        categ = -1;
        return;
    }

    try {
        this->comp_term(1, pred, term);
    } catch (...) {
        throw except_comp_categ::fail;
    }

    if (term[0] > this->mpc_apog) {
        categ = 1;
        return;
    }

    categ = 0;
}

}

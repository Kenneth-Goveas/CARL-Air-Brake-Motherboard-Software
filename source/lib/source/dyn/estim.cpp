#include <cmath>

#include <iterator>
#include <algorithm>
#include <string>
#include <vector>

#include <math/linalg.hpp>
#include <sys/logging.hpp>
#include <dat/csvread.hpp>
#include <dyn/estim.hpp>

namespace estim {

bool tracker::exist = false;

bool tracker::instantiate (void) {
    if (tracker::exist) {
        return false;
    } else {
        tracker::exist = true;
        return true;
    }
}

instance::instance (const std::string & atm_path, int regr_count, double kalm_pos_var, double kalm_acc_var) try
  : logging("estim"), init(false),
    atm_dat(atm_path),
    regr_count(regr_count), regr_idx(0), regr_sum_pos(0), regr_sum_time(0), regr_sum_pos_time(0), regr_sum_time_time(0),
    kalm_pos_var(kalm_pos_var), kalm_acc_var(kalm_acc_var),
    mode_regr(true), mode_kalm(false) {

    this->logging.inf(
        "Initializing instance: ",
        "Atm path: ", atm_path, ", Regr count: ", this->regr_count, ", ",
        "Kalm pos var: ", this->kalm_pos_var, ", Kalm acc var: ", this->kalm_acc_var
    );

    if (!instance::instantiate()) {
        this->logging.err("Failed to initialize instance (Instance already exists)");
        throw except_ctor::fail;
    }

    if (this->regr_count <= 1) {
        this->logging.err("Failed to initialize instance (Invalid regressor iteration count)");
        throw except_ctor::fail;
    }

    if (this->kalm_pos_var <= 0) {
        this->logging.err("Failed to initialize instance (Invalid Kalman filter position variance)");
        throw except_ctor::fail;
    }

    if (this->kalm_acc_var <= 0) {
        this->logging.err("Failed to initialize instance (Invalid Kalman filter acceleration variance)");
        throw except_ctor::fail;
    }

    try {
        this->atm_load();
    } catch (...) {
        this->logging.err("Failed to initialize instance");
        throw except_ctor::fail;
    }

    this->init = true;
} catch (csvread::instance::except_ctor) {
    throw except_ctor::fail;
}

linalg::fvector instance::get_estm (const linalg::fvector & sens) {
    linalg::fvector estm;
    linalg::fvector rot, qua, lia;
    double pres;
    double lia_time, pres_time;

    this->logging.inf("Getting estimate vector: Sens: ", sens);

    if (!this->init) {
        this->logging.err("Failed to get estimate vector (Instance not initialized)");
        throw except_get_estm::fail;
    }

    if (linalg::dim(sens) != 17) {
        this->logging.err("Failed to get estimate vector (Invalid sensing vector)");
        throw except_get_estm::fail;
    }

    if (sens[10] <= 0) {
        this->logging.err("Failed to get estimate vector (Invalid sensing vector)");
        throw except_get_estm::fail;
    }

    if (sens[11] <= 0) {
        this->logging.err("Failed to get estimate vector (Invalid sensing vector)");
        throw except_get_estm::fail;
    }

    if (sens[12] <= 0) {
        this->logging.err("Failed to get estimate vector (Invalid sensing vector)");
        throw except_get_estm::fail;
    }

    if (sens[13] <= 0) {
        this->logging.err("Failed to get estimate vector (Invalid sensing vector)");
        throw except_get_estm::fail;
    }

    if (sens[14] <= 0) {
        this->logging.err("Failed to get estimate vector (Invalid sensing vector)");
        throw except_get_estm::fail;
    }

    if (sens[15] <= 0) {
        this->logging.err("Failed to get estimate vector (Invalid sensing vector)");
        throw except_get_estm::fail;
    }

    if (sens[16] <= 0) {
        this->logging.err("Failed to get estimate vector (Invalid sensing vector)");
        throw except_get_estm::fail;
    }

    rot = {sens[0], sens[1], sens[2]};
    qua = {sens[3], sens[4], sens[5], sens[6]};
    lia = {sens[7], sens[8], sens[9]};
    pres = sens[10];

    lia_time = sens[14];
    pres_time = sens[15];

    this->comp_ang(rot, qua);
    this->comp_lin(qua, lia, pres, lia_time, pres_time);

    estm = {this->estm_pos_ver, this->estm_pos_ang, this->estm_vel_ver, this->estm_vel_ang};

    this->logging.inf("Got estimate vector: ", estm);

    return estm;
}

void instance::atm_load (void) {
    std::vector<std::string> head;
    std::vector<double> body;
    std::vector<std::vector<double>> data;
    std::vector<bool> covered;
    double pres, altd;
    int pres_col, altd_col;
    int pres_idx;

    this->logging.inf("Creating atmospheric lookup table");

    try {
        head = this->atm_dat.get_head();
    } catch (...) {
        this->logging.err("Failed to create atmospheric lookup table");
        throw except_atm_load::fail;
    }

    pres_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Pressure (Pa)"));
    altd_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Altitude (m)"));

    if (pres_col == int(head.size()) || altd_col == int(head.size())) {
        this->logging.err("Failed to create atmospheric lookup table (Missing column)");
        throw except_atm_load::fail;
    }

    try {
        while (true) {
            body = this->atm_dat.get_body();

            pres = body[pres_col];
            altd = body[altd_col];

            data.push_back({pres, altd});
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
        if (!std::binary_search(this->atm_pres.begin(), this->atm_pres.end(), (*itr)[0])) {
            this->atm_pres.insert(std::lower_bound(this->atm_pres.begin(), this->atm_pres.end(), (*itr)[0]), (*itr)[0]);
        }
    }

    if (this->atm_pres.size() < 2) {
        this->logging.err("Failed to create atmospheric lookup table (Insufficient data)");
        throw except_atm_load::fail;
    }

    this->atm_altd = std::vector<double>(this->atm_pres.size());
    covered = std::vector<bool>(this->atm_pres.size(), false);

    for (auto itr = data.begin(); itr < data.end(); itr++) {
        pres_idx = std::distance(this->atm_pres.begin(), std::find(this->atm_pres.begin(), this->atm_pres.end(), (*itr)[0]));

        if (covered[pres_idx]) {
            this->logging.err("Failed to create atmospheric lookup table (Repeated data point)");
            throw except_atm_load::fail;
        }

        this->atm_altd[pres_idx] = (*itr)[1];

        covered[pres_idx] = true;
    }

    for (auto elem : covered) {
        if (!elem) {
            this->logging.err("Failed to create atmospheric lookup table (Missing data point)");
            throw except_atm_load::fail;
        }
    }
}

void instance::atm_find (double pres, double & altd) {
    int pres_idx1, pres_idx2;
    double pres_mean, pres_diff;
    double altd_mean, altd_grad_pres;

    if (pres < this->atm_pres.front()) {
        pres_idx1 = 0;
    } else if (pres >= this->atm_pres.back()) {
        pres_idx1 = this->atm_pres.size() - 2;
    } else {
        pres_idx1 = std::distance(this->atm_pres.begin(), std::upper_bound(this->atm_pres.begin(), this->atm_pres.end(), pres)) - 1;
    }
    pres_idx2 = pres_idx1 + 1;

    pres_mean = 0.5 * (this->atm_pres[pres_idx1] + this->atm_pres[pres_idx2]);
    pres_diff = pres - pres_mean;

    altd_mean = 0.5 * (this->atm_altd[pres_idx1] + this->atm_altd[pres_idx2]);
    altd_grad_pres = (this->atm_altd[pres_idx2] - this->atm_altd[pres_idx1]) / (this->atm_pres[pres_idx2] - this->atm_pres[pres_idx1]);

    altd = altd_mean + altd_grad_pres * pres_diff;
}

void instance::regr_iter (double pos, double acc, double pos_time) {
    double aux[3];

    if (this->regr_idx > 0 && pos_time <= this->regr_time) {
        return;
    }

    this->regr_idx++;

    this->regr_time = pos_time;
    this->regr_sum_pos += pos;
    this->regr_sum_time += pos_time;
    this->regr_sum_pos_time += pos * pos_time;
    this->regr_sum_time_time += pos_time * pos_time;

    if (this->regr_idx == 1) {
        this->estm_pos_ver = pos;
        this->estm_vel_ver = 0;
    } else {
        aux[0] = this->regr_sum_pos * this->regr_sum_time_time - this->regr_sum_time * this->regr_sum_pos_time;
        aux[1] = this->regr_idx * this->regr_sum_pos_time - this->regr_sum_pos * this->regr_sum_time;
        aux[2] = this->regr_idx * this->regr_sum_time_time - this->regr_sum_time * this->regr_sum_time;

        this->estm_pos_ver = (aux[0] + aux[1] * pos_time) / aux[2];
        this->estm_vel_ver = aux[1] / aux[2];
    }

    if (this->regr_idx == this->regr_count) {
        this->mode_regr = false;
        this->mode_kalm = true;

        this->kalm_time = pos_time;
        this->kalm_stat = {estm_pos_ver, estm_vel_ver};
        this->kalm_stat_var = {{0, 0}, {0, 0}};
        this->kalm_acc = acc;
    }
}

void instance::kalm_pred (double acc, double acc_time) {
    linalg::fmatrix matr_tran, matr_inpt, matr_nois;
    double step = acc_time - this->kalm_time;

    matr_tran = {{1, step}, {0, 1}};
    matr_inpt = {{step * step / 2}, {step}};
    matr_nois = {
        {this->kalm_acc_var * step * step * step * step / 4, this->kalm_acc_var * step * step * step / 2},
        {this->kalm_acc_var * step * step * step / 2, this->kalm_acc_var * step * step}
    };

    this->kalm_stat = matr_tran * this->kalm_stat + matr_inpt * linalg::fvector({this->kalm_acc});
    this->kalm_stat_var = matr_tran * this->kalm_stat_var * linalg::tran(matr_tran) + matr_nois;

    this->kalm_acc = acc;
    this->kalm_time = acc_time;
}

void instance::kalm_updt (double pos) {
    linalg::fmatrix matr_obsv, matr_gain, matr_nois;

    matr_obsv = {{1, 0}};
    matr_nois = {{this->kalm_pos_var}};
    matr_gain = this->kalm_stat_var * linalg::tran(matr_obsv)
                * linalg::inv(matr_obsv * this->kalm_stat_var * linalg::tran(matr_obsv) + matr_nois);

    this->kalm_stat += matr_gain * (linalg::fvector({pos}) - matr_obsv * this->kalm_stat);
    this->kalm_stat_var -= matr_gain * matr_obsv * this->kalm_stat_var;

    this->kalm_stat_var = (this->kalm_stat_var + linalg::tran(this->kalm_stat_var)) / 2;
}

void instance::kalm_iter (double pos, double acc, double pos_time, double acc_time) {
    bool use_pos, use_acc;

    use_pos = (pos_time > this->kalm_time);
    use_acc = (acc_time > this->kalm_time);

    if (pos_time >= acc_time) {
        if (use_acc) {
            this->kalm_pred(acc, acc_time);
        }
        if (use_pos) {
            this->kalm_pred(this->kalm_acc, pos_time);
            this->kalm_updt(pos);
        }
    } else {
        if (use_pos) {
            this->kalm_pred(this->kalm_acc, pos_time);
            this->kalm_updt(pos);
        }
        if (use_acc) {
            this->kalm_pred(acc, acc_time);
        }
    }

    this->estm_pos_ver = this->kalm_stat[0];
    this->estm_vel_ver = this->kalm_stat[1];
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

void instance::comp_ang (const linalg::fvector & rot, const linalg::fvector & qua) {
    double aux[5];

    aux[0] = 2 * (qua[1] * qua[3] - qua[0] * qua[2]);
    aux[1] = rot[1] * (1 - 2 * (qua[1] * qua[1] + qua[2] * qua[2]));
    aux[2] = 2 * rot[2] * (qua[0] * qua[1] + qua[2] * qua[3]);
    aux[3] = std::sqrt(rot[1] * rot[1] + rot[2] * rot[2]);
    aux[4] = std::sqrt(1 - aux[0] * aux[0]);

    this->estm_pos_ang = std::acos(aux[0]);
    if (aux[4] != 0) {
        this->estm_vel_ang = (aux[1] - aux[2]) / aux[4];
    } else {
        this->estm_vel_ang = std::copysign(aux[3], M_PI / 2 - this->estm_pos_ang);
    }
}

void instance::comp_lin (const linalg::fvector & qua, const linalg::fvector & lia, double pres, double lia_time, double pres_time) {
    double pos, acc;
    double pos_time, acc_time;
    linalg::fvector aux[2];

    this->atm_find(pres, pos);

    this->qua_conj(qua, aux[0]);
    this->qua_tran(lia, aux[0], aux[1]);
    acc = aux[1][2];

    pos_time = pres_time;
    acc_time = lia_time;

    if (this->mode_regr) {
        this->regr_iter(pos, acc, pos_time);
    } else if (this->mode_kalm) {
        this->kalm_iter(pos, acc, pos_time, acc_time);
    }
}

}

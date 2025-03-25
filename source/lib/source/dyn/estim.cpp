#include <cmath>

#include <iterator>
#include <algorithm>
#include <string>
#include <vector>

#include <math/linalg.hpp>

#include <sys/logging.hpp>

#include <dat/csvread.hpp>

#include <dyn/estim.hpp>

namespace estim :: intern {

std::string mod = "estim";

bool fail;

std::vector<double> atm_pres;
std::vector<double> atm_altd;

int regr_count;
double regr_time;
double regr_sum_pos = 0;
double regr_sum_time = 0;
double regr_sum_pos_time = 0;
double regr_sum_time_time = 0;

double kalm_time;
linalg::fvector kalm_stat;
linalg::fmatrix kalm_stat_var;
double kalm_acc;
double kalm_acc_var;
double kalm_pos_var;

bool mode_regr = true;
bool mode_kalm = false;

double estm_pos_ver;
double estm_pos_ang;
double estm_vel_ver;
double estm_vel_ang;

void atm_load (std::string path);
double atm_find (double pres);

void regr_iter (double pos, double acc, double pos_time);

void kalm_pred (double acc, double acc_time);
void kalm_updt (double pos);
void kalm_iter (double pos, double acc, double pos_time, double acc_time);

linalg::fvector qua_conj (linalg::fvector qua);
linalg::fvector qua_prod (linalg::fvector qua1, linalg::fvector qua2);
linalg::fvector qua_tran (linalg::fvector vec, linalg::fvector qua);

void comp_ang (linalg::fvector rot, linalg::fvector qua);
void comp_lin (linalg::fvector qua, linalg::fvector lia, double pres, double lia_time, double pres_time);

}

namespace estim {

bool fail (void) {
    return fail;
}

void init (std::string atm_path, int regr_count, double kalm_pos_var, double kalm_acc_var) {
    logging::inf(intern::mod,
        "Initializing estimation system: ",
        "Atm path: ", atm_path, ", Regr count: ", regr_count, ", Pos var: ", kalm_pos_var, "m², Acc var: ", kalm_acc_var, "m²/s⁴"
    );

    if (regr_count <= 1) {
        logging::err(intern::mod,
            "Failed to initialize estimation system (Invalid regression count)"
        );
        intern::fail = true;
        return;
    }

    if (kalm_pos_var <= 0) {
        logging::err(intern::mod,
            "Failed to initialize estimation system (Invalid vertical position variance)"
        );
        intern::fail = true;
        return;
    }

    if (kalm_acc_var <= 0) {
        logging::err(intern::mod,
            "Failed to initialize estimation system (Invalid vertical acceleration variance)"
        );
        intern::fail = true;
        return;
    }

    intern::atm_load(atm_path);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize estimation system"
        );
        intern::fail = true;
        return;
    }

    intern::regr_count = regr_count;

    intern::kalm_pos_var = kalm_pos_var;
    intern::kalm_acc_var = kalm_acc_var;

    intern::fail = false;
}

linalg::fvector get_estm (linalg::fvector sens) {
    linalg::fvector estm;
    linalg::fvector rot, qua, lia;
    double pres, temp;
    double rot_time, qua_time, lia_time, pres_time, temp_time;

    logging::inf(intern::mod,
        "Getting estimate vector: Sens: ", sens
    );

    if (linalg::dim(sens) != 17) {
        logging::err(intern::mod,
            "Failed to get estimate vector (Invalid sensing vector)"
        );
        intern::fail = true;
        return estm;
    }

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

    if (pres <= 0) {
        logging::err(intern::mod,
            "Failed to get estimate vector (Invalid sensing vector)"
        );
        intern::fail = true;
        return estm;
    }

    if (temp <= 0) {
        logging::err(intern::mod,
            "Failed to get estimate vector (Invalid sensing vector)"
        );
        intern::fail = true;
        return estm;
    }

    if (rot_time <= 0) {
        logging::err(intern::mod,
            "Failed to get estimate vector (Invalid sensing vector)"
        );
        intern::fail = true;
        return estm;
    }

    if (qua_time <= 0) {
        logging::err(intern::mod,
            "Failed to get estimate vector (Invalid sensing vector)"
        );
        intern::fail = true;
        return estm;
    }

    if (lia_time <= 0) {
        logging::err(intern::mod,
            "Failed to get estimate vector (Invalid sensing vector)"
        );
        intern::fail = true;
        return estm;
    }

    if (pres_time <= 0) {
        logging::err(intern::mod,
            "Failed to get estimate vector (Invalid sensing vector)"
        );
        intern::fail = true;
        return estm;
    }

    if (temp_time <= 0) {
        logging::err(intern::mod,
            "Failed to get estimate vector (Invalid sensing vector)"
        );
        intern::fail = true;
        return estm;
    }

    intern::comp_ang(rot, qua);
    intern::comp_lin(qua, lia, pres, lia_time, pres_time);

    logging::inf(intern::mod,
        "Got estimate vector: ", estm
    );

    intern::fail = false;
    return estm;
}

}

namespace estim :: intern {

void atm_load (std::string path) {
    int count;
    std::vector<std::string> head;
    std::vector<double> body;
    std::vector<std::vector<double>> data;
    std::vector<bool> covered;
    double pres, altd;
    int pres_col, altd_col;
    int pres_idx;

    logging::inf(mod,
        "Creating atmospheric lookup table"
    );

    logging::inf(mod,
        "Loading raw data"
    );

    csvread::open(path);
    if (csvread::fail()) {
        logging::err(mod,
            "Failed to load raw data"
        );
        logging::err(mod,
            "Failed to create atmospheric lookup table"
        );
        fail = true;
        return;
    }

    logging::inf(mod,
        "Processing raw data"
    );

    head = csvread::get_head();

    pres_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Pressure (Pa)"));
    altd_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Altitude (m)"));

    if (pres_col == head.size() || altd_col == head.size()) {
        logging::err(mod,
            "Failed to process raw data (Missing column)"
        );
        logging::wrn(mod,
            "Clearing raw data"
        );
        csvread::close();
        logging::err(mod,
            "Failed to create atmospheric lookup table"
        );
        fail = true;
        return;
    }

    while (true) {
        body = csvread::get_body();
        if (csvread::eof()) {
            break;
        }

        pres = body[pres_col];
        altd = body[altd_col];

        data.push_back({pres, altd});
    }

    for (auto itr = data.begin(); itr < data.end(); itr++) {
        if (!std::binary_search(atm_pres.begin(), atm_pres.end(), (*itr)[0])) {
            atm_pres.insert(std::lower_bound(atm_pres.begin(), atm_pres.end(), (*itr)[0]), (*itr)[0]);
        }
    }

    if (atm_pres.size() < 2) {
        logging::err(mod,
            "Failed to process raw data (Insufficient data)"
        );
        logging::wrn(mod,
            "Clearing raw data"
        );
        csvread::close();
        logging::err(mod,
            "Failed to create atmospheric lookup table"
        );
        fail = true;
        return;
    }

    atm_altd = std::vector<double>(atm_pres.size());
    covered = std::vector<bool>(atm_pres.size(), false);

    for (auto itr = data.begin(); itr < data.end(); itr++) {
        pres_idx = std::distance(atm_pres.begin(), std::find(atm_pres.begin(), atm_pres.end(), (*itr)[0]));

        if (covered[pres_idx]) {
            logging::err(mod,
                "Failed to process raw data (Repeated data point)"
            );
            logging::wrn(mod,
                "Clearing raw data"
            );
            csvread::close();
            logging::err(mod,
                "Failed to create atmospheric lookup table"
            );
            fail = true;
            return;
        }

        atm_altd[pres_idx] = (*itr)[1];

        covered[pres_idx] = true;
    }

    for (auto elem : covered) {
        if (!elem) {
            logging::err(mod,
                "Failed to process raw data (Missing data point)"
            );
            logging::wrn(mod,
                "Clearing raw data"
            );
            csvread::close();
            logging::err(mod,
                "Failed to create atmospheric lookup table"
            );
            fail = true;
            return;
        }
    }

    logging::inf(mod,
        "Clearing raw data"
    );

    csvread::close();

    fail = false;
}

double atm_find (double pres) {
    double altd;
    int pres_idx1, pres_idx2;
    double pres_mean, pres_diff;
    double altd_mean, altd_grad_pres;

    if (pres < atm_pres.front()) {
        pres_idx1 = 0;
    } else if (pres >= atm_pres.back()) {
        pres_idx1 = atm_pres.size() - 2;
    } else {
        pres_idx1 = std::distance(atm_pres.begin(), std::upper_bound(atm_pres.begin(), atm_pres.end(), pres)) - 1;
    }
    pres_idx2 = pres_idx1 + 1;

    pres_mean = 0.5 * (atm_pres[pres_idx1] + atm_pres[pres_idx2]);
    pres_diff = pres - pres_mean;

    altd_mean = 0.5 * (atm_altd[pres_idx1] + atm_altd[pres_idx2]);
    altd_grad_pres = (atm_altd[pres_idx2] - atm_altd[pres_idx1]) / (atm_pres[pres_idx2] - atm_pres[pres_idx1]);

    altd = altd_mean + altd_grad_pres * pres_diff;

    return altd;
}

void regr_iter (double pos, double acc, double pos_time) {
    static int count = 0;
    double aux[3];

    if (count > 0 && pos_time <= regr_time) {
        return;
    }

    count++;

    regr_time = pos_time;
    regr_sum_pos += pos;
    regr_sum_time += pos_time;
    regr_sum_pos_time += pos * pos_time;
    regr_sum_time_time += pos_time * pos_time;

    if (count == 1) {
        estm_pos_ver = pos;
        estm_vel_ver = 0;
    } else {
        aux[0] = regr_sum_pos * regr_sum_time_time - regr_sum_time * regr_sum_pos_time;
        aux[1] = count * regr_sum_pos_time - regr_sum_pos * regr_sum_time;
        aux[2] = count * regr_sum_time_time - regr_sum_time * regr_sum_time;

        estm_pos_ver = (aux[0] + aux[1] * pos_time) / aux[2];
        estm_vel_ver = aux[1] / aux[2];
    }

    if (count == regr_count) {
        mode_regr = false;
        mode_kalm = true;

        kalm_time = pos_time;
        kalm_stat = {estm_pos_ver, estm_vel_ver};
        kalm_stat_var = {{0, 0}, {0, 0}};
        kalm_acc = acc;
    }
}

void kalm_pred (double acc, double acc_time) {
    linalg::fmatrix matr_tran, matr_inpt, matr_nois;
    double step;

    matr_tran = {{1, step}, {0, 1}};
    matr_inpt = {{step * step / 2}, {step}};
    matr_nois = {
        {kalm_acc_var * step * step * step * step / 4, kalm_acc_var * step * step * step / 2},
        {kalm_acc_var * step * step * step / 2, kalm_acc_var * step * step}
    };

    kalm_stat = matr_tran * kalm_stat + matr_inpt * linalg::fvector({kalm_acc});
    kalm_stat_var = matr_tran * kalm_stat_var * linalg::tran(matr_tran) + matr_nois;

    kalm_acc = acc;
    kalm_time = acc_time;
}

void kalm_updt (double pos) {
    linalg::fmatrix matr_obsv, matr_gain, matr_nois;

    matr_obsv = {{1, 0}};
    matr_nois = {{kalm_pos_var}};
    matr_gain = kalm_stat_var * linalg::tran(matr_obsv) * linalg::inv(matr_obsv * kalm_stat_var * linalg::tran(matr_obsv) + matr_nois);

    kalm_stat += matr_gain * (linalg::fvector({pos}) - matr_obsv * kalm_stat);
    kalm_stat_var -= matr_gain * matr_obsv * kalm_stat_var;

    kalm_stat_var = (kalm_stat_var + linalg::tran(kalm_stat_var)) / 2;
}

void kalm_iter (double pos, double acc, double pos_time, double acc_time) {
    bool use_pos, use_acc;

    use_pos = (pos_time > kalm_time);
    use_acc = (acc_time > kalm_time);

    if (pos_time >= acc_time) {
        if (use_acc) {
            kalm_pred(acc, acc_time);
        }
        if (use_pos) {
            kalm_pred(kalm_acc, pos_time);
            kalm_updt(pos);
        }
    } else {
        if (use_pos) {
            kalm_pred(kalm_acc, pos_time);
            kalm_updt(pos);
        }
        if (use_acc) {
            kalm_pred(acc, acc_time);
        }
    }

    estm_pos_ver = kalm_stat[0];
    estm_vel_ver = kalm_stat[1];
}

linalg::fvector qua_conj (linalg::fvector qua) {
    linalg::fvector res = {qua[0], -qua[1], -qua[2], -qua[3]};
    return res;
}

linalg::fvector qua_prod (linalg::fvector qua1, linalg::fvector qua2) {
    linalg::fvector res = {
        qua1[0] * qua2[0] - qua1[1] * qua2[1] - qua1[2] * qua2[2] - qua1[3] * qua2[3],
        qua1[0] * qua2[1] + qua1[1] * qua2[0] + qua1[2] * qua2[3] - qua1[3] * qua2[2],
        qua1[0] * qua2[2] - qua1[1] * qua2[3] + qua1[2] * qua2[0] + qua1[3] * qua2[1],
        qua1[0] * qua2[3] + qua1[1] * qua2[2] - qua1[2] * qua2[1] + qua1[3] * qua2[0]
    };
    return res;
}

linalg::fvector qua_tran (linalg::fvector vec, linalg::fvector qua) {
    linalg::fvector res = {0, vec[0], vec[1], vec[2]};
    res = qua_prod(qua_prod(qua_conj(qua), res), qua);
    res = {res[1], res[2], res[3]};
    return res;
}

void comp_ang (linalg::fvector rot, linalg::fvector qua) {
    double aux[5];

    aux[0] = 2 * (qua[1] * qua[3] - qua[0] * qua[2]);
    aux[1] = rot[1] * (1 - 2 * (qua[1] * qua[1] + qua[2] * qua[2]));
    aux[2] = 2 * rot[2] * (qua[0] * qua[1] + qua[2] * qua[3]);
    aux[3] = std::sqrt(rot[1] * rot[1] + rot[2] * rot[2]);
    aux[4] = std::sqrt(1 - aux[0] * aux[0]);

    estm_pos_ang = std::acos(aux[0]);
    if (aux[4] != 0) {
        estm_vel_ang = (aux[1] - aux[2]) / aux[4];
    } else {
        estm_vel_ang = std::copysign(aux[3], M_PI / 2 - estm_pos_ang);
    }
}

void comp_lin (linalg::fvector qua, linalg::fvector lia, double pres, double lia_time, double pres_time) {
    double pos, acc;
    double pos_time, acc_time;
    double altd;

    altd = atm_find(pres);

    pos = altd;
    acc = qua_tran(lia, qua_conj(qua))[2];
    pos_time = pres_time;
    acc_time = lia_time;

    if (mode_regr) {
        regr_iter(pos, acc, pos_time);
    } else if (mode_kalm) {
        kalm_iter(pos, acc, pos_time, acc_time);
    }
}

}

#include <cmath>

#include <iterator>
#include <utility>
#include <algorithm>
#include <string>
#include <vector>
#include <tuple>

#include <math/linalg.hpp>
#include <math/randnum.hpp>

#include <sys/logging.hpp>

#include <dat/csvread.hpp>

#include <dyn/simul.hpp>

namespace simul :: intern {

std::string mod = "simul";

bool fail;

std::vector<double> aer_attk;
std::vector<double> aer_mach;
std::vector<double> aer_brak;
std::vector<std::vector<std::vector<double>>> aer_side;
std::vector<std::vector<std::vector<double>>> aer_drag;
std::vector<std::vector<std::vector<double>>> aer_cprs;

std::vector<double> atm_altd;
std::vector<double> atm_pres;
std::vector<double> atm_temp;
std::vector<double> atm_dens;

double env_grvty;
double env_gcnst;
double env_molar;
double env_gamma;

double vhc_mass;
double vhc_iner;
double vhc_cmas;
double vhc_area;

linalg::fmatrix imu_rot_var;
linalg::fmatrix imu_qua_var;
linalg::fmatrix imu_lia_var;
double imu_rot_rate;
double imu_qua_rate;
double imu_lia_rate;

double bar_pres_var;
double bar_temp_var;
double bar_pres_rate;
double bar_temp_rate;

double act_turn;

double time_prev, time_curr;
double ctrl;
linalg::fvector stat;
linalg::fvector sens;

void aer_load (std::string path);
std::tuple<double, double, double> aer_find (double attk, double mach, double brak);

void atm_load (std::string path);
std::tuple<double, double, double> atm_find (double altd);

linalg::fvector qua_conj (linalg::fvector qua);
linalg::fvector qua_prod (linalg::fvector qua1, linalg::fvector qua2);
linalg::fvector qua_tran (linalg::fvector vec, linalg::fvector qua);

linalg::fvector comp_derv (double ctrl, linalg::fvector stat);
linalg::fvector comp_sens (double time, double ctrl, linalg::fvector stat, linalg::fvector sens);

}

namespace simul {

bool fail (void) {
    return intern::fail;
}

void init (
    std::string aer_path, std::string atm_path,
    double env_grvty, double env_gcnst, double env_molar, double env_gamma,
    double vhc_mass, double vhc_iner, double vhc_cmas, double vhc_area,
    linalg::fmatrix imu_rot_var, linalg::fmatrix imu_qua_var, linalg::fmatrix imu_lia_var,
    double imu_rot_rate, double imu_qua_rate, double imu_lia_rate,
    double bar_pres_var, double bar_temp_var, double bar_pres_rate, double bar_temp_rate,
    double act_turn
) {
    logging::inf(intern::mod,
        "Initializing simulation system: ",
        "Aer path: ", aer_path, ", Atm path: ", atm_path, ", ",
        "Grvty: ", env_grvty, "m/s², Gas const: ", env_gcnst, "J/(mol K), Mol mass: ", env_molar, "kg/mol, Gamma: ", env_gamma, ", ",
        "Mass: ", vhc_mass, "kg, Iner: ", vhc_iner, "kg m², Cent mass: ", vhc_cmas, "m, Cross sect area: ", vhc_area, "m², ",
        "Ang vel var: ", imu_rot_var, "rad²/s², Att qua var: ", imu_qua_var, "rad², Lin acc var: ", imu_lia_var, "m²/s⁴, ",
        "Ang vel rate: ", imu_rot_rate, "Hz, Att qua rate: ", imu_qua_rate, "Hz, Lin acc rate: ", imu_lia_rate, "Hz, ",
        "Pres var: ", bar_pres_var, "Pa², Temp var: ", bar_temp_var, "K², ",
        "Pres rate: ", bar_pres_rate, "Hz, Temp rate: ", bar_temp_rate, "Hz, ",
        "Act turn rate: ", act_turn, "/s"
    );

    if (env_grvty <= 0) {
        logging::err(intern::mod,
            "Failed to initialize simulation system (Invalid gravitational acceleration)"
        );
        intern::fail = true;
        return;
    }

    if (env_gcnst <= 0) {
        logging::err(intern::mod,
            "Failed to initialize simulation system (Invalid ideal gas constant)"
        );
        intern::fail = true;
        return;
    }

    if (env_molar <= 0) {
        logging::err(intern::mod,
            "Failed to initialize simulation system (Invalid atmospheric molar mass)"
        );
        intern::fail = true;
        return;
    }

    if (env_gamma <= 0) {
        logging::err(intern::mod,
            "Failed to initialize simulation system (Invalid atmospheric gamma factor)"
        );
        intern::fail = true;
        return;
    }

    if (vhc_mass <= 0) {
        logging::err(intern::mod,
            "Failed to initialize simulation system (Invalid vehicular mass)"
        );
        intern::fail = true;
        return;
    }

    if (vhc_iner <= 0) {
        logging::err(intern::mod,
            "Failed to initialize simulation system (Invalid vehicular moment of inertia)"
        );
        intern::fail = true;
        return;
    }

    if (vhc_cmas <= 0) {
        logging::err(intern::mod,
            "Failed to initialize simulation system (Invalid vehicular center of mass)"
        );
        intern::fail = true;
        return;
    }

    if (vhc_area <= 0) {
        logging::err(intern::mod,
            "Failed to initialize simulation system (Invalid vehicular cross sectional area)"
        );
        intern::fail = true;
        return;
    }

    if (linalg::rows(imu_rot_var) != 3 || linalg::cols(imu_rot_var) != 3 || !linalg::is_sym_pdf(imu_rot_var)) {
        logging::err(intern::mod,
            "Failed to initialize simulation system (Invalid angular velocity variance)"
        );
        intern::fail = true;
        return;
    }

    if (linalg::rows(imu_qua_var) != 3 || linalg::cols(imu_qua_var) != 3 || !linalg::is_sym_pdf(imu_qua_var)) {
        logging::err(intern::mod,
            "Failed to initialize simulation system (Invalid attitude quaternion variance)"
        );
        intern::fail = true;
        return;
    }

    if (linalg::rows(imu_lia_var) != 3 || linalg::cols(imu_lia_var) != 3 || !linalg::is_sym_pdf(imu_lia_var)) {
        logging::err(intern::mod,
            "Failed to initialize simulation system (Invalid linear acceleration variance)"
        );
        intern::fail = true;
        return;
    }

    if (imu_rot_rate <= 0) {
        logging::err(intern::mod,
            "Failed to initialize simulation system (Invalid angular velocity data rate)"
        );
        intern::fail = true;
        return;
    }

    if (imu_qua_rate <= 0) {
        logging::err(intern::mod,
            "Failed to initialize simulation system (Invalid attitude quaternion data rate)"
        );
        intern::fail = true;
        return;
    }

    if (imu_lia_rate <= 0) {
        logging::err(intern::mod,
            "Failed to initialize simulation system (Invalid linear acceleration data rate)"
        );
        intern::fail = true;
        return;
    }

    if (bar_pres_var <= 0) {
        logging::err(intern::mod,
            "Failed to initialize simulation system (Invalid pressure variance)"
        );
        intern::fail = true;
        return;
    }

    if (bar_temp_var <= 0) {
        logging::err(intern::mod,
            "Failed to initialize simulation system (Invalid temperature variance)"
        );
        intern::fail = true;
        return;
    }

    if (bar_pres_rate <= 0) {
        logging::err(intern::mod,
            "Failed to initialize simulation system (Invalid pressure data rate)"
        );
        intern::fail = true;
        return;
    }

    if (bar_temp_rate <= 0) {
        logging::err(intern::mod,
            "Failed to initialize simulation system (Invalid temperature data rate)"
        );
        intern::fail = true;
        return;
    }

    if (act_turn <= 0) {
        logging::err(intern::mod,
            "Failed to initialize simulation system (Invalid actuator turning rate)"
        );
        intern::fail = true;
        return;
    }

    intern::aer_load(aer_path);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize simulation system"
        );
        intern::fail = true;
        return;
    }

    intern::atm_load(atm_path);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize simulation system"
        );
        intern::fail = true;
        return;
    }

    intern::env_grvty = env_grvty;
    intern::env_gcnst = env_gcnst;
    intern::env_molar = env_molar;
    intern::env_gamma = env_gamma;

    intern::vhc_mass = vhc_mass;
    intern::vhc_iner = vhc_iner;
    intern::vhc_cmas = vhc_cmas;
    intern::vhc_area = vhc_area;

    intern::imu_rot_var = imu_rot_var;
    intern::imu_qua_var = imu_qua_var;
    intern::imu_lia_var = imu_lia_var;
    intern::imu_rot_rate = imu_rot_rate;
    intern::imu_qua_rate = imu_qua_rate;
    intern::imu_lia_rate = imu_lia_rate;

    intern::bar_pres_var = bar_pres_var;
    intern::bar_temp_var = bar_temp_var;
    intern::bar_pres_rate = bar_pres_rate;
    intern::bar_temp_rate = bar_temp_rate;

    intern::act_turn = act_turn;

    intern::fail = false;
}

void put_time (double time) {
    static bool start = true;

    logging::inf(intern::mod,
        "Setting time: ", time, "s"
    );

    if (start) {
        if (time < 0) {
            logging::err(intern::mod,
                "Failed to set time (Invalid time)"
            );
            intern::fail = true;
            return;
        }
        intern::time_prev = time;
        intern::time_curr = time;
        start = false;
    } else {
        if (time <= intern::time_prev) {
            logging::err(intern::mod,
                "Failed to set time (Invalid time)"
            );
            intern::fail = true;
            return;
        }
        intern::time_curr = time;
    }

    intern::fail = false;
}

void put_ctrl (double ctrl) {
    logging::inf(intern::mod,
        "Setting control signal: ", ctrl
    );

    if (ctrl < 0 || ctrl > 1) {
        logging::err(intern::mod,
            "Failed to set control signal (Invalid control signal)"
        );
        intern::fail = true;
        return;
    }

    intern::ctrl = ctrl;

    intern::fail = false;
}

void put_stat (linalg::fvector stat) {
    double pos_ver, pos_hor, pos_ang, vel_ver, vel_hor, vel_ang, brak;

    logging::inf(intern::mod,
        "Setting state vector: ", stat
    );

    if (linalg::dim(stat) != 7) {
        logging::err(intern::mod,
            "Failed to set state vector (Invalid state vector)"
        );
        intern::fail = true;
        return;
    }

    pos_ver = stat[0];
    pos_hor = stat[1];
    pos_ang = stat[2];
    vel_ver = stat[3];
    vel_hor = stat[4];
    vel_ang = stat[5];
    brak = stat[6];

    if (pos_ang < -M_PI || pos_ang > M_PI) {
        logging::err(intern::mod,
            "Failed to set state vector (Invalid state vector)"
        );
        intern::fail = true;
        return;
    }

    if (brak < 0 || brak > 1) {
        logging::err(intern::mod,
            "Failed to set state vector (Invalid state vector)"
        );
        intern::fail = true;
        return;
    }

    intern::stat = stat;

    intern::fail = false;
}

linalg::fvector get_stat (void) {
    linalg::fvector stat;

    logging::inf(intern::mod,
        "Getting state vector"
    );

    stat = intern::stat;

    logging::inf(intern::mod,
        "Got state vector: ", stat
    );

    return stat;
}

linalg::fvector get_sens (void) {
    linalg::fvector sens;

    logging::inf(intern::mod,
        "Getting sensing vector"
    );

    sens = intern::sens;

    logging::inf(intern::mod,
        "Got sensing vector: ", sens
    );

    return sens;
}

void update (void) {
    double time_diff = intern::time_curr - intern::time_prev;
    double pos_ver, pos_hor, pos_ang, vel_ver, vel_hor, vel_ang, brak;
    linalg::fvector runge_kutta_k1, runge_kutta_k2, runge_kutta_k3, runge_kutta_k4;

    logging::inf(intern::mod,
        "Propagating system"
    );

    runge_kutta_k1 = intern::comp_derv(intern::ctrl, intern::stat);
    runge_kutta_k2 = intern::comp_derv(intern::ctrl, intern::stat + runge_kutta_k1 * time_diff / 2);
    runge_kutta_k3 = intern::comp_derv(intern::ctrl, intern::stat + runge_kutta_k2 * time_diff / 2);
    runge_kutta_k4 = intern::comp_derv(intern::ctrl, intern::stat + runge_kutta_k3 * time_diff);

    intern::stat += (runge_kutta_k1 + 2 * runge_kutta_k2 + 2 * runge_kutta_k3 + runge_kutta_k4) * time_diff / 6;

    pos_ver = intern::stat[0];
    pos_hor = intern::stat[1];
    pos_ang = intern::stat[2];
    vel_ver = intern::stat[3];
    vel_hor = intern::stat[4];
    vel_ang = intern::stat[5];
    brak = intern::stat[6];

    while (pos_ang < -M_PI) {
        pos_ang += 2 * M_PI;
    }

    while (pos_ang > M_PI) {
        pos_ang -= 2 * M_PI;
    }

    if (brak < 0) {
        brak = 0;
    }

    if (brak > 1) {
        brak = 1;
    }

    intern::stat = {pos_ver, pos_hor, pos_ang, vel_ver, vel_hor, vel_ang, brak};

    intern::sens = intern::comp_sens(intern::time_curr, intern::ctrl, intern::stat, intern::sens);

    intern::time_prev = intern::time_curr;
}

}

namespace simul :: intern {

void aer_load (std::string path) {
    int count;
    std::vector<std::string> head;
    std::vector<double> body;
    std::vector<std::vector<double>> data;
    std::vector<std::vector<std::vector<bool>>> covered;
    double attk, mach, brak, side, drag, cprs;
    int attk_col, mach_col, brak_col, side_col, drag_col, cprs_col;
    int attk_idx, mach_idx, brak_idx;

    logging::inf(mod,
        "Creating aerodynamic lookup table"
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
            "Failed to create aerodynamic lookup table"
        );
        fail = true;
        return;
    }

    logging::inf(mod,
        "Processing raw data"
    );

    head = csvread::get_head();

    attk_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Angle of attack (rad)"));
    mach_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Mach number"));
    brak_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Brake deployment"));
    side_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Side force coefficient"));
    drag_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Drag force coefficient"));
    cprs_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Center of pressure (m)"));

    if (
           attk_col == head.size() || mach_col == head.size() || brak_col == head.size()
        || side_col == head.size() || drag_col == head.size() || cprs_col == head.size()
    ) {
        logging::err(mod,
            "Failed to process raw data (Missing column)"
        );
        logging::wrn(mod,
            "Clearing raw data"
        );
        csvread::close();
        logging::err(mod,
            "Failed to create aerodynamic lookup table"
        );
        fail = true;
        return;
    }

    while (true) {
        body = csvread::get_body();
        if (csvread::eof()) {
            break;
        }

        attk = body[attk_col];
        mach = body[mach_col];
        brak = body[brak_col];
        side = body[side_col];
        drag = body[drag_col];
        cprs = body[cprs_col];

        data.push_back({attk, mach, brak, side, drag, cprs});
    }

    for (auto itr = data.begin(); itr < data.end(); itr++) {
        if (!std::binary_search(aer_attk.begin(), aer_attk.end(), (*itr)[0])) {
            aer_attk.insert(std::lower_bound(aer_attk.begin(), aer_attk.end(), (*itr)[0]), (*itr)[0]);
        }
        if (!std::binary_search(aer_mach.begin(), aer_mach.end(), (*itr)[1])) {
            aer_mach.insert(std::lower_bound(aer_mach.begin(), aer_mach.end(), (*itr)[1]), (*itr)[1]);
        }
        if (!std::binary_search(aer_brak.begin(), aer_brak.end(), (*itr)[2])) {
            aer_brak.insert(std::lower_bound(aer_brak.begin(), aer_brak.end(), (*itr)[2]), (*itr)[2]);
        }
    }

    if (aer_attk.size() < 2 || aer_mach.size() < 2 || aer_brak.size() < 2) {
        logging::err(mod,
            "Failed to process raw data (Insufficient data)"
        );
        logging::wrn(mod,
            "Clearing raw data"
        );
        csvread::close();
        logging::err(mod,
            "Failed to create aerodynamic lookup table"
        );
        fail = true;
        return;
    }

    aer_side = std::vector<std::vector<std::vector<double>>>(
        aer_attk.size(), std::vector<std::vector<double>>(
            aer_mach.size(), std::vector<double>(
                aer_brak.size()
            )
        )
    );

    aer_drag = std::vector<std::vector<std::vector<double>>>(
        aer_attk.size(), std::vector<std::vector<double>>(
            aer_mach.size(), std::vector<double>(
                aer_brak.size()
            )
        )
    );

    aer_cprs = std::vector<std::vector<std::vector<double>>>(
        aer_attk.size(), std::vector<std::vector<double>>(
            aer_mach.size(), std::vector<double>(
                aer_brak.size()
            )
        )
    );

    covered = std::vector<std::vector<std::vector<bool>>>(
        aer_attk.size(), std::vector<std::vector<bool>>(
            aer_mach.size(), std::vector<bool>(
                aer_brak.size(), false
            )
        )
    );

    for (auto itr = data.begin(); itr < data.end(); itr++) {
        attk_idx = std::distance(aer_attk.begin(), std::find(aer_attk.begin(), aer_attk.end(), (*itr)[0]));
        mach_idx = std::distance(aer_mach.begin(), std::find(aer_mach.begin(), aer_mach.end(), (*itr)[1]));
        brak_idx = std::distance(aer_brak.begin(), std::find(aer_brak.begin(), aer_brak.end(), (*itr)[2]));

        if (covered[attk_idx][mach_idx][brak_idx]) {
            logging::err(mod,
                "Failed to process raw data (Repeated data point)"
            );
            logging::wrn(mod,
                "Clearing raw data"
            );
            csvread::close();
            logging::err(mod,
                "Failed to create aerodynamic lookup table"
            );
            fail = true;
            return;
        }

        aer_side[attk_idx][mach_idx][brak_idx] = (*itr)[3];
        aer_drag[attk_idx][mach_idx][brak_idx] = (*itr)[4];
        aer_cprs[attk_idx][mach_idx][brak_idx] = (*itr)[5];

        covered[attk_idx][mach_idx][brak_idx] = true;
    }

    for (auto elem1 : covered) {
        for (auto elem2 : elem1) {
            for (auto elem3 : elem2) {
                if (!elem3) {
                    logging::err(mod,
                        "Failed to process raw data (Missing data point)"
                    );
                    logging::wrn(mod,
                        "Clearing raw data"
                    );
                    csvread::close();
                    logging::err(mod,
                        "Failed to create aerodynamic lookup table"
                    );
                    fail = true;
                    return;
                }
            }
        }
    }

    logging::inf(mod,
        "Clearing raw data"
    );

    csvread::close();

    fail = false;
}

std::tuple<double, double, double> aer_find (double attk, double mach, double brak) {
    double side, drag, cprs;
    int attk_idx1, attk_idx2, mach_idx1, mach_idx2, brak_idx1, brak_idx2;
    double attk_mean, mach_mean, brak_mean, attk_diff, mach_diff, brak_diff;
    double side_mean, side_grad_attk, side_grad_mach, side_grad_brak;
    double drag_mean, drag_grad_attk, drag_grad_mach, drag_grad_brak;
    double cprs_mean, cprs_grad_attk, cprs_grad_mach, cprs_grad_brak;

    if (attk < aer_attk.front()) {
        attk_idx1 = 0;
    } else if (attk >= aer_attk.back()) {
        attk_idx1 = aer_attk.size() - 2;
    } else {
        attk_idx1 = std::distance(aer_attk.begin(), std::upper_bound(aer_attk.begin(), aer_attk.end(), attk)) - 1;
    }
    attk_idx2 = attk_idx1 + 1;

    if (mach < aer_mach.front()) {
        mach_idx1 = 0;
    } else if (mach >= aer_mach.back()) {
        mach_idx1 = aer_mach.size() - 2;
    } else {
        mach_idx1 = std::distance(aer_mach.begin(), std::upper_bound(aer_mach.begin(), aer_mach.end(), mach)) - 1;
    }
    mach_idx2 = mach_idx1 + 1;

    if (brak < aer_brak.front()) {
        brak_idx1 = 0;
    } else if (brak >= aer_brak.back()) {
        brak_idx1 = aer_brak.size() - 2;
    } else {
        brak_idx1 = std::distance(aer_brak.begin(), std::upper_bound(aer_brak.begin(), aer_brak.end(), brak)) - 1;
    }
    brak_idx2 = brak_idx1 + 1;

    attk_mean = 0.5 * (aer_attk[attk_idx1] + aer_attk[attk_idx2]);
    mach_mean = 0.5 * (aer_mach[mach_idx1] + aer_mach[mach_idx2]);
    brak_mean = 0.5 * (aer_brak[brak_idx1] + aer_brak[brak_idx2]);

    attk_diff = attk - attk_mean;
    mach_diff = mach - mach_mean;
    brak_diff = brak - brak_mean;

    side_mean = 0.125 * (
        + aer_side[attk_idx1][mach_idx1][brak_idx1]
        + aer_side[attk_idx1][mach_idx1][brak_idx2]
        + aer_side[attk_idx1][mach_idx2][brak_idx1]
        + aer_side[attk_idx1][mach_idx2][brak_idx2]
        + aer_side[attk_idx2][mach_idx1][brak_idx1]
        + aer_side[attk_idx2][mach_idx1][brak_idx2]
        + aer_side[attk_idx2][mach_idx2][brak_idx1]
        + aer_side[attk_idx2][mach_idx2][brak_idx2]
    );

    side_grad_attk = 0.25 * (
        - aer_side[attk_idx1][mach_idx1][brak_idx1]
        - aer_side[attk_idx1][mach_idx1][brak_idx2]
        - aer_side[attk_idx1][mach_idx2][brak_idx1]
        - aer_side[attk_idx1][mach_idx2][brak_idx2]
        + aer_side[attk_idx2][mach_idx1][brak_idx1]
        + aer_side[attk_idx2][mach_idx1][brak_idx2]
        + aer_side[attk_idx2][mach_idx2][brak_idx1]
        + aer_side[attk_idx2][mach_idx2][brak_idx2]
    ) / (aer_attk[attk_idx2] - aer_attk[attk_idx1]);

    side_grad_mach = 0.25 * (
        - aer_side[attk_idx1][mach_idx1][brak_idx1]
        - aer_side[attk_idx1][mach_idx1][brak_idx2]
        + aer_side[attk_idx1][mach_idx2][brak_idx1]
        + aer_side[attk_idx1][mach_idx2][brak_idx2]
        - aer_side[attk_idx2][mach_idx1][brak_idx1]
        - aer_side[attk_idx2][mach_idx1][brak_idx2]
        + aer_side[attk_idx2][mach_idx2][brak_idx1]
        + aer_side[attk_idx2][mach_idx2][brak_idx2]
    ) / (aer_mach[mach_idx2] - aer_mach[mach_idx1]);

    side_grad_brak = 0.25 * (
        - aer_side[attk_idx1][mach_idx1][brak_idx1]
        + aer_side[attk_idx1][mach_idx1][brak_idx2]
        - aer_side[attk_idx1][mach_idx2][brak_idx1]
        + aer_side[attk_idx1][mach_idx2][brak_idx2]
        - aer_side[attk_idx2][mach_idx1][brak_idx1]
        + aer_side[attk_idx2][mach_idx1][brak_idx2]
        - aer_side[attk_idx2][mach_idx2][brak_idx1]
        + aer_side[attk_idx2][mach_idx2][brak_idx2]
    ) / (aer_brak[brak_idx2] - aer_brak[brak_idx1]);

    drag_mean = 0.125 * (
        + aer_drag[attk_idx1][mach_idx1][brak_idx1]
        + aer_drag[attk_idx1][mach_idx1][brak_idx2]
        + aer_drag[attk_idx1][mach_idx2][brak_idx1]
        + aer_drag[attk_idx1][mach_idx2][brak_idx2]
        + aer_drag[attk_idx2][mach_idx1][brak_idx1]
        + aer_drag[attk_idx2][mach_idx1][brak_idx2]
        + aer_drag[attk_idx2][mach_idx2][brak_idx1]
        + aer_drag[attk_idx2][mach_idx2][brak_idx2]
    );

    drag_grad_attk = 0.25 * (
        - aer_drag[attk_idx1][mach_idx1][brak_idx1]
        - aer_drag[attk_idx1][mach_idx1][brak_idx2]
        - aer_drag[attk_idx1][mach_idx2][brak_idx1]
        - aer_drag[attk_idx1][mach_idx2][brak_idx2]
        + aer_drag[attk_idx2][mach_idx1][brak_idx1]
        + aer_drag[attk_idx2][mach_idx1][brak_idx2]
        + aer_drag[attk_idx2][mach_idx2][brak_idx1]
        + aer_drag[attk_idx2][mach_idx2][brak_idx2]
    ) / (aer_attk[attk_idx2] - aer_attk[attk_idx1]);

    drag_grad_mach = 0.25 * (
        - aer_drag[attk_idx1][mach_idx1][brak_idx1]
        - aer_drag[attk_idx1][mach_idx1][brak_idx2]
        + aer_drag[attk_idx1][mach_idx2][brak_idx1]
        + aer_drag[attk_idx1][mach_idx2][brak_idx2]
        - aer_drag[attk_idx2][mach_idx1][brak_idx1]
        - aer_drag[attk_idx2][mach_idx1][brak_idx2]
        + aer_drag[attk_idx2][mach_idx2][brak_idx1]
        + aer_drag[attk_idx2][mach_idx2][brak_idx2]
    ) / (aer_mach[mach_idx2] - aer_mach[mach_idx1]);

    drag_grad_brak = 0.25 * (
        - aer_drag[attk_idx1][mach_idx1][brak_idx1]
        + aer_drag[attk_idx1][mach_idx1][brak_idx2]
        - aer_drag[attk_idx1][mach_idx2][brak_idx1]
        + aer_drag[attk_idx1][mach_idx2][brak_idx2]
        - aer_drag[attk_idx2][mach_idx1][brak_idx1]
        + aer_drag[attk_idx2][mach_idx1][brak_idx2]
        - aer_drag[attk_idx2][mach_idx2][brak_idx1]
        + aer_drag[attk_idx2][mach_idx2][brak_idx2]
    ) / (aer_brak[brak_idx2] - aer_brak[brak_idx1]);

    cprs_mean = 0.125 * (
        + aer_cprs[attk_idx1][mach_idx1][brak_idx1]
        + aer_cprs[attk_idx1][mach_idx1][brak_idx2]
        + aer_cprs[attk_idx1][mach_idx2][brak_idx1]
        + aer_cprs[attk_idx1][mach_idx2][brak_idx2]
        + aer_cprs[attk_idx2][mach_idx1][brak_idx1]
        + aer_cprs[attk_idx2][mach_idx1][brak_idx2]
        + aer_cprs[attk_idx2][mach_idx2][brak_idx1]
        + aer_cprs[attk_idx2][mach_idx2][brak_idx2]
    );

    cprs_grad_attk = 0.25 * (
        - aer_cprs[attk_idx1][mach_idx1][brak_idx1]
        - aer_cprs[attk_idx1][mach_idx1][brak_idx2]
        - aer_cprs[attk_idx1][mach_idx2][brak_idx1]
        - aer_cprs[attk_idx1][mach_idx2][brak_idx2]
        + aer_cprs[attk_idx2][mach_idx1][brak_idx1]
        + aer_cprs[attk_idx2][mach_idx1][brak_idx2]
        + aer_cprs[attk_idx2][mach_idx2][brak_idx1]
        + aer_cprs[attk_idx2][mach_idx2][brak_idx2]
    ) / (aer_attk[attk_idx2] - aer_attk[attk_idx1]);

    cprs_grad_mach = 0.25 * (
        - aer_cprs[attk_idx1][mach_idx1][brak_idx1]
        - aer_cprs[attk_idx1][mach_idx1][brak_idx2]
        + aer_cprs[attk_idx1][mach_idx2][brak_idx1]
        + aer_cprs[attk_idx1][mach_idx2][brak_idx2]
        - aer_cprs[attk_idx2][mach_idx1][brak_idx1]
        - aer_cprs[attk_idx2][mach_idx1][brak_idx2]
        + aer_cprs[attk_idx2][mach_idx2][brak_idx1]
        + aer_cprs[attk_idx2][mach_idx2][brak_idx2]
    ) / (aer_mach[mach_idx2] - aer_mach[mach_idx1]);

    cprs_grad_brak = 0.25 * (
        - aer_cprs[attk_idx1][mach_idx1][brak_idx1]
        + aer_cprs[attk_idx1][mach_idx1][brak_idx2]
        - aer_cprs[attk_idx1][mach_idx2][brak_idx1]
        + aer_cprs[attk_idx1][mach_idx2][brak_idx2]
        - aer_cprs[attk_idx2][mach_idx1][brak_idx1]
        + aer_cprs[attk_idx2][mach_idx1][brak_idx2]
        - aer_cprs[attk_idx2][mach_idx2][brak_idx1]
        + aer_cprs[attk_idx2][mach_idx2][brak_idx2]
    ) / (aer_brak[brak_idx2] - aer_brak[brak_idx1]);

    side = side_mean + side_grad_attk * attk_diff + side_grad_mach * mach_diff + side_grad_brak * brak_diff;
    drag = drag_mean + drag_grad_attk * attk_diff + drag_grad_mach * mach_diff + drag_grad_brak * brak_diff;
    cprs = cprs_mean + cprs_grad_attk * attk_diff + cprs_grad_mach * mach_diff + cprs_grad_brak * brak_diff;

    return {side, drag, cprs};
}

void atm_load (std::string path) {
    int count;
    std::vector<std::string> head;
    std::vector<double> body;
    std::vector<std::vector<double>> data;
    std::vector<bool> covered;
    double altd, pres, temp, dens;
    int altd_col, pres_col, temp_col, dens_col;
    int altd_idx;

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

    altd_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Altitude (m)"));
    pres_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Pressure (Pa)"));
    temp_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Temperature (K)"));
    dens_col = std::distance(head.begin(), std::find(head.begin(), head.end(), "Density (kg/m³)"));

    if (altd_col == head.size() || pres_col == head.size() || temp_col == head.size() || dens_col == head.size()) {
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

        altd = body[altd_col];
        pres = body[pres_col];
        temp = body[temp_col];
        dens = body[dens_col];

        data.push_back({altd, pres, temp, dens});
    }

    for (auto itr = data.begin(); itr < data.end(); itr++) {
        if (!std::binary_search(atm_altd.begin(), atm_altd.end(), (*itr)[0])) {
            atm_altd.insert(std::lower_bound(atm_altd.begin(), atm_altd.end(), (*itr)[0]), (*itr)[0]);
        }
    }

    if (atm_altd.size() < 2) {
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

    atm_pres = std::vector<double>(atm_altd.size());
    atm_temp = std::vector<double>(atm_altd.size());
    atm_dens = std::vector<double>(atm_altd.size());

    covered = std::vector<bool>(atm_altd.size(), false);

    for (auto itr = data.begin(); itr < data.end(); itr++) {
        altd_idx = std::distance(atm_altd.begin(), std::find(atm_altd.begin(), atm_altd.end(), (*itr)[0]));

        if (covered[altd_idx]) {
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

        atm_pres[altd_idx] = (*itr)[1];
        atm_temp[altd_idx] = (*itr)[2];
        atm_dens[altd_idx] = (*itr)[3];

        covered[altd_idx] = true;
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

std::tuple<double, double, double> atm_find (double altd) {
    double pres, temp, dens;
    int altd_idx1, altd_idx2;
    double altd_mean, altd_diff;
    double pres_mean, pres_grad_altd;
    double temp_mean, temp_grad_altd;
    double dens_mean, dens_grad_altd;

    if (altd < atm_altd.front()) {
        altd_idx1 = 0;
    } else if (altd >= atm_altd.back()) {
        altd_idx1 = atm_altd.size() - 2;
    } else {
        altd_idx1 = std::distance(atm_altd.begin(), std::upper_bound(atm_altd.begin(), atm_altd.end(), altd)) - 1;
    }
    altd_idx2 = altd_idx1 + 1;

    altd_mean = 0.5 * (atm_altd[altd_idx1] + atm_altd[altd_idx2]);
    altd_diff = altd - altd_mean;

    pres_mean = 0.5 * (atm_pres[altd_idx1] + atm_pres[altd_idx2]);
    pres_grad_altd = (atm_pres[altd_idx2] - atm_pres[altd_idx1]) / (atm_altd[altd_idx2] - atm_altd[altd_idx1]);

    temp_mean = 0.5 * (atm_temp[altd_idx1] + atm_temp[altd_idx2]);
    temp_grad_altd = (atm_temp[altd_idx2] - atm_temp[altd_idx1]) / (atm_altd[altd_idx2] - atm_altd[altd_idx1]);

    dens_mean = 0.5 * (atm_dens[altd_idx1] + atm_dens[altd_idx2]);
    dens_grad_altd = (atm_dens[altd_idx2] - atm_dens[altd_idx1]) / (atm_altd[altd_idx2] - atm_altd[altd_idx1]);

    pres = pres_mean + pres_grad_altd * altd_diff;
    temp = temp_mean + temp_grad_altd * altd_diff;
    dens = dens_mean + dens_grad_altd * altd_diff;

    return {pres, temp, dens};
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

linalg::fvector comp_derv (double ctrl, linalg::fvector stat) {
    linalg::fvector derv;
    double pos_ver, pos_hor, pos_ang, vel_ver, vel_hor, vel_ang, acc_ver, acc_hor, acc_ang, brak, turn;
    double attk, mach, side, drag, cprs, altd, pres, temp, dens;
    double vel_par, vel_per, vel_tot, vel_snd;
    double forc_wght, forc_side, forc_drag, forc_ver, forc_hor, torq;
    double cos_pos_ang, sin_pos_ang;

    pos_ver = stat[0];
    pos_hor = stat[1];
    pos_ang = stat[2];
    vel_ver = stat[3];
    vel_hor = stat[4];
    vel_ang = stat[5];
    brak = stat[6];

    if (vel_ver > 0) {
        altd = pos_ver;
        std::tie(pres, temp, dens) = atm_find(altd);

        cos_pos_ang = std::cos(pos_ang);
        sin_pos_ang = std::sin(pos_ang);

        vel_par = vel_ver * cos_pos_ang + vel_hor * sin_pos_ang;
        vel_per = vel_hor * cos_pos_ang - vel_ver * sin_pos_ang;
        vel_tot = std::sqrt(vel_ver * vel_ver + vel_hor * vel_hor);
        vel_snd = std::sqrt(env_gamma * env_gcnst * temp / env_molar);

        attk = std::atan2(vel_per, vel_par);
        mach = vel_tot / vel_snd;
        std::tie(side, drag, cprs) = aer_find(attk, mach, brak);

        forc_wght = vhc_mass * env_grvty;
        forc_side = 0.5 * dens * vel_tot * vel_tot * vhc_area * side * attk;
        forc_drag = 0.5 * dens * vel_tot * vel_tot * vhc_area * drag;
        forc_ver = forc_side * sin_pos_ang - forc_drag * cos_pos_ang - forc_wght;
        forc_hor = -(forc_side * cos_pos_ang + forc_drag * sin_pos_ang);
        torq = forc_side * (cprs - vhc_cmas);
    } else {
        forc_ver = -vhc_mass * env_grvty;
        forc_hor = 0;
        torq = 0;
    }

    acc_ver = forc_ver / vhc_mass;
    acc_hor = forc_hor / vhc_mass;
    acc_ang = torq / vhc_iner;

    if (ctrl > brak) {
        turn = act_turn;
    } else if (ctrl < brak) {
        turn = -act_turn;
    } else {
        turn = 0;
    }

    derv = {vel_ver, vel_hor, vel_ang, acc_ver, acc_hor, acc_ang, turn};

    return derv;
}

linalg::fvector comp_sens (double time, double ctrl, linalg::fvector stat, linalg::fvector sens) {
    static double ang_aln = randnum::unif(-M_PI, M_PI), ang_ins = randnum::unif(-M_PI, M_PI);
    linalg::fvector derv;
    double pos_ver, pos_hor, pos_ang, vel_ver, vel_hor, vel_ang, acc_ver, acc_hor, acc_ang, brak, turn;
    linalg::fvector rot, qua, lia;
    double pres, temp;
    double rot_time, qua_time, lia_time, pres_time, temp_time;
    double altd;
    linalg::fvector trn_eth_lch, trn_lch_aln, trn_aln_ins, trn_aln_fix, trn_eth_ins, trn_fix_ins;
    linalg::fvector nois_rot, nois_qua, nois_lia;
    double nois_pres, nois_temp;
    double nois_qua_ang;
    linalg::fvector nois_qua_dir;
    bool updt_rot, updt_qua, updt_lia, updt_pres, updt_temp;

    derv = comp_derv(ctrl, stat);

    pos_ver = stat[0];
    pos_hor = stat[1];
    pos_ang = stat[2];
    vel_ver = stat[3];
    vel_hor = stat[4];
    vel_ang = stat[5];
    acc_ver = derv[3];
    acc_hor = derv[4];
    acc_ang = derv[5];
    brak = stat[6];
    turn = derv[6];

    altd = pos_ver;

    trn_eth_lch = {0.5, -0.5, -0.5, -0.5};
    trn_lch_aln = {std::cos(0.5 * ang_aln), std::sin(0.5 * ang_aln), 0, 0};
    trn_aln_ins = {std::cos(0.5 * pos_ang), 0, std::sin(0.5 * pos_ang) * std::cos(ang_ins), std::sin(0.5 * pos_ang) * std::sin(ang_ins)};
    trn_aln_fix = {std::cos(0.5 * ang_ins - 0.25 * M_PI), std::sin(0.5 * ang_ins - 0.25 * M_PI), 0, 0};

    trn_eth_ins = qua_prod(qua_prod(trn_eth_lch, trn_lch_aln), trn_aln_ins);
    trn_eth_ins /= linalg::norm(trn_eth_ins);

    trn_fix_ins = qua_prod(qua_conj(trn_aln_fix), trn_aln_ins);
    trn_fix_ins /= linalg::norm(trn_fix_ins);

    if (linalg::is_null(sens)) {
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

        updt_rot = (time >= rot_time + 1.0 / imu_rot_rate);
        updt_qua = (time >= qua_time + 1.0 / imu_qua_rate);
        updt_lia = (time >= lia_time + 1.0 / imu_lia_rate);
        updt_pres = (time >= pres_time + 1.0 / bar_pres_rate);
        updt_temp = (time >= temp_time + 1.0 / bar_temp_rate);
    }

    if (updt_rot) {
        nois_rot = randnum::gauss({0, 0, 0}, imu_rot_var);

        rot = qua_tran({0, 0, vel_ang}, trn_fix_ins);
        rot += nois_rot;

        rot_time = time;
    }

    if (updt_qua) {
        nois_qua = randnum::gauss({0, 0, 0}, imu_qua_var);

        nois_qua_ang = linalg::norm(nois_qua);
        nois_qua_dir = nois_qua / linalg::norm(nois_qua);

        nois_qua = {0, nois_qua_dir[0], nois_qua_dir[1], nois_qua_dir[2]};
        nois_qua *= std::sin(0.5 * nois_qua_ang);
        nois_qua[0] = std::cos(0.5 * nois_qua_ang);

        qua = trn_eth_ins;
        qua = qua_prod(qua, nois_qua);
        qua /= linalg::norm(qua);

        qua_time = time;
    }

    if (updt_lia) {
        nois_lia = randnum::gauss({0, 0, 0}, imu_lia_var);

        lia = qua_tran({acc_ver, acc_hor, 0}, trn_fix_ins);
        lia += nois_lia;

        lia_time = time;
    }

    if (updt_pres) {
        nois_pres = randnum::gauss(0, bar_pres_var);

        std::tie(pres, std::ignore, std::ignore) = atm_find(altd);
        pres += nois_pres;

        pres_time = time;
    }

    if (updt_temp) {
        nois_temp = randnum::gauss(0, bar_temp_var);

        std::tie(std::ignore, temp, std::ignore) = atm_find(altd);
        temp += nois_temp;

        temp_time = time;
    }

    sens = {
        rot[0], rot[1], rot[2], qua[0], qua[1], qua[2], qua[3], lia[0], lia[1], lia[2], pres, temp,
        rot_time, qua_time, lia_time, pres_time, temp_time
    };

    return sens;
}

}

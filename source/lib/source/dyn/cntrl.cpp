#include <cmath>

#include <iterator>
#include <algorithm>
#include <string>
#include <vector>
#include <tuple>

#include <math/linalg.hpp>

#include <sys/logging.hpp>

#include <dat/csvread.hpp>

#include <dyn/cntrl.hpp>

namespace cntrl :: intern {

std::string mod = "cntrl";

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

double mpc_apog;
double mpc_enab;
double mpc_step;
double mpc_horz;
double mpc_tol;

void aer_load (std::string path);
std::tuple<double, double, double> aer_find (double attk, double mach, double brak);

void atm_load (std::string path);
std::tuple<double, double, double> atm_find (double altd);

linalg::fvector comp_stat (linalg::fvector estm);
linalg::fvector comp_derv (double ctrl, linalg::fvector stat);
linalg::fvector comp_pred (double ctrl, linalg::fvector stat);
linalg::fvector comp_term (double ctrl, linalg::fvector pred);
int comp_categ (double ctrl, linalg::fvector estm);

}

namespace cntrl {

bool fail (void) {
    return intern::fail;
}

void init (
    std::string aer_path, std::string atm_path,
    double env_grvty, double env_gcnst, double env_molar, double env_gamma,
    double vhc_mass, double vhc_iner, double vhc_cmas, double vhc_area,
    double mpc_apog, double mpc_enab, double mpc_step, double mpc_horz, double mpc_tol
) {
    logging::inf(intern::mod,
        "Initializing control system: ",
        "Aer path: ", aer_path, ", Atm path: ", atm_path, ", ",
        "Grvty: ", env_grvty, "m/s², Gas const: ", env_gcnst, "J/(mol K), Mol mass: ", env_molar, "kg/mol, Gamma: ", env_gamma, ", ",
        "Mass: ", vhc_mass, "kg, Iner: ", vhc_iner, "kg m², Cent mass: ", vhc_cmas, "m, Cross sect area: ", vhc_area, "m², ",
        "Apog altd: ", mpc_apog, "m, Enab altd: ", mpc_enab, "m, Time step: ", mpc_step, "s, Pred horz: ", mpc_horz, "s, Tol: ", mpc_tol
    );

    if (env_grvty <= 0) {
        logging::err(intern::mod,
            "Failed to initialize control system (Invalid gravitational acceleration)"
        );
        intern::fail = true;
        return;
    }

    if (env_gcnst <= 0) {
        logging::err(intern::mod,
            "Failed to initialize control system (Invalid ideal gas constant)"
        );
        intern::fail = true;
        return;
    }

    if (env_molar <= 0) {
        logging::err(intern::mod,
            "Failed to initialize control system (Invalid atmospheric molar mass)"
        );
        intern::fail = true;
        return;
    }

    if (env_gamma <= 0) {
        logging::err(intern::mod,
            "Failed to initialize control system (Invalid atmospheric gamma factor)"
        );
        intern::fail = true;
        return;
    }

    if (vhc_mass <= 0) {
        logging::err(intern::mod,
            "Failed to initialize control system (Invalid vehicular mass)"
        );
        intern::fail = true;
        return;
    }

    if (vhc_iner <= 0) {
        logging::err(intern::mod,
            "Failed to initialize control system (Invalid vehicular moment of inertia)"
        );
        intern::fail = true;
        return;
    }

    if (vhc_cmas) {
        logging::err(intern::mod,
            "Failed to initialize control system (Invalid vehicular center of mass)"
        );
        intern::fail = true;
        return;
    }

    if (vhc_area) {
        logging::err(intern::mod,
            "Failed to initialize control system (Invalid vehicular cross sectional area)"
        );
        intern::fail = true;
        return;
    }

    if (mpc_apog <= 0) {
        logging::err(intern::mod,
            "Failed to initialize control system (Invalid apogee altitude)"
        );
        intern::fail = true;
        return;
    }

    if (mpc_enab <= 0) {
        logging::err(intern::mod,
            "Failed to initialize control system (Invalid enable altitude)"
        );
        intern::fail = true;
        return;
    }

    if (mpc_step <= 0) {
        logging::err(intern::mod,
            "Failed to initialize control system (Invalid time step)"
        );
        intern::fail = true;
        return;
    }

    if (mpc_horz <= 0) {
        logging::err(intern::mod,
            "Failed to initialize control system (Invalid prediction horizon)"
        );
        intern::fail = true;
        return;
    }

    if (mpc_tol <= 0) {
        logging::err(intern::mod,
            "Failed to initialize control system (Invalid tolerance)"
        );
        intern::fail = true;
        return;
    }

    intern::aer_load(aer_path);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize control system"
        );
        intern::fail = true;
        return;
    }

    intern::atm_load(atm_path);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize control system"
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

    intern::mpc_apog = mpc_apog;
    intern::mpc_enab = mpc_enab;
    intern::mpc_step = mpc_step;
    intern::mpc_horz = mpc_horz;
    intern::mpc_tol = mpc_tol;

    intern::fail = false;
}

double get_ctrl (linalg::fvector estm) {
    double ctrl;
    double pos_ver, pos_ang, vel_ver, vel_ang;
    double ctrl_min = 0, ctrl_mid = 0.5, ctrl_max = 1;
    int categ_min, categ_mid, categ_max;

    logging::inf(intern::mod,
        "Getting control signal: Estm: ", estm
    );

    if (linalg::dim(estm) != 4) {
        logging::err(intern::mod,
            "Failed to get control signal (Invalid estimate vector)"
        );
        intern::fail = true;
        return ctrl;
    }

    pos_ver = estm[0];
    pos_ang = estm[1];
    vel_ver = estm[2];
    vel_ang = estm[3];

    if (pos_ang < 0 || pos_ang > M_PI) {
        logging::err(intern::mod,
            "Failed to get control signal (Invalid estimate vector)"
        );
        intern::fail = true;
        return ctrl;
    }

    if (pos_ver <= intern::mpc_enab) {
        ctrl = 0;
    } else if (vel_ver <= 0) {
        ctrl = 0;
    } else if (pos_ver >= intern::mpc_apog) {
        ctrl = 1;
    } else {
        while (ctrl_max - ctrl_min > intern::mpc_tol) {
            categ_min = intern::comp_categ(ctrl_min, estm);
            categ_mid = intern::comp_categ(ctrl_mid, estm);
            categ_max = intern::comp_categ(ctrl_max, estm);

            if (categ_min == 1 && categ_mid == 1 && categ_max == 1) {
                ctrl_min = ctrl_max;
            } else if (categ_min == 1 && categ_mid == 1 && categ_max == 0) {
                ctrl_min = ctrl_mid;
            } else if (categ_min == 1 && categ_mid == 1 && categ_max == -1) {
                ctrl_min = ctrl_mid;
            } else if (categ_min == 1 && categ_mid == 0 && categ_max == 0) {
                ctrl_max = ctrl_mid;
            } else if (categ_min == 1 && categ_mid == 0 && categ_max == -1) {
                ctrl_max = ctrl_mid;
            } else if (categ_min == 1 && categ_mid == -1 && categ_max == -1) {
                ctrl_max = ctrl_mid;
            } else if (categ_min == 0 && categ_mid == 0 && categ_max == 0) {
                ctrl_max = ctrl_min;
            } else if (categ_min == 0 && categ_mid == 0 && categ_max == -1) {
                ctrl_max = ctrl_min;
            } else if (categ_min == 0 && categ_mid == -1 && categ_max == -1) {
                ctrl_max = ctrl_min;
            } else if (categ_min == -1 && categ_mid == -1 && categ_max == -1) {
                ctrl_max = ctrl_min;
            } else {
                ctrl_min = 0;
                ctrl_max = 0;
            }
            ctrl_mid = 0.5 * (ctrl_min + ctrl_max);
        }
        ctrl = ctrl_mid;
    }

    logging::inf(intern::mod,
        "Got control signal: ", ctrl
    );

    intern::fail = false;
    return ctrl;
}

}

namespace cntrl :: intern {

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

linalg::fvector comp_stat (linalg::fvector estm) {
    linalg::fvector stat;
    double pos_ver, pos_hor, pos_ang, vel_ver, vel_hor, vel_ang;

    pos_ver = estm[0];
    pos_ang = estm[1];
    vel_ver = estm[2];
    vel_ang = estm[3];

    pos_hor = 0;
    vel_hor = vel_ver * std::tan(pos_ang);

    stat = {pos_ver, pos_hor, pos_ang, vel_ver, vel_hor, vel_ang};

    return stat;
}

linalg::fvector comp_derv (double ctrl, linalg::fvector stat) {
    linalg::fvector derv;
    double pos_ver, pos_hor, pos_ang, vel_ver, vel_hor, vel_ang, acc_ver, acc_hor, acc_ang;
    double attk, mach, brak, side, drag, cprs, altd, pres, temp, dens;
    double vel_par, vel_per, vel_tot, vel_snd;
    double forc_wght, forc_side, forc_drag, forc_ver, forc_hor, torq;
    double cos_pos_ang, sin_pos_ang;

    pos_ver = stat[0];
    pos_hor = stat[1];
    pos_ang = stat[2];
    vel_ver = stat[3];
    vel_hor = stat[4];
    vel_ang = stat[5];

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
    brak = ctrl;
    std::tie(side, drag, cprs) = aer_find(attk, mach, brak);

    forc_wght = vhc_mass * env_grvty;
    forc_side = 0.5 * dens * vel_tot * vel_tot * vhc_area * side * attk;
    forc_drag = 0.5 * dens * vel_tot * vel_tot * vhc_area * drag;
    forc_ver = forc_side * sin_pos_ang - forc_drag * cos_pos_ang - forc_wght;
    forc_hor = -(forc_side * cos_pos_ang + forc_drag * sin_pos_ang);
    torq = forc_side * (cprs - vhc_cmas);

    acc_ver = forc_ver / vhc_mass;
    acc_hor = forc_hor / vhc_mass;
    acc_ang = torq / vhc_iner;

    derv = {vel_ver, vel_hor, vel_ang, acc_ver, acc_hor, acc_ang};

    return derv;
}

linalg::fvector comp_pred (double ctrl, linalg::fvector stat) {
    linalg::fvector pred;
    double pos_ver, pos_hor, pos_ang, vel_ver, vel_hor, vel_ang;
    linalg::fvector runge_kutta_k1, runge_kutta_k2, runge_kutta_k3, runge_kutta_k4;
    int count = 0;

    pred = stat;

    do {
        runge_kutta_k1 = comp_derv(ctrl, pred);
        runge_kutta_k2 = comp_derv(ctrl, pred + runge_kutta_k1 * mpc_step / 2);
        runge_kutta_k3 = comp_derv(ctrl, pred + runge_kutta_k2 * mpc_step / 2);
        runge_kutta_k4 = comp_derv(ctrl, pred + runge_kutta_k3 * mpc_step);

        pred += (runge_kutta_k1 + 2 * runge_kutta_k2 + 2 * runge_kutta_k3 + runge_kutta_k4) * mpc_step / 6;

        pos_ver = pred[0];
        pos_hor = pred[1];
        pos_ang = pred[2];
        vel_ver = pred[3];
        vel_hor = pred[4];
        vel_ang = pred[5];

        while (pos_ang < -M_PI) {
            pos_ang += 2 * M_PI;
        }

        while (pos_ang > M_PI) {
            pos_ang -= 2 * M_PI;
        }

        pred = {pos_ver, pos_hor, pos_ang, vel_ver, vel_hor, vel_ang};

        count++;
    } while (mpc_horz > count * mpc_step);

    return pred;
}

linalg::fvector comp_term (double ctrl, linalg::fvector pred) {
    linalg::fvector term;
    double pos_ver, pos_hor, pos_ang, vel_ver, vel_hor, vel_ang;
    linalg::fvector runge_kutta_k1, runge_kutta_k2, runge_kutta_k3, runge_kutta_k4;

    term = pred;

    do {
        runge_kutta_k1 = comp_derv(ctrl, term);
        runge_kutta_k2 = comp_derv(ctrl, term + runge_kutta_k1 * mpc_step / 2);
        runge_kutta_k3 = comp_derv(ctrl, term + runge_kutta_k2 * mpc_step / 2);
        runge_kutta_k4 = comp_derv(ctrl, term + runge_kutta_k3 * mpc_step);

        term += (runge_kutta_k1 + 2 * runge_kutta_k2 + 2 * runge_kutta_k3 + runge_kutta_k4) * mpc_step / 6;

        pos_ver = term[0];
        pos_hor = term[1];
        pos_ang = term[2];
        vel_ver = term[3];
        vel_hor = term[4];
        vel_ang = term[5];

        while (pos_ang < -M_PI) {
            pos_ang += 2 * M_PI;
        }

        while (pos_ang > M_PI) {
            pos_ang -= 2 * M_PI;
        }

        term = {pos_ver, pos_hor, pos_ang, vel_ver, vel_hor, vel_ang};
    } while (vel_ver > 0);

    return term;
}

int comp_categ (double ctrl, linalg::fvector estm) {
    linalg::fvector stat, pred, term;
    double pos_ver, pos_hor, pos_ang, vel_ver, vel_hor, vel_ang;

    stat = comp_stat(estm);
    pred = comp_pred(ctrl, stat);

    term = comp_term(0, pred);

    pos_ver = term[0];
    pos_hor = term[1];
    pos_ang = term[2];
    vel_ver = term[3];
    vel_hor = term[4];
    vel_ang = term[5];

    if (pos_ver < mpc_apog) {
        return -1;
    }

    term = comp_term(1, pred);

    pos_ver = term[0];
    pos_hor = term[1];
    pos_ang = term[2];
    vel_ver = term[3];
    vel_hor = term[4];
    vel_ang = term[5];

    if (pos_ver > mpc_apog) {
        return 1;
    }

    return 0;
}

}

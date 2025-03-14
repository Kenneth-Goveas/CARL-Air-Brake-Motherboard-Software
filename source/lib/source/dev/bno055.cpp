#include <cstddef>
#include <cstdint>

#include <string>
#include <vector>

#include <math/linalg.hpp>

#include <sys/logging.hpp>

#include <dev/i2c.hpp>
#include <dev/bno055.hpp>

namespace bno055 :: intern {

std::string mod = "bno055";

bool fail;

std::vector<int> lev_sys(2);
std::vector<int> lev_acc(2);
std::vector<int> lev_mag(2);
std::vector<int> lev_gyr(2);

std::vector<int> rad_acc(2);
std::vector<int> rad_mag(2);

std::vector<linalg::ivector> off_acc(2, linalg::ivector(3));
std::vector<linalg::ivector> off_mag(2, linalg::ivector(3));
std::vector<linalg::ivector> off_gyr(2, linalg::ivector(3));

std::vector<bool> valid_lev_sys(2);
std::vector<bool> valid_lev_acc(2);
std::vector<bool> valid_lev_mag(2);
std::vector<bool> valid_lev_gyr(2);

std::vector<bool> valid_rad_acc(2);
std::vector<bool> valid_rad_mag(2);

std::vector<bool> valid_off_acc(2);
std::vector<bool> valid_off_mag(2);
std::vector<bool> valid_off_gyr(2);

std::vector<linalg::fvector> rot(2, linalg::fvector(3));
std::vector<linalg::fvector> qua(2, linalg::fvector(4));
std::vector<linalg::fvector> lia(2, linalg::fvector(3));

std::vector<bool> valid_rot(2);
std::vector<bool> valid_qua(2);
std::vector<bool> valid_lia(2);

std::vector<bool> sta_idl(2);
std::vector<bool> sta_err(2);
std::vector<bool> sta_perinit(2);
std::vector<bool> sta_sysinit(2);
std::vector<bool> sta_strun(2);
std::vector<bool> sta_fusrun(2);
std::vector<bool> sta_nfusrun(2);

std::vector<bool> err_perinit(2);
std::vector<bool> err_sysinit(2);
std::vector<bool> err_stfail(2);
std::vector<bool> err_valrng(2);
std::vector<bool> err_adrrng(2);
std::vector<bool> err_wrfail(2);
std::vector<bool> err_lowpwr(2);
std::vector<bool> err_accpwr(2);
std::vector<bool> err_fuscfg(2);
std::vector<bool> err_syscfg(2);

std::vector<bool> mode_cal(2);
std::vector<bool> mode_nrm(2);

void put_reg (int id, std::uint8_t reg, std::uint8_t dat);
std::vector<std::uint8_t> get_reg (int id, std::uint8_t reg, std::size_t len);

void put_trg_rst (int id);
void put_pwr_nrm (int id);
void put_opr_cfg (int id);
void put_opr_ndof (int id);
void put_unt_si (int id);
void put_axm (int id, axis axis_x, axis axis_y, axis axis_z);
void put_axs (int id, axis axis_x, axis axis_y, axis axis_z);
void put_caldat (int id, int rad_acc, int rad_mag, linalg::ivector off_acc, linalg::ivector off_mag, linalg::ivector off_gyr);

void get_sta (int id);
void get_err (int id);
void get_callev (int id);
void get_caldat (int id);
void get_datrot (int id);
void get_datqua (int id);
void get_datlia (int id);

}

namespace bno055 {

const axis axis_px((BNO055_PRST_SGN_P << 8) | BNO055_PRST_MAP_X, "+X");
const axis axis_nx((BNO055_PRST_SGN_N << 8) | BNO055_PRST_MAP_X, "-X");
const axis axis_py((BNO055_PRST_SGN_P << 8) | BNO055_PRST_MAP_Y, "+Y");
const axis axis_ny((BNO055_PRST_SGN_N << 8) | BNO055_PRST_MAP_Y, "-Y");
const axis axis_pz((BNO055_PRST_SGN_P << 8) | BNO055_PRST_MAP_Z, "+Z");
const axis axis_nz((BNO055_PRST_SGN_N << 8) | BNO055_PRST_MAP_Z, "-Z");

axis :: axis (int val, const char * text) {
    this->val = val;
    this->text = text;
}

axis :: operator int (void) const {
    return this->val;
}

axis :: operator logging::buffer (void) const {
    return logging::buffer(this->text);
}

bool fail (void) {
    return intern::fail;
}

void init (int id, axis axis_x, axis axis_y, axis axis_z) {
    logging::inf(intern::mod,
        "Initializing sensor #", id, ": X axis: ", axis_x, ", Y axis: ", axis_y, ", Z axis: ", axis_z
    );

    if (id != 1 && id != 2) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor does not exist)"
        );
        intern::fail = true;
        return;
    }

    intern::put_trg_rst(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    do {
        intern::get_sta(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id
            );
            intern::fail = true;
            return;
        }

        if (intern::sta_err[id - 1]) {
            intern::get_err(id);
            if (intern::fail) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id
                );
                intern::fail = true;
                return;
            }

            if (intern::err_perinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor peripheral initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_sysinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor system initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_stfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor self test error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_valrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor value range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_adrrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor address range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_wrfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor register write error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_lowpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor low power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_accpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor accelerometer power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_fuscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor fusion config error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_syscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor system config error)"
                );
                intern::fail = true;
                return;
            }

            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor unknown error)"
            );

            intern::fail = true;
            return;
        }
    } while (!intern::sta_idl[id - 1]);

    intern::put_pwr_nrm(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::get_sta(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    if (intern::sta_err[id - 1]) {
        intern::get_err(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id
            );
            intern::fail = true;
            return;
        }

        if (intern::err_perinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor fusion config error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system config error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor unknown error)"
        );

        intern::fail = true;
        return;
    }

    intern::put_opr_cfg(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::get_sta(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    if (intern::sta_err[id - 1]) {
        intern::get_err(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id
            );
            intern::fail = true;
            return;
        }

        if (intern::err_perinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor fusion config error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system config error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor unknown error)"
        );

        intern::fail = true;
        return;
    }

    intern::put_unt_si(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::get_sta(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    if (intern::sta_err[id - 1]) {
        intern::get_err(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id
            );
            intern::fail = true;
            return;
        }

        if (intern::err_perinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor fusion config error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system config error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor unknown error)"
        );

        intern::fail = true;
        return;
    }

    intern::put_axm(id, axis_x, axis_y, axis_z);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::get_sta(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    if (intern::sta_err[id - 1]) {
        intern::get_err(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id
            );
            intern::fail = true;
            return;
        }

        if (intern::err_perinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor fusion config error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system config error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor unknown error)"
        );

        intern::fail = true;
        return;
    }

    intern::put_axs(id, axis_x, axis_y, axis_z);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::get_sta(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    if (intern::sta_err[id - 1]) {
        intern::get_err(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id
            );
            intern::fail = true;
            return;
        }

        if (intern::err_perinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor fusion config error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system config error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor unknown error)"
        );

        intern::fail = true;
        return;
    }

    intern::put_opr_ndof(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::get_sta(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    if (intern::sta_err[id - 1]) {
        intern::get_err(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id
            );
            intern::fail = true;
            return;
        }

        if (intern::err_perinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor fusion config error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system config error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor unknown error)"
        );

        intern::fail = true;
        return;
    }

    intern::mode_cal[id - 1] = true;
    intern::mode_nrm[id - 1] = false;

    intern::fail = false;
}

void init (
    int id, axis axis_x, axis axis_y, axis axis_z,
    int rad_acc, int rad_mag, linalg::ivector off_acc, linalg::ivector off_mag, linalg::ivector off_gyr
) {
    logging::inf(intern::mod,
        "Initializing sensor #", id, ": ",
        "X axis: ", axis_x, ", Y axis: ", axis_y, ", Z axis: ", axis_z, ", ",
        "Acc rad: ", rad_acc, ", Mag rad: ", rad_mag, ", Acc off: ", off_acc, ", Mag off: ", off_mag, ", Gyr off: ", off_gyr
    );

    if (id != 1 && id != 2) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor does not exist)"
        );
        intern::fail = true;
        return;
    }

    if (linalg::dim(off_acc) != 3) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Incorrect accelerometer calibration offset dimension)"
        );
        intern::fail = true;
        return;
    }

    if (linalg::dim(off_mag) != 3) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Incorrect magnetometer calibration offset dimension)"
        );
        intern::fail = true;
        return;
    }

    if (linalg::dim(off_gyr) != 3) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Incorrect gyroscope calibration offset dimension)"
        );
        intern::fail = true;
        return;
    }

    intern::put_trg_rst(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    do {
        intern::get_sta(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id
            );
            intern::fail = true;
            return;
        }

        if (intern::sta_err[id - 1]) {
            intern::get_err(id);
            if (intern::fail) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id
                );
                intern::fail = true;
                return;
            }

            if (intern::err_perinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor peripheral initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_sysinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor system initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_stfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor self test error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_valrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor value range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_adrrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor address range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_wrfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor register write error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_lowpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor low power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_accpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor accelerometer power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_fuscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor fusion config error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_syscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " (Sensor system config error)"
                );
                intern::fail = true;
                return;
            }

            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor unknown error)"
            );

            intern::fail = true;
            return;
        }
    } while (!intern::sta_idl[id - 1]);

    intern::put_pwr_nrm(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::get_sta(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    if (intern::sta_err[id - 1]) {
        intern::get_err(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id
            );
            intern::fail = true;
            return;
        }

        if (intern::err_perinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor fusion config error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system config error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor unknown error)"
        );

        intern::fail = true;
        return;
    }

    intern::put_opr_cfg(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::get_sta(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    if (intern::sta_err[id - 1]) {
        intern::get_err(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id
            );
            intern::fail = true;
            return;
        }

        if (intern::err_perinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor fusion config error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system config error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor unknown error)"
        );

        intern::fail = true;
        return;
    }

    intern::put_unt_si(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::get_sta(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    if (intern::sta_err[id - 1]) {
        intern::get_err(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id
            );
            intern::fail = true;
            return;
        }

        if (intern::err_perinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor fusion config error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system config error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor unknown error)"
        );

        intern::fail = true;
        return;
    }

    intern::put_axm(id, axis_x, axis_y, axis_z);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::get_sta(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    if (intern::sta_err[id - 1]) {
        intern::get_err(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id
            );
            intern::fail = true;
            return;
        }

        if (intern::err_perinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor fusion config error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system config error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor unknown error)"
        );

        intern::fail = true;
        return;
    }

    intern::put_axs(id, axis_x, axis_y, axis_z);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::get_sta(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    if (intern::sta_err[id - 1]) {
        intern::get_err(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id
            );
            intern::fail = true;
            return;
        }

        if (intern::err_perinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor fusion config error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system config error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor unknown error)"
        );

        intern::fail = true;
        return;
    }

    intern::put_caldat(id, rad_acc, rad_mag, off_acc, off_mag, off_gyr);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::get_sta(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    if (intern::sta_err[id - 1]) {
        intern::get_err(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id
            );
            intern::fail = true;
            return;
        }

        if (intern::err_perinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor fusion config error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system config error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor unknown error)"
        );

        intern::fail = true;
        return;
    }

    intern::put_opr_ndof(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::get_sta(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    if (intern::sta_err[id - 1]) {
        intern::get_err(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id
            );
            intern::fail = true;
            return;
        }

        if (intern::err_perinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor fusion config error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor system config error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor unknown error)"
        );

        intern::fail = true;
        return;
    }

    intern::mode_cal[id - 1] = false;
    intern::mode_nrm[id - 1] = true;

    intern::fail = false;
}

int get_lev_sys (int id) {
    int lev_sys;

    logging::inf(intern::mod,
        "Getting sensor #", id, " system calibration level"
    );

    if (id != 1 && id != 2) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " system calibration level (Sensor does not exist)"
        );
        intern::fail = true;
        return lev_sys;
    }

    if (!intern::valid_lev_sys[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " system calibration level (Reading unavailable)"
        );
        intern::fail = true;
        return lev_sys;
    }

    lev_sys = intern::lev_sys[id - 1];

    logging::inf(intern::mod,
        "Got sensor #", id, " system calibration level: ", lev_sys
    );

    intern::fail = false;
    return lev_sys;
}

int get_lev_acc (int id) {
    int lev_acc;

    logging::inf(intern::mod,
        "Getting sensor #", id, " accelerometer calibration level"
    );

    if (id != 1 && id != 2) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " accelerometer calibration level (Sensor does not exist)"
        );
        intern::fail = true;
        return lev_acc;
    }

    if (!intern::valid_lev_acc[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " accelerometer calibration level (Reading unavailable)"
        );
        intern::fail = true;
        return lev_acc;
    }

    lev_acc = intern::lev_acc[id - 1];

    logging::inf(intern::mod,
        "Got sensor #", id, " accelerometer calibration level: ", lev_acc
    );

    intern::fail = false;
    return lev_acc;
}

int get_lev_mag (int id) {
    int lev_mag;

    logging::inf(intern::mod,
        "Getting sensor #", id, " magnetometer calibration level"
    );

    if (id != 1 && id != 2) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " magnetometer calibration level (Sensor does not exist)"
        );
        intern::fail = true;
        return lev_mag;
    }

    if (!intern::valid_lev_mag[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " magnetometer calibration level (Reading unavailable)"
        );
        intern::fail = true;
        return lev_mag;
    }

    lev_mag = intern::lev_mag[id - 1];

    logging::inf(intern::mod,
        "Got sensor #", id, " magnetometer calibration level: ", lev_mag
    );

    intern::fail = false;
    return lev_mag;
}

int get_lev_gyr (int id) {
    int lev_gyr;

    logging::inf(intern::mod,
        "Getting sensor #", id, " gyroscope calibration level"
    );

    if (id != 1 && id != 2) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " gyroscope calibration level (Sensor does not exist)"
        );
        intern::fail = true;
        return lev_gyr;
    }

    if (!intern::valid_lev_gyr[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " gyroscope calibration level (Reading unavailable)"
        );
        intern::fail = true;
        return lev_gyr;
    }

    lev_gyr = intern::lev_gyr[id - 1];

    logging::inf(intern::mod,
        "Got sensor #", id, " gyroscope calibration level: ", lev_gyr
    );

    intern::fail = false;
    return lev_gyr;
}

int get_rad_acc (int id) {
    int rad_acc;

    logging::inf(intern::mod,
        "Getting sensor #", id, " accelerometer calibration radius"
    );

    if (id != 1 && id != 2) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " accelerometer calibration radius (Sensor does not exist)"
        );
        intern::fail = true;
        return rad_acc;
    }

    if (!intern::valid_rad_acc[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " accelerometer calibration radius (Reading unavailable)"
        );
        intern::fail = true;
        return rad_acc;
    }

    rad_acc = intern::rad_acc[id - 1];

    logging::inf(intern::mod,
        "Got sensor #", id, " accelerometer calibration radius: ", rad_acc
    );

    intern::fail = false;
    return rad_acc;
}

int get_rad_mag (int id) {
    int rad_mag;

    logging::inf(intern::mod,
        "Getting sensor #", id, " magnetometer calibration radius"
    );

    if (id != 1 && id != 2) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " magnetometer calibration radius (Sensor does not exist)"
        );
        intern::fail = true;
        return rad_mag;
    }

    if (!intern::valid_rad_mag[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " magnetometer calibration radius (Reading unavailable)"
        );
        intern::fail = true;
        return rad_mag;
    }

    rad_mag = intern::rad_mag[id - 1];

    logging::inf(intern::mod,
        "Got sensor #", id, " magnetometer calibration radius: ", rad_mag
    );

    intern::fail = false;
    return rad_mag;
}

linalg::ivector get_off_acc (int id) {
    linalg::ivector off_acc;

    logging::inf(intern::mod,
        "Getting sensor #", id, " accelerometer calibration offset"
    );

    if (id != 1 && id != 2) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " accelerometer calibration offset (Sensor does not exist)"
        );
        intern::fail = true;
        return off_acc;
    }

    if (!intern::valid_off_acc[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " accelerometer calibration offset (Reading unavailable)"
        );
        intern::fail = true;
        return off_acc;
    }

    off_acc = intern::off_acc[id - 1];

    logging::inf(intern::mod,
        "Got sensor #", id, " accelerometer calibration offset: ", off_acc
    );

    intern::fail = false;
    return off_acc;
}

linalg::ivector get_off_mag (int id) {
    linalg::ivector off_mag;

    logging::inf(intern::mod,
        "Getting sensor #", id, " magnetometer calibration offset"
    );

    if (id != 1 && id != 2) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " magnetometer calibration offset (Sensor does not exist)"
        );
        intern::fail = true;
        return off_mag;
    }

    if (!intern::valid_off_mag[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " magnetometer calibration offset (Reading unavailable)"
        );
        intern::fail = true;
        return off_mag;
    }

    off_mag = intern::off_mag[id - 1];

    logging::inf(intern::mod,
        "Got sensor #", id, " magnetometer calibration offset: ", off_mag
    );

    intern::fail = false;
    return off_mag;
}

linalg::ivector get_off_gyr (int id) {
    linalg::ivector off_gyr;

    logging::inf(intern::mod,
        "Getting sensor #", id, " gyroscope calibration offset"
    );

    if (id != 1 && id != 2) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " gyroscope calibration offset (Sensor does not exist)"
        );
        intern::fail = true;
        return off_gyr;
    }

    if (!intern::valid_off_gyr[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " gyroscope calibration offset (Reading unavailable)"
        );
        intern::fail = true;
        return off_gyr;
    }

    off_gyr = intern::off_gyr[id - 1];

    logging::inf(intern::mod,
        "Got sensor #", id, " gyroscope calibration offset: ", off_gyr
    );

    intern::fail = false;
    return off_gyr;
}

linalg::fvector get_rot (int id) {
    linalg::fvector rot;

    logging::inf(intern::mod,
        "Getting sensor #", id, " angular velocity reading"
    );

    if (id != 1 && id != 2) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " angular velocity reading (Sensor does not exist)"
        );
        intern::fail = true;
        return rot;
    }

    if (!intern::valid_rot[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " angular velocity reading (Reading unavailable)"
        );
        intern::fail = true;
        return rot;
    }

    rot = intern::rot[id - 1];

    logging::inf(intern::mod,
        "Got sensor #", id, " angular velocity reading: ", rot, "rad/s"
    );

    intern::fail = false;
    return rot;
}

linalg::fvector get_qua (int id) {
    linalg::fvector qua;

    logging::inf(intern::mod,
        "Getting sensor #", id, " attitude quaternion reading"
    );

    if (id != 1 && id != 2) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " attitude quaternion reading (Sensor does not exist)"
        );
        intern::fail = true;
        return qua;
    }

    if (!intern::valid_qua[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " attitude quaternion reading (Reading unavailable)"
        );
        intern::fail = true;
        return qua;
    }

    qua = intern::qua[id - 1];

    logging::inf(intern::mod,
        "Got sensor #", id, " attitude quaternion reading: ", qua
    );

    intern::fail = false;
    return qua;
}

linalg::fvector get_lia (int id) {
    linalg::fvector lia;

    logging::inf(intern::mod,
        "Getting sensor #", id, " linear acceleration reading"
    );

    if (id != 1 && id != 2) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " linear acceleration reading (Sensor does not exist)"
        );
        intern::fail = true;
        return lia;
    }

    if (!intern::valid_lia[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " linear acceleration reading (Reading unavailable)"
        );
        intern::fail = true;
        return lia;
    }

    lia = intern::lia[id - 1];

    logging::inf(intern::mod,
        "Got sensor #", id, " linear acceleration reading: ", lia, "m/s²"
    );

    intern::fail = false;
    return lia;
}

void update (int id) {
    logging::inf(intern::mod,
        "Updating sensor #", id, " readings"
    );

    if (id != 1 && id != 2) {
        logging::err(intern::mod,
            "Failed to update sensor #", id, " readings (Sensor does not exist)"
        );
        intern::fail = true;
        return;
    }

    if (intern::mode_cal[id - 1]) {
        intern::valid_lev_sys[id - 1] = false;
        intern::valid_lev_acc[id - 1] = false;
        intern::valid_lev_mag[id - 1] = false;
        intern::valid_lev_gyr[id - 1] = false;

        intern::valid_rad_acc[id - 1] = false;
        intern::valid_rad_mag[id - 1] = false;

        intern::valid_off_acc[id - 1] = false;
        intern::valid_off_mag[id - 1] = false;
        intern::valid_off_gyr[id - 1] = false;

        intern::get_sta(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings"
            );
            intern::fail = true;
            return;
        }

        if (intern::sta_err[id - 1]) {
            intern::get_err(id);
            if (intern::fail) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_perinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor peripheral initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_sysinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor system initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_stfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor self test error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_valrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor value range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_adrrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor address range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_wrfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor register write error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_lowpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor low power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_accpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor accelerometer power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_fuscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor fusion config error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_syscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor system config error)"
                );
                intern::fail = true;
                return;
            }

            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings (Sensor unknown error)"
            );

            intern::fail = true;
            return;
        }

        intern::get_callev(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings"
            );
            intern::fail = true;
            return;
        }

        intern::get_sta(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings"
            );
            intern::fail = true;
            return;
        }

        if (intern::sta_err[id - 1]) {
            intern::get_err(id);
            if (intern::fail) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_perinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor peripheral initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_sysinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor system initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_stfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor self test error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_valrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor value range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_adrrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor address range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_wrfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor register write error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_lowpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor low power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_accpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor accelerometer power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_fuscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor fusion config error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_syscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor system config error)"
                );
                intern::fail = true;
                return;
            }

            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings (Sensor unknown error)"
            );

            intern::fail = true;
            return;
        }

        intern::get_caldat(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings"
            );
            intern::fail = true;
            return;
        }

        intern::valid_lev_sys[id - 1] = true;
        intern::valid_lev_acc[id - 1] = true;
        intern::valid_lev_mag[id - 1] = true;
        intern::valid_lev_gyr[id - 1] = true;

        intern::valid_rad_acc[id - 1] = true;
        intern::valid_rad_mag[id - 1] = true;

        intern::valid_off_acc[id - 1] = true;
        intern::valid_off_mag[id - 1] = true;
        intern::valid_off_gyr[id - 1] = true;
    }

    if (intern::mode_nrm[id - 1]) {
        intern::valid_rot[id - 1] = false;
        intern::valid_qua[id - 1] = false;
        intern::valid_lia[id - 1] = false;

        intern::get_sta(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings"
            );
            intern::fail = true;
            return;
        }

        if (intern::sta_err[id - 1]) {
            intern::get_err(id);
            if (intern::fail) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_perinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor peripheral initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_sysinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor system initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_stfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor self test error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_valrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor value range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_adrrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor address range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_wrfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor register write error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_lowpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor low power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_accpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor accelerometer power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_fuscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor fusion config error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_syscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor system config error)"
                );
                intern::fail = true;
                return;
            }

            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings (Sensor unknown error)"
            );

            intern::fail = true;
            return;
        }

        intern::get_datrot(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings"
            );
            intern::fail = true;
            return;
        }

        intern::get_sta(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings"
            );
            intern::fail = true;
            return;
        }

        if (intern::sta_err[id - 1]) {
            intern::get_err(id);
            if (intern::fail) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_perinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor peripheral initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_sysinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor system initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_stfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor self test error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_valrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor value range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_adrrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor address range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_wrfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor register write error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_lowpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor low power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_accpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor accelerometer power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_fuscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor fusion config error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_syscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor system config error)"
                );
                intern::fail = true;
                return;
            }

            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings (Sensor unknown error)"
            );

            intern::fail = true;
            return;
        }

        intern::get_datqua(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings"
            );
            intern::fail = true;
            return;
        }

        intern::get_sta(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings"
            );
            intern::fail = true;
            return;
        }

        if (intern::sta_err[id - 1]) {
            intern::get_err(id);
            if (intern::fail) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_perinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor peripheral initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_sysinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor system initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_stfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor self test error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_valrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor value range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_adrrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor address range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_wrfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor register write error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_lowpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor low power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_accpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor accelerometer power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_fuscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor fusion config error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_syscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings (Sensor system config error)"
                );
                intern::fail = true;
                return;
            }

            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings (Sensor unknown error)"
            );

            intern::fail = true;
            return;
        }

        intern::get_datlia(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings"
            );
            intern::fail = true;
            return;
        }

        intern::valid_rot[id - 1] = true;
        intern::valid_qua[id - 1] = true;
        intern::valid_lia[id - 1] = true;
    }

    intern::fail = false;
}

}

namespace bno055 :: intern {

void put_reg (int id, std::uint8_t reg, std::uint8_t dat) {
    std::uint8_t addr;
    std::vector<std::uint8_t> cmd = {reg, dat};

    logging::inf(mod,
        "Writing to sensor #", id, ": Reg: ", reg, ", Data: ", dat
    );

    switch (id) {
        case 1:
            addr = BNO055_ADDR1;
            break;
        case 2:
            addr = BNO055_ADDR2;
            break;
    }

    i2c::send(addr, cmd);
    i2c::exec();
    if (i2c::fail()) {
        logging::err(mod,
            "Failed to write to sensor #", id
        );
        fail = true;
        return;
    }

    fail = false;
}

std::vector<std::uint8_t> get_reg (int id, std::uint8_t reg, std::size_t len) {
    std::uint8_t addr;
    std::vector<std::uint8_t> cmd = {reg}, dat(len);

    logging::inf(mod,
        "Reading from sensor #", id, ": Reg: ", reg, ", Len: ", int(len)
    );

    switch (id) {
        case 1:
            addr = BNO055_ADDR1;
            break;
        case 2:
            addr = BNO055_ADDR2;
            break;
    }

    i2c::send(addr, cmd);
    i2c::recv(addr, dat);
    i2c::exec();
    if (i2c::fail()) {
        logging::err(mod,
            "Failed to read from sensor #", id
        );
        fail = true;
        return dat;
    }

    logging::inf(mod,
        "Read from sensor #", id, ": Data: ", dat
    );

    fail = false;
    return dat;
}

void put_trg_rst (int id) {
    std::uint8_t trg = 0x00;

    logging::inf(mod,
        "Resetting sensor #", id
    );

    trg = BNO055_PUT_BITS(trg, BNO055_BIT_RST, 1);

    put_reg(id, BNO055_REG_TRG, trg);
    if (fail) {
        logging::err(mod,
            "Failed to reset sensor #", id
        );
        fail = true;
        return;
    }

    fail = false;
}

void put_pwr_nrm (int id) {
    std::uint8_t pwr = 0x00;

    logging::inf(mod,
        "Setting sensor #", id, " power mode: Normal"
    );

    pwr = BNO055_PUT_BITS(pwr, BNO055_BIT_PWR, BNO055_PRST_PWR_NRM);

    put_reg(id, BNO055_REG_PWR, pwr);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " power mode"
        );
        fail = true;
        return;
    }

    fail = false;
}

void put_opr_cfg (int id) {
    std::uint8_t opr = 0x00;

    logging::inf(mod,
        "Setting sensor #", id, " operating mode: Config"
    );

    opr = BNO055_PUT_BITS(opr, BNO055_BIT_OPR, BNO055_PRST_OPR_CFG);

    put_reg(id, BNO055_REG_OPR, opr);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " operating mode"
        );
        fail = true;
        return;
    }

    fail = false;
}

void put_opr_ndof (int id) {
    std::uint8_t opr = 0x00;

    logging::inf(mod,
        "Setting sensor #", id, " operating mode: NDOF"
    );

    opr = BNO055_PUT_BITS(opr, BNO055_BIT_OPR, BNO055_PRST_OPR_NDOF);

    put_reg(id, BNO055_REG_OPR, opr);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " operating mode"
        );
        fail = true;
        return;
    }

    fail = false;
}

void put_unt_si (int id) {
    std::uint8_t unt = 0x00;

    logging::inf(mod,
        "Setting sensor #", id, " measurement units: Ori: Android, Acc unt: m/s², Gyr unt: rad/s"
    );

    unt = BNO055_PUT_BITS(unt, BNO055_BIT_ORI, BNO055_PRST_ORI_ANDR);
    unt = BNO055_PUT_BITS(unt, BNO055_BIT_UNTACC, BNO055_PRST_UNTACC_MS2);
    unt = BNO055_PUT_BITS(unt, BNO055_BIT_UNTGYR, BNO055_PRST_UNTGYR_RPS);

    put_reg(id, BNO055_REG_UNT, unt);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " measurement units"
        );
        fail = true;
        return;
    }

    fail = false;
}

void put_axm (int id, axis axis_x, axis axis_y, axis axis_z) {
    std::uint8_t axm = 0x00;

    logging::inf(mod,
        "Setting sensor #", id, " axis map"
    );

    axm = BNO055_PUT_BITS(axm, BNO055_BIT_MAPX, int(axis_x) & 0xFF);
    axm = BNO055_PUT_BITS(axm, BNO055_BIT_MAPY, int(axis_y) & 0xFF);
    axm = BNO055_PUT_BITS(axm, BNO055_BIT_MAPZ, int(axis_z) & 0xFF);

    put_reg(id, BNO055_REG_AXM, axm);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " axis map"
        );
        fail = true;
        return;
    }

    fail = false;
}

void put_axs (int id, axis axis_x, axis axis_y, axis axis_z) {
    std::uint8_t axs = 0x00;

    logging::inf(mod,
        "Setting sensor #", id, " axis signs"
    );

    axs = BNO055_PUT_BITS(axs, BNO055_BIT_SGNX, int(axis_x) >> 8);
    axs = BNO055_PUT_BITS(axs, BNO055_BIT_SGNY, int(axis_y) >> 8);
    axs = BNO055_PUT_BITS(axs, BNO055_BIT_SGNZ, int(axis_z) >> 8);

    put_reg(id, BNO055_REG_AXS, axs);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " axis signs"
        );
        fail = true;
        return;
    }

    fail = false;
}

void put_caldat (int id, int rad_acc, int rad_mag, linalg::ivector off_acc, linalg::ivector off_mag, linalg::ivector off_gyr) {
    logging::inf(mod,
        "Setting sensor #", id, " calibration data"
    );

    put_reg(id, BNO055_REG_RADACCL, std::int16_t(rad_acc) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_RADACCM, std::int16_t(rad_acc) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_RADMAGL, std::int16_t(rad_mag) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_RADMAGM, std::int16_t(rad_mag) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFACCXL, std::int16_t(off_acc[0]) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFACCXM, std::int16_t(off_acc[0]) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFACCYL, std::int16_t(off_acc[1]) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFACCYM, std::int16_t(off_acc[1]) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFACCZL, std::int16_t(off_acc[2]) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFACCZM, std::int16_t(off_acc[2]) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFMAGXL, std::int16_t(off_mag[0]) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFMAGXM, std::int16_t(off_mag[0]) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFMAGYL, std::int16_t(off_mag[1]) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFMAGYM, std::int16_t(off_mag[1]) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFMAGZL, std::int16_t(off_mag[2]) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFMAGZM, std::int16_t(off_mag[2]) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFGYRXL, std::int16_t(off_gyr[0]) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFGYRXM, std::int16_t(off_gyr[0]) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFGYRYL, std::int16_t(off_gyr[1]) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFGYRYM, std::int16_t(off_gyr[1]) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFGYRZL, std::int16_t(off_gyr[2]) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFGYRZM, std::int16_t(off_gyr[2]) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    fail = false;
}

void get_sta (int id) {
    std::vector<std::uint8_t> sta;

    logging::inf(mod,
        "Getting sensor #", id, " status flags"
    );

    sta_idl[id - 1] = false;
    sta_err[id - 1] = false;
    sta_perinit[id - 1] = false;
    sta_sysinit[id - 1] = false;
    sta_strun[id - 1] = false;
    sta_fusrun[id - 1] = false;
    sta_nfusrun[id - 1] = false;

    sta = get_reg(id, BNO055_REG_STA, 1);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", id, " status flags"
        );
        fail = true;
        return;
    }

    switch (sta[0]) {
        case BNO055_PRST_STA_IDL:
            sta_idl[id - 1] = true;
            break;
        case BNO055_PRST_STA_ERR:
            sta_err[id - 1] = true;
            break;
        case BNO055_PRST_STA_PERINIT:
            sta_perinit[id - 1] = true;
            break;
        case BNO055_PRST_STA_SYSINIT:
            sta_sysinit[id - 1] = true;
            break;
        case BNO055_PRST_STA_STRUN:
            sta_strun[id - 1] = true;
            break;
        case BNO055_PRST_STA_FUSRUN:
            sta_fusrun[id - 1] = true;
            break;
        case BNO055_PRST_STA_NFUSRUN:
            sta_nfusrun[id - 1] = true;
            break;
    }

    logging::inf(mod,
        "Got sensor #", id, " status flags: ",
        "Idle: ", bool(sta_idl[id - 1]), ", ",
        "Err: ", bool(sta_err[id - 1]), ", ",
        "Per init: ", bool(sta_perinit[id - 1]), ", ",
        "Sys init: ", bool(sta_sysinit[id - 1]), ", ",
        "Self test: ", bool(sta_strun[id - 1]), ", ",
        "Fus mode: ", bool(sta_fusrun[id - 1]), ", ",
        "Nfus mode: ", bool(sta_nfusrun[id - 1])
    );

    fail = false;
}

void get_err (int id) {
    std::vector<std::uint8_t> err;

    logging::inf(mod,
        "Getting sensor #", id, " error flags"
    );

    err_perinit[id - 1] = false;
    err_sysinit[id - 1] = false;
    err_stfail[id - 1] = false;
    err_valrng[id - 1] = false;
    err_adrrng[id - 1] = false;
    err_wrfail[id - 1] = false;
    err_lowpwr[id - 1] = false;
    err_accpwr[id - 1] = false;
    err_fuscfg[id - 1] = false;
    err_syscfg[id - 1] = false;

    err = get_reg(id, BNO055_REG_ERR, 1);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", id, " error flags"
        );
        fail = true;
        return;
    }

    switch (err[0]) {
        case BNO055_PRST_ERR_PERINIT:
            err_perinit[id - 1] = true;
            break;
        case BNO055_PRST_ERR_SYSINIT:
            err_sysinit[id - 1] = true;
            break;
        case BNO055_PRST_ERR_STFAIL:
            err_stfail[id - 1] = true;
            break;
        case BNO055_PRST_ERR_VALRNG:
            err_valrng[id - 1] = true;
            break;
        case BNO055_PRST_ERR_ADRRNG:
            err_adrrng[id - 1] = true;
            break;
        case BNO055_PRST_ERR_WRFAIL:
            err_wrfail[id - 1] = true;
            break;
        case BNO055_PRST_ERR_LOWPWR:
            err_lowpwr[id - 1] = true;
            break;
        case BNO055_PRST_ERR_ACCPWR:
            err_accpwr[id - 1] = true;
            break;
        case BNO055_PRST_ERR_FUSCFG:
            err_fuscfg[id - 1] = true;
            break;
        case BNO055_PRST_ERR_SYSCFG:
            err_syscfg[id - 1] = true;
            break;
    }

    logging::inf(mod,
        "Got sensor #", id, " error flags: ",
        "Per init err: ", bool(err_perinit[id - 1]), ", ",
        "Sys init err: ", bool(err_sysinit[id - 1]), ", ",
        "Self test err: ", bool(err_stfail[id - 1]), ", ",
        "Val rng err: ", bool(err_valrng[id - 1]), ", ",
        "Adr rng err: ", bool(err_adrrng[id - 1]), ", ",
        "Reg write err: ", bool(err_wrfail[id - 1]), ", ",
        "Low pwr err: ", bool(err_lowpwr[id - 1]), ", ",
        "Acc pwr err: ", bool(err_accpwr[id - 1]), ", ",
        "Fus cfg err: ", bool(err_fuscfg[id - 1]), ", ",
        "Sys cfg err: ", bool(err_syscfg[id - 1])
    );

    fail = false;
}

void get_callev (int id) {
    std::vector<std::uint8_t> callev;

    logging::inf(mod,
        "Getting sensor #", id, " calibration levels"
    );

    callev = get_reg(id, BNO055_REG_CALLEV, 1);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", id, " calibration levels"
        );
        fail = true;
        return;
    }

    lev_sys[id - 1] = BNO055_GET_BITS(callev[0], BNO055_BIT_LEVSYS);
    lev_acc[id - 1] = BNO055_GET_BITS(callev[0], BNO055_BIT_LEVACC);
    lev_mag[id - 1] = BNO055_GET_BITS(callev[0], BNO055_BIT_LEVMAG);
    lev_gyr[id - 1] = BNO055_GET_BITS(callev[0], BNO055_BIT_LEVGYR);

    logging::inf(mod,
        "Got sensor #", id, " calibration levels: ",
        "Sys lev: ", lev_sys[id - 1], ", ",
        "Acc lev: ", lev_acc[id - 1], ", ",
        "Mag lev: ", lev_mag[id - 1], ", ",
        "Gyr lev: ", lev_gyr[id - 1]
    );

    fail = false;
}

void get_caldat (int id) {
    std::vector<std::uint8_t> caldat;

    logging::inf(mod,
        "Getting sensor #", id, " calibration data"
    );

    caldat = get_reg(id, BNO055_REG_CALDAT, 22);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    rad_acc[id - 1] = std::int16_t(caldat[18] | (caldat[19] << 8));
    rad_mag[id - 1] = std::int16_t(caldat[20] | (caldat[21] << 8));

    off_acc[id - 1][0] = std::int16_t(caldat[0] | (caldat[1] << 8));
    off_acc[id - 1][1] = std::int16_t(caldat[2] | (caldat[3] << 8));
    off_acc[id - 1][2] = std::int16_t(caldat[4] | (caldat[5] << 8));

    off_mag[id - 1][0] = std::int16_t(caldat[6] | (caldat[7] << 8));
    off_mag[id - 1][1] = std::int16_t(caldat[8] | (caldat[9] << 8));
    off_mag[id - 1][2] = std::int16_t(caldat[10] | (caldat[11] << 8));

    off_gyr[id - 1][0] = std::int16_t(caldat[12] | (caldat[13] << 8));
    off_gyr[id - 1][1] = std::int16_t(caldat[14] | (caldat[15] << 8));
    off_gyr[id - 1][2] = std::int16_t(caldat[16] | (caldat[17] << 8));

    logging::inf(mod,
        "Got sensor #", id, " calibration data: ",
        "Acc rad: ", rad_acc[id - 1], ", ",
        "Mag rad: ", rad_mag[id - 1], ", ",
        "Acc off: ", off_acc[id - 1], ", ",
        "Mag off: ", off_mag[id - 1], ", ",
        "Gyr off: ", off_gyr[id - 1]
    );

    fail = false;
}

void get_datrot (int id) {
    std::vector<std::uint8_t> datrot;

    logging::inf(mod,
        "Getting sensor #", id, " angular velocity measurement data"
    );

    datrot = get_reg(id, BNO055_REG_DATROT, 6);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", id, " angular velocity measurement data"
        );
        fail = true;
        return;
    }

    rot[id - 1][0] = std::int16_t(datrot[0] | (datrot[1] << 8));
    rot[id - 1][1] = std::int16_t(datrot[2] | (datrot[3] << 8));
    rot[id - 1][2] = std::int16_t(datrot[4] | (datrot[5] << 8));

    rot[id - 1] /= 900;

    logging::inf(mod,
        "Got sensor #", id, " angular velocity measurement data: ", rot[id - 1], "rad/s"
    );

    fail = false;
}

void get_datqua (int id) {
    std::vector<std::uint8_t> datqua;

    logging::inf(mod,
        "Getting sensor #", id, " attitude quaternion measurement data"
    );

    datqua = get_reg(id, BNO055_REG_DATQUA, 8);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", id, " attitude quaternion measurement data"
        );
        fail = true;
        return;
    }

    qua[id - 1][0] = std::int16_t(datqua[0] | (datqua[1] << 8));
    qua[id - 1][1] = std::int16_t(datqua[2] | (datqua[3] << 8));
    qua[id - 1][2] = std::int16_t(datqua[4] | (datqua[5] << 8));
    qua[id - 1][3] = std::int16_t(datqua[6] | (datqua[7] << 8));

    qua[id - 1] /= 16384;

    logging::inf(mod,
        "Got sensor #", id, " attitude quaternion measurement data: ", qua[id - 1]
    );

    fail = false;
}

void get_datlia (int id) {
    std::vector<std::uint8_t> datlia;

    logging::inf(mod,
        "Getting sensor #", id, " linear acceleration measurement data"
    );

    datlia = get_reg(id, BNO055_REG_DATLIA, 6);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", id, " linear acceleration measurement data"
        );
        fail = true;
        return;
    }

    lia[id - 1][0] = std::int16_t(datlia[0] | (datlia[1] << 8));
    lia[id - 1][1] = std::int16_t(datlia[2] | (datlia[3] << 8));
    lia[id - 1][2] = std::int16_t(datlia[4] | (datlia[5] << 8));

    lia[id - 1] /= 100;

    logging::inf(mod,
        "Got sensor #", id, " linear acceleration measurement data: ", lia[id - 1], "m/s²"
    );

    fail = false;
}

}

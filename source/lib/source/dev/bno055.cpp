#include <cstddef>
#include <cstdint>

#include <iomanip>
#include <iostream>
#include <sstream>
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

std::string conv_val_to_oupt (bool val);
std::string conv_val_to_oupt (int val);
std::string conv_val_to_oupt (linalg::ivector val);
std::string conv_val_to_oupt (linalg::fvector val);
std::string conv_hex_to_oupt (std::uint8_t hex);
std::string conv_hex_to_oupt (std::vector<std::uint8_t> hex);
std::string conv_axis_to_oupt (axis axis);

void put_reg (int id, std::uint8_t reg, std::uint8_t dat);
std::vector<std::uint8_t> get_reg (int id, std::uint8_t reg, std::size_t len);

void put_trg_rst (int id);
void put_pwr_nrm (int id);
void put_opr_cfg (int id);
void put_opr_ndof (int id);
void put_unt_si (int id);
void put_axm (int id, axis axis_x, axis axis_y, axis axis_z);
void put_axs (int id, axis axis_x, axis axis_y, axis axis_z);
void put_caldat (
    int id, int rad_acc, int rad_mag,
    linalg::ivector off_acc, linalg::ivector off_mag, linalg::ivector off_gyr
);

void get_sta (int id);
void get_err (int id);
void get_callev (int id);
void get_caldat (int id);
void get_datrot (int id);
void get_datqua (int id);
void get_datlia (int id);

}

namespace bno055 {

bool fail (void) {
    return intern::fail;
}

void init (int id, axis axis_x, axis axis_y, axis axis_z) {
    std::string oupt_axis_x, oupt_axis_y, oupt_axis_z;

    oupt_axis_x = intern::conv_axis_to_oupt(axis_x);
    oupt_axis_y = intern::conv_axis_to_oupt(axis_y);
    oupt_axis_z = intern::conv_axis_to_oupt(axis_z);
    logging::inf(intern::mod,
        "Initializing sensor #", id, ": ",
        "X axis: ", oupt_axis_x, ", ",
        "Y axis: ", oupt_axis_y, ", ",
        "Z axis: ", oupt_axis_z
    );

    if (!(id == 1 || id == 2)) {
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
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor peripheral initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_sysinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor system initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_stfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor self test error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_valrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor register value range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_adrrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor register address range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_wrfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor register write error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_lowpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor low power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_accpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor accelerometer power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_fuscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor fusion algorithm configuration error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_syscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor system configuration error)"
                );
                intern::fail = true;
                return;
            }

            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor unknown error)"
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
                "Failed to initialize sensor #", id, " ",
                "(Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor fusion algorithm configuration error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system configuration error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " ",
            "(Sensor unknown error)"
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
                "Failed to initialize sensor #", id, " ",
                "(Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor fusion algorithm configuration error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system configuration error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " ",
            "(Sensor unknown error)"
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
                "Failed to initialize sensor #", id, " ",
                "(Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor fusion algorithm configuration error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system configuration error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " ",
            "(Sensor unknown error)"
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
                "Failed to initialize sensor #", id, " ",
                "(Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor fusion algorithm configuration error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system configuration error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " ",
            "(Sensor unknown error)"
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
                "Failed to initialize sensor #", id, " ",
                "(Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor fusion algorithm configuration error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system configuration error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " ",
            "(Sensor unknown error)"
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
                "Failed to initialize sensor #", id, " ",
                "(Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor fusion algorithm configuration error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system configuration error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " ",
            "(Sensor unknown error)"
        );

        intern::fail = true;
        return;
    }

    intern::mode_cal[id - 1] = true;
    intern::mode_nrm[id - 1] = false;

    intern::fail = false;
}

void init (
    int id, axis axis_x, axis axis_y, axis axis_z, int rad_acc, int rad_mag,
    linalg::ivector off_acc, linalg::ivector off_mag, linalg::ivector off_gyr
) {
    std::string oupt_axis_x, oupt_axis_y, oupt_axis_z;
    std::string oupt_rad_acc, oupt_rad_mag;
    std::string oupt_off_acc, oupt_off_mag, oupt_off_gyr;

    oupt_axis_x = intern::conv_axis_to_oupt(axis_x);
    oupt_axis_y = intern::conv_axis_to_oupt(axis_y);
    oupt_axis_z = intern::conv_axis_to_oupt(axis_z);
    oupt_rad_acc = intern::conv_val_to_oupt(rad_acc);
    oupt_rad_mag = intern::conv_val_to_oupt(rad_mag);
    oupt_off_acc = intern::conv_val_to_oupt(off_acc);
    oupt_off_mag = intern::conv_val_to_oupt(off_mag);
    oupt_off_gyr = intern::conv_val_to_oupt(off_gyr);
    logging::inf(intern::mod,
        "Initializing sensor #", id, ": ",
        "X axis: ", oupt_axis_x, ", ",
        "Y axis: ", oupt_axis_y, ", ",
        "Z axis: ", oupt_axis_z, ", ",
        "Acc rad: ", oupt_rad_acc, ", ",
        "Mag rad: ", oupt_rad_mag, ", ",
        "Acc off: ", oupt_off_acc, ", ",
        "Mag off: ", oupt_off_mag, ", ",
        "Gyr off: ", oupt_off_gyr
    );

    if (!(id == 1 || id == 2)) {
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
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor peripheral initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_sysinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor system initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_stfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor self test error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_valrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor register value range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_adrrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor register address range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_wrfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor register write error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_lowpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor low power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_accpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor accelerometer power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_fuscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor fusion algorithm configuration error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_syscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to initialize sensor #", id, " ",
                    "(Sensor system configuration error)"
                );
                intern::fail = true;
                return;
            }

            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor unknown error)"
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
                "Failed to initialize sensor #", id, " ",
                "(Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor fusion algorithm configuration error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system configuration error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " ",
            "(Sensor unknown error)"
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
                "Failed to initialize sensor #", id, " ",
                "(Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor fusion algorithm configuration error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system configuration error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " ",
            "(Sensor unknown error)"
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
                "Failed to initialize sensor #", id, " ",
                "(Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor fusion algorithm configuration error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system configuration error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " ",
            "(Sensor unknown error)"
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
                "Failed to initialize sensor #", id, " ",
                "(Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor fusion algorithm configuration error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system configuration error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " ",
            "(Sensor unknown error)"
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
                "Failed to initialize sensor #", id, " ",
                "(Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor fusion algorithm configuration error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system configuration error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " ",
            "(Sensor unknown error)"
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
                "Failed to initialize sensor #", id, " ",
                "(Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor fusion algorithm configuration error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system configuration error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " ",
            "(Sensor unknown error)"
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
                "Failed to initialize sensor #", id, " ",
                "(Sensor peripheral initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_sysinit[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system initialization error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_stfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor self test error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_valrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register value range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_adrrng[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register address range error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_wrfail[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor register write error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_lowpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor low power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_accpwr[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor accelerometer power mode error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_fuscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor fusion algorithm configuration error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_syscfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " ",
                "(Sensor system configuration error)"
            );
            intern::fail = true;
            return;
        }

        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " ",
            "(Sensor unknown error)"
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
    std::string oupt;

    logging::inf(intern::mod,
        "Getting sensor #", id, " system calibration level"
    );

    if (!(id == 1 || id == 2)) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " system calibration level ",
            "(Sensor does not exist)"
        );
        intern::fail = true;
        return lev_sys;
    }

    if (!intern::valid_lev_sys[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " system calibration level ",
            "(Reading unavailable)"
        );
        intern::fail = true;
        return lev_sys;
    }

    lev_sys = intern::lev_sys[id - 1];

    oupt = intern::conv_val_to_oupt(lev_sys);
    logging::inf(intern::mod,
        "Got sensor #", id, " system calibration level: ", oupt
    );

    intern::fail = false;
    return lev_sys;
}

int get_lev_acc (int id) {
    int lev_acc;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting sensor #", id, " accelerometer calibration level"
    );

    if (!(id == 1 || id == 2)) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " accelerometer calibration level ",
            "(Sensor does not exist)"
        );
        intern::fail = true;
        return lev_acc;
    }

    if (!intern::valid_lev_acc[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " accelerometer calibration level ",
            "(Reading unavailable)"
        );
        intern::fail = true;
        return lev_acc;
    }

    lev_acc = intern::lev_acc[id - 1];

    oupt = intern::conv_val_to_oupt(lev_acc);
    logging::inf(intern::mod,
        "Got sensor #", id, " accelerometer calibration level: ", oupt
    );

    intern::fail = false;
    return lev_acc;
}

int get_lev_mag (int id) {
    int lev_mag;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting sensor #", id, " magnetometer calibration level"
    );

    if (!(id == 1 || id == 2)) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " magnetometer calibration level ",
            "(Sensor does not exist)"
        );
        intern::fail = true;
        return lev_mag;
    }

    if (!intern::valid_lev_mag[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " magnetometer calibration level ",
            "(Reading unavailable)"
        );
        intern::fail = true;
        return lev_mag;
    }

    lev_mag = intern::lev_mag[id - 1];

    oupt = intern::conv_val_to_oupt(lev_mag);
    logging::inf(intern::mod,
        "Got sensor #", id, " magnetometer calibration level: ", oupt
    );

    intern::fail = false;
    return lev_mag;
}

int get_lev_gyr (int id) {
    int lev_gyr;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting sensor #", id, " gyroscope calibration level"
    );

    if (!(id == 1 || id == 2)) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " gyroscope calibration level ",
            "(Sensor does not exist)"
        );
        intern::fail = true;
        return lev_gyr;
    }

    if (!intern::valid_lev_gyr[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " gyroscope calibration level ",
            "(Reading unavailable)"
        );
        intern::fail = true;
        return lev_gyr;
    }

    lev_gyr = intern::lev_gyr[id - 1];

    oupt = intern::conv_val_to_oupt(lev_gyr);
    logging::inf(intern::mod,
        "Got sensor #", id, " gyroscope calibration level: ", oupt
    );

    intern::fail = false;
    return lev_gyr;
}

int get_rad_acc (int id) {
    int rad_acc;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting sensor #", id, " accelerometer calibration radius"
    );

    if (!(id == 1 || id == 2)) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " accelerometer calibration radius ",
            "(Sensor does not exist)"
        );
        intern::fail = true;
        return rad_acc;
    }

    if (!intern::valid_rad_acc[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " accelerometer calibration radius ",
            "(Reading unavailable)"
        );
        intern::fail = true;
        return rad_acc;
    }

    rad_acc = intern::rad_acc[id - 1];

    oupt = intern::conv_val_to_oupt(rad_acc);
    logging::inf(intern::mod,
        "Got sensor #", id, " accelerometer calibration radius: ", oupt
    );

    intern::fail = false;
    return rad_acc;
}

int get_rad_mag (int id) {
    int rad_mag;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting sensor #", id, " magnetometer calibration radius"
    );

    if (!(id == 1 || id == 2)) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " magnetometer calibration radius ",
            "(Sensor does not exist)"
        );
        intern::fail = true;
        return rad_mag;
    }

    if (!intern::valid_rad_mag[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " magnetometer calibration radius ",
            "(Reading unavailable)"
        );
        intern::fail = true;
        return rad_mag;
    }

    rad_mag = intern::rad_mag[id - 1];

    oupt = intern::conv_val_to_oupt(rad_mag);
    logging::inf(intern::mod,
        "Got sensor #", id, " magnetometer calibration radius: ", oupt
    );

    intern::fail = false;
    return rad_mag;
}

linalg::ivector get_off_acc (int id) {
    linalg::ivector off_acc;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting sensor #", id, " accelerometer calibration offset"
    );

    if (!(id == 1 || id == 2)) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " accelerometer calibration offset ",
            "(Sensor does not exist)"
        );
        intern::fail = true;
        return off_acc;
    }

    if (!intern::valid_off_acc[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " accelerometer calibration offset ",
            "(Reading unavailable)"
        );
        intern::fail = true;
        return off_acc;
    }

    off_acc = intern::off_acc[id - 1];

    oupt = intern::conv_val_to_oupt(off_acc);
    logging::inf(intern::mod,
        "Got sensor #", id, " accelerometer calibration offset: ", oupt
    );

    intern::fail = false;
    return off_acc;
}

linalg::ivector get_off_mag (int id) {
    linalg::ivector off_mag;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting sensor #", id, " magnetometer calibration offset"
    );

    if (!(id == 1 || id == 2)) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " magnetometer calibration offset ",
            "(Sensor does not exist)"
        );
        intern::fail = true;
        return off_mag;
    }

    if (!intern::valid_off_mag[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " magnetometer calibration offset ",
            "(Reading unavailable)"
        );
        intern::fail = true;
        return off_mag;
    }

    off_mag = intern::off_mag[id - 1];

    oupt = intern::conv_val_to_oupt(off_mag);
    logging::inf(intern::mod,
        "Got sensor #", id, " magnetometer calibration offset: ", oupt
    );

    intern::fail = false;
    return off_mag;
}

linalg::ivector get_off_gyr (int id) {
    linalg::ivector off_gyr;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting sensor #", id, " gyroscope calibration offset"
    );

    if (!(id == 1 || id == 2)) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " gyroscope calibration offset ",
            "(Sensor does not exist)"
        );
        intern::fail = true;
        return off_gyr;
    }

    if (!intern::valid_off_gyr[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " gyroscope calibration offset ",
            "(Reading unavailable)"
        );
        intern::fail = true;
        return off_gyr;
    }

    off_gyr = intern::off_gyr[id - 1];

    oupt = intern::conv_val_to_oupt(off_gyr);
    logging::inf(intern::mod,
        "Got sensor #", id, " gyroscope calibration offset: ", oupt
    );

    intern::fail = false;
    return off_gyr;
}

linalg::fvector get_rot (int id) {
    linalg::fvector rot;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting sensor #", id, " angular velocity reading"
    );

    if (!(id == 1 || id == 2)) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " angular velocity reading ",
            "(Sensor does not exist)"
        );
        intern::fail = true;
        return rot;
    }

    if (!intern::valid_rot[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " angular velocity reading ",
            "(Reading unavailable)"
        );
        intern::fail = true;
        return rot;
    }

    rot = intern::rot[id - 1];

    oupt = intern::conv_val_to_oupt(rot);
    logging::inf(intern::mod,
        "Got sensor #", id, " angular velocity reading: ", oupt, "rad/s"
    );

    intern::fail = false;
    return rot;
}

linalg::fvector get_qua (int id) {
    linalg::fvector qua;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting sensor #", id, " attitude quaternion reading"
    );

    if (!(id == 1 || id == 2)) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " attitude quaternion reading ",
            "(Sensor does not exist)"
        );
        intern::fail = true;
        return qua;
    }

    if (!intern::valid_qua[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " attitude quaternion reading ",
            "(Reading unavailable)"
        );
        intern::fail = true;
        return qua;
    }

    qua = intern::qua[id - 1];

    oupt = intern::conv_val_to_oupt(qua);
    logging::inf(intern::mod,
        "Got sensor #", id, " attitude quaternion reading: ", oupt
    );

    intern::fail = false;
    return qua;
}

linalg::fvector get_lia (int id) {
    linalg::fvector lia;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting sensor #", id, " linear acceleration reading"
    );

    if (!(id == 1 || id == 2)) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " linear acceleration reading ",
            "(Sensor does not exist)"
        );
        intern::fail = true;
        return lia;
    }

    if (!intern::valid_lia[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " linear acceleration reading ",
            "(Reading unavailable)"
        );
        intern::fail = true;
        return lia;
    }

    lia = intern::lia[id - 1];

    oupt = intern::conv_val_to_oupt(lia);
    logging::inf(intern::mod,
        "Got sensor #", id, " linear acceleration reading: ", oupt, "m/s²"
    );

    intern::fail = false;
    return lia;
}

void update (int id) {
    logging::inf(intern::mod,
        "Updating sensor #", id, " readings"
    );

    if (!(id == 1 || id == 2)) {
        logging::err(intern::mod,
            "Failed to update sensor #", id, " readings ",
            "(Sensor does not exist)"
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
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor peripheral initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_sysinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor system initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_stfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor self test error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_valrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor register value range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_adrrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor register address range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_wrfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor register write error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_lowpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor low power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_accpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor accelerometer power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_fuscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor fusion algorithm configuration error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_syscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor system configuration error)"
                );
                intern::fail = true;
                return;
            }

            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings ",
                "(Sensor unknown error)"
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
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor peripheral initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_sysinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor system initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_stfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor self test error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_valrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor register value range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_adrrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor register address range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_wrfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor register write error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_lowpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor low power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_accpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor accelerometer power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_fuscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor fusion algorithm configuration error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_syscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor system configuration error)"
                );
                intern::fail = true;
                return;
            }

            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings ",
                "(Sensor unknown error)"
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
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor peripheral initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_sysinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor system initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_stfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor self test error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_valrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor register value range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_adrrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor register address range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_wrfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor register write error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_lowpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor low power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_accpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor accelerometer power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_fuscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor fusion algorithm configuration error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_syscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor system configuration error)"
                );
                intern::fail = true;
                return;
            }

            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings ",
                "(Sensor unknown error)"
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
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor peripheral initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_sysinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor system initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_stfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor self test error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_valrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor register value range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_adrrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor register address range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_wrfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor register write error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_lowpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor low power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_accpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor accelerometer power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_fuscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor fusion algorithm configuration error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_syscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor system configuration error)"
                );
                intern::fail = true;
                return;
            }

            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings ",
                "(Sensor unknown error)"
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
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor peripheral initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_sysinit[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor system initialization error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_stfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor self test error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_valrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor register value range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_adrrng[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor register address range error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_wrfail[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor register write error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_lowpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor low power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_accpwr[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor accelerometer power mode error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_fuscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor fusion algorithm configuration error)"
                );
                intern::fail = true;
                return;
            }

            if (intern::err_syscfg[id - 1]) {
                logging::err(intern::mod,
                    "Failed to update sensor #", id, " readings ",
                    "(Sensor system configuration error)"
                );
                intern::fail = true;
                return;
            }

            logging::err(intern::mod,
                "Failed to update sensor #", id, " readings ",
                "(Sensor unknown error)"
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

std::string conv_val_to_oupt (bool val) {
    std::string oupt;
    if (val) {
        oupt = "True";
    } else {
        oupt = "False";
    }
    return oupt;
}

std::string conv_val_to_oupt (int val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << std::showpos << val;
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (linalg::ivector val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        ostr << std::showpos << val[i];
        if (i < linalg::dim(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (linalg::fvector val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        ostr << std::showpos << std::scientific << std::setprecision(2)
             << val[i];
        if (i < linalg::dim(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    oupt = ostr.str();

    return oupt;
}

std::string conv_hex_to_oupt (std::uint8_t hex) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "0x"
         << std::uppercase << std::hex << std::setfill('0') << std::setw(2)
         << (int)hex;
    oupt = ostr.str();

    return oupt;
}

std::string conv_hex_to_oupt (std::vector<std::uint8_t> hex) {
    std::ostringstream ostr;
    std::string oupt;

    for (auto i = 0; i < hex.size(); i++) {
        ostr << "0x"
             << std::uppercase << std::hex << std::setfill('0') << std::setw(2)
             << (int)(hex[i]);
        if (i < hex.size() - 1) {
            ostr << " ";
        }
    }
    oupt = ostr.str();

    return oupt;
}

std::string conv_axis_to_oupt (axis axis) {
    switch (axis) {
        case axis_px:
            return std::string("+X");
        case axis_nx:
            return std::string("-X");
        case axis_py:
            return std::string("+Y");
        case axis_ny:
            return std::string("-Y");
        case axis_pz:
            return std::string("+Z");
        case axis_nz:
            return std::string("-Z");
        default:
            return std::string("");
    }
}

void put_reg (int id, std::uint8_t reg, std::uint8_t dat) {
    std::uint8_t addr;
    std::vector<std::uint8_t> cmd(2);
    std::string oupt_reg, oupt_dat;

    oupt_reg = conv_hex_to_oupt(reg);
    oupt_dat = conv_hex_to_oupt(dat);
    logging::inf(mod,
        "Writing to sensor #", id, ": Reg: ", oupt_reg, ", Data: ", oupt_dat
    );

    switch (id) {
        case 1:
            addr = BNO055_ADDR1;
            break;
        case 2:
            addr = BNO055_ADDR2;
            break;
    }

    cmd[0] = reg;
    cmd[1] = dat;

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
    std::vector<std::uint8_t> cmd(1), dat(len);
    std::string oupt_reg, oupt_dat;

    oupt_reg = conv_hex_to_oupt(reg);
    logging::inf(mod,
        "Reading from sensor #", id, ": Reg: ", oupt_reg, ", Len: ", len
    );

    switch (id) {
        case 1:
            addr = BNO055_ADDR1;
            break;
        case 2:
            addr = BNO055_ADDR2;
            break;
    }

    cmd[0] = reg;

    i2c::send(addr, cmd);
    i2c::recv(addr, &dat);
    i2c::exec();
    if (i2c::fail()) {
        logging::err(mod,
            "Failed to read from sensor #", id
        );
        fail = true;
        return dat;
    }

    oupt_dat = conv_hex_to_oupt(dat);
    logging::inf(mod,
        "Read from sensor #", id, ": Data: ", oupt_dat
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
        "Setting sensor #", id, " measurement units: ",
        "Ori: Android, Acc unt: m/s², Gyr unt: rad/s"
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

    axm = BNO055_PUT_BITS(axm, BNO055_BIT_MAPX, axis_x & 0xFF);
    axm = BNO055_PUT_BITS(axm, BNO055_BIT_MAPY, axis_y & 0xFF);
    axm = BNO055_PUT_BITS(axm, BNO055_BIT_MAPZ, axis_z & 0xFF);

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

    axs = BNO055_PUT_BITS(axs, BNO055_BIT_SGNX, axis_x >> 8);
    axs = BNO055_PUT_BITS(axs, BNO055_BIT_SGNY, axis_y >> 8);
    axs = BNO055_PUT_BITS(axs, BNO055_BIT_SGNZ, axis_z >> 8);

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

void put_caldat (
    int id, int rad_acc, int rad_mag,
    linalg::ivector off_acc, linalg::ivector off_mag, linalg::ivector off_gyr
) {
    logging::inf(mod,
        "Setting sensor #", id, " calibration parameters"
    );

    put_reg(id, BNO055_REG_RADACCL, (std::int16_t)(rad_acc) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_RADACCM, (std::int16_t)(rad_acc) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_RADMAGL, (std::int16_t)(rad_mag) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_RADMAGM, (std::int16_t)(rad_mag) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFACCXL, (std::int16_t)(off_acc[0]) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFACCXM, (std::int16_t)(off_acc[0]) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFACCYL, (std::int16_t)(off_acc[1]) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFACCYM, (std::int16_t)(off_acc[1]) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFACCZL, (std::int16_t)(off_acc[2]) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFACCZM, (std::int16_t)(off_acc[2]) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFMAGXL, (std::int16_t)(off_mag[0]) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFMAGXM, (std::int16_t)(off_mag[0]) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFMAGYL, (std::int16_t)(off_mag[1]) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFMAGYM, (std::int16_t)(off_mag[1]) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFMAGZL, (std::int16_t)(off_mag[2]) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFMAGZM, (std::int16_t)(off_mag[2]) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFGYRXL, (std::int16_t)(off_gyr[0]) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFGYRXM, (std::int16_t)(off_gyr[0]) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFGYRYL, (std::int16_t)(off_gyr[1]) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFGYRYM, (std::int16_t)(off_gyr[1]) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFGYRZL, (std::int16_t)(off_gyr[2]) & 0xFF);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    put_reg(id, BNO055_REG_OFGYRZM, (std::int16_t)(off_gyr[2]) >> 8);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    fail = false;
}

void get_sta (int id) {
    std::vector<std::uint8_t> sta;
    std::string oupt_sta_idl;
    std::string oupt_sta_err;
    std::string oupt_sta_perinit;
    std::string oupt_sta_sysinit;
    std::string oupt_sta_strun;
    std::string oupt_sta_fusrun;
    std::string oupt_sta_nfusrun;

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

    oupt_sta_idl = conv_val_to_oupt(sta_idl[id - 1]);
    oupt_sta_err = conv_val_to_oupt(sta_err[id - 1]);
    oupt_sta_perinit = conv_val_to_oupt(sta_perinit[id - 1]);
    oupt_sta_sysinit = conv_val_to_oupt(sta_sysinit[id - 1]);
    oupt_sta_strun = conv_val_to_oupt(sta_strun[id - 1]);
    oupt_sta_fusrun = conv_val_to_oupt(sta_fusrun[id - 1]);
    oupt_sta_nfusrun = conv_val_to_oupt(sta_nfusrun[id - 1]);

    logging::inf(mod,
        "Got sensor #", id, " status flags: ",
        "Idle: ", oupt_sta_idl, ", ",
        "Error: ", oupt_sta_err, ", ",
        "Periph init: ", oupt_sta_perinit, ", ",
        "System init: ", oupt_sta_sysinit, ", ",
        "Self test: ", oupt_sta_strun, ", ",
        "Fusion: ", oupt_sta_fusrun, ", ",
        "Non fusion: ", oupt_sta_nfusrun
    );

    fail = false;
}

void get_err (int id) {
    std::vector<std::uint8_t> err;
    std::string oupt_err_perinit;
    std::string oupt_err_sysinit;
    std::string oupt_err_stfail;
    std::string oupt_err_valrng;
    std::string oupt_err_adrrng;
    std::string oupt_err_wrfail;
    std::string oupt_err_lowpwr;
    std::string oupt_err_accpwr;
    std::string oupt_err_fuscfg;
    std::string oupt_err_syscfg;

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

    oupt_err_perinit = conv_val_to_oupt(err_perinit[id - 1]);
    oupt_err_sysinit = conv_val_to_oupt(err_sysinit[id - 1]);
    oupt_err_stfail = conv_val_to_oupt(err_stfail[id - 1]);
    oupt_err_valrng = conv_val_to_oupt(err_valrng[id - 1]);
    oupt_err_adrrng = conv_val_to_oupt(err_adrrng[id - 1]);
    oupt_err_wrfail = conv_val_to_oupt(err_wrfail[id - 1]);
    oupt_err_lowpwr = conv_val_to_oupt(err_lowpwr[id - 1]);
    oupt_err_accpwr = conv_val_to_oupt(err_accpwr[id - 1]);
    oupt_err_fuscfg = conv_val_to_oupt(err_fuscfg[id - 1]);
    oupt_err_syscfg = conv_val_to_oupt(err_syscfg[id - 1]);

    logging::inf(mod,
        "Got sensor #", id, " error flags: ",
        "Periph init: ", oupt_err_perinit, ", ",
        "System init: ", oupt_err_sysinit, ", ",
        "Self test: ", oupt_err_stfail, ", ",
        "Val range: ", oupt_err_valrng, ", ",
        "Addr range: ", oupt_err_adrrng, ", ",
        "Reg write: ", oupt_err_wrfail, ", ",
        "Low pwr: ", oupt_err_lowpwr, ", ",
        "Acc pwr: ", oupt_err_accpwr, ", ",
        "Fusion config: ", oupt_err_fuscfg, ", ",
        "System config: ", oupt_err_syscfg
    );

    fail = false;
}

void get_callev (int id) {
    std::vector<std::uint8_t> callev;
    std::string oupt_lev_sys, oupt_lev_acc, oupt_lev_mag, oupt_lev_gyr;

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

    oupt_lev_sys = conv_val_to_oupt(lev_sys[id - 1]);
    oupt_lev_acc = conv_val_to_oupt(lev_acc[id - 1]);
    oupt_lev_mag = conv_val_to_oupt(lev_mag[id - 1]);
    oupt_lev_gyr = conv_val_to_oupt(lev_gyr[id - 1]);

    logging::inf(mod,
        "Got sensor #", id, " calibration levels: ",
        "Sys lev: ", oupt_lev_sys, ", ",
        "Acc lev: ", oupt_lev_acc, ", ",
        "Mag lev: ", oupt_lev_mag, ", ",
        "Gyr lev: ", oupt_lev_gyr
    );

    fail = false;
}

void get_caldat (int id) {
    std::vector<std::uint8_t> caldat;
    std::string oupt_rad_acc, oupt_rad_mag;
    std::string oupt_off_acc, oupt_off_mag, oupt_off_gyr;

    logging::inf(mod,
        "Getting sensor #", id, " calibration parameters"
    );

    caldat = get_reg(id, BNO055_REG_CALDAT, 22);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    rad_acc[id - 1] = (std::int16_t)(caldat[18] | (caldat[19] << 8));
    rad_mag[id - 1] = (std::int16_t)(caldat[20] | (caldat[21] << 8));

    off_acc[id - 1][0] = (std::int16_t)(caldat[0] | (caldat[1] << 8));
    off_acc[id - 1][1] = (std::int16_t)(caldat[2] | (caldat[3] << 8));
    off_acc[id - 1][2] = (std::int16_t)(caldat[4] | (caldat[5] << 8));

    off_mag[id - 1][0] = (std::int16_t)(caldat[6] | (caldat[7] << 8));
    off_mag[id - 1][1] = (std::int16_t)(caldat[8] | (caldat[9] << 8));
    off_mag[id - 1][2] = (std::int16_t)(caldat[10] | (caldat[11] << 8));

    off_gyr[id - 1][0] = (std::int16_t)(caldat[12] | (caldat[13] << 8));
    off_gyr[id - 1][1] = (std::int16_t)(caldat[14] | (caldat[15] << 8));
    off_gyr[id - 1][2] = (std::int16_t)(caldat[16] | (caldat[17] << 8));

    oupt_rad_acc = conv_val_to_oupt(rad_acc[id - 1]);
    oupt_rad_mag = conv_val_to_oupt(rad_mag[id - 1]);
    oupt_off_acc = conv_val_to_oupt(off_acc[id - 1]);
    oupt_off_mag = conv_val_to_oupt(off_mag[id - 1]);
    oupt_off_gyr = conv_val_to_oupt(off_gyr[id - 1]);

    logging::inf(mod,
        "Got sensor #", id, " calibration parameters: ",
        "Acc rad: ", oupt_rad_acc, ", ",
        "Mag rad: ", oupt_rad_mag, ", ",
        "Acc off: ", oupt_off_acc, ", ",
        "Mag off: ", oupt_off_mag, ", ",
        "Gyr off: ", oupt_off_gyr
    );

    fail = false;
}

void get_datrot (int id) {
    std::vector<std::uint8_t> datrot;
    std::string oupt;

    logging::inf(mod,
        "Getting sensor #", id, " angular velocity reading"
    );

    datrot = get_reg(id, BNO055_REG_DATROT, 6);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", id, " angular velocity reading"
        );
        fail = true;
        return;
    }

    rot[id - 1][0] = (std::int16_t)(datrot[0] | (datrot[1] << 8));
    rot[id - 1][1] = (std::int16_t)(datrot[2] | (datrot[3] << 8));
    rot[id - 1][2] = (std::int16_t)(datrot[4] | (datrot[5] << 8));

    rot[id - 1] /= 900;

    oupt = conv_val_to_oupt(rot[id - 1]);
    logging::inf(mod,
        "Got sensor #", id, " angular velocity reading: ", oupt, "rad/s"
    );

    fail = false;
}

void get_datqua (int id) {
    std::vector<std::uint8_t> datqua;
    std::string oupt;

    logging::inf(mod,
        "Getting sensor #", id, " attitude quaternion reading"
    );

    datqua = get_reg(id, BNO055_REG_DATQUA, 8);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", id, " attitude quaternion reading"
        );
        fail = true;
        return;
    }

    qua[id - 1][0] = (std::int16_t)(datqua[0] | (datqua[1] << 8));
    qua[id - 1][1] = (std::int16_t)(datqua[2] | (datqua[3] << 8));
    qua[id - 1][2] = (std::int16_t)(datqua[4] | (datqua[5] << 8));
    qua[id - 1][3] = (std::int16_t)(datqua[6] | (datqua[7] << 8));

    qua[id - 1] /= 16384;

    oupt = conv_val_to_oupt(qua[id - 1]);
    logging::inf(mod,
        "Got sensor #", id, " attitude quaternion reading: ", oupt
    );

    fail = false;
}

void get_datlia (int id) {
    std::vector<std::uint8_t> datlia;
    std::string oupt;

    logging::inf(mod,
        "Getting sensor #", id, " linear acceleration reading"
    );

    datlia = get_reg(id, BNO055_REG_DATLIA, 6);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", id, " linear acceleration reading"
        );
        fail = true;
        return;
    }

    lia[id - 1][0] = (std::int16_t)(datlia[0] | (datlia[1] << 8));
    lia[id - 1][1] = (std::int16_t)(datlia[2] | (datlia[3] << 8));
    lia[id - 1][2] = (std::int16_t)(datlia[4] | (datlia[5] << 8));

    lia[id - 1] /= 100;

    oupt = conv_val_to_oupt(lia[id - 1]);
    logging::inf(mod,
        "Got sensor #", id, " linear acceleration reading: ", oupt, "m/s²"
    );

    fail = false;
}

}

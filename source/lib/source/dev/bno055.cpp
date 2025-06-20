#include <cstdint>

#include <vector>

#include <math/linalg.hpp>
#include <sys/logging.hpp>
#include <dev/i2c.hpp>
#include <dev/bno055.hpp>

namespace bno055 {

bool tracker::exist1 = false, tracker::exist2 = false;

bool tracker::instantiate (int id) {
    switch (id) {
        case 1:
            if (tracker::exist1) {
                return false;
            } else {
                tracker::exist1 = true;
                return true;
            }
        case 2:
            if (tracker::exist2) {
                return false;
            } else {
                tracker::exist2 = true;
                return true;
            }
        default:
            return false;
    }
}

bool tracker::validate (int id) {
    return (id == 1 || id == 2);
}

instance::instance (i2c::instance & i2c, int id, axis axis_x, axis axis_y, axis axis_z)
  : logging("bno055"), init(false), id(id),
    i2c(&i2c), addr(id == 1 ? BNO055_ADDR1 : id == 2 ? BNO055_ADDR2 : 0),
    sta_idl(false), sta_err(false), sta_perinit(false), sta_sysinit(false), sta_slftest(false), sta_fus(false), sta_nfus(false),
    err_perinit(false), err_sysinit(false), err_slftest(false), err_regval(false), err_regadr(false), err_regwri(false),
    err_lowpwr(false), err_accpwr(false), err_fuscfg(false), err_syscfg(false),
    cal_off_acc_data(3), cal_off_mag_data(3), cal_off_gyr_data(3),
    cal_lev_sys_valid(false), cal_lev_acc_valid(false), cal_lev_mag_valid(false), cal_lev_gyr_valid(false),
    cal_rad_acc_valid(false), cal_rad_mag_valid(false),
    cal_off_acc_valid(false), cal_off_mag_valid(false), cal_off_gyr_valid(false),
    mea_rot_data(3), mea_qua_data(4), mea_lia_data(3),
    mea_rot_valid(false), mea_qua_valid(false), mea_lia_valid(false),
    mode_cal(true), mode_mea(false) {

    bool error = false;

    this->logging.inf("Initializing instance #", this->id, ": X axis: ", axis_x, ", Y axis: ", axis_y, ", Z axis: ", axis_z);

    if (!instance::validate(this->id)) {
        this->logging.err("Failed to initialize instance #", this->id, " (Invalid ID)");
        throw except_ctor::fail;
    }

    if (!instance::instantiate(this->id)) {
        this->logging.err("Failed to initialize instance #", this->id, " (Instance already exists)");
        throw except_ctor::fail;
    }

    this->logging.inf("Configuring sensor #", this->id);

    try {
        this->put_trg_rst();
        do {
            this->get_sta();
            if (this->sta_err) {
                this->get_err();
                error = true;
                throw except_intern{};
            }
        } while (!this->sta_idl);

        this->put_pwr_nrm();
        this->get_sta();
        if (this->sta_err) {
            this->get_err();
            error = true;
            throw except_intern{};
        }

        this->put_opr_cfg();
        this->get_sta();
        if (this->sta_err) {
            this->get_err();
            error = true;
            throw except_intern{};
        }

        this->put_unt_si();
        this->get_sta();
        if (this->sta_err) {
            this->get_err();
            error = true;
            throw except_intern{};
        }

        this->put_axm(axis_x, axis_y, axis_z);
        this->get_sta();
        if (this->sta_err) {
            this->get_err();
            error = true;
            throw except_intern{};
        }

        this->put_axs(axis_x, axis_y, axis_z);
        this->get_sta();
        if (this->sta_err) {
            this->get_err();
            error = true;
            throw except_intern{};
        }

        this->put_opr_ndof();
        this->get_sta();
        if (this->sta_err) {
            this->get_err();
            error = true;
            throw except_intern{};
        }
    } catch (...) {
        if (error) {
            if (this->err_perinit) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor peripheral initialization error)");
            } else if (this->err_sysinit) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor system initialization error)");
            } else if (this->err_slftest) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor self test error)");
            } else if (this->err_regval) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor register value error)");
            } else if (this->err_regadr) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor register address error)");
            } else if (this->err_regwri) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor register write error)");
            } else if (this->err_lowpwr) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor low power mode error)");
            } else if (this->err_accpwr) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor accelerometer power mode error)");
            } else if (this->err_fuscfg) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor fusion config error)");
            } else if (this->err_syscfg) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor system config error)");
            } else {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor unknown error)");
            }
        } else {
            this->logging.err("Failed to configure sensor #", this->id);
        }
        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    this->init = true;
}

instance::instance (
    i2c::instance & i2c, int id, axis axis_x, axis axis_y, axis axis_z,
    int rad_acc, int rad_mag, const linalg::ivector & off_acc, const linalg::ivector & off_mag, const linalg::ivector & off_gyr
) : logging("bno055"), init(false), id(id),
    i2c(&i2c), addr(id == 1 ? BNO055_ADDR1 : id == 2 ? BNO055_ADDR2 : 0),
    sta_idl(false), sta_err(false), sta_perinit(false), sta_sysinit(false), sta_slftest(false), sta_fus(false), sta_nfus(false),
    err_perinit(false), err_sysinit(false), err_slftest(false), err_regval(false), err_regadr(false), err_regwri(false),
    err_lowpwr(false), err_accpwr(false), err_fuscfg(false), err_syscfg(false),
    cal_off_acc_data(3), cal_off_mag_data(3), cal_off_gyr_data(3),
    cal_lev_sys_valid(false), cal_lev_acc_valid(false), cal_lev_mag_valid(false), cal_lev_gyr_valid(false),
    cal_rad_acc_valid(false), cal_rad_mag_valid(false),
    cal_off_acc_valid(false), cal_off_mag_valid(false), cal_off_gyr_valid(false),
    mea_rot_data(3), mea_qua_data(4), mea_lia_data(3),
    mea_rot_valid(false), mea_qua_valid(false), mea_lia_valid(false),
    mode_cal(false), mode_mea(true) {

    bool error = false;

    this->logging.inf(
        "Initializing instance #", this->id, ": ",
        "X axis: ", axis_x, ", Y axis: ", axis_y, ", Z axis: ", axis_z, ", ",
        "Acc radius: ", rad_acc, ", Mag radius: ", rad_mag, ", Acc offset: ", off_acc, ", Mag offset: ", off_mag, ", Gyr offset: ", off_gyr
    );

    if (!instance::validate(this->id)) {
        this->logging.err("Failed to initialize instance #", this->id, " (Invalid ID)");
        throw except_ctor::fail;
    }

    if (!instance::instantiate(this->id)) {
        this->logging.err("Failed to initialize instance #", this->id, " (Instance already exists)");
        throw except_ctor::fail;
    }

    this->logging.inf("Configuring sensor #", this->id);

    try {
        this->put_trg_rst();
        do {
            this->get_sta();
            if (this->sta_err) {
                this->get_err();
                error = true;
                throw except_intern{};
            }
        } while (!this->sta_idl);

        this->put_pwr_nrm();
        this->get_sta();
        if (this->sta_err) {
            this->get_err();
            error = true;
            throw except_intern{};
        }

        this->put_opr_cfg();
        this->get_sta();
        if (this->sta_err) {
            this->get_err();
            error = true;
            throw except_intern{};
        }

        this->put_unt_si();
        this->get_sta();
        if (this->sta_err) {
            this->get_err();
            error = true;
            throw except_intern{};
        }

        this->put_axm(axis_x, axis_y, axis_z);
        this->get_sta();
        if (this->sta_err) {
            this->get_err();
            error = true;
            throw except_intern{};
        }

        this->put_axs(axis_x, axis_y, axis_z);
        this->get_sta();
        if (this->sta_err) {
            this->get_err();
            error = true;
            throw except_intern{};
        }

        this->put_caldat(rad_acc, rad_mag, off_acc, off_mag, off_gyr);
        this->get_sta();
        if (this->sta_err) {
            this->get_err();
            error = true;
            throw except_intern{};
        }

        this->put_opr_ndof();
        this->get_sta();
        if (this->sta_err) {
            this->get_err();
            error = true;
            throw except_intern{};
        }
    } catch (...) {
        if (error) {
            if (this->err_perinit) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor peripheral initialization error)");
            } else if (this->err_sysinit) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor system initialization error)");
            } else if (this->err_slftest) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor self test error)");
            } else if (this->err_regval) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor register value error)");
            } else if (this->err_regadr) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor register address error)");
            } else if (this->err_regwri) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor register write error)");
            } else if (this->err_lowpwr) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor low power mode error)");
            } else if (this->err_accpwr) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor accelerometer power mode error)");
            } else if (this->err_fuscfg) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor fusion config error)");
            } else if (this->err_syscfg) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor system config error)");
            } else {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor unknown error)");
            }
        } else {
            this->logging.err("Failed to configure sensor #", this->id);
        }
        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    this->init = true;
}

int instance::get_lev_sys (void) {
    this->logging.inf("Getting sensor #", this->id, " system calibration level");

    if (!this->init) {
        this->logging.err("Failed to get sensor #", this->id, " system calibration level (Instance not initialized)");
        throw except_get_lev_sys::fail;
    }

    if (!this->mode_cal) {
        this->logging.err("Failed to get sensor #", this->id, " system calibration level (Sensor not in calibration mode)");
        throw except_get_lev_sys::fail;
    }

    if (!this->cal_lev_sys_valid) {
        this->logging.err("Failed to get sensor #", this->id, " system calibration level (Reading unavailable)");
        throw except_get_lev_sys::fail;
    }

    this->logging.inf("Got sensor #", this->id, " system calibration level: ", this->cal_lev_sys_data);

    return this->cal_lev_sys_data;
}

int instance::get_lev_acc (void) {
    this->logging.inf("Getting sensor #", this->id, " accelerometer calibration level");

    if (!this->init) {
        this->logging.err("Failed to get sensor #", this->id, " accelerometer calibration level (Instance not initialized)");
        throw except_get_lev_acc::fail;
    }

    if (!this->mode_cal) {
        this->logging.err("Failed to get sensor #", this->id, " accelerometer calibration level (Sensor not in calibration mode)");
        throw except_get_lev_acc::fail;
    }

    if (!this->cal_lev_acc_valid) {
        this->logging.err("Failed to get sensor #", this->id, " accelerometer calibration level (Reading unavailable)");
        throw except_get_lev_acc::fail;
    }

    this->logging.inf("Got sensor #", this->id, " accelerometer calibration level: ", this->cal_lev_acc_data);

    return this->cal_lev_acc_data;
}

int instance::get_lev_mag (void) {
    this->logging.inf("Getting sensor #", this->id, " magnetometer calibration level");

    if (!this->init) {
        this->logging.err("Failed to get sensor #", this->id, " magnetometer calibration level (Instance not initialized)");
        throw except_get_lev_mag::fail;
    }

    if (!this->mode_cal) {
        this->logging.err("Failed to get sensor #", this->id, " magnetometer calibration level (Sensor not in calibration mode)");
        throw except_get_lev_mag::fail;
    }

    if (!this->cal_lev_mag_valid) {
        this->logging.err("Failed to get sensor #", this->id, " magnetometer calibration level (Reading unavailable)");
        throw except_get_lev_mag::fail;
    }

    this->logging.inf("Got sensor #", this->id, " magnetometer calibration level: ", this->cal_lev_mag_data);

    return this->cal_lev_mag_data;
}

int instance::get_lev_gyr (void) {
    this->logging.inf("Getting sensor #", this->id, " gyroscope calibration level");

    if (!this->init) {
        this->logging.err("Failed to get sensor #", this->id, " gyroscope calibration level (Instance not initialized)");
        throw except_get_lev_gyr::fail;
    }

    if (!this->mode_cal) {
        this->logging.err("Failed to get sensor #", this->id, " gyroscope calibration level (Sensor not in calibration mode)");
        throw except_get_lev_gyr::fail;
    }

    if (!this->cal_lev_gyr_valid) {
        this->logging.err("Failed to get sensor #", this->id, " gyroscope calibration level (Reading unavailable)");
        throw except_get_lev_gyr::fail;
    }

    this->logging.inf("Got sensor #", this->id, " gyroscope calibration level: ", this->cal_lev_gyr_data);

    return this->cal_lev_gyr_data;
}

int instance::get_rad_acc (void) {
    this->logging.inf("Getting sensor #", this->id, " accelerometer calibration radius");

    if (!this->init) {
        this->logging.err("Failed to get sensor #", this->id, " accelerometer calibration radius (Instance not initialized)");
        throw except_get_rad_acc::fail;
    }

    if (!this->mode_cal) {
        this->logging.err("Failed to get sensor #", this->id, " accelerometer calibration radius (Sensor not in calibration mode)");
        throw except_get_rad_acc::fail;
    }

    if (!this->cal_rad_acc_valid) {
        this->logging.err("Failed to get sensor #", this->id, " accelerometer calibration radius (Reading unavailable)");
        throw except_get_rad_acc::fail;
    }

    this->logging.inf("Got sensor #", this->id, " accelerometer calibration radius: ", this->cal_rad_acc_data);

    return this->cal_rad_acc_data;
}

int instance::get_rad_mag (void) {
    this->logging.inf("Getting sensor #", this->id, " magenetometer calibration radius");

    if (!this->init) {
        this->logging.err("Failed to get sensor #", this->id, " magenetometer calibration radius (Instance not initialized)");
        throw except_get_rad_mag::fail;
    }

    if (!this->mode_cal) {
        this->logging.err("Failed to get sensor #", this->id, " magenetometer calibration radius (Sensor not in calibration mode)");
        throw except_get_rad_mag::fail;
    }

    if (!this->cal_rad_mag_valid) {
        this->logging.err("Failed to get sensor #", this->id, " magenetometer calibration radius (Reading unavailable)");
        throw except_get_rad_mag::fail;
    }

    this->logging.inf("Got sensor #", this->id, " magenetometer calibration radius: ", this->cal_rad_mag_data);

    return this->cal_rad_mag_data;
}

linalg::ivector instance::get_off_acc (void) {
    this->logging.inf("Getting sensor #", this->id, " accelerometer calibration offset");

    if (!this->init) {
        this->logging.err("Failed to get sensor #", this->id, " accelerometer calibration offset (Instance not initialized)");
        throw except_get_off_acc::fail;
    }

    if (!this->mode_cal) {
        this->logging.err("Failed to get sensor #", this->id, " accelerometer calibration offset (Sensor not in calibration mode)");
        throw except_get_off_acc::fail;
    }

    if (!this->cal_off_acc_valid) {
        this->logging.err("Failed to get sensor #", this->id, " accelerometer calibration offset (Reading unavailable)");
        throw except_get_off_acc::fail;
    }

    this->logging.inf("Got sensor #", this->id, " accelerometer calibration offset: ", this->cal_off_acc_data);

    return this->cal_off_acc_data;
}

linalg::ivector instance::get_off_mag (void) {
    this->logging.inf("Getting sensor #", this->id, " magenetometer calibration offset");

    if (!this->init) {
        this->logging.err("Failed to get sensor #", this->id, " magenetometer calibration offset (Instance not initialized)");
        throw except_get_off_mag::fail;
    }

    if (!this->mode_cal) {
        this->logging.err("Failed to get sensor #", this->id, " magenetometer calibration offset (Sensor not in calibration mode)");
        throw except_get_off_mag::fail;
    }

    if (!this->cal_off_mag_valid) {
        this->logging.err("Failed to get sensor #", this->id, " magenetometer calibration offset (Reading unavailable)");
        throw except_get_off_mag::fail;
    }

    this->logging.inf("Got sensor #", this->id, " magenetometer calibration offset: ", this->cal_off_mag_data);

    return this->cal_off_mag_data;
}

linalg::ivector instance::get_off_gyr (void) {
    this->logging.inf("Getting sensor #", this->id, " gyroscope calibration offset");

    if (!this->init) {
        this->logging.err("Failed to get sensor #", this->id, " gyroscope calibration offset (Instance not initialized)");
        throw except_get_off_gyr::fail;
    }

    if (!this->mode_cal) {
        this->logging.err("Failed to get sensor #", this->id, " gyroscope calibration offset (Sensor not in calibration mode)");
        throw except_get_off_gyr::fail;
    }

    if (!this->cal_off_gyr_valid) {
        this->logging.err("Failed to get sensor #", this->id, " gyroscope calibration offset (Reading unavailable)");
        throw except_get_off_gyr::fail;
    }

    this->logging.inf("Got sensor #", this->id, " gyroscope calibration offset: ", this->cal_off_gyr_data);

    return this->cal_off_gyr_data;
}

linalg::fvector instance::get_rot (void) {
    this->logging.inf("Getting sensor #", this->id, " rotational velocity reading");

    if (!this->init) {
        this->logging.err("Failed to get sensor #", this->id, " rotational velocity reading (Instance not initialized)");
        throw except_get_rot::fail;
    }

    if (!this->mode_mea) {
        this->logging.err("Failed to get sensor #", this->id, " rotational velocity reading (Sensor not in measurement mode)");
        throw except_get_rot::fail;
    }

    if (!this->mea_rot_valid) {
        this->logging.err("Failed to get sensor #", this->id, " rotational velocity reading (Reading unavailable)");
        throw except_get_rot::fail;
    }

    this->logging.inf("Got sensor #", this->id, " rotational velocity reading: ", this->mea_rot_data);

    return this->mea_rot_data;
}

linalg::fvector instance::get_qua (void) {
    this->logging.inf("Getting sensor #", this->id, " attitude quaternion reading");

    if (!this->init) {
        this->logging.err("Failed to get sensor #", this->id, " attitude quaternion reading (Instance not initialized)");
        throw except_get_qua::fail;
    }

    if (!this->mode_mea) {
        this->logging.err("Failed to get sensor #", this->id, " attitude quaternion reading (Sensor not in measurement mode)");
        throw except_get_qua::fail;
    }

    if (!this->mea_qua_valid) {
        this->logging.err("Failed to get sensor #", this->id, " attitude quaternion reading (Reading unavailable)");
        throw except_get_qua::fail;
    }

    this->logging.inf("Got sensor #", this->id, " attitude quaternion reading: ", this->mea_qua_data);

    return this->mea_qua_data;
}

linalg::fvector instance::get_lia (void) {
    this->logging.inf("Getting sensor #", this->id, " linear acceleration reading");

    if (!this->init) {
        this->logging.err("Failed to get sensor #", this->id, " linear acceleration reading (Instance not initialized)");
        throw except_get_lia::fail;
    }

    if (!this->mode_mea) {
        this->logging.err("Failed to get sensor #", this->id, " linear acceleration reading (Sensor not in measurement mode)");
        throw except_get_lia::fail;
    }

    if (!this->mea_lia_valid) {
        this->logging.err("Failed to get sensor #", this->id, " linear acceleration reading (Reading unavailable)");
        throw except_get_lia::fail;
    }

    this->logging.inf("Got sensor #", this->id, " linear acceleration reading: ", this->mea_lia_data);

    return this->mea_lia_data;
}

bool instance::valid_rad_acc (void) {
    this->logging.inf("Checking sensor #", this->id, " accelerometer calibration radius");

    if (!this->init) {
        this->logging.err("Failed to check sensor #", this->id, " accelerometer calibration radius (Instance not initialized)");
        throw except_valid_rad_acc::fail;
    }

    if (!this->mode_cal) {
        this->logging.err("Failed to check sensor #", this->id, " accelerometer calibration radius (Sensor not in calibration mode)");
        throw except_valid_rad_acc::fail;
    }

    if (!this->cal_rad_acc_valid) {
        this->logging.err("Failed to check sensor #", this->id, " accelerometer calibration radius (Reading unavailable)");
        throw except_valid_rad_acc::fail;
    }

    if (this->cal_rad_acc_data >= -2048 && this->cal_rad_acc_data <= 2048) {
        this->logging.inf("Sensor #", this->id, " accelerometer calibration radius is valid");
        return true;
    } else {
        this->logging.inf("Sensor #", this->id, " accelerometer calibration radius is invalid");
        return false;
    }
}

bool instance::valid_rad_mag (void) {
    this->logging.inf("Checking sensor #", this->id, " magnetometer calibration radius");

    if (!this->init) {
        this->logging.err("Failed to check sensor #", this->id, " magnetometer calibration radius (Instance not initialized)");
        throw except_valid_rad_mag::fail;
    }

    if (!this->mode_cal) {
        this->logging.err("Failed to check sensor #", this->id, " magnetometer calibration radius (Sensor not in calibration mode)");
        throw except_valid_rad_mag::fail;
    }

    if (!this->cal_rad_mag_valid) {
        this->logging.err("Failed to check sensor #", this->id, " magnetometer calibration radius (Reading unavailable)");
        throw except_valid_rad_mag::fail;
    }

    if (this->cal_rad_mag_data >= 144 && this->cal_rad_mag_data <= 1280) {
        this->logging.inf("Sensor #", this->id, " magnetometer calibration radius is valid");
        return true;
    } else {
        this->logging.inf("Sensor #", this->id, " magnetometer calibration radius is invalid");
        return false;
    }
}

bool instance::valid_off_acc (void) {
    this->logging.inf("Checking sensor #", this->id, " accelerometer calibration offset");

    if (!this->init) {
        this->logging.err("Failed to check sensor #", this->id, " accelerometer calibration offset (Instance not initialized)");
        throw except_valid_off_acc::fail;
    }

    if (!this->mode_cal) {
        this->logging.err("Failed to check sensor #", this->id, " accelerometer calibration offset (Sensor not in calibration mode)");
        throw except_valid_off_acc::fail;
    }

    if (!this->cal_off_acc_valid) {
        this->logging.err("Failed to check sensor #", this->id, " accelerometer calibration offset (Reading unavailable)");
        throw except_valid_off_acc::fail;
    }

    if (this->cal_off_acc_data >= linalg::ivector({-500, -500, -500}) && this->cal_off_acc_data <= linalg::ivector({500, 500, 500})) {
        this->logging.inf("Sensor #", this->id, " accelerometer calibration offset is valid");
        return true;
    } else {
        this->logging.inf("Sensor #", this->id, " accelerometer calibration offset is invalid");
        return false;
    }
}

bool instance::valid_off_mag (void) {
    this->logging.inf("Checking sensor #", this->id, " magnetometer calibration offset");

    if (!this->init) {
        this->logging.err("Failed to check sensor #", this->id, " magnetometer calibration offset (Instance not initialized)");
        throw except_valid_off_mag::fail;
    }

    if (!this->mode_cal) {
        this->logging.err("Failed to check sensor #", this->id, " magnetometer calibration offset (Sensor not in calibration mode)");
        throw except_valid_off_mag::fail;
    }

    if (!this->cal_off_mag_valid) {
        this->logging.err("Failed to check sensor #", this->id, " magnetometer calibration offset (Reading unavailable)");
        throw except_valid_off_mag::fail;
    }

    if (this->cal_off_mag_data >= linalg::ivector({-6400, -6400, -6400}) && this->cal_off_mag_data <= linalg::ivector({6400, 6400, 6400})) {
        this->logging.inf("Sensor #", this->id, " magnetometer calibration offset is valid");
        return true;
    } else {
        this->logging.inf("Sensor #", this->id, " magnetometer calibration offset is invalid");
        return false;
    }
}

bool instance::valid_off_gyr (void) {
    this->logging.inf("Checking sensor #", this->id, " gyroscope calibration offset");

    if (!this->init) {
        this->logging.err("Failed to check sensor #", this->id, " gyroscope calibration offset (Instance not initialized)");
        throw except_valid_off_gyr::fail;
    }

    if (!this->mode_cal) {
        this->logging.err("Failed to check sensor #", this->id, " gyroscope calibration offset (Sensor not in calibration mode)");
        throw except_valid_off_gyr::fail;
    }

    if (!this->cal_off_gyr_valid) {
        this->logging.err("Failed to check sensor #", this->id, " gyroscope calibration offset (Reading unavailable)");
        throw except_valid_off_gyr::fail;
    }

    if (this->cal_off_gyr_data >= linalg::ivector({-2000, -2000, -2000}) && this->cal_off_gyr_data <= linalg::ivector({2000, 2000, 2000})) {
        this->logging.inf("Sensor #", this->id, " gyroscope calibration offset is valid");
        return true;
    } else {
        this->logging.inf("Sensor #", this->id, " gyroscope calibration offset is invalid");
        return false;
    }
}

void instance::update (void) {
    bool error = false;

    this->logging.inf("Updating sensor #", this->id, " readings");

    if (!this->init) {
        this->logging.err("Failed to update sensor #", this->id, " readings (Instance not initialized)");
        throw except_update::fail;
    }

    try {
        if (this->mode_cal) {
            this->cal_lev_sys_valid = false;
            this->cal_lev_acc_valid = false;
            this->cal_lev_mag_valid = false;
            this->cal_lev_gyr_valid = false;

            this->cal_rad_acc_valid = false;
            this->cal_rad_mag_valid = false;

            this->cal_off_acc_valid = false;
            this->cal_off_mag_valid = false;
            this->cal_off_gyr_valid = false;

            this->get_sta();
            if (this->sta_err) {
                this->get_err();
                error = true;
                throw except_intern{};
            }

            this->get_callev();

            this->get_sta();
            if (this->sta_err) {
                this->get_err();
                error = true;
                throw except_intern{};
            }

            this->get_caldat();

            this->cal_lev_sys_valid = true;
            this->cal_lev_acc_valid = true;
            this->cal_lev_mag_valid = true;
            this->cal_lev_gyr_valid = true;

            this->cal_rad_acc_valid = true;
            this->cal_rad_mag_valid = true;

            this->cal_off_acc_valid = true;
            this->cal_off_mag_valid = true;
            this->cal_off_gyr_valid = true;
        }

        if (this->mode_mea) {
            this->mea_rot_valid = false;
            this->mea_qua_valid = false;
            this->mea_lia_valid = false;

            this->get_sta();
            if (this->sta_err) {
                this->get_err();
                error = true;
                throw except_intern{};
            }

            this->get_mearot();

            this->get_sta();
            if (this->sta_err) {
                this->get_err();
                error = true;
                throw except_intern{};
            }

            this->get_meaqua();

            this->get_sta();
            if (this->sta_err) {
                this->get_err();
                error = true;
                throw except_intern{};
            }

            this->get_mealia();

            this->mea_rot_valid = true;
            this->mea_qua_valid = true;
            this->mea_lia_valid = true;
        }
    } catch (...) {
        if (error) {
            if (this->err_perinit) {
                this->logging.err("Failed to update sensor #", this->id, " readings (Sensor peripheral initialization error)");
            } else if (this->err_sysinit) {
                this->logging.err("Failed to update sensor #", this->id, " readings (Sensor system initialization error)");
            } else if (this->err_slftest) {
                this->logging.err("Failed to update sensor #", this->id, " readings (Sensor self test error)");
            } else if (this->err_regval) {
                this->logging.err("Failed to update sensor #", this->id, " readings (Sensor register value error)");
            } else if (this->err_regadr) {
                this->logging.err("Failed to update sensor #", this->id, " readings (Sensor register address error)");
            } else if (this->err_regwri) {
                this->logging.err("Failed to update sensor #", this->id, " readings (Sensor register write error)");
            } else if (this->err_lowpwr) {
                this->logging.err("Failed to update sensor #", this->id, " readings (Sensor low power mode error)");
            } else if (this->err_accpwr) {
                this->logging.err("Failed to update sensor #", this->id, " readings (Sensor accelerometer power mode error)");
            } else if (this->err_fuscfg) {
                this->logging.err("Failed to update sensor #", this->id, " readings (Sensor fusion config error)");
            } else if (this->err_syscfg) {
                this->logging.err("Failed to update sensor #", this->id, " readings (Sensor system config error)");
            } else {
                this->logging.err("Failed to update sensor #", this->id, " readings (Sensor unknown error)");
            }
        } else {
            this->logging.err("Failed to udpate sensor #", this->id, " readings");
        }
        throw except_update::fail;
    }
}

void instance::put_reg (std::uint8_t reg, std::uint8_t data) {
    std::vector<std::uint8_t> cmd = {reg, data};

    this->logging.inf("Writing data to sensor #", this->id, ": Reg: ", reg, ", Data: ", data);

    try {
        this->i2c->send(this->addr, cmd);
        this->i2c->exec();
    } catch (...) {
        this->logging.err("Failed to write data to sensor #", this->id);
        throw except_put_reg::fail;
    }
}

void instance::get_reg (std::uint8_t reg, std::vector<std::uint8_t> & data) {
    std::vector<std::uint8_t> cmd = {reg};

    this->logging.inf("Reading data from sensor #", this->id, ": Reg: ", reg, ", Len: ", int(data.size()));

    try {
        this->i2c->send(this->addr, cmd);
        this->i2c->recv(this->addr, data);
        this->i2c->exec();
    } catch (...) {
        this->logging.err("Failed to read data from sensor #", this->id);
        throw except_get_reg::fail;
    }

    this->logging.inf("Read data from sensor #", this->id, ": Data: ", data);
}

void instance::put_trg_rst (void) {
    std::uint8_t trg = 0x00;

    this->logging.inf("Resetting sensor #", this->id);

    trg = BNO055_PUT_BITS(trg, BNO055_BIT_RST, 1);

    try {
        this->put_reg(BNO055_REG_TRG, trg);
    } catch (...) {
        this->logging.err("Failed to reset sensor #", this->id);
        throw except_put_trg_rst::fail;
    }
}

void instance::put_pwr_nrm (void) {
    std::uint8_t pwr = 0x00;

    this->logging.inf("Setting sensor #", this->id, " power mode to normal");

    pwr = BNO055_PUT_BITS(pwr, BNO055_BIT_PWR, BNO055_PRST_PWR_NRM);

    try {
        this->put_reg(BNO055_REG_PWR, pwr);
    } catch (...) {
        this->logging.err("Failed to set sensor #", this->id, " power mode to normal");
        throw except_put_pwr_nrm::fail;
    }
}

void instance::put_opr_cfg (void) {
    std::uint8_t opr = 0x00;

    this->logging.inf("Setting sensor #", this->id, " operating mode to config");

    opr = BNO055_PUT_BITS(opr, BNO055_BIT_OPR, BNO055_PRST_OPR_CFG);

    try {
        this->put_reg(BNO055_REG_OPR, opr);
    } catch (...) {
        this->logging.err("Failed to set sensor #", this->id, " operating mode to config");
        throw except_put_opr_cfg::fail;
    }
}

void instance::put_opr_ndof (void) {
    std::uint8_t opr = 0x00;

    this->logging.inf("Setting sensor #", this->id, " operating mode to NDOF");

    opr = BNO055_PUT_BITS(opr, BNO055_BIT_OPR, BNO055_PRST_OPR_NDOF);

    try {
        this->put_reg(BNO055_REG_OPR, opr);
    } catch (...) {
        this->logging.err("Failed to set sensor #", this->id, " operating mode to NDOF");
        throw except_put_opr_cfg::fail;
    }
}

void instance::put_unt_si (void) {
    std::uint8_t unt = 0x00;

    this->logging.inf("Setting sensor #", this->id, " measurement units to SI");

    unt = BNO055_PUT_BITS(unt, BNO055_BIT_ORI, BNO055_PRST_ORI_ANDR);
    unt = BNO055_PUT_BITS(unt, BNO055_BIT_UNA, BNO055_PRST_UNA_MS2);
    unt = BNO055_PUT_BITS(unt, BNO055_BIT_UNG, BNO055_PRST_UNG_RPS);

    try {
        this->put_reg(BNO055_REG_UNT, unt);
    } catch (...) {
        this->logging.err("Failed to set sensor #", this->id, " measurement units to SI");
        throw except_put_unt_si::fail;
    }
}

void instance::put_axm (axis axis_x, axis axis_y, axis axis_z) {
    std::uint8_t axm = 0x00;

    this->logging.inf("Configuring sensor #", this->id, " axis mappings");

    axm = BNO055_PUT_BITS(axm, BNO055_BIT_MAPX, int(axis_x) & 0xFF);
    axm = BNO055_PUT_BITS(axm, BNO055_BIT_MAPY, int(axis_y) & 0xFF);
    axm = BNO055_PUT_BITS(axm, BNO055_BIT_MAPZ, int(axis_z) & 0xFF);

    try {
        this->put_reg(BNO055_REG_AXM, axm);
    } catch (...) {
        this->logging.err("Failed to configure sensor #", this->id, " axis mappings");
        throw except_put_axm::fail;
    }
}

void instance::put_axs (axis axis_x, axis axis_y, axis axis_z) {
    std::uint8_t axs = 0x00;

    this->logging.inf("Configuring sensor #", this->id, " axis signs");

    axs = BNO055_PUT_BITS(axs, BNO055_BIT_SGNX, int(axis_x) >> 8);
    axs = BNO055_PUT_BITS(axs, BNO055_BIT_SGNY, int(axis_y) >> 8);
    axs = BNO055_PUT_BITS(axs, BNO055_BIT_SGNZ, int(axis_z) >> 8);

    try {
        this->put_reg(BNO055_REG_AXS, axs);
    } catch (...) {
        this->logging.err("Failed to configure sensor #", this->id, " axis signs");
        throw except_put_axs::fail;
    }
}

void instance::put_caldat (
    int rad_acc, int rad_mag, const linalg::ivector & off_acc, const linalg::ivector & off_mag, const linalg::ivector & off_gyr
) {
    this->logging.inf("Configuring sensor #", this->id, " calibration parameters");

    try {
        this->put_reg(BNO055_REG_CALRADACCL, std::int16_t(rad_acc) & 0xFF);
        this->put_reg(BNO055_REG_CALRADACCM, std::int16_t(rad_acc) >> 8);
        this->put_reg(BNO055_REG_CALRADMAGL, std::int16_t(rad_mag) & 0xFF);
        this->put_reg(BNO055_REG_CALRADMAGM, std::int16_t(rad_mag) >> 8);
        this->put_reg(BNO055_REG_CALOFFACCXL, std::int16_t(off_acc[0]) & 0xFF);
        this->put_reg(BNO055_REG_CALOFFACCXM, std::int16_t(off_acc[0]) >> 8);
        this->put_reg(BNO055_REG_CALOFFACCYL, std::int16_t(off_acc[1]) & 0xFF);
        this->put_reg(BNO055_REG_CALOFFACCYM, std::int16_t(off_acc[1]) >> 8);
        this->put_reg(BNO055_REG_CALOFFACCZL, std::int16_t(off_acc[2]) & 0xFF);
        this->put_reg(BNO055_REG_CALOFFACCZM, std::int16_t(off_acc[2]) >> 8);
        this->put_reg(BNO055_REG_CALOFFMAGXL, std::int16_t(off_mag[0]) & 0xFF);
        this->put_reg(BNO055_REG_CALOFFMAGXM, std::int16_t(off_mag[0]) >> 8);
        this->put_reg(BNO055_REG_CALOFFMAGYL, std::int16_t(off_mag[1]) & 0xFF);
        this->put_reg(BNO055_REG_CALOFFMAGYM, std::int16_t(off_mag[1]) >> 8);
        this->put_reg(BNO055_REG_CALOFFMAGZL, std::int16_t(off_mag[2]) & 0xFF);
        this->put_reg(BNO055_REG_CALOFFMAGZM, std::int16_t(off_mag[2]) >> 8);
        this->put_reg(BNO055_REG_CALOFFGYRXL, std::int16_t(off_gyr[0]) & 0xFF);
        this->put_reg(BNO055_REG_CALOFFGYRXM, std::int16_t(off_gyr[0]) >> 8);
        this->put_reg(BNO055_REG_CALOFFGYRYL, std::int16_t(off_gyr[1]) & 0xFF);
        this->put_reg(BNO055_REG_CALOFFGYRYM, std::int16_t(off_gyr[1]) >> 8);
        this->put_reg(BNO055_REG_CALOFFGYRZL, std::int16_t(off_gyr[2]) & 0xFF);
        this->put_reg(BNO055_REG_CALOFFGYRZM, std::int16_t(off_gyr[2]) >> 8);
    } catch (...) {
        this->logging.err("Failed to configure sensor #", this->id, " calibration parameters");
        throw except_put_caldat::fail;
    }
}

void instance::get_sta (void) {
    std::vector<std::uint8_t> sta(1);

    this->logging.inf("Getting sensor #", this->id, " status flags");

    try {
        this->get_reg(BNO055_REG_STA, sta);
    } catch (...) {
        this->logging.err("Failed to get sensor #", this->id, " status flags");
        throw except_get_sta::fail;
    }

    this->sta_idl = false;
    this->sta_err = false;
    this->sta_perinit = false;
    this->sta_sysinit = false;
    this->sta_slftest = false;
    this->sta_fus = false;
    this->sta_nfus = false;

    switch (sta[0]) {
        case BNO055_PRST_STA_IDL:
            this->sta_idl = true;
            break;
        case BNO055_PRST_STA_ERR:
            this->sta_err = true;
            break;
        case BNO055_PRST_STA_PERINIT:
            this->sta_perinit = true;
            break;
        case BNO055_PRST_STA_SYSINIT:
            this->sta_sysinit = true;
            break;
        case BNO055_PRST_STA_SLFTEST:
            this->sta_slftest = true;
            break;
        case BNO055_PRST_STA_FUS:
            this->sta_fus = true;
            break;
        case BNO055_PRST_STA_NFUS:
            this->sta_nfus = true;
            break;
    }

    this->logging.inf(
        "Got sensor #", this->id, " status flags: ",
        "Idle: ", this->sta_idl, ", ",
        "Error: ", this->sta_err, ", ",
        "Periph init: ", this->sta_perinit, ", ",
        "Sys init: ", this->sta_sysinit, ", ",
        "Self test: ", this->sta_slftest, ", ",
        "Fusion: ", this->sta_fus, ", ",
        "Non fusion: ", this->sta_nfus
    );
}

void instance::get_err (void) {
    std::vector<std::uint8_t> err(1);

    this->logging.inf("Getting sensor #", this->id, " error flags");

    try {
        this->get_reg(BNO055_REG_ERR, err);
    } catch (...) {
        this->logging.err("Failed to get sensor #", this->id, " error flags");
        throw except_get_err::fail;
    }

    this->err_perinit = false;
    this->err_sysinit = false;
    this->err_slftest = false;
    this->err_regval = false;
    this->err_regadr = false;
    this->err_regwri = false;
    this->err_lowpwr = false;
    this->err_accpwr = false;
    this->err_fuscfg = false;
    this->err_syscfg = false;

    switch (err[0]) {
        case BNO055_PRST_ERR_PERINIT:
            this->err_perinit = true;
            break;
        case BNO055_PRST_ERR_SYSINIT:
            this->err_sysinit = true;
            break;
        case BNO055_PRST_ERR_SLFTEST:
            this->err_slftest = true;
            break;
        case BNO055_PRST_ERR_REGVAL:
            this->err_regval = true;
            break;
        case BNO055_PRST_ERR_REGADR:
            this->err_regadr = true;
            break;
        case BNO055_PRST_ERR_REGWRI:
            this->err_regwri = true;
            break;
        case BNO055_PRST_ERR_LOWPWR:
            this->err_lowpwr = true;
            break;
        case BNO055_PRST_ERR_ACCPWR:
            this->err_accpwr = true;
            break;
        case BNO055_PRST_ERR_FUSCFG:
            this->err_fuscfg = true;
            break;
        case BNO055_PRST_ERR_SYSCFG:
            this->err_syscfg = true;
            break;
    }

    this->logging.inf(
        "Got sensor #", this->id, " error flags: ",
        "Periph init error: ", this->err_perinit, ", ",
        "Sys init error: ", this->err_sysinit, ", ",
        "Self test error: ", this->err_slftest, ", ",
        "Reg value error: ", this->err_regval, ", ",
        "Reg addr error: ", this->err_regadr, ", ",
        "Reg write error: ", this->err_regwri, ", ",
        "Low pwr error: ", this->err_lowpwr, ", ",
        "Acc pwr error: ", this->err_accpwr, ", ",
        "Fusion config error: ", this->err_fuscfg, ", ",
        "Sys config error: ", this->err_syscfg
    );
}

void instance::get_callev (void) {
    std::vector<std::uint8_t> callev(1);

    this->logging.inf("Getting sensor #", this->id, " calibration levels");

    try {
        this->get_reg(BNO055_REG_CALLEV, callev);
    } catch (...) {
        this->logging.err("Failed to get sensor #", this->id, " calibration levels");
        throw except_get_callev::fail;
    }

    this->cal_lev_sys_data = BNO055_GET_BITS(callev[0], BNO055_BIT_LEVSYS);
    this->cal_lev_acc_data = BNO055_GET_BITS(callev[0], BNO055_BIT_LEVACC);
    this->cal_lev_mag_data = BNO055_GET_BITS(callev[0], BNO055_BIT_LEVMAG);
    this->cal_lev_gyr_data = BNO055_GET_BITS(callev[0], BNO055_BIT_LEVGYR);

    this->logging.inf(
        "Got sensor #", this->id, " calibration levels: ",
        "Sys level: ", this->cal_lev_sys_data, ", ",
        "Acc level: ", this->cal_lev_acc_data, ", ",
        "Mag level: ", this->cal_lev_mag_data, ", ",
        "Gyr level: ", this->cal_lev_gyr_data
    );
}

void instance::get_caldat (void) {
    std::vector<std::uint8_t> caldat(22);

    this->logging.inf("Getting sensor #", this->id, " calibration parameters");

    try {
        this->get_reg(BNO055_REG_CALDAT, caldat);
    } catch (...) {
        this->logging.err("Failed to get sensor #", this->id, " calibration parameters");
        throw except_get_caldat::fail;
    }

    this->cal_rad_acc_data = std::int16_t(caldat[18] | caldat[19] << 8);
    this->cal_rad_mag_data = std::int16_t(caldat[20] | caldat[21] << 8);

    this->cal_off_acc_data[0] = std::int16_t(caldat[0] | caldat[1] << 8);
    this->cal_off_acc_data[1] = std::int16_t(caldat[2] | caldat[3] << 8);
    this->cal_off_acc_data[2] = std::int16_t(caldat[4] | caldat[5] << 8);

    this->cal_off_mag_data[0] = std::int16_t(caldat[6] | caldat[7] << 8);
    this->cal_off_mag_data[1] = std::int16_t(caldat[8] | caldat[9] << 8);
    this->cal_off_mag_data[2] = std::int16_t(caldat[10] | caldat[11] << 8);

    this->cal_off_gyr_data[0] = std::int16_t(caldat[12] | caldat[13] << 8);
    this->cal_off_gyr_data[1] = std::int16_t(caldat[14] | caldat[15] << 8);
    this->cal_off_gyr_data[2] = std::int16_t(caldat[16] | caldat[17] << 8);

    this->logging.inf(
        "Got sensor #", this->id, " calibration parameters: ",
        "Acc radius: ", this->cal_rad_acc_data, ", ",
        "Mag radius: ", this->cal_rad_mag_data, ", ",
        "Acc offset: ", this->cal_off_acc_data, ", ",
        "Mag offset: ", this->cal_off_mag_data, ", ",
        "Gyr offset: ", this->cal_off_gyr_data
    );
}

void instance::get_mearot (void) {
    std::vector<std::uint8_t> mearot(6);

    this->logging.inf("Getting sensor #", this->id, " rotational velocity measurement data");

    try {
        this->get_reg(BNO055_REG_MEAROT, mearot);
    } catch (...) {
        this->logging.err("Failed to get sensor #", this->id, " rotational velocity measurement data");
        throw except_get_mearot::fail;
    }

    this->mea_rot_data[0] = std::int16_t(mearot[0] | mearot[1] << 8);
    this->mea_rot_data[1] = std::int16_t(mearot[2] | mearot[3] << 8);
    this->mea_rot_data[2] = std::int16_t(mearot[4] | mearot[5] << 8);

    this->mea_rot_data /= 900;

    this->logging.inf("Got sensor #", this->id, " rotational velocity measurement data: ", this->mea_rot_data);
}

void instance::get_meaqua (void) {
    std::vector<std::uint8_t> meaqua(8);

    this->logging.inf("Getting sensor #", this->id, " attitude quaternion measurement data");

    try {
        this->get_reg(BNO055_REG_MEAQUA, meaqua);
    } catch (...) {
        this->logging.err("Failed to get sensor #", this->id, " attitude quaternion measurement data");
        throw except_get_meaqua::fail;
    }

    this->mea_qua_data[0] = std::int16_t(meaqua[0] | meaqua[1] << 8);
    this->mea_qua_data[1] = std::int16_t(meaqua[2] | meaqua[3] << 8);
    this->mea_qua_data[2] = std::int16_t(meaqua[4] | meaqua[5] << 8);
    this->mea_qua_data[3] = std::int16_t(meaqua[6] | meaqua[7] << 8);

    this->mea_qua_data /= 16384;

    this->logging.inf("Got sensor #", this->id, " attitude quaternion measurement data: ", this->mea_qua_data);
}

void instance::get_mealia (void) {
    std::vector<std::uint8_t> mealia(6);

    this->logging.inf("Getting sensor #", this->id, " linear acceleration measurement data");

    try {
        this->get_reg(BNO055_REG_MEALIA, mealia);
    } catch (...) {
        this->logging.err("Failed to get sensor #", this->id, " linear acceleration data");
        throw except_get_mealia::fail;
    }

    this->mea_lia_data[0] = std::int16_t(mealia[0] | mealia[1] << 8);
    this->mea_lia_data[1] = std::int16_t(mealia[2] | mealia[3] << 8);
    this->mea_lia_data[2] = std::int16_t(mealia[4] | mealia[5] << 8);

    this->mea_lia_data /= 100;

    this->logging.inf("Got sensor #", this->id, " linear acceleration measurement data: ", this->mea_lia_data);
}

logging::stream & operator << (logging::stream & strm, axis axis) {
    switch (axis) {
        case axis::px:
            strm << "+X";
            return strm;
        case axis::nx:
            strm << "-X";
            return strm;
        case axis::py:
            strm << "+Y";
            return strm;
        case axis::ny:
            strm << "-Y";
            return strm;
        case axis::pz:
            strm << "+Z";
            return strm;
        case axis::nz:
            strm << "-Z";
            return strm;
        default:
            return strm;
    }
}

}

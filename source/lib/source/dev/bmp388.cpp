#include <cstdint>
#include <cmath>

#include <vector>

#include <sys/logging.hpp>
#include <dev/i2c.hpp>
#include <dev/bmp388.hpp>

namespace bmp388 {

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

instance::instance (i2c::instance & i2c, int id, int drate, int ovsmp_pres, int ovsmp_temp, int iir)
  : logging("bmp388"), init(false), id(id),
    i2c(&i2c), addr(id == 1 ? BMP388_ADDR1 : id == 2 ? BMP388_ADDR2 : 0),
    sta_crdy(false), sta_drdyp(false), sta_drdyt(false),
    err_ftl(false), err_cmd(false), err_cfg(false),
    cal_pres_data(11), cal_temp_data(3),
    mea_pres_valid(false), mea_temp_valid(false) {

    bool error = false;

    this->logging.inf(
        "Initializing instance #", this->id, ": ",
        "Data rate code: ", drate, ", Pres ovsmp code: ", ovsmp_pres, ", Temp ovsmp code: ", ovsmp_temp, ", IIR code: ", iir
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

    if (drate < 0 || drate > 17) {
        this->logging.err("Failed to configure sensor #", this->id, " (Invalid data rate code)");
        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    if (ovsmp_pres < 0 || ovsmp_pres > 5) {
        this->logging.err("Failed to configure sensor #", this->id, " (Invalid pressure oversampling code)");
        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    if (ovsmp_temp < 0 || ovsmp_temp > 5) {
        this->logging.err("Failed to configure sensor #", this->id, " (Invalid temperature oversampling code)");
        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    if (iir < 0 || iir > 7) {
        this->logging.err("Failed to configure sensor #", this->id, " (Invalid IIR filter code)");
        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    try {
        do {
            this->get_err();
            if (this->err_ftl || this->err_cmd || this->err_cfg) {
                error = true;
                throw except_intern{};
            }
            this->get_sta();
        } while (!this->sta_crdy);

        this->put_cmd_rst();
        this->get_err();
        if (this->err_ftl || this->err_cmd || this->err_cfg) {
            error = true;
            throw except_intern{};
        }

        this->put_pwr_slp();
        this->get_err();
        if (this->err_ftl || this->err_cmd || this->err_cfg) {
            error = true;
            throw except_intern{};
        }

        this->put_odr(drate);
        this->get_err();
        if (this->err_ftl || this->err_cmd || this->err_cfg) {
            error = true;
            throw except_intern{};
        }

        this->put_osr(ovsmp_pres, ovsmp_temp);
        this->get_err();
        if (this->err_ftl || this->err_cmd || this->err_cfg) {
            error = true;
            throw except_intern{};
        }

        this->put_cfg(iir);
        this->get_err();
        if (this->err_ftl || this->err_cmd || this->err_cfg) {
            error = true;
            throw except_intern{};
        }

        this->get_cal();

        this->put_pwr_nrm();
        this->get_err();
        if (this->err_ftl || this->err_cmd || this->err_cfg) {
            error = true;
            throw except_intern{};
        }
    } catch (...) {
        if (error) {
            if (this->err_ftl) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor fatal error)");
            } else if (this->err_cmd) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor command error)");
            } else if (this->err_cfg) {
                this->logging.err("Failed to configure sensor #", this->id, " (Sensor config error)");
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

double instance::get_pres (void) {
    this->logging.inf("Getting sensor #", this->id, " pressure reading");

    if (!this->init) {
        this->logging.err("Failed to get sensor #", this->id, " pressure reading (Instance not initialized)");
        throw except_get_pres::fail;
    }

    if (!this->mea_pres_valid) {
        this->logging.err("Failed to get sensor #", this->id, " pressure reading (Reading unavailable)");
        throw except_get_pres::fail;
    }

    this->logging.inf("Got sensor #", this->id, " pressure reading: ", this->mea_pres_data);

    return this->mea_pres_data;
}

double instance::get_temp (void) {
    this->logging.inf("Getting sensor #", this->id, " temperature reading");

    if (!this->init) {
        this->logging.err("Failed to get sensor #", this->id, " temperature reading (Instance not initialized)");
        throw except_get_temp::fail;
    }

    if (!this->mea_temp_valid) {
        this->logging.err("Failed to get sensor #", this->id, " temperature reading (Reading unavailable)");
        throw except_get_temp::fail;
    }

    this->logging.inf("Got sensor #", this->id, " temperature reading: ", this->mea_temp_data);

    return this->mea_temp_data;
}

void instance::update (void) {
    bool error = false, blank = false;

    this->logging.inf("Updating sensor #", this->id, " readings");

    if (!this->init) {
        this->logging.err("Failed to update sensor #", this->id, " readings (Instance not initialized)");
        throw except_update::fail;
    }

    try {
        this->mea_pres_valid = false;
        this->mea_temp_valid = false;

        this->get_err();
        if (this->err_ftl || this->err_cmd || this->err_cfg) {
            error = true;
            throw except_intern{};
        }

        this->get_sta();
        if (!this->sta_drdyp || !this->sta_drdyt) {
            blank = true;
            throw except_intern{};
        }

        this->get_err();
        if (this->err_ftl || this->err_cmd || this->err_cfg) {
            error = true;
            throw except_intern{};
        }

        this->get_mea();

        this->mea_pres_valid = true;
        this->mea_temp_valid = true;
    } catch (...) {
        if (blank) {
            this->logging.wrn("Sensor #", this->id, " measurement incomplete");
            throw except_update::blank;
        } else if (error) {
            if (this->err_ftl) {
                this->logging.err("Failed to update sensor #", this->id, " readings (Sensor fatal error)");
            } else if (this->err_cmd) {
                this->logging.err("Failed to update sensor #", this->id, " readings (Sensor command error)");
            } else if (this->err_cfg) {
                this->logging.err("Failed to update sensor #", this->id, " readings (Sensor config error)");
            } else {
                this->logging.err("Failed to update sensor #", this->id, " readings (Sensor unknown error)");
            }
            throw except_update::fail;
        } else {
            this->logging.err("Failed to update sensor #", this->id, " readings");
            throw except_update::fail;
        }
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

void instance::put_cmd_rst (void) {
    std::uint8_t cmd = BMP388_PRST_CMD_RST;

    this->logging.inf("Resetting sensor #", this->id);

    try {
        this->put_reg(BMP388_REG_CMD, cmd);
    } catch (...) {
        this->logging.err("Failed to reset sensor #", this->id);
        throw except_put_cmd_rst::fail;
    }
}

void instance::put_pwr_slp (void) {
    std::uint8_t pwr = 0x00;

    this->logging.inf("Setting sensor #", this->id, " power mode to sleep");

    pwr = BMP388_PUT_BITS(pwr, BMP388_BIT_ENABP, 1);
    pwr = BMP388_PUT_BITS(pwr, BMP388_BIT_ENABT, 1);
    pwr = BMP388_PUT_BITS(pwr, BMP388_BIT_MODE, BMP388_PRST_MODE_SLP);

    try {
        this->put_reg(BMP388_REG_PWR, pwr);
    } catch (...) {
        this->logging.err("Failed to set sensor #", this->id, " power mode to sleep");
        throw except_put_pwr_slp::fail;
    }
}

void instance::put_pwr_nrm (void) {
    std::uint8_t pwr = 0x00;

    this->logging.inf("Setting sensor #", this->id, " power mode to normal");

    pwr = BMP388_PUT_BITS(pwr, BMP388_BIT_ENABP, 1);
    pwr = BMP388_PUT_BITS(pwr, BMP388_BIT_ENABT, 1);
    pwr = BMP388_PUT_BITS(pwr, BMP388_BIT_MODE, BMP388_PRST_MODE_NRM);

    try {
        this->put_reg(BMP388_REG_PWR, pwr);
    } catch (...) {
        this->logging.err("Failed to set sensor #", this->id, " power mode to normal");
        throw except_put_pwr_nrm::fail;
    }
}

void instance::put_odr (int drate) {
    std::uint8_t odr = 0x00;

    this->logging.inf("Configuring sensor #", this->id, " data rate");

    odr = BMP388_PUT_BITS(odr, BMP388_BIT_ODR, drate);

    try {
        this->put_reg(BMP388_REG_ODR, odr);
    } catch (...) {
        this->logging.err("Failed to configure sensor #", this->id, " data rate");
        throw except_put_odr::fail;
    }
}

void instance::put_osr (int ovsmp_pres, int ovsmp_temp) {
    std::uint8_t osr = 0x00;

    this->logging.inf("Configuring sensor #", this->id, " oversampling");

    osr = BMP388_PUT_BITS(osr, BMP388_BIT_OSRP, ovsmp_pres);
    osr = BMP388_PUT_BITS(osr, BMP388_BIT_OSRT, ovsmp_temp);

    try {
        this->put_reg(BMP388_REG_OSR, osr);
    } catch (...) {
        this->logging.err("Failed to configure sensor #", this->id, " oversampling");
        throw except_put_osr::fail;
    }
}

void instance::put_cfg (int iir) {
    std::uint8_t cfg = 0x00;

    this->logging.inf("Configuring sensor #", this->id, " IIR filter");

    cfg = BMP388_PUT_BITS(cfg, BMP388_BIT_IIR, iir);

    try {
        this->put_reg(BMP388_REG_CFG, cfg);
    } catch (...) {
        this->logging.err("Failed to configure sensor #", this->id, " IIR filter");
        throw except_put_cfg::fail;
    }
}

void instance::get_sta (void) {
    std::vector<std::uint8_t> sta(1);

    this->logging.inf("Getting sensor #", this->id, " status flags");

    try {
        this->get_reg(BMP388_REG_STA, sta);
    } catch (...) {
        this->logging.err("Failed to get sensor #", this->id, " status flags");
        throw except_get_sta::fail;
    }

    if (BMP388_GET_BITS(sta[0], BMP388_BIT_CRDY)) {
        this->sta_crdy = true;
    } else {
        this->sta_crdy = false;
    }

    if (BMP388_GET_BITS(sta[0], BMP388_BIT_DRDYP)) {
        this->sta_drdyp = true;
    } else {
        this->sta_drdyp = false;
    }

    if (BMP388_GET_BITS(sta[0], BMP388_BIT_DRDYT)) {
        this->sta_drdyt = true;
    } else {
        this->sta_drdyt = false;
    }

    this->logging.inf(
        "Got sensor #", this->id, " status flags: ",
        "Cmd ready: ", this->sta_crdy, ", ",
        "Pres data ready: ", this->sta_drdyp, ", ",
        "Temp data ready: ", this->sta_drdyt
    );
}

void instance::get_err (void) {
    std::vector<std::uint8_t> err(1);

    this->logging.inf("Getting sensor #", this->id, " error flags");

    try {
        this->get_reg(BMP388_REG_ERR, err);
    } catch (...) {
        this->logging.err("Failed to get sensor #", this->id, " error flags");
        throw except_get_err::fail;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_FTL)) {
        this->err_ftl = true;
    } else {
        this->err_ftl = false;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CMD)) {
        this->err_cmd = true;
    } else {
        this->err_cmd = false;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CFG)) {
        this->err_cfg = true;
    } else {
        this->err_cfg = false;
    }

    this->logging.inf(
        "Got sensor #", this->id, " error flags: ",
        "Ftl error: ", this->err_ftl, ", ",
        "Cmd error: ", this->err_cmd, ", ",
        "Cfg error: ", this->err_cfg
    );
}

void instance::get_cal (void) {
    std::vector<std::uint8_t> cal(21);

    this->logging.inf("Getting sensor #", this->id, " calibration parameters");

    try {
        this->get_reg(BMP388_REG_CAL, cal);
    } catch (...) {
        this->logging.err("Failed to get sensor #", this->id, " calibration parameters");
        throw except_get_cal::fail;
    }

    this->cal_pres_data[0] = double(std::int16_t(cal[5] | cal[6] << 8));
    this->cal_pres_data[1] = double(std::int16_t(cal[7] | cal[8] << 8));
    this->cal_pres_data[2] = double(std::int8_t(cal[9]));
    this->cal_pres_data[3] = double(std::int8_t(cal[10]));
    this->cal_pres_data[4] = double(std::uint16_t(cal[11] | cal[12] << 8));
    this->cal_pres_data[5] = double(std::uint16_t(cal[13] | cal[14] << 8));
    this->cal_pres_data[6] = double(std::int8_t(cal[15]));
    this->cal_pres_data[7] = double(std::int8_t(cal[16]));
    this->cal_pres_data[8] = double(std::int16_t(cal[17] | cal[18] << 8));
    this->cal_pres_data[9] = double(std::int8_t(cal[19]));
    this->cal_pres_data[10] = double(std::int8_t(cal[20]));

    this->cal_temp_data[0] = double(std::uint16_t(cal[0] | cal[1] << 8));
    this->cal_temp_data[1] = double(std::uint16_t(cal[2] | cal[3] << 8));
    this->cal_temp_data[2] = double(std::int8_t(cal[4]));

    this->cal_pres_data[0] -= std::pow(2, 14);
    this->cal_pres_data[1] -= std::pow(2, 14);

    this->cal_pres_data[0] /= std::pow(2, 20);
    this->cal_pres_data[1] /= std::pow(2, 29);
    this->cal_pres_data[2] /= std::pow(2, 32);
    this->cal_pres_data[3] /= std::pow(2, 37);
    this->cal_pres_data[4] *= std::pow(2, 3);
    this->cal_pres_data[5] /= std::pow(2, 6);
    this->cal_pres_data[6] /= std::pow(2, 8);
    this->cal_pres_data[7] /= std::pow(2, 15);
    this->cal_pres_data[8] /= std::pow(2, 48);
    this->cal_pres_data[9] /= std::pow(2, 48);
    this->cal_pres_data[10] /= std::pow(2, 65);

    this->cal_temp_data[0] *= std::pow(2, 8);
    this->cal_temp_data[1] /= std::pow(2, 30);
    this->cal_temp_data[2] /= std::pow(2, 48);

    this->logging.inf(
        "Got sensor #", this->id, " calibration parameters: Pres calib: ", this->cal_pres_data, ", Temp calib: ", this->cal_temp_data
    );
}

void instance::get_mea (void) {
    std::vector<std::uint8_t> mea(6);
    double raw_pres, raw_temp;
    double aux[6];

    this->logging.inf("Getting sensor #", this->id, " measurement data");

    try {
        this->get_reg(BMP388_REG_MEA, mea);
    } catch (...) {
        this->logging.err("Failed to get sensor #", this->id, " measurement data");
        throw except_get_mea::fail;
    }

    raw_pres = double(std::uint32_t(mea[0] | mea[1] << 8 | mea[2] << 16));
    raw_temp = double(std::uint32_t(mea[3] | mea[4] << 8 | mea[5] << 16));

    aux[0] = raw_temp - this->cal_temp_data[0];
    aux[1] = aux[0] * this->cal_temp_data[1];

    this->mea_temp_data = aux[1] + aux[0] * aux[0] * this->cal_temp_data[2];

    aux[0] = this->cal_pres_data[5] * this->mea_temp_data;
    aux[1] = this->cal_pres_data[6] * this->mea_temp_data * this->mea_temp_data;
    aux[2] = this->cal_pres_data[7] * this->mea_temp_data * this->mea_temp_data * this->mea_temp_data;
    aux[4] = this->cal_pres_data[4] + aux[0] + aux[1] + aux[2];

    aux[0] = this->cal_pres_data[1] * this->mea_temp_data;
    aux[1] = this->cal_pres_data[2] * this->mea_temp_data * this->mea_temp_data;
    aux[2] = this->cal_pres_data[3] * this->mea_temp_data * this->mea_temp_data * this->mea_temp_data;
    aux[5] = raw_pres * (this->cal_pres_data[0] + aux[0] + aux[1] + aux[2]);

    aux[0] = raw_pres * raw_pres;
    aux[1] = this->cal_pres_data[8] + this->cal_pres_data[9] * this->mea_temp_data;
    aux[2] = aux[0] * aux[1];
    aux[3] = aux[2] + this->cal_pres_data[10] * raw_pres * raw_pres * raw_pres;

    this->mea_pres_data = aux[3] + aux[4] + aux[5];

    this->mea_temp_data += 273.15;

    this->logging.inf("Got sensor #", this->id, " measurement data: Pres: ", this->mea_pres_data, ", Temp: ", this->mea_temp_data);
}

}

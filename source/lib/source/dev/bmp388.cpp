#include <cstddef>
#include <cstdint>
#include <cmath>

#include <string>
#include <vector>

#include <sys/logging.hpp>

#include <dev/i2c.hpp>
#include <dev/bmp388.hpp>

namespace bmp388 :: intern {

std::string mod = "bmp388";

bool fail;
bool incomp;

std::vector<std::vector<double>> cal_pres(2, std::vector<double>(11));
std::vector<std::vector<double>> cal_temp(2, std::vector<double>(3));

std::vector<double> pres(2);
std::vector<double> temp(2);

std::vector<bool> valid_pres(2);
std::vector<bool> valid_temp(2);

std::vector<bool> sta_crdy(2);
std::vector<bool> sta_drdyp(2);
std::vector<bool> sta_drdyt(2);

std::vector<bool> err_ftl(2);
std::vector<bool> err_cmd(2);
std::vector<bool> err_cfg(2);

void put_reg (int id, std::uint8_t reg, std::uint8_t dat);
std::vector<std::uint8_t> get_reg (int id, std::uint8_t reg, std::size_t len);

void put_cmd_rst (int id);
void put_pwr_slp (int id);
void put_pwr_nrm (int id);
void put_odr (int id, drate drate);
void put_osr (int id, ovsmp ovsmp_pres, ovsmp ovsmp_temp);
void put_cfg (int id, iir iir);

void get_sta (int id);
void get_err (int id);
void get_cal (int id);
void get_dat (int id);

}

namespace bmp388 {

const drate drate_200_hz(BMP388_PRST_ODR_200_HZ, "200Hz");
const drate drate_100_hz(BMP388_PRST_ODR_100_HZ, "100Hz");
const drate drate_50_hz(BMP388_PRST_ODR_50_HZ, "50Hz");
const drate drate_25_hz(BMP388_PRST_ODR_25_HZ, "25Hz");
const drate drate_25_2_hz(BMP388_PRST_ODR_25_2_HZ, "25/2Hz");
const drate drate_25_4_hz(BMP388_PRST_ODR_25_4_HZ, "25/4Hz");
const drate drate_25_8_hz(BMP388_PRST_ODR_25_8_HZ, "25/8Hz");
const drate drate_25_16_hz(BMP388_PRST_ODR_25_16_HZ, "25/16Hz");
const drate drate_25_32_hz(BMP388_PRST_ODR_25_32_HZ, "25/32Hz");
const drate drate_25_64_hz(BMP388_PRST_ODR_25_64_HZ, "25/64Hz");
const drate drate_25_128_hz(BMP388_PRST_ODR_25_128_HZ, "25/128Hz");
const drate drate_25_256_hz(BMP388_PRST_ODR_25_256_HZ, "25/256Hz");
const drate drate_25_512_hz(BMP388_PRST_ODR_25_512_HZ, "25/512Hz");
const drate drate_25_1024_hz(BMP388_PRST_ODR_25_1024_HZ, "25/1024Hz");
const drate drate_25_2048_hz(BMP388_PRST_ODR_25_2048_HZ, "25/2048Hz");
const drate drate_25_4096_hz(BMP388_PRST_ODR_25_4096_HZ, "25/4096Hz");
const drate drate_25_8192_hz(BMP388_PRST_ODR_25_8192_HZ, "25/8192Hz");
const drate drate_25_16384_hz(BMP388_PRST_ODR_25_16384_HZ, "25/16384Hz");

const ovsmp ovsmp_1x(BMP388_PRST_OSR_1X, "1x");
const ovsmp ovsmp_2x(BMP388_PRST_OSR_2X, "2x");
const ovsmp ovsmp_4x(BMP388_PRST_OSR_4X, "4x");
const ovsmp ovsmp_8x(BMP388_PRST_OSR_8X, "8x");
const ovsmp ovsmp_16x(BMP388_PRST_OSR_16X, "16x");
const ovsmp ovsmp_32x(BMP388_PRST_OSR_32X, "32x");

const iir iir_0(BMP388_PRST_IIR_0, "0");
const iir iir_1(BMP388_PRST_IIR_1, "1");
const iir iir_3(BMP388_PRST_IIR_3, "3");
const iir iir_7(BMP388_PRST_IIR_7, "7");
const iir iir_15(BMP388_PRST_IIR_15, "15");
const iir iir_31(BMP388_PRST_IIR_31, "31");
const iir iir_63(BMP388_PRST_IIR_63, "63");
const iir iir_127(BMP388_PRST_IIR_127, "127");

drate :: drate (int val, const char * text) {
    this->val = val;
    this->text = text;
}

drate :: operator int (void) const {
    return this->val;
}

drate :: operator logging::buffer (void) const {
    return logging::buffer(this->text);
}

ovsmp :: ovsmp (int val, const char * text) {
    this->val = val;
    this->text = text;
}

ovsmp :: operator int (void) const {
    return this->val;
}

ovsmp :: operator logging::buffer (void) const {
    return logging::buffer(this->text);
}

iir :: iir (int val, const char * text) {
    this->val = val;
    this->text = text;
}

iir :: operator int (void) const {
    return this->val;
}

iir :: operator logging::buffer (void) const {
    return logging::buffer(this->text);
}

bool fail (void) {
    return intern::fail;
}

bool incomp (void) {
    return intern::incomp;
}

void init (int id, drate drate, ovsmp ovsmp_pres, ovsmp ovsmp_temp, iir iir) {
    logging::inf(intern::mod,
        "Initializing sensor #", id, ": Data rate: ", drate, ", Pres ovsmp: ", ovsmp_pres, ", Temp ovsmp: ", ovsmp_temp, ", IIR coef: ", iir
    );

    if (id != 1 && id != 2) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor does not exist)"
        );
        intern::fail = true;
        return;
    }

    do {
        intern::get_err(id);
        if (intern::fail) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id
            );
            intern::fail = true;
            return;
        }

        if (intern::err_ftl[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor fatal error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_cmd[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor command error)"
            );
            intern::fail = true;
            return;
        }

        if (intern::err_cfg[id - 1]) {
            logging::err(intern::mod,
                "Failed to initialize sensor #", id, " (Sensor config error)"
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
    } while (!intern::sta_crdy[id - 1]);

    intern::put_cmd_rst(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::get_err(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    if (intern::err_ftl[id - 1]) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor fatal error)"
        );
        intern::fail = true;
        return;
    }

    if (intern::err_cmd[id - 1]) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor command error)"
        );
        intern::fail = true;
        return;
    }

    if (intern::err_cfg[id - 1]) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor config error)"
        );
        intern::fail = true;
        return;
    }

    intern::put_pwr_slp(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::get_err(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    if (intern::err_ftl[id - 1]) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor fatal error)"
        );
        intern::fail = true;
        return;
    }

    if (intern::err_cmd[id - 1]) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor command error)"
        );
        intern::fail = true;
        return;
    }

    if (intern::err_cfg[id - 1]) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor config error)"
        );
        intern::fail = true;
        return;
    }

    intern::put_odr(id, drate);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::get_err(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    if (intern::err_ftl[id - 1]) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor fatal error)"
        );
        intern::fail = true;
        return;
    }

    if (intern::err_cmd[id - 1]) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor command error)"
        );
        intern::fail = true;
        return;
    }

    if (intern::err_cfg[id - 1]) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor config error)"
        );
        intern::fail = true;
        return;
    }

    intern::put_osr(id, ovsmp_pres, ovsmp_temp);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::get_err(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    if (intern::err_ftl[id - 1]) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor fatal error)"
        );
        intern::fail = true;
        return;
    }

    if (intern::err_cmd[id - 1]) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor command error)"
        );
        intern::fail = true;
        return;
    }

    if (intern::err_cfg[id - 1]) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor config error)"
        );
        intern::fail = true;
        return;
    }

    intern::put_cfg(id, iir);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::get_err(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    if (intern::err_ftl[id - 1]) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor fatal error)"
        );
        intern::fail = true;
        return;
    }

    if (intern::err_cmd[id - 1]) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor command error)"
        );
        intern::fail = true;
        return;
    }

    if (intern::err_cfg[id - 1]) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor config error)"
        );
        intern::fail = true;
        return;
    }

    intern::get_cal(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::put_pwr_nrm(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    intern::get_err(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id
        );
        intern::fail = true;
        return;
    }

    if (intern::err_ftl[id - 1]) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor fatal error)"
        );
        intern::fail = true;
        return;
    }

    if (intern::err_cmd[id - 1]) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor command error)"
        );
        intern::fail = true;
        return;
    }

    if (intern::err_cfg[id - 1]) {
        logging::err(intern::mod,
            "Failed to initialize sensor #", id, " (Sensor config error)"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

double get_pres (int id) {
    double pres;

    logging::inf(intern::mod,
        "Getting sensor #", id, " pressure reading"
    );

    if (id != 1 && id != 2) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " pressure reading (Sensor does not exist)"
        );
        intern::fail = true;
        return pres;
    }

    if (!intern::valid_pres[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " pressure reading (Reading unavailable)"
        );
        intern::fail = true;
        return pres;
    }

    pres = intern::pres[id - 1];

    logging::inf(intern::mod,
        "Got sensor #", id, " pressure reading: ", pres, "Pa"
    );

    intern::fail = false;
    return pres;
}

double get_temp (int id) {
    double temp;

    logging::inf(intern::mod,
        "Getting sensor #", id, " temperature reading"
    );

    if (id != 1 && id != 2) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " temperature reading (Sensor does not exist)"
        );
        intern::fail = true;
        return temp;
    }

    if (!intern::valid_temp[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " temperature reading (Reading unavailable)"
        );
        intern::fail = true;
        return temp;
    }

    temp = intern::temp[id - 1];

    logging::inf(intern::mod,
        "Got sensor #", id, " temperature reading: ", temp, "K"
    );

    intern::fail = false;
    return temp;
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
        intern::incomp = false;
        return;
    }

    intern::valid_pres[id - 1] = false;
    intern::valid_temp[id - 1] = false;

    intern::get_err(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to update sensor #", id, " readings"
        );
        intern::fail = true;
        intern::incomp = false;
        return;
    }

    if (intern::err_ftl[id - 1]) {
        logging::err(intern::mod,
            "Failed to update sensor #", id, " readings (Sensor fatal error)"
        );
        intern::fail = true;
        intern::incomp = false;
        return;
    }

    if (intern::err_cmd[id - 1]) {
        logging::err(intern::mod,
            "Failed to update sensor #", id, " readings (Sensor command error)"
        );
        intern::fail = true;
        intern::incomp = false;
        return;
    }

    if (intern::err_cfg[id - 1]) {
        logging::err(intern::mod,
            "Failed to update sensor #", id, " readings (Sensor config error)"
        );
        intern::fail = true;
        intern::incomp = false;
        return;
    }

    intern::get_sta(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to update sensor #", id, " readings"
        );
        intern::fail = true;
        intern::incomp = false;
        return;
    }

    if (!intern::sta_drdyp[id - 1] || !intern::sta_drdyt[id - 1]) {
        logging::wrn(intern::mod,
            "Sensor #", id, " measurement incomplete"
        );
        intern::fail = false;
        intern::incomp = true;
        return;
    }

    intern::get_err(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to update sensor #", id, " readings"
        );
        intern::fail = true;
        intern::incomp = false;
        return;
    }

    if (intern::err_ftl[id - 1]) {
        logging::err(intern::mod,
            "Failed to update sensor #", id, " readings (Sensor fatal error)"
        );
        intern::fail = true;
        intern::incomp = false;
        return;
    }

    if (intern::err_cmd[id - 1]) {
        logging::err(intern::mod,
            "Failed to update sensor #", id, " readings (Sensor command error)"
        );
        intern::fail = true;
        intern::incomp = false;
        return;
    }

    if (intern::err_cfg[id - 1]) {
        logging::err(intern::mod,
            "Failed to update sensor #", id, " readings (Sensor config error)"
        );
        intern::fail = true;
        intern::incomp = false;
        return;
    }

    intern::get_dat(id);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to update sensor #", id, " readings"
        );
        intern::fail = true;
        intern::incomp = false;
        return;
    }

    intern::valid_pres[id - 1] = true;
    intern::valid_temp[id - 1] = true;

    intern::fail = false;
    intern::incomp = false;
}

}

namespace bmp388 :: intern {

void put_reg (int id, std::uint8_t reg, std::uint8_t dat) {
    std::uint8_t addr;
    std::vector<std::uint8_t> cmd = {reg, dat};

    logging::inf(mod,
        "Writing to sensor #", id, ": Reg: ", reg, ", Data: ", dat
    );

    switch (id) {
        case 1:
            addr = BMP388_ADDR1;
            break;
        case 2:
            addr = BMP388_ADDR2;
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
            addr = BMP388_ADDR1;
            break;
        case 2:
            addr = BMP388_ADDR2;
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

void put_cmd_rst (int id) {
    std::uint8_t cmd;

    logging::inf(mod,
        "Resetting sensor #", id
    );

    cmd = BMP388_PRST_CMD_RST;

    put_reg(id, BMP388_REG_CMD, cmd);
    if (fail) {
        logging::err(mod,
            "Failed to reset sensor #", id
        );
        fail = true;
        return;
    }

    fail = false;
}

void put_pwr_slp (int id) {
    std::uint8_t pwr = 0x00;

    logging::inf(mod,
        "Setting sensor #", id, " power mode: Sleep"
    );

    pwr = BMP388_PUT_BITS(pwr, BMP388_BIT_ENABP, 1);
    pwr = BMP388_PUT_BITS(pwr, BMP388_BIT_ENABT, 1);
    pwr = BMP388_PUT_BITS(pwr, BMP388_BIT_MODE, BMP388_PRST_MODE_SLP);

    put_reg(id, BMP388_REG_PWR, pwr);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " power mode"
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

    pwr = BMP388_PUT_BITS(pwr, BMP388_BIT_ENABP, 1);
    pwr = BMP388_PUT_BITS(pwr, BMP388_BIT_ENABT, 1);
    pwr = BMP388_PUT_BITS(pwr, BMP388_BIT_MODE, BMP388_PRST_MODE_NRM);

    put_reg(id, BMP388_REG_PWR, pwr);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " power mode"
        );
        fail = true;
        return;
    }

    fail = false;
}

void put_odr (int id, drate drate) {
    std::uint8_t odr = 0x00;

    logging::inf(mod,
        "Setting sensor #", id, " data rate"
    );

    odr = BMP388_PUT_BITS(odr, BMP388_BIT_ODR, int(drate));

    put_reg(id, BMP388_REG_ODR, odr);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " data rate"
        );
        fail = true;
        return;
    }

    fail = false;
}

void put_osr (int id, ovsmp ovsmp_pres, ovsmp ovsmp_temp) {
    std::uint8_t osr = 0x00;

    logging::inf(mod,
        "Setting sensor #", id, " oversampling"
    );

    osr = BMP388_PUT_BITS(osr, BMP388_BIT_OSRP, int(ovsmp_pres));
    osr = BMP388_PUT_BITS(osr, BMP388_BIT_OSRT, int(ovsmp_temp));

    put_reg(id, BMP388_REG_OSR, osr);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " oversampling"
        );
        fail = true;
        return;
    }

    fail = false;
}

void put_cfg (int id, iir iir) {
    std::uint8_t cfg = 0x00;

    logging::inf(mod,
        "Setting sensor #", id, " IIR filter coefficient"
    );

    cfg = BMP388_PUT_BITS(cfg, BMP388_BIT_IIR, int(iir));

    put_reg(id, BMP388_REG_CFG, cfg);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " IIR filter coefficient"
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

    sta = get_reg(id, BMP388_REG_STA, 1);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", id, " status flags"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(sta[0], BMP388_BIT_CRDY)) {
        sta_crdy[id - 1] = true;
    } else {
        sta_crdy[id - 1] = false;
    }

    if (BMP388_GET_BITS(sta[0], BMP388_BIT_DRDYP)) {
        sta_drdyp[id - 1] = true;
    } else {
        sta_drdyp[id - 1] = false;
    }

    if (BMP388_GET_BITS(sta[0], BMP388_BIT_DRDYT)) {
        sta_drdyt[id - 1] = true;
    } else {
        sta_drdyt[id - 1] = false;
    }

    logging::inf(mod,
        "Got sensor #", id, " status flags: ",
        "Cmd rdy: ", bool(sta_crdy[id - 1]), ", ",
        "Pres drdy: ", bool(sta_drdyp[id - 1]), ", ",
        "Temp drdy: ", bool(sta_drdyt[id - 1])
    );

    fail = false;
}

void get_err (int id) {
    std::vector<std::uint8_t> err;

    logging::inf(mod,
        "Getting sensor #", id, " error flags"
    );

    err = get_reg(id, BMP388_REG_ERR, 1);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", id, " error flags"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_FTL)) {
        err_ftl[id - 1] = true;
    } else {
        err_ftl[id - 1] = false;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CMD)) {
        err_cmd[id - 1] = true;
    } else {
        err_cmd[id - 1] = false;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CFG)) {
        err_cfg[id - 1] = true;
    } else {
        err_cfg[id - 1] = false;
    }

    logging::inf(mod,
        "Got sensor #", id, " error flags: ",
        "Ftl err: ", bool(err_ftl[id - 1]), ", ",
        "Cmd err: ", bool(err_cmd[id - 1]), ", ",
        "Cfg err: ", bool(err_cfg[id - 1])
    );

    fail = false;
}

void get_cal (int id) {
    std::vector<std::uint8_t> cal;

    logging::inf(mod,
        "Getting sensor #", id, " calibration data"
    );

    cal = get_reg(id, BMP388_REG_CAL, 21);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", id, " calibration data"
        );
        fail = true;
        return;
    }

    cal_pres[id - 1][0] = double(std::int16_t(cal[5] | (cal[6] << 8)));
    cal_pres[id - 1][1] = double(std::int16_t(cal[7] | (cal[8] << 8)));
    cal_pres[id - 1][2] = double(std::int8_t(cal[9]));
    cal_pres[id - 1][3] = double(std::int8_t(cal[10]));
    cal_pres[id - 1][4] = double(std::uint16_t(cal[11] | (cal[12] << 8)));
    cal_pres[id - 1][5] = double(std::uint16_t(cal[13] | (cal[14] << 8)));
    cal_pres[id - 1][6] = double(std::int8_t(cal[15]));
    cal_pres[id - 1][7] = double(std::int8_t(cal[16]));
    cal_pres[id - 1][8] = double(std::int16_t(cal[17] | (cal[18] << 8)));
    cal_pres[id - 1][9] = double(std::int8_t(cal[19]));
    cal_pres[id - 1][10] = double(std::int8_t(cal[20]));

    cal_temp[id - 1][0] = double(std::uint16_t(cal[0] | cal[1] << 8));
    cal_temp[id - 1][1] = double(std::uint16_t(cal[2] | cal[3] << 8));
    cal_temp[id - 1][2] = double(std::int8_t(cal[4]));

    cal_pres[id - 1][0] -= std::pow(2, 14);
    cal_pres[id - 1][1] -= std::pow(2, 14);

    cal_pres[id - 1][0] /= std::pow(2, 20);
    cal_pres[id - 1][1] /= std::pow(2, 29);
    cal_pres[id - 1][2] /= std::pow(2, 32);
    cal_pres[id - 1][3] /= std::pow(2, 37);
    cal_pres[id - 1][4] *= std::pow(2, 3);
    cal_pres[id - 1][5] /= std::pow(2, 6);
    cal_pres[id - 1][6] /= std::pow(2, 8);
    cal_pres[id - 1][7] /= std::pow(2, 15);
    cal_pres[id - 1][8] /= std::pow(2, 48);
    cal_pres[id - 1][9] /= std::pow(2, 48);
    cal_pres[id - 1][10] /= std::pow(2, 65);

    cal_temp[id - 1][0] *= std::pow(2, 8);
    cal_temp[id - 1][1] /= std::pow(2, 30);
    cal_temp[id - 1][2] /= std::pow(2, 48);

    logging::inf(mod,
        "Got sensor #", id, " calibration data: Pres cal: ", cal_pres[id - 1], ", Temp cal: ", cal_temp[id - 1]
    );

    fail = false;
}

void get_dat (int id) {
    std::vector<std::uint8_t> dat;
    double raw_pres, raw_temp;
    double aux[6];

    logging::inf(mod,
        "Getting sensor #", id, " measurement data"
    );

    dat = get_reg(id, BMP388_REG_DAT, 6);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", id, " measurement data"
        );
        fail = true;
        return;
    }

    raw_pres = dat[0] | (dat[1] << 8) | (dat[2] << 16);
    raw_temp = dat[3] | (dat[4] << 8) | (dat[5] << 16);

    aux[0] = raw_temp - cal_temp[id - 1][0];
    aux[1] = aux[0] * cal_temp[id - 1][1];

    temp[id - 1] = aux[1] + aux[0] * aux[0] * cal_temp[id - 1][2];

    aux[0] = cal_pres[id - 1][5] * temp[id - 1];
    aux[1] = cal_pres[id - 1][6] * temp[id - 1] * temp[id - 1];
    aux[2] = cal_pres[id - 1][7] * temp[id - 1] * temp[id - 1] * temp[id - 1];
    aux[4] = cal_pres[id - 1][4] + aux[0] + aux[1] + aux[2];

    aux[0] = cal_pres[id - 1][1] * temp[id - 1];
    aux[1] = cal_pres[id - 1][2] * temp[id - 1] * temp[id - 1];
    aux[2] = cal_pres[id - 1][3] * temp[id - 1] * temp[id - 1] * temp[id - 1];
    aux[5] = raw_pres * (cal_pres[id - 1][0] + aux[0] + aux[1] + aux[2]);

    aux[0] = raw_pres * raw_pres;
    aux[1] = cal_pres[id - 1][8] + cal_pres[id - 1][9] * temp[id - 1];
    aux[2] = aux[0] * aux[1];
    aux[3] = aux[2] + cal_pres[id - 1][10] * raw_pres * raw_pres * raw_pres;

    pres[id - 1] = aux[3] + aux[4] + aux[5];

    temp[id - 1] += 273.15;

    logging::inf(mod,
        "Got sensor #", id, " measurement data: Pres: ", pres[id - 1], "Pa, Temp: ", temp[id - 1], "K"
    );

    fail = false;
}

}

#include <cstddef>
#include <cstdint>
#include <cmath>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <math/linalg.hpp>

#include <sys/logging.hpp>

#include <dev/i2c.hpp>
#include <dev/bmp388.hpp>

namespace bmp388 :: intern {

std::string mod = "bmp388";

bool fail;
bool incomp;

double pres1, temp1;
double pres2, temp2;

bool crdy1, drdy_pres1, drdy_temp1;
bool crdy2, drdy_pres2, drdy_temp2;

linalg::fvector cal_pres1(11), cal_temp1(3);
linalg::fvector cal_pres2(11), cal_temp2(3);

std::string conv_val_to_oupt (bool val);
std::string conv_val_to_oupt (double val);
std::string conv_val_to_oupt (linalg::fvector val);
std::string conv_hex_to_oupt (std::uint8_t hex);
std::string conv_hex_to_oupt (std::vector<std::uint8_t> hex);
std::string conv_drate_to_oupt (drate drate);
std::string conv_ovsmp_to_oupt (ovsmp ovsmp);
std::string conv_iir_to_oupt (iir iir);

void put_reg (int num, std::uint8_t reg, std::uint8_t dat);
std::vector<std::uint8_t> get_reg (int num, std::uint8_t reg, std::size_t len);

void put_pwr_slp (int num);
void put_pwr_nrm (int num);
void put_odr (int num, drate drate);
void put_osr (int num, ovsmp ovsmp_pres, ovsmp ovsmp_temp);
void put_cfg (int num, iir iir);

void get_sta (int num);
void get_cal (int num);
void get_dat (int num);

void wait (int num);
void reset (int num);

void init (int num, drate drate, ovsmp ovsmp_pres, ovsmp ovsmp_temp, iir iir);
void update (int num);

}

namespace bmp388 {

bool fail (void) {
    return intern::fail;
}

bool incomp (void) {
    return intern::incomp;
}

void init (
    drate drate1, ovsmp ovsmp_pres1, ovsmp ovsmp_temp1, iir iir1,
    drate drate2, ovsmp ovsmp_pres2, ovsmp ovsmp_temp2, iir iir2
) {
    logging::inf(intern::mod,
        "Initializing sensors"
    );

    intern::init(1, drate1, ovsmp_pres1, ovsmp_temp1, iir1);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensors"
        );
        intern::fail = true;
        return;
    }

    intern::init(2, drate2, ovsmp_pres2, ovsmp_temp2, iir2);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to initialize sensors"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

double get_pres (void) {
    double pres;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting average pressure reading"
    );

    if (!std::isnan(intern::pres1) && !std::isnan(intern::pres2)) {
        pres = 0.5 * (intern::pres1 + intern::pres2);
    } else if (!std::isnan(intern::pres1)) {
        pres = intern::pres1;
    } else if (!std::isnan(intern::pres2)) {
        pres = intern::pres2;
    } else {
        logging::err(intern::mod,
            "Failed to get average pressure reading (Invalid readings)"
        );
        intern::fail = true;
        return pres;
    }

    oupt = intern::conv_val_to_oupt(pres);
    logging::inf(intern::mod,
        "Got average pressure reading: ", oupt, "Pa"
    );

    intern::fail = false;
    return pres;
}

double get_temp (void) {
    double temp;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting average temperature reading"
    );

    if (!std::isnan(intern::temp1) && !std::isnan(intern::temp2)) {
        temp = 0.5 * (intern::temp1 + intern::temp2);
    } else if (!std::isnan(intern::temp1)) {
        temp = intern::temp1;
    } else if (!std::isnan(intern::temp2)) {
        temp = intern::temp2;
    } else {
        logging::err(intern::mod,
            "Failed to get average temperature reading (Invalid readings)"
        );
        intern::fail = true;
        return temp;
    }

    oupt = intern::conv_val_to_oupt(temp);
    logging::inf(intern::mod,
        "Got average temperature reading: ", oupt, "°C"
    );

    intern::fail = false;
    return temp;
}

void update (void) {
    bool fail1, incomp1;
    bool fail2, incomp2;

    logging::inf(intern::mod,
        "Getting sensor readings"
    );

    intern::update(1);
    if (intern::fail) {
        logging::wrn(intern::mod,
            "Sensor #1 readings unavailable due to failure"
        );
        fail1 = true;
        incomp1 = false;
    } else if (intern::incomp) {
        logging::wrn(intern::mod,
            "Sensor #1 readings unavailable due to incomplete measurement"
        );
        fail1 = false;
        incomp1 = true;
    } else {
        fail1 = false;
        incomp1 = false;
    }

    intern::update(2);
    if (intern::fail) {
        logging::wrn(intern::mod,
            "Sensor #2 readings unavailable due to failure"
        );
        fail2 = true;
        incomp2 = false;
    } else if (intern::incomp) {
        logging::wrn(intern::mod,
            "Sensor #2 readings unavailable due to incomplete measurement"
        );
        fail2 = false;
        incomp2 = true;
    } else {
        fail2 = false;
        incomp2 = false;
    }

    if (fail1 && fail2) {
        logging::err(intern::mod,
            "Failed to get sensor readings"
        );
        intern::fail = true;
        intern::incomp = false;
        return;
    }

    if ((fail1 || incomp1) && (fail2 || incomp2)) {
        logging::wrn(intern::mod,
            "Sensor measurements incomplete"
        );
        intern::fail = false;
        intern::incomp = true;
        return;
    }

    intern::fail = false;
    intern::incomp = false;
}

}

namespace bmp388 :: intern {

std::string conv_val_to_oupt (bool val) {
    std::string oupt;
    if (val) {
        oupt = "True";
    } else {
        oupt = "False";
    }
    return oupt;
}

std::string conv_val_to_oupt (double val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << std::showpos << std::scientific << std::setprecision(2) << val;
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

std::string conv_drate_to_oupt (drate drate) {
    switch (drate) {
        case drate_200_hz:
            return std::string("200Hz");
        case drate_100_hz:
            return std::string("100Hz");
        case drate_50_hz:
            return std::string("50Hz");
        case drate_25_hz:
            return std::string("25Hz");
        case drate_25_2_hz:
            return std::string("25/2Hz");
        case drate_25_4_hz:
            return std::string("25/4Hz");
        case drate_25_8_hz:
            return std::string("25/8Hz");
        case drate_25_16_hz:
            return std::string("25/16Hz");
        case drate_25_32_hz:
            return std::string("25/32Hz");
        case drate_25_64_hz:
            return std::string("25/64Hz");
        case drate_25_128_hz:
            return std::string("25/128Hz");
        case drate_25_256_hz:
            return std::string("25/256Hz");
        case drate_25_512_hz:
            return std::string("25/512Hz");
        case drate_25_1024_hz:
            return std::string("25/1024Hz");
        case drate_25_2048_hz:
            return std::string("25/2048Hz");
        case drate_25_4096_hz:
            return std::string("25/4096Hz");
        case drate_25_8192_hz:
            return std::string("25/8192Hz");
        case drate_25_16384_hz:
            return std::string("25/16384Hz");
        default:
            return std::string("");
    }
}

std::string conv_ovsmp_to_oupt (ovsmp ovsmp) {
    switch (ovsmp) {
        case ovsmp_1x:
            return std::string("1x");
        case ovsmp_2x:
            return std::string("2x");
        case ovsmp_4x:
            return std::string("4x");
        case ovsmp_8x:
            return std::string("8x");
        case ovsmp_16x:
            return std::string("16x");
        case ovsmp_32x:
            return std::string("32x");
        default:
            return std::string("");
    }
}

std::string conv_iir_to_oupt (iir iir) {
    switch (iir) {
        case iir_0:
            return std::string("0");
        case iir_1:
            return std::string("1");
        case iir_3:
            return std::string("3");
        case iir_7:
            return std::string("7");
        case iir_15:
            return std::string("15");
        case iir_31:
            return std::string("31");
        case iir_63:
            return std::string("63");
        case iir_127:
            return std::string("127");
        default:
            return std::string("");
    }
}

void put_reg (int num, std::uint8_t reg, std::uint8_t dat) {
    std::uint8_t addr;
    std::vector<std::uint8_t> cmd(2);
    std::string oupt_reg, oupt_dat;

    oupt_reg = conv_hex_to_oupt(reg);
    oupt_dat = conv_hex_to_oupt(dat);
    logging::inf(mod,
        "Writing to sensor #", num, ": Reg: ", oupt_reg, ", Data: ", oupt_dat
    );

    if (num == 1) {
        addr = BMP388_ADDR1;
    } else if (num == 2) {
        addr = BMP388_ADDR2;
    } else {
        logging::err(mod,
            "Failed to write to sensor #", num, " (Sensor does not exist)"
        );
        fail = true;
        return;
    }

    cmd[0] = reg;
    cmd[1] = dat;

    i2c::send(addr, cmd);
    i2c::exec();
    if (i2c::fail()) {
        logging::err(mod,
            "Failed to write to sensor #", num
        );
        fail = true;
        return;
    }

    fail = false;
}

std::vector<std::uint8_t> get_reg (int num, std::uint8_t reg, std::size_t len) {
    std::uint8_t addr;
    std::vector<std::uint8_t> cmd(1), dat(len);
    std::string oupt_reg, oupt_dat;

    oupt_reg = conv_hex_to_oupt(reg);
    logging::inf(mod,
        "Reading from sensor #", num, ": Reg: ", oupt_reg, ", Len: ", len
    );

    if (num == 1) {
        addr = BMP388_ADDR1;
    } else if (num == 2) {
        addr = BMP388_ADDR2;
    } else {
        logging::err(mod,
            "Failed to read from sensor #", num, " (Sensor does not exist)"
        );
        fail = true;
        return dat;
    }

    cmd[0] = reg;

    i2c::send(addr, cmd);
    i2c::recv(addr, &dat);
    i2c::exec();
    if (i2c::fail()) {
        logging::err(mod,
            "Failed to read from sensor #", num
        );
        fail = true;
        return dat;
    }

    oupt_dat = conv_hex_to_oupt(dat);
    logging::inf(mod,
        "Read from sensor #", num, ": Data: ", oupt_dat
    );

    fail = false;
    return dat;
}

void put_pwr_slp (int num) {
    std::uint8_t pwr = 0x00;
    std::vector<std::uint8_t> err;

    logging::inf(mod,
        "Setting sensor #", num, " power mode: Sleep"
    );

    pwr = BMP388_PUT_BITS(pwr, BMP388_BIT_ENABP, 1);
    pwr = BMP388_PUT_BITS(pwr, BMP388_BIT_ENABT, 1);
    pwr = BMP388_PUT_BITS(pwr, BMP388_BIT_MODE, BMP388_PRST_MODE_SLP);

    put_reg(num, BMP388_REG_PWR, pwr);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", num, " power mode"
        );
        fail = true;
        return;
    }

    err = get_reg(num, BMP388_REG_ERR, 1);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", num, " power mode"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_FTLERR)) {
        logging::err(mod,
            "Failed to set sensor #", num, " power mode ",
            "(Sensor fatal error)"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CMDERR)) {
        logging::err(mod,
            "Failed to set sensor #", num, " power mode ",
            "(Sensor command error)"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CFGERR)) {
        logging::err(mod,
            "Failed to set sensor #", num, " power mode ",
            "(Sensor config error)"
        );
        fail = true;
        return;
    }

    fail = false;
}

void put_pwr_nrm (int num) {
    std::uint8_t pwr = 0x00;
    std::vector<std::uint8_t> err;

    logging::inf(mod,
        "Setting sensor #", num, " power mode: Normal"
    );

    pwr = BMP388_PUT_BITS(pwr, BMP388_BIT_ENABP, 1);
    pwr = BMP388_PUT_BITS(pwr, BMP388_BIT_ENABT, 1);
    pwr = BMP388_PUT_BITS(pwr, BMP388_BIT_MODE, BMP388_PRST_MODE_NRM);

    put_reg(num, BMP388_REG_PWR, pwr);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", num, " power mode"
        );
        fail = true;
        return;
    }

    err = get_reg(num, BMP388_REG_ERR, 1);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", num, " power mode"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_FTLERR)) {
        logging::err(mod,
            "Failed to set sensor #", num, " power mode ",
            "(Sensor fatal error)"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CMDERR)) {
        logging::err(mod,
            "Failed to set sensor #", num, " power mode ",
            "(Sensor command error)"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CFGERR)) {
        logging::err(mod,
            "Failed to set sensor #", num, " power mode ",
            "(Sensor config error)"
        );
        fail = true;
        return;
    }

    fail = false;
}

void put_odr (int num, drate drate) {
    std::uint8_t odr = 0x00;
    std::vector<std::uint8_t> err;
    std::string oupt;

    oupt = conv_drate_to_oupt(drate);
    logging::inf(mod,
        "Setting sensor #", num, " data rate: ", oupt
    );

    odr = BMP388_PUT_BITS(odr, BMP388_BIT_ODR, drate);

    put_reg(num, BMP388_REG_ODR, odr);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", num, " data rate"
        );
        fail = true;
        return;
    }

    err = get_reg(num, BMP388_REG_ERR, 1);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", num, " data rate"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_FTLERR)) {
        logging::err(mod,
            "Failed to set sensor #", num, " data rate ",
            "(Sensor fatal error)"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CMDERR)) {
        logging::err(mod,
            "Failed to set sensor #", num, " data rate ",
            "(Sensor command error)"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CFGERR)) {
        logging::err(mod,
            "Failed to set sensor #", num, " data rate ",
            "(Sensor config error)"
        );
        fail = true;
        return;
    }

    fail = false;
}

void put_osr (int num, ovsmp ovsmp_pres, ovsmp ovsmp_temp) {
    std::uint8_t osr = 0x00;
    std::vector<std::uint8_t> err;
    std::string oupt_ovsmp_pres, oupt_ovsmp_temp;

    oupt_ovsmp_pres = conv_ovsmp_to_oupt(ovsmp_pres);
    oupt_ovsmp_temp = conv_ovsmp_to_oupt(ovsmp_temp);
    logging::inf(mod,
        "Setting sensor #", num, " oversampling: ",
        "Pres ovsmp: ", oupt_ovsmp_pres, ", Temp ovsmp: ", oupt_ovsmp_temp
    );

    osr = BMP388_PUT_BITS(osr, BMP388_BIT_OSRP, ovsmp_pres);
    osr = BMP388_PUT_BITS(osr, BMP388_BIT_OSRT, ovsmp_temp);

    put_reg(num, BMP388_REG_OSR, osr);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", num, " oversampling"
        );
        fail = true;
        return;
    }

    err = get_reg(num, BMP388_REG_ERR, 1);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", num, " oversampling"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_FTLERR)) {
        logging::err(mod,
            "Failed to set sensor #", num, " oversampling ",
            "(Sensor fatal error)"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CMDERR)) {
        logging::err(mod,
            "Failed to set sensor #", num, " oversampling ",
            "(Sensor command error)"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CFGERR)) {
        logging::err(mod,
            "Failed to set sensor #", num, " oversampling ",
            "(Sensor config error)"
        );
        fail = true;
        return;
    }

    fail = false;
}

void put_cfg (int num, iir iir) {
    std::uint8_t cfg = 0x00;
    std::vector<std::uint8_t> err;
    std::string oupt;

    oupt = conv_iir_to_oupt(iir);
    logging::inf(mod,
        "Setting sensor #", num, " IIR filter coefficient: ", oupt
    );

    cfg = BMP388_PUT_BITS(cfg, BMP388_BIT_IIR, iir);

    put_reg(num, BMP388_REG_CFG, cfg);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", num, " IIR filter coefficient"
        );
        fail = true;
        return;
    }

    err = get_reg(num, BMP388_REG_ERR, 1);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", num, " IIR filter coefficient"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_FTLERR)) {
        logging::err(mod,
            "Failed to set sensor #", num, " IIR filter coefficient ",
            "(Sensor fatal error)"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CMDERR)) {
        logging::err(mod,
            "Failed to set sensor #", num, " IIR filter coefficient ",
            "(Sensor command error)"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CFGERR)) {
        logging::err(mod,
            "Failed to set sensor #", num, " IIR filter coefficient ",
            "(Sensor config error)"
        );
        fail = true;
        return;
    }

    fail = false;
}

void get_sta (int num) {
    std::vector<std::uint8_t> sta, err;
    std::string oupt_crdy, oupt_drdy_pres, oupt_drdy_temp;

    logging::inf(mod,
        "Getting sensor #", num, " status"
    );

    err = get_reg(num, BMP388_REG_ERR, 1);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", num, " status"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_FTLERR)) {
        logging::err(mod,
            "Failed to get sensor #", num, " status ",
            "(Sensor fatal error)"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CMDERR)) {
        logging::err(mod,
            "Failed to get sensor #", num, " status ",
            "(Sensor command error)"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CFGERR)) {
        logging::err(mod,
            "Failed to get sensor #", num, " status ",
            "(Sensor config error)"
        );
        fail = true;
        return;
    }

    sta = get_reg(num, BMP388_REG_STA, 1);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", num, " status"
        );
        fail = true;
        return;
    }

    if (num == 1) {
        if (BMP388_GET_BITS(sta[0], BMP388_BIT_CRDY)) {
            crdy1 = true;
        } else {
            crdy1 = false;
        }

        if (BMP388_GET_BITS(sta[0], BMP388_BIT_DRDYP)) {
            drdy_pres1 = true;
        } else {
            drdy_pres1 = false;
        }

        if (BMP388_GET_BITS(sta[0], BMP388_BIT_DRDYT)) {
            drdy_temp1 = true;
        } else {
            drdy_temp1 = false;
        }

        oupt_crdy = conv_val_to_oupt(crdy1);
        oupt_drdy_pres = conv_val_to_oupt(drdy_pres1);
        oupt_drdy_temp = conv_val_to_oupt(drdy_temp1);

        logging::inf(mod,
            "Got sensor #", num, " status: ",
            "Cmd rdy: ", oupt_crdy, ", ",
            "Pres drdy: ", oupt_drdy_pres, ", Temp drdy: ", oupt_drdy_temp
        );
    }

    if (num == 2) {
        if (BMP388_GET_BITS(sta[0], BMP388_BIT_CRDY)) {
            crdy2 = true;
        } else {
            crdy2 = false;
        }

        if (BMP388_GET_BITS(sta[0], BMP388_BIT_DRDYP)) {
            drdy_pres2 = true;
        } else {
            drdy_pres2 = false;
        }

        if (BMP388_GET_BITS(sta[0], BMP388_BIT_DRDYT)) {
            drdy_temp2 = true;
        } else {
            drdy_temp2 = false;
        }

        oupt_crdy = conv_val_to_oupt(crdy2);
        oupt_drdy_pres = conv_val_to_oupt(drdy_pres2);
        oupt_drdy_temp = conv_val_to_oupt(drdy_temp2);

        logging::inf(mod,
            "Got sensor #", num, " status: ",
            "Cmd rdy: ", oupt_crdy, ", ",
            "Pres drdy: ", oupt_drdy_pres, ", Temp drdy: ", oupt_drdy_temp
        );
    }

    fail = false;
}

void get_cal (int num) {
    std::vector<std::uint8_t> cal, err;
    std::string oupt_cal_pres, oupt_cal_temp;

    logging::inf(mod,
        "Getting sensor #", num, " calibration parameters"
    );

    err = get_reg(num, BMP388_REG_ERR, 1);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", num, " calibration parameters"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_FTLERR)) {
        logging::err(mod,
            "Failed to get sensor #", num, " calibration parameters ",
            "(Sensor fatal error)"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CMDERR)) {
        logging::err(mod,
            "Failed to get sensor #", num, " calibration parameters ",
            "(Sensor command error)"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CFGERR)) {
        logging::err(mod,
            "Failed to get sensor #", num, " calibration parameters ",
            "(Sensor config error)"
        );
        fail = true;
        return;
    }

    cal = get_reg(num, BMP388_REG_CAL, 21);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", num, " calibration parameters"
        );
        fail = true;
        return;
    }

    if (num == 1) {
        cal_pres1[0] = (double)((std::int16_t)(cal[5] | (cal[6] << 8)));
        cal_pres1[1] = (double)((std::int16_t)(cal[7] | (cal[8] << 8)));
        cal_pres1[2] = (double)((std::int8_t)(cal[9]));
        cal_pres1[3] = (double)((std::int8_t)(cal[10]));
        cal_pres1[4] = (double)((std::uint16_t)(cal[11] | (cal[12] << 8)));
        cal_pres1[5] = (double)((std::uint16_t)(cal[13] | (cal[14] << 8)));
        cal_pres1[6] = (double)((std::int8_t)(cal[15]));
        cal_pres1[7] = (double)((std::int8_t)(cal[16]));
        cal_pres1[8] = (double)((std::int16_t)(cal[17] | (cal[18] << 8)));
        cal_pres1[9] = (double)((std::int8_t)(cal[19]));
        cal_pres1[10] = (double)((std::int8_t)(cal[20]));
        cal_temp1[0] = (double)((std::uint16_t)(cal[0] | (cal[1] << 8)));
        cal_temp1[1] = (double)((std::uint16_t)(cal[2] | (cal[3] << 8)));
        cal_temp1[2] = (double)((std::int8_t)(cal[4]));

        cal_pres1[0] -= std::pow(2, 14);
        cal_pres1[1] -= std::pow(2, 14);

        cal_pres1[0] /= std::pow(2, 20);
        cal_pres1[1] /= std::pow(2, 29);
        cal_pres1[2] /= std::pow(2, 32);
        cal_pres1[3] /= std::pow(2, 37);
        cal_pres1[4] *= std::pow(2, 3);
        cal_pres1[5] /= std::pow(2, 6);
        cal_pres1[6] /= std::pow(2, 8);
        cal_pres1[7] /= std::pow(2, 15);
        cal_pres1[8] /= std::pow(2, 48);
        cal_pres1[9] /= std::pow(2, 48);
        cal_pres1[10] /= std::pow(2, 65);
        cal_temp1[0] *= std::pow(2, 8);
        cal_temp1[1] /= std::pow(2, 30);
        cal_temp1[2] /= std::pow(2, 48);

        oupt_cal_pres = conv_val_to_oupt(cal_pres1);
        oupt_cal_temp = conv_val_to_oupt(cal_temp1);
        logging::inf(mod,
            "Got sensor #", num, " calibration parameters: ",
            "Pres cal: ", oupt_cal_pres, ", Temp cal: ", oupt_cal_temp
        );
    }

    if (num == 2) {
        cal_pres2[0] = (double)((std::int16_t)(cal[5] | (cal[6] << 8)));
        cal_pres2[1] = (double)((std::int16_t)(cal[7] | (cal[8] << 8)));
        cal_pres2[2] = (double)((std::int8_t)(cal[9]));
        cal_pres2[3] = (double)((std::int8_t)(cal[10]));
        cal_pres2[4] = (double)((std::uint16_t)(cal[11] | (cal[12] << 8)));
        cal_pres2[5] = (double)((std::uint16_t)(cal[13] | (cal[14] << 8)));
        cal_pres2[6] = (double)((std::int8_t)(cal[15]));
        cal_pres2[7] = (double)((std::int8_t)(cal[16]));
        cal_pres2[8] = (double)((std::int16_t)(cal[17] | (cal[18] << 8)));
        cal_pres2[9] = (double)((std::int8_t)(cal[19]));
        cal_pres2[10] = (double)((std::int8_t)(cal[20]));
        cal_temp2[0] = (double)((std::uint16_t)(cal[0] | (cal[1] << 8)));
        cal_temp2[1] = (double)((std::uint16_t)(cal[2] | (cal[3] << 8)));
        cal_temp2[2] = (double)((std::int8_t)(cal[4]));

        cal_pres2[0] -= std::pow(2, 14);
        cal_pres2[1] -= std::pow(2, 14);

        cal_pres2[0] /= std::pow(2, 20);
        cal_pres2[1] /= std::pow(2, 29);
        cal_pres2[2] /= std::pow(2, 32);
        cal_pres2[3] /= std::pow(2, 37);
        cal_pres2[4] *= std::pow(2, 3);
        cal_pres2[5] /= std::pow(2, 6);
        cal_pres2[6] /= std::pow(2, 8);
        cal_pres2[7] /= std::pow(2, 15);
        cal_pres2[8] /= std::pow(2, 48);
        cal_pres2[9] /= std::pow(2, 48);
        cal_pres2[10] /= std::pow(2, 65);
        cal_temp2[0] *= std::pow(2, 8);
        cal_temp2[1] /= std::pow(2, 30);
        cal_temp2[2] /= std::pow(2, 48);

        oupt_cal_pres = conv_val_to_oupt(cal_pres2);
        oupt_cal_temp = conv_val_to_oupt(cal_temp2);
        logging::inf(mod,
            "Got sensor #", num, " calibration parameters: ",
            "Pres cal: ", oupt_cal_pres, ", Temp cal: ", oupt_cal_temp
        );
    }

    fail = false;
}

void get_dat (int num) {
    std::vector<std::uint8_t> dat, err;
    double raw_pres, raw_temp;
    double aux[6];
    std::string oupt_pres, oupt_temp;

    logging::inf(mod,
        "Getting sensor #", num, " data"
    );

    err = get_reg(num, BMP388_REG_ERR, 1);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", num, " data"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_FTLERR)) {
        logging::err(mod,
            "Failed to get sensor #", num, " data ",
            "(Sensor fatal error)"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CMDERR)) {
        logging::err(mod,
            "Failed to get sensor #", num, " data ",
            "(Sensor command error)"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CFGERR)) {
        logging::err(mod,
            "Failed to get sensor #", num, " data ",
            "(Sensor config error)"
        );
        fail = true;
        return;
    }

    dat = get_reg(num, BMP388_REG_DAT, 6);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", num, " data"
        );
        fail = true;
        return;
    }

    if (num == 1) {
        raw_pres = dat[0] | (dat[1] << 8) | (dat[2] << 16);
        raw_temp = dat[3] | (dat[4] << 8) | (dat[5] << 16);

        aux[0] = raw_temp - cal_temp1[0];
        aux[1] = aux[0] * cal_temp1[1];

        temp1 = aux[1] + aux[0] * aux[0] * cal_temp1[2];

        aux[0] = cal_pres1[5] * temp1;
        aux[1] = cal_pres1[6] * temp1 * temp1;
        aux[2] = cal_pres1[7] * temp1 * temp1 * temp1;
        aux[4] = cal_pres1[4] + aux[0] + aux[1] + aux[2];

        aux[0] = cal_pres1[1] * temp1;
        aux[1] = cal_pres1[2] * temp1 * temp1;
        aux[2] = cal_pres1[3] * temp1 * temp1 * temp1;
        aux[5] = raw_pres * (cal_pres1[0] + aux[0] + aux[1] + aux[2]);

        aux[0] = raw_pres * raw_pres;
        aux[1] = cal_pres1[8] + cal_pres1[9] * temp1;
        aux[2] = aux[0] * aux[1];
        aux[3] = aux[2] + cal_pres1[10] * raw_pres * raw_pres * raw_pres;

        pres1 = aux[3] + aux[4] + aux[5];

        oupt_pres = conv_val_to_oupt(pres1);
        oupt_temp = conv_val_to_oupt(temp1);
        logging::inf(mod,
            "Got sensor #", num, " data: ",
            "Pres: ", oupt_pres, "Pa, Temp: ", oupt_temp, "°C"
        );
    }

    if (num == 2) {
        raw_pres = dat[0] | (dat[1] << 8) | (dat[2] << 16);
        raw_temp = dat[3] | (dat[4] << 8) | (dat[5] << 16);

        aux[0] = raw_temp - cal_temp2[0];
        aux[1] = aux[0] * cal_temp2[1];

        temp2 = aux[1] + aux[0] * aux[0] * cal_temp2[2];

        aux[0] = cal_pres2[5] * temp2;
        aux[1] = cal_pres2[6] * temp2 * temp2;
        aux[2] = cal_pres2[7] * temp2 * temp2 * temp2;
        aux[4] = cal_pres2[4] + aux[0] + aux[1] + aux[2];

        aux[0] = cal_pres2[1] * temp2;
        aux[1] = cal_pres2[2] * temp2 * temp2;
        aux[2] = cal_pres2[3] * temp2 * temp2 * temp2;
        aux[5] = raw_pres * (cal_pres2[0] + aux[0] + aux[1] + aux[2]);

        aux[0] = raw_pres * raw_pres;
        aux[1] = cal_pres2[8] + cal_pres2[9] * temp2;
        aux[2] = aux[0] * aux[1];
        aux[3] = aux[2] + cal_pres2[10] * raw_pres * raw_pres * raw_pres;

        pres2 = aux[3] + aux[4] + aux[5];

        oupt_pres = conv_val_to_oupt(pres2);
        oupt_temp = conv_val_to_oupt(temp2);
        logging::inf(mod,
            "Got sensor #", num, " data: ",
            "Pres: ", oupt_pres, "Pa, Temp: ", oupt_temp, "°C"
        );
    }

    fail = false;
}

void wait (int num) {
    logging::inf(mod,
        "Waiting for sensor #", num, " to become ready"
    );

    while (true) {
        get_sta(num);
        if (fail) {
            logging::err(mod,
                "Failed to wait for sensor #", num, " to become ready"
            );
            fail = true;
            return;
        }

        if (num == 1 && crdy1) {
            break;
        }

        if (num == 2 && crdy2) {
            break;
        }
    }

    fail = false;
}

void reset (int num) {
    std::uint8_t cmd;
    std::vector<std::uint8_t> err;

    logging::inf(mod,
        "Resetting sensor #", num
    );

    cmd = BMP388_PRST_CMD_RST;

    put_reg(num, BMP388_REG_CMD, cmd);
    if (fail) {
        logging::err(mod,
            "Failed to reset sensor #", num
        );
        fail = true;
        return;
    }

    err = get_reg(num, BMP388_REG_ERR, 1);
    if (fail) {
        logging::err(mod,
            "Failed to reset sensor #", num
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_FTLERR)) {
        logging::err(mod,
            "Failed to reset sensor #", num, " (Sensor fatal error)"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CMDERR)) {
        logging::err(mod,
            "Failed to reset sensor #", num, " (Sensor command error)"
        );
        fail = true;
        return;
    }

    if (BMP388_GET_BITS(err[0], BMP388_BIT_CFGERR)) {
        logging::err(mod,
            "Failed to reset sensor #", num, " (Sensor config error)"
        );
        fail = true;
        return;
    }

    fail = false;
}

void init (int num, drate drate, ovsmp ovsmp_pres, ovsmp ovsmp_temp, iir iir) {
    std::string oupt_drate, oupt_ovsmp_pres, oupt_ovsmp_temp, oupt_iir;

    oupt_drate = conv_drate_to_oupt(drate);
    oupt_ovsmp_pres = conv_ovsmp_to_oupt(ovsmp_pres);
    oupt_ovsmp_temp = conv_ovsmp_to_oupt(ovsmp_temp);
    oupt_iir = conv_iir_to_oupt(iir);
    logging::inf(mod,
        "Initializing sensor #", num, ": ",
        "Data rate: ", oupt_drate, ", ",
        "Pres ovsmp: ", oupt_ovsmp_pres, ", ",
        "Temp ovsmp: ", oupt_ovsmp_temp, ", ",
        "IIR coef: ", oupt_iir
    );

    wait(num);
    if (fail) {
        logging::err(mod,
            "Failed to initialize sensor #", num
        );
        fail = true;
        return;
    }

    reset(num);
    if (fail) {
        logging::err(mod,
            "Failed to initialize sensor #", num
        );
        fail = true;
        return;
    }

    put_pwr_slp(num);
    if (fail) {
        logging::err(mod,
            "Failed to initialize sensor #", num
        );
        fail = true;
        return;
    }

    put_odr(num, drate);
    if (fail) {
        logging::err(mod,
            "Failed to initialize sensor #", num
        );
        fail = true;
        return;
    }

    put_osr(num, ovsmp_pres, ovsmp_temp);
    if (fail) {
        logging::err(mod,
            "Failed to initialize sensor #", num
        );
        fail = true;
        return;
    }

    put_cfg(num, iir);
    if (fail) {
        logging::err(mod,
            "Failed to initialize sensor #", num
        );
        fail = true;
        return;
    }

    get_cal(num);
    if (fail) {
        logging::err(mod,
            "Failed to initialize sensor #", num
        );
        fail = true;
        return;
    }

    put_pwr_nrm(num);
    if (fail) {
        logging::err(mod,
            "Failed to initialize sensor #", num
        );
        fail = true;
        return;
    }

    fail = false;
}

void update (int num) {
    logging::inf(mod,
        "Getting sensor #", num, " readings"
    );

    if (num == 1) {
        pres1 = NAN;
        temp1 = NAN;
    }

    if (num == 2) {
        pres2 = NAN;
        temp2 = NAN;
    }

    get_sta(num);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", num, " readings"
        );
        fail = true;
        incomp = false;
        return;
    }

    if (num == 1 && !(drdy_pres1 && drdy_temp1)) {
        logging::wrn(mod,
            "Sensor #", num, " measurement incomplete"
        );
        fail = false;
        incomp = true;
        return;
    }

    if (num == 2 && !(drdy_pres2 && drdy_temp2)) {
        logging::wrn(mod,
            "Sensor #", num, " measurement incomplete"
        );
        fail = false;
        incomp = true;
        return;
    }

    get_dat(num);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", num, " readings"
        );
        fail = true;
        incomp = false;
        return;
    }

    fail = false;
    incomp = false;
}

}

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

std::vector<linalg::fvector> cal_pres(2, linalg::fvector(11));
std::vector<linalg::fvector> cal_temp(2, linalg::fvector(3));

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

std::string conv_val_to_oupt (bool val);
std::string conv_val_to_oupt (double val);
std::string conv_val_to_oupt (linalg::fvector val);
std::string conv_hex_to_oupt (std::uint8_t hex);
std::string conv_hex_to_oupt (std::vector<std::uint8_t> hex);
std::string conv_drate_to_oupt (drate drate);
std::string conv_ovsmp_to_oupt (ovsmp ovsmp);
std::string conv_iir_to_oupt (iir iir);

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

bool fail (void) {
    return intern::fail;
}

bool incomp (void) {
    return intern::incomp;
}

void init (int id, drate drate, ovsmp ovsmp_pres, ovsmp ovsmp_temp, iir iir) {
    std::string oupt_drate, oupt_ovsmp_pres, oupt_ovsmp_temp, oupt_iir;

    oupt_drate = intern::conv_drate_to_oupt(drate);
    oupt_ovsmp_pres = intern::conv_ovsmp_to_oupt(ovsmp_pres);
    oupt_ovsmp_temp = intern::conv_ovsmp_to_oupt(ovsmp_temp);
    oupt_iir = intern::conv_iir_to_oupt(iir);
    logging::inf(intern::mod,
        "Initializing sensor #", id, ": ",
        "Data rate: ", oupt_drate, ", ",
        "Pres ovsmp: ", oupt_ovsmp_pres, ", ",
        "Temp ovsmp: ", oupt_ovsmp_temp, ", ",
        "IIR coef: ", oupt_iir
    );

    if (!(id == 1 || id == 2)) {
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
    std::string oupt;

    logging::inf(intern::mod,
        "Getting sensor #", id, " pressure reading"
    );

    if (!(id == 1 || id == 2)) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " pressure reading ",
            "(Sensor does not exist)"
        );
        intern::fail = true;
        return pres;
    }

    if (!intern::valid_pres[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " pressure reading ",
            "(Reading unavailable)"
        );
        intern::fail = true;
        return pres;
    }

    pres = intern::pres[id - 1];

    oupt = intern::conv_val_to_oupt(pres);
    logging::inf(intern::mod,
        "Got sensor #", id, " pressure reading: ", oupt, "Pa"
    );

    intern::fail = false;
    return pres;
}

double get_temp (int id) {
    double temp;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting sensor #", id, " temperature reading"
    );

    if (!(id == 1 || id == 2)) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " temperature reading ",
            "(Sensor does not exist)"
        );
        intern::fail = true;
        return temp;
    }

    if (!intern::valid_temp[id - 1]) {
        logging::err(intern::mod,
            "Failed to get sensor #", id, " temperature reading ",
            "(Reading unavailable)"
        );
        intern::fail = true;
        return temp;
    }

    temp = intern::temp[id - 1];

    oupt = intern::conv_val_to_oupt(temp);
    logging::inf(intern::mod,
        "Got sensor #", id, " temperature reading: ", oupt, "°C"
    );

    intern::fail = false;
    return temp;
}

void update (int id) {
    std::string oupt_pres, oupt_temp;

    logging::inf(intern::mod,
        "Updating sensor #", id, " readings"
    );

    if (!(id == 1 || id == 2)) {
        logging::err(intern::mod,
            "Failed to update sensor #", id, " readings ",
            "(Sensor does not exist)"
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

    if (!(intern::sta_drdyp[id - 1] && intern::sta_drdyt[id - 1])) {
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

    oupt_pres = intern::conv_val_to_oupt(intern::pres[id - 1]);
    oupt_temp = intern::conv_val_to_oupt(intern::temp[id - 1]);
    logging::inf(intern::mod,
        "Updated sensor #", id, " readings: ",
        "Pres: ", oupt_pres, "Pa, Temp: ", oupt_temp, "°C"
    );

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
            addr = BMP388_ADDR1;
            break;
        case 2:
            addr = BMP388_ADDR2;
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
            addr = BMP388_ADDR1;
            break;
        case 2:
            addr = BMP388_ADDR2;
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
        logging::err(
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
        logging::err(
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

    odr = BMP388_PUT_BITS(odr, BMP388_BIT_ODR, drate);

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

    osr = BMP388_PUT_BITS(osr, BMP388_BIT_OSRP, ovsmp_pres);
    osr = BMP388_PUT_BITS(osr, BMP388_BIT_OSRT, ovsmp_temp);

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
        "Setting sensor #", id, " IIR coefficient"
    );

    cfg = BMP388_PUT_BITS(cfg, BMP388_BIT_IIR, iir);

    put_reg(id, BMP388_REG_CFG, cfg);
    if (fail) {
        logging::err(mod,
            "Failed to set sensor #", id, " IIR coefficient"
        );
        fail = true;
        return;
    }

    fail = false;
}

void get_sta (int id) {
    std::vector<std::uint8_t> sta;
    std::string oupt_sta_crdy, oupt_sta_drdyp, oupt_sta_drdyt;

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

    oupt_sta_crdy = conv_val_to_oupt(sta_crdy[id - 1]);
    oupt_sta_drdyp = conv_val_to_oupt(sta_drdyp[id - 1]);
    oupt_sta_drdyt = conv_val_to_oupt(sta_drdyt[id - 1]);
    logging::inf(mod,
        "Got sensor #", id, " status flags: ",
        "Cmd rdy: ", oupt_sta_crdy, ", ",
        "Pres drdy: ", oupt_sta_drdyp, ", ",
        "Temp drdy: ", oupt_sta_drdyt
    );

    fail = false;
}

void get_err (int id) {
    std::vector<std::uint8_t> err;
    std::string oupt_err_ftl, oupt_err_cmd, oupt_err_cfg;

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

    oupt_err_ftl = conv_val_to_oupt(err_ftl[id - 1]);
    oupt_err_cmd = conv_val_to_oupt(err_cmd[id - 1]);
    oupt_err_cfg = conv_val_to_oupt(err_cfg[id - 1]);
    logging::inf(mod,
        "Got sensor #", id, " error flags: ",
        "Fatal err: ", oupt_err_ftl, ", ",
        "Cmd err: ", oupt_err_cmd, ", ",
        "Config err: ", oupt_err_cfg
    );

    fail = false;
}

void get_cal (int id) {
    std::vector<std::uint8_t> cal;
    std::string oupt_cal_pres, oupt_cal_temp;

    logging::inf(mod,
        "Getting sensor #", id, " calibration parameters"
    );

    cal = get_reg(id, BMP388_REG_CAL, 21);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", id, " calibration parameters"
        );
        fail = true;
        return;
    }

    cal_pres[id - 1][0] = (double)((std::int16_t)(cal[5] | (cal[6] << 8)));
    cal_pres[id - 1][1] = (double)((std::int16_t)(cal[7] | (cal[8] << 8)));
    cal_pres[id - 1][2] = (double)((std::int8_t)(cal[9]));
    cal_pres[id - 1][3] = (double)((std::int8_t)(cal[10]));
    cal_pres[id - 1][4] = (double)((std::uint16_t)(cal[11] | (cal[12] << 8)));
    cal_pres[id - 1][5] = (double)((std::uint16_t)(cal[13] | (cal[14] << 8)));
    cal_pres[id - 1][6] = (double)((std::int8_t)(cal[15]));
    cal_pres[id - 1][7] = (double)((std::int8_t)(cal[16]));
    cal_pres[id - 1][8] = (double)((std::int16_t)(cal[17] | (cal[18] << 8)));
    cal_pres[id - 1][9] = (double)((std::int8_t)(cal[19]));
    cal_pres[id - 1][10] = (double)((std::int8_t)(cal[20]));
    cal_temp[id - 1][0] = (double)((std::uint16_t)(cal[0] | (cal[1] << 8)));
    cal_temp[id - 1][1] = (double)((std::uint16_t)(cal[2] | (cal[3] << 8)));
    cal_temp[id - 1][2] = (double)((std::int8_t)(cal[4]));

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

    oupt_cal_pres = conv_val_to_oupt(cal_pres[id - 1]);
    oupt_cal_temp = conv_val_to_oupt(cal_temp[id - 1]);
    logging::inf(mod,
        "Got sensor #", id, " calibration parameters: ",
        "Pres cal: ", oupt_cal_pres, ", Temp cal: ", oupt_cal_temp
    );

    fail = false;
}

void get_dat (int id) {
    std::vector<std::uint8_t> dat;
    double raw_pres, raw_temp;
    double aux[6];
    std::string oupt_pres, oupt_temp;

    logging::inf(mod,
        "Getting sensor #", id, " data"
    );

    dat = get_reg(id, BMP388_REG_DAT, 6);
    if (fail) {
        logging::err(mod,
            "Failed to get sensor #", id, " data"
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

    fail = false;
}

}

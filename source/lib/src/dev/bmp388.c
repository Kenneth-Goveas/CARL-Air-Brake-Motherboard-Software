#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <math.h>

#include <util/log.h>

#include <dev/i2c.h>
#include <dev/bmp388.h>

LOG_DECLARE_MODULE("bmp388");

static double _cal_t1[3], _cal_p1[11];
static double _cal_t2[3], _cal_p2[11];

static double _pres1, _temp1, _alt1;
static double _pres2, _temp2, _alt2;

static int _conv_num_to_addr (uint8_t * addr, int num) {
    switch (num) {
        case 1:
            *addr = BMP388_ADDR1;
            return 0;
        case 2:
            *addr = BMP388_ADDR2;
            return 0;
        default:
            return -1;
    }
}

static void _conv_buf_to_str (char ** str, const uint8_t * buf, size_t len) {
    *str = (char *)malloc(3 * len + 5);

    sprintf(*str, "(0x) ");
    for (size_t i = 0; i < len; i++) {
        sprintf(*str + 3 * i + 5, "%02" PRIx8 " ", buf[i]);
    }

    (*str)[3 * len + 4] = '\0';
}

static void _conv_drate_to_str (char ** str, bmp388_drate_t drate) {
    switch (drate) {
        case BMP388_DRATE_200_000_HZ:
            asprintf(str, "200Hz");
            break;
        case BMP388_DRATE_100_000_HZ:
            asprintf(str, "100Hz");
            break;
        case BMP388_DRATE_050_000_HZ:
            asprintf(str, "50Hz");
            break;
        case BMP388_DRATE_025_000_HZ:
            asprintf(str, "25Hz");
            break;
        case BMP388_DRATE_012_500_HZ:
            asprintf(str, "12.5Hz");
            break;
        case BMP388_DRATE_006_250_HZ:
            asprintf(str, "6.25Hz");
            break;
        case BMP388_DRATE_003_125_HZ:
            asprintf(str, "3.125Hz");
            break;
        case BMP388_DRATE_001_562_HZ:
            asprintf(str, "1.562Hz");
            break;
        case BMP388_DRATE_000_781_HZ:
            asprintf(str, "0.781Hz");
            break;
        case BMP388_DRATE_000_391_HZ:
            asprintf(str, "0.391Hz");
            break;
        case BMP388_DRATE_000_195_HZ:
            asprintf(str, "0.195Hz");
            break;
        case BMP388_DRATE_000_098_HZ:
            asprintf(str, "0.098Hz");
            break;
        case BMP388_DRATE_000_049_HZ:
            asprintf(str, "0.049Hz");
            break;
        case BMP388_DRATE_000_024_HZ:
            asprintf(str, "0.024Hz");
            break;
        case BMP388_DRATE_000_012_HZ:
            asprintf(str, "0.012Hz");
            break;
        case BMP388_DRATE_000_006_HZ:
            asprintf(str, "0.006Hz");
            break;
        case BMP388_DRATE_000_003_HZ:
            asprintf(str, "0.003Hz");
            break;
        case BMP388_DRATE_000_002_HZ:
            asprintf(str, "0.002Hz");
            break;
    }
}

static void _conv_ovsmp_to_str (
    char ** str, bmp388_ovsmp_t pres_ovsmp, bmp388_ovsmp_t temp_ovsmp
) {
    char * str_pres, * str_temp;

    switch (pres_ovsmp) {
        case BMP388_OVSMP_01X:
            asprintf(&str_pres, "1x");
            break;
        case BMP388_OVSMP_02X:
            asprintf(&str_pres, "2x");
            break;
        case BMP388_OVSMP_04X:
            asprintf(&str_pres, "4x");
            break;
        case BMP388_OVSMP_08X:
            asprintf(&str_pres, "8x");
            break;
        case BMP388_OVSMP_16X:
            asprintf(&str_pres, "16x");
            break;
        case BMP388_OVSMP_32X:
            asprintf(&str_pres, "32x");
            break;
    }

    switch (temp_ovsmp) {
        case BMP388_OVSMP_01X:
            asprintf(&str_temp, "1x");
            break;
        case BMP388_OVSMP_02X:
            asprintf(&str_temp, "2x");
            break;
        case BMP388_OVSMP_04X:
            asprintf(&str_temp, "4x");
            break;
        case BMP388_OVSMP_08X:
            asprintf(&str_temp, "8x");
            break;
        case BMP388_OVSMP_16X:
            asprintf(&str_temp, "16x");
            break;
        case BMP388_OVSMP_32X:
            asprintf(&str_temp, "32x");
            break;
    }

    asprintf(str, "Pres ovsmp: %s, Temp ovsmp: %s", str_pres, str_temp);

    free(str_pres);
    free(str_temp);
}

static void _conv_iir_to_str (char ** str, bmp388_iir_t iir) {
    switch (iir) {
        case BMP388_IIR_000:
            asprintf(str, "0");
            break;
        case BMP388_IIR_001:
            asprintf(str, "1");
            break;
        case BMP388_IIR_003:
            asprintf(str, "3");
            break;
        case BMP388_IIR_007:
            asprintf(str, "7");
            break;
        case BMP388_IIR_015:
            asprintf(str, "15");
            break;
        case BMP388_IIR_031:
            asprintf(str, "31");
            break;
        case BMP388_IIR_063:
            asprintf(str, "63");
            break;
        case BMP388_IIR_127:
            asprintf(str, "127");
            break;
    }
}

static int _reg_write (int num, uint8_t reg, uint8_t val) {
    int ret;
    uint8_t addr;
    uint8_t cmd [] = {reg, val};

    ret = _conv_num_to_addr(&addr, num);
    if (ret < 0) {
        LOG_ERR("Sensor #%d does not exist", num);
        return -1;
    }

    LOG_INF(
        "Writing to sensor #%d: Reg: 0x%02" PRIx8 ", Data: 0x%02" PRIx8,
        num, reg, val
    );

    i2c_send(addr, cmd, 2);

    ret = i2c_exec();
    if (ret < 0) {
        LOG_ERR("Failed to write to sensor #%d", num);
        return -1;
    }

    return 0;
}

static int _reg_read (int num, uint8_t reg, uint8_t * buf, size_t len) {
    int ret;
    uint8_t addr;
    uint8_t cmd [] = {reg};
    char * buf_str;

    ret = _conv_num_to_addr(&addr, num);
    if (ret < 0) {
        LOG_ERR("Sensor #%d does not exist", num);
        return -1;
    }

    LOG_INF(
        "Reading from sensor #%d: Reg: 0x%02" PRIx8 ", Len: %zu",
        num, reg, len
    );

    i2c_send(addr, cmd, 1);
    i2c_recv(addr, buf, len);

    ret = i2c_exec();
    if (ret < 0) {
        LOG_ERR("Failed to read from sensor #%d", num);
        return -1;
    }

    _conv_buf_to_str(&buf_str, buf, len);
    LOG_INF("Read from sensor #%d: Data: %s", buf_str);
    free(buf_str);

    return 0;
}

static int _set_pwr_ctrl_slp (int num) {
    int ret;
    uint8_t err, pwr_ctrl;

    LOG_INF("Setting sensor #%d power mode: Sleep", num);

    pwr_ctrl = 0;
    pwr_ctrl = BMP388_SET_BITS(pwr_ctrl, BMP388_BIT_PRES_EN, 1);
    pwr_ctrl = BMP388_SET_BITS(pwr_ctrl, BMP388_BIT_TEMP_EN, 1);
    pwr_ctrl = BMP388_SET_BITS(pwr_ctrl, BMP388_BIT_MODE, BMP388_PRST_MODE_SLP);

    ret = _reg_write(num, BMP388_REG_PWR_CTRL, pwr_ctrl);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d power mode", num);
        return -1;
    }

    ret = _reg_read(num, BMP388_REG_ERR, &err, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d power mode", num);
        return -1;
    }

    if (
           BMP388_GET_BITS(err, BMP388_BIT_FTL_ERR)
        || BMP388_GET_BITS(err, BMP388_BIT_CMD_ERR)
        || BMP388_GET_BITS(err, BMP388_BIT_CFG_ERR)
    ) {
        LOG_ERR("Failed to set sensor #%d power mode (Sensor error)", num);
        return -1;
    }

    return 0;
}

static int _set_pwr_ctrl_nrm (int num) {
    int ret;
    uint8_t err, pwr_ctrl;

    LOG_INF("Setting sensor #%d power mode: Normal", num);

    pwr_ctrl = 0;
    pwr_ctrl = BMP388_SET_BITS(pwr_ctrl, BMP388_BIT_PRES_EN, 1);
    pwr_ctrl = BMP388_SET_BITS(pwr_ctrl, BMP388_BIT_TEMP_EN, 1);
    pwr_ctrl = BMP388_SET_BITS(pwr_ctrl, BMP388_BIT_MODE, BMP388_PRST_MODE_NRM);

    ret = _reg_write(num, BMP388_REG_PWR_CTRL, pwr_ctrl);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d power mode", num);
        return -1;
    }

    ret = _reg_read(num, BMP388_REG_ERR, &err, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d power mode", num);
        return -1;
    }

    if (
           BMP388_GET_BITS(err, BMP388_BIT_FTL_ERR)
        || BMP388_GET_BITS(err, BMP388_BIT_CMD_ERR)
        || BMP388_GET_BITS(err, BMP388_BIT_CFG_ERR)
    ) {
        LOG_ERR("Failed to set sensor #%d power mode (Sensor error)", num);
        return -1;
    }

    return 0;
}

static int _set_odr (int num, bmp388_drate_t drate) {
    int ret;
    uint8_t err, odr;
    char * drate_str;

    _conv_drate_to_str(&drate_str, drate);
    LOG_INF("Setting sensor #%d data rate: %s", num, drate_str);
    free(drate_str);

    odr = 0;
    odr = BMP388_SET_BITS(odr, BMP388_BIT_ODR, drate);

    ret = _reg_write(num, BMP388_REG_ODR, odr);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d data rate", num);
        return -1;
    }

    ret = _reg_read(num, BMP388_REG_ERR, &err, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d data rate", num);
        return -1;
    }

    if (
           BMP388_GET_BITS(err, BMP388_BIT_FTL_ERR)
        || BMP388_GET_BITS(err, BMP388_BIT_CMD_ERR)
        || BMP388_GET_BITS(err, BMP388_BIT_CFG_ERR)
    ) {
        LOG_ERR("Failed to set sensor #%d data rate (Sensor error)", num);
        return -1;
    }

    return 0;
}

static int _set_osr (
    int num, bmp388_ovsmp_t pres_ovsmp, bmp388_ovsmp_t temp_ovsmp
) {
    int ret;
    uint8_t err, osr;
    char * ovsmp_str;

    _conv_ovsmp_to_str(&ovsmp_str, pres_ovsmp, temp_ovsmp);
    LOG_INF("Setting sensor #%d oversampling: %s", num, ovsmp_str);
    free(ovsmp_str);

    osr = 0;
    osr = BMP388_SET_BITS(osr, BMP388_BIT_OSRP, pres_ovsmp);
    osr = BMP388_SET_BITS(osr, BMP388_BIT_OSRT, temp_ovsmp);

    ret = _reg_write(num, BMP388_REG_OSR, osr);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d oversampling", num);
        return -1;
    }

    ret = _reg_read(num, BMP388_REG_ERR, &err, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d oversampling", num);
        return -1;
    }

    if (
           BMP388_GET_BITS(err, BMP388_BIT_FTL_ERR)
        || BMP388_GET_BITS(err, BMP388_BIT_CMD_ERR)
        || BMP388_GET_BITS(err, BMP388_BIT_CFG_ERR)
    ) {
        LOG_ERR("Failed to set sensor #%d oversampling (Sensor error)", num);
        return -1;
    }

    return 0;
}

static int _set_cfg (int num, bmp388_iir_t iir) {
    int ret;
    uint8_t err, cfg;
    char * iir_str;

    _conv_iir_to_str(&iir_str, iir);
    LOG_INF("Setting sensor #%d IIR filter coefficient: %s", num, iir_str);
    free(iir_str);

    cfg = 0;
    cfg = BMP388_SET_BITS(cfg, BMP388_BIT_IIR, iir);

    ret = _reg_write(num, BMP388_REG_CFG, cfg);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d IIR filter coefficient", num);
        return -1;
    }

    ret = _reg_read(num, BMP388_REG_ERR, &err, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d IIR filter coefficient", num);
        return -1;
    }

    if (
           BMP388_GET_BITS(err, BMP388_BIT_FTL_ERR)
        || BMP388_GET_BITS(err, BMP388_BIT_CMD_ERR)
        || BMP388_GET_BITS(err, BMP388_BIT_CFG_ERR)
    ) {
        LOG_ERR(
            "Failed to set sensor #%d IIR filter coefficient (Sensor error)",
            num
        );
        return -1;
    }

    return 0;
}

static int _get_stat (
    int num, bool * cmd_rdy, bool * drdy_pres, bool * drdy_temp
) {
    int ret;
    uint8_t err, stat;

    LOG_INF("Getting sensor #%d status information", num);

    ret = _reg_read(num, BMP388_REG_ERR, &err, 1);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d status information", num);
        return -1;
    }

    if (
           BMP388_GET_BITS(err, BMP388_BIT_FTL_ERR)
        || BMP388_GET_BITS(err, BMP388_BIT_CMD_ERR)
        || BMP388_GET_BITS(err, BMP388_BIT_CFG_ERR)
    ) {
        LOG_ERR(
            "Failed to get sensor #%d status information (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_read(num, BMP388_REG_STAT, &stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d status information", num);
        return -1;
    }

    if (BMP388_GET_BITS(stat, BMP388_BIT_CMD_RDY)) {
        *cmd_rdy = true;
    } else {
        *cmd_rdy = false;
    }

    if (BMP388_GET_BITS(stat, BMP388_BIT_DRDY_PRES)) {
        *drdy_pres = true;
    } else {
        *drdy_pres = false;
    }

    if (BMP388_GET_BITS(stat, BMP388_BIT_DRDY_TEMP)) {
        *drdy_temp = true;
    } else {
        *drdy_temp = false;
    }

    LOG_INF(
        "Got sensor #%d status information: "
        "Cmd rdy: %s, Pres drdy: %s, Temp drdy: %s",
        num,
        *cmd_rdy ? "True" : "False",
        *drdy_pres ? "True" : "False",
        *drdy_temp ? "True" : "False"
    );

    return 0;
}

static int _get_cal (int num, double * cal_t, double * cal_p) {
    int ret;
    uint8_t err, cal[21];
    uint16_t cal_t1, cal_t2;
    int8_t cal_t3;
    int16_t cal_p1, cal_p2, cal_p9;
    int8_t cal_p3, cal_p4, cal_p7, cal_p8, cal_p10, cal_p11;
    uint16_t cal_p5, cal_p6;

    LOG_INF("Getting sensor #%d calibration parameters", num);

    ret = _reg_read(num, BMP388_REG_ERR, &err, 1);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d calibration parameters", num);
        return -1;
    }

    if (
           BMP388_GET_BITS(err, BMP388_BIT_FTL_ERR)
        || BMP388_GET_BITS(err, BMP388_BIT_CMD_ERR)
        || BMP388_GET_BITS(err, BMP388_BIT_CFG_ERR)
    ) {
        LOG_ERR(
            "Failed to get sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_read(num, BMP388_REG_CAL, cal, 21);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d calibration parameters", num);
        return -1;
    }

    cal_t1 = cal[0] | (cal[1] << 8);
    cal_t2 = cal[2] | (cal[3] << 8);
    cal_t3 = cal[4];
    cal_p1 = cal[5] | (cal[6] << 8);
    cal_p2 = cal[7] | (cal[8] << 8);
    cal_p3 = cal[9];
    cal_p4 = cal[10];
    cal_p5 = cal[11] | (cal[12] << 8);
    cal_p6 = cal[13] | (cal[14] << 8);
    cal_p7 = cal[15];
    cal_p8 = cal[16];
    cal_p9 = cal[17] | (cal[18] << 8);
    cal_p10 = cal[19];
    cal_p11 = cal[20];

    cal_t[0] = (double)(cal_t1) * pow(2, 8);
    cal_t[1] = (double)(cal_t2) / pow(2, 30);
    cal_t[2] = (double)(cal_t3) / pow(2, 48);
    cal_p[0] = ((double)(cal_p1) - pow(2, 14)) / pow(2, 20);
    cal_p[1] = ((double)(cal_p2) - pow(2, 14)) / pow(2, 29);
    cal_p[2] = (double)(cal_p3) / pow(2, 32);
    cal_p[3] = (double)(cal_p4) / pow(2, 37);
    cal_p[4] = (double)(cal_p5) * pow(2, 3);
    cal_p[5] = (double)(cal_p6) / pow(2, 6);
    cal_p[6] = (double)(cal_p7) / pow(2, 8);
    cal_p[7] = (double)(cal_p8) / pow(2, 15);
    cal_p[8] = (double)(cal_p9) / pow(2, 48);
    cal_p[9] = (double)(cal_p10) / pow(2, 48);
    cal_p[10] = (double)(cal_p11) / pow(2, 65);

    LOG_INF(
        "Got sensor #%d calibration parameters: "
        "Cal T: ["
        "%.3e, %.3e, %.3e"
        "], "
        "Cal P: ["
        "%.3e, %.3e, %.3e, %.3e, %.3e, %.3e, %.3e, %.3e, %.3e, %.3e, %.3e"
        "]",
        num,
        cal_t[0], cal_t[1], cal_t[2],
        cal_p[0], cal_p[1], cal_p[2], cal_p[3], cal_p[4], cal_p[5], cal_p[6],
        cal_p[7], cal_p[8], cal_p[9], cal_p[10]
    );

    return 0;
}

static int _get_data (
    int num,
    double * pres, double * temp, double * alt,
    const double * cal_t, const double * cal_p
) {
    int ret;
    uint8_t err, data[6];
    double pres_raw, temp_raw;
    double aux[6];

    LOG_INF("Getting sensor #%d data", num);

    ret = _reg_read(num, BMP388_REG_ERR, &err, 1);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d data", num);
        return -1;
    }

    if (
           BMP388_GET_BITS(err, BMP388_BIT_FTL_ERR)
        || BMP388_GET_BITS(err, BMP388_BIT_CMD_ERR)
        || BMP388_GET_BITS(err, BMP388_BIT_CFG_ERR)
    ) {
        LOG_ERR("Failed to get sensor #%d data (Sensor error)", num);
        return -1;
    }

    ret = _reg_read(num, BMP388_REG_DATA, data, 6);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d data", num);
        return -1;
    }

    pres_raw = data[0] | (data[1] << 8) | (data[2] << 16);
    temp_raw = data[3] | (data[4] << 8) | (data[5] << 16);

    aux[0] = temp_raw - cal_t[0];
    aux[1] = aux[0] * cal_t[1];

    *temp = aux[1] + aux[0] * aux[0] * cal_t[2];

    aux[0] = cal_p[5] * (*temp);
    aux[1] = cal_p[6] * (*temp) * (*temp);
    aux[2] = cal_p[7] * (*temp) * (*temp) * (*temp);
    aux[4] = cal_p[4] + aux[0] + aux[1] + aux[2];

    aux[0] = cal_p[1] * (*temp);
    aux[1] = cal_p[2] * (*temp) * (*temp);
    aux[2] = cal_p[3] * (*temp) * (*temp) * (*temp);
    aux[5] = pres_raw * (cal_p[0] + aux[0] + aux[1] + aux[2]);

    aux[0] = pres_raw * pres_raw;
    aux[1] = cal_p[8] + cal_p[9] * (*temp);
    aux[2] = aux[0] * aux[1];
    aux[3] = aux[2] + cal_p[10] * pres_raw * pres_raw * pres_raw;

    *pres = aux[3] + aux[4] + aux[5];

    *alt = 44330.8 - 4952.76 * pow(*pres, 0.190154);

    LOG_INF(
        "Got sensor #%d data: Pres: %.3ePa, Temp: %.3e°C, Alt: %.3em",
        num, *pres, *temp, *alt
    );

    return 0;
}

static int _sys_wait (int num) {
    int ret;
    bool cmd_rdy, drdy_pres, drdy_temp;

    LOG_INF("Waiting for sensor #%d to become ready", num);

    do {
        ret = _get_stat(num, &cmd_rdy, &drdy_pres, &drdy_temp);
        if (ret < 0) {
            LOG_ERR("Failed to wait for sensor #%d to become ready", num);
            return -1;
        }
    } while (!cmd_rdy);

    return 0;
}

static int _sys_reset (int num) {
    int ret;
    uint8_t err, cmd;

    LOG_INF("Resetting sensor #%d", num);

    cmd = BMP388_PRST_CMD_RST;

    ret = _reg_write(num, BMP388_REG_CMD, cmd);
    if (ret < 0) {
        LOG_ERR("Failed to reset sensor #%d", num);
        return -1;
    }

    ret = _reg_read(num, BMP388_REG_ERR, &err, 1);
    if (ret < 0) {
        LOG_ERR("Failed to reset sensor #%d", num);
        return -1;
    }

    if (
           BMP388_GET_BITS(err, BMP388_BIT_FTL_ERR)
        || BMP388_GET_BITS(err, BMP388_BIT_CMD_ERR)
        || BMP388_GET_BITS(err, BMP388_BIT_CFG_ERR)
    ) {
        LOG_ERR("Failed to reset sensor #%d (Sensor error)", num);
        return -1;
    }

    return 0;
}

static int _init (
    int num,
    bmp388_drate_t drate,
    bmp388_ovsmp_t pres_ovsmp, bmp388_ovsmp_t temp_ovsmp,
    bmp388_iir_t iir,
    double * cal_t, double * cal_p
) {
    int ret;

    LOG_INF("Initializing sensor #%d", num);

    ret = _sys_wait(num);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d", num);
        return -1;
    }

    ret = _sys_reset(num);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d", num);
        return -1;
    }

    ret = _set_pwr_ctrl_slp(num);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d", num);
        return -1;
    }

    ret = _set_odr(num, drate);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d", num);
        return -1;
    }

    ret = _set_osr(num, pres_ovsmp, temp_ovsmp);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d", num);
        return -1;
    }

    ret = _set_cfg(num, iir);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d", num);
        return -1;
    }

    ret = _get_cal(num, cal_t, cal_p);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d", num);
        return -1;
    }

    ret = _set_pwr_ctrl_nrm(num);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d", num);
        return -1;
    }

    return 0;
}

static int _update (
    int num,
    double * pres, double * temp, double * alt,
    const double * cal_t, const double * cal_p
) {
    int ret;
    bool cmd_rdy, drdy_pres, drdy_temp;

    *pres = NAN;
    *temp = NAN;
    *alt = NAN;

    LOG_INF("Getting sensor #%d readings", num);

    ret = _get_stat(num, &cmd_rdy, &drdy_pres, &drdy_temp);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d readings", num);
        return -1;
    }

    if (!(drdy_pres && drdy_temp)) {
        LOG_WRN("Sensor #%d measurement incomplete", num);
        return 0;
    }

    ret = _get_data(num, pres, temp, alt, cal_t, cal_p);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d readings", num);
        return -1;
    }

    return 1;
}

int bmp388_init (
    bmp388_drate_t drate1, bmp388_drate_t drate2,
    bmp388_ovsmp_t pres_ovsmp1, bmp388_ovsmp_t pres_ovsmp2,
    bmp388_ovsmp_t temp_ovsmp1, bmp388_ovsmp_t temp_ovsmp2,
    bmp388_iir_t iir1, bmp388_iir_t iir2
) {
    int ret;

    LOG_INF("Initializing sensors");

    ret = _init(1, drate1, pres_ovsmp1, temp_ovsmp1, iir1, _cal_t1, _cal_p1);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensors");
        return -1;
    }

    ret = _init(2, drate2, pres_ovsmp2, temp_ovsmp2, iir2, _cal_t2, _cal_p2);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensors");
        return -1;
    }

    return 0;
}

int bmp388_update (void) {
    int ret;
    bool fail1, fail2;
    bool avail1, avail2;

    LOG_INF("Getting sensor readings");

    ret = _update(1, &_pres1, &_temp1, &_alt1, _cal_t1, _cal_p1);
    if (ret < 0) {
        LOG_WRN("Sensor #1 readings unavailable due to failure");
        fail1 = true;
        avail1 = false;
    } else if (ret == 0) {
        LOG_WRN("Sensor #1 readings unavailable due to incomplete measurement");
        fail1 = false;
        avail1 = false;
    } else {
        fail1 = false;
        avail1 = true;
    }

    ret = _update(2, &_pres2, &_temp2, &_alt2, _cal_t2, _cal_p2);
    if (ret < 0) {
        LOG_WRN("Sensor #2 readings unavailable due to failure");
        fail2 = true;
        avail2 = false;
    } else if (ret == 0) {
        LOG_WRN("Sensor #2 readings unavailable due to incomplete measurement");
        fail2 = false;
        avail2 = false;
    } else {
        fail2 = false;
        avail2 = true;
    }

    if (fail1 && fail2) {
        LOG_ERR("Failed to get sensor readings");
        return -1;
    }

    if (!(avail1 || avail2)) {
        LOG_WRN("Sensor measurements incomplete");
        return 0;
    }

    return 1;
}

void bmp388_get_pres (double * pres) {
    LOG_INF("Getting combined pressure reading");

    if (!isnan(_pres1) && !isnan(_pres2)) {
        *pres = 0.5 * (_pres1 + _pres2);
    } else if (!isnan(_pres1)) {
        *pres = _pres1;
    } else if (!isnan(_pres2)) {
        *pres = _pres2;
    } else {
        *pres = NAN;
    }

    LOG_INF("Got combined pressure reading: %.3ePa", *pres);
}

void bmp388_get_temp (double * temp) {
    LOG_INF("Getting combined temperature reading");

    if (!isnan(_temp1) && !isnan(_temp2)) {
        *temp = 0.5 * (_temp1 + _temp2);
    } else if (!isnan(_temp1)) {
        *temp = _temp1;
    } else if (!isnan(_temp2)) {
        *temp = _temp2;
    } else {
        *temp = NAN;
    }

    LOG_INF("Got combined temperature reading: %.3e°C", *temp);
}

void bmp388_get_alt (double * alt) {
    LOG_INF("Getting combined altitude reading");

    if (!isnan(_alt1) && !isnan(_alt2)) {
        *alt = 0.5 * (_alt1 + _alt2);
    } else if (!isnan(_alt1)) {
        *alt = _alt1;
    } else if (!isnan(_alt2)) {
        *alt = _alt2;
    } else {
        *alt = NAN;
    }

    LOG_INF("Got combined altitude reading: %.3em", *alt);
}

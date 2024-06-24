#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <math.h>

#include <util/log.h>

#include <dev/i2c.h>
#include <dev/bno055.h>

LOG_DECLARE_MODULE("bno055");

static double _rot1[3], _att1[4], _acc1[3];
static double _rot2[3], _att2[4], _acc2[3];

static int _conv_num_to_addr (uint8_t * addr, int num) {
    switch (num) {
        case 1:
            *addr = BNO055_ADDR1;
            return 0;
        case 2:
            *addr = BNO055_ADDR2;
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

static void _conv_axes_to_map_str (char ** str, const bno055_axis_t * axes) {
    char map[3];

    for (int i = 0; i < 3; i++) {
        switch ((uint8_t)(axes[i])) {
            case BNO055_PRST_MAP_X:
                map[i] = 'X';
                break;
            case BNO055_PRST_MAP_Y:
                map[i] = 'Y';
                break;
            case BNO055_PRST_MAP_Z:
                map[i] = 'Z';
                break;
            default:
                map[i] = '?';
                break;
        }
    }

    asprintf(str, "Map X: %c, Map Y: %c, Map Z: %c", map[0], map[1], map[2]);
}

static void _conv_axes_to_sgn_str (char ** str, const bno055_axis_t * axes) {
    char sgn[3];

    for (int i = 0; i < 3; i++) {
        switch ((uint8_t)(axes[i] >> 8)) {
            case BNO055_PRST_SGN_P:
                sgn[i] = 'P';
                break;
            case BNO055_PRST_SGN_N:
                sgn[i] = 'N';
                break;
            default:
                sgn[i] = '?';
                break;
        }
    }

    asprintf(str, "Sign X: %c, Sign Y: %c, Sign Z: %c", sgn[0], sgn[1], sgn[2]);
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

static int _sys_reset (int num) {
    int ret;
    uint8_t sys_stat, sys_trig;

    LOG_INF("Resetting sensor #%d", num);

    sys_trig = 0;
    sys_trig = BNO055_SET_BITS(sys_trig, BNO055_BIT_RST_SYS, 1);

    ret = _reg_write(num, BNO055_REG_SYS_TRIG, sys_trig);
    if (ret < 0) {
        LOG_ERR("Failed to reset sensor #%d", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to reset sensor #%d", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR("Failed to reset sensor #%d (Sensor error)", num);
        return -1;
    }

    return 0;
}

static int _sys_wait (int num) {
    int ret;
    uint8_t sys_stat;

    LOG_INF("Waiting for sensor #%d to become ready", num);

    do {
        ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
        if (ret < 0) {
            LOG_ERR("Failed to wait for sensor #%d to become ready", num);
            return -1;
        }

        if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
            LOG_ERR(
                "Failed to wait for sensor #%d to become ready (Sensor error)",
                num
            );
            return -1;
        }
    } while (sys_stat != BNO055_PRST_SYS_STAT_IDLE);

    return 0;
}

static int _set_pwr_mode (int num) {
    int ret;
    uint8_t sys_stat, pwr_mode;

    LOG_INF("Setting sensor #%d power mode: Normal", num);

    pwr_mode = 0;
    pwr_mode = BNO055_SET_BITS(
        pwr_mode, BNO055_BIT_PWR_MODE, BNO055_PRST_PWR_MODE_NRM
    );

    ret = _reg_write(num, BNO055_REG_PWR_MODE, pwr_mode);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d power mode", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d power mode", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR("Failed to set sensor #%d power mode (Sensor error)", num);
        return -1;
    }

    return 0;
}

static int _set_opr_mode_cfg (int num) {
    int ret;
    uint8_t sys_stat, opr_mode;

    LOG_INF("Setting sensor #%d operating mode: Config", num);

    opr_mode = 0;
    opr_mode = BNO055_SET_BITS(
        opr_mode, BNO055_BIT_OPR_MODE, BNO055_PRST_OPR_MODE_CFG
    );

    ret = _reg_write(num, BNO055_REG_OPR_MODE, opr_mode);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d operating mode", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d operating mode", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR("Failed to set sensor #%d operating mode (Sensor error)", num);
        return -1;
    }

    return 0;
}

static int _set_opr_mode_ndof (int num) {
    int ret;
    uint8_t sys_stat, opr_mode;

    LOG_INF("Setting sensor #%d operating mode: NDOF", num);

    opr_mode = 0;
    opr_mode = BNO055_SET_BITS(
        opr_mode, BNO055_BIT_OPR_MODE, BNO055_PRST_OPR_MODE_NDOF
    );

    ret = _reg_write(num, BNO055_REG_OPR_MODE, opr_mode);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d operating mode", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d operating mode", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR("Failed to set sensor #%d operating mode (Sensor error)", num);
        return -1;
    }

    return 0;
}

static int _set_unit_sel (int num) {
    int ret;
    uint8_t sys_stat, unit_sel;

    LOG_INF(
        "Setting sensor #%d measurement units: "
        "Ori: Android, Gyr: rad/s, Acc: m/s²",
        num
    );

    unit_sel = 0;
    unit_sel = BNO055_SET_BITS(unit_sel, BNO055_BIT_ORI, BNO055_PRST_ORI_ANDR);
    unit_sel = BNO055_SET_BITS(
        unit_sel, BNO055_BIT_GYR_UNIT, BNO055_PRST_GYR_UNIT_RPS
    );
    unit_sel = BNO055_SET_BITS(
        unit_sel, BNO055_BIT_ACC_UNIT, BNO055_PRST_ACC_UNIT_MS2
    );

    ret = _reg_write(num, BNO055_REG_UNIT_SEL, unit_sel);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d measurement units", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d measurement units", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d measurement units (Sensor error)",
            num
        );
        return -1;
    }

    return 0;
}

static int _set_axis_map (int num, const bno055_axis_t * axes) {
    int ret;
    uint8_t sys_stat, axis_map;
    char * axes_map_str;

    _conv_axes_to_map_str(&axes_map_str, axes);
    LOG_INF("Setting sensor #%d axis mappings: %s", num, axes_map_str);
    free(axes_map_str);

    axis_map = 0;
    axis_map = BNO055_SET_BITS(axis_map, BNO055_BIT_MAPX, (uint8_t)(axes[0]));
    axis_map = BNO055_SET_BITS(axis_map, BNO055_BIT_MAPY, (uint8_t)(axes[1]));
    axis_map = BNO055_SET_BITS(axis_map, BNO055_BIT_MAPZ, (uint8_t)(axes[2]));

    ret = _reg_write(num, BNO055_REG_AXIS_MAP, axis_map);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d axis mappings", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d axis mappings", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR("Failed to set sensor #%d axis mappings (Sensor error)", num);
        return -1;
    }

    return 0;
}

static int _set_axis_sgn (int num, const bno055_axis_t * axes) {
    int ret;
    uint8_t sys_stat, axis_sgn;
    char * axes_sgn_str;

    _conv_axes_to_sgn_str(&axes_sgn_str, axes);
    LOG_INF("Setting sensor #%d axis signs: %s", num, axes_sgn_str);
    free(axes_sgn_str);

    axis_sgn = 0;
    axis_sgn = BNO055_SET_BITS(axis_sgn, BNO055_BIT_SGNX, axes[0] >> 8);
    axis_sgn = BNO055_SET_BITS(axis_sgn, BNO055_BIT_SGNY, axes[1] >> 8);
    axis_sgn = BNO055_SET_BITS(axis_sgn, BNO055_BIT_SGNZ, axes[2] >> 8);

    ret = _reg_write(num, BNO055_REG_AXIS_SGN, axis_sgn);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d axis signs", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d axis signs", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR("Failed to set sensor #%d axis signs (Sensor error)", num);
        return -1;
    }

    return 0;
}

static int _set_cal_data (
    int num,
    int16_t acc_rad, int16_t mag_rad,
    const int16_t * acc_off, const int16_t * mag_off, const int16_t * gyr_off
) {
    int ret;
    uint8_t sys_stat;

    LOG_INF(
        "Setting sensor #%d calibration parameters: "
        "Acc rad: %" PRId16 ", "
        "Mag rad: %" PRId16 ", "
        "Acc off: [%" PRId16 ", %" PRId16 ", %" PRId16 "], "
        "Mag off: [%" PRId16 ", %" PRId16 ", %" PRId16 "], "
        "Gyr off: [%" PRId16 ", %" PRId16 ", %" PRId16 "]",
        num,
        acc_rad, mag_rad,
        acc_off[0], acc_off[1], acc_off[2],
        mag_off[0], mag_off[1], mag_off[2],
        gyr_off[0], gyr_off[1], gyr_off[2]
    );

    ret = _reg_write(num, BNO055_REG_ACC_XOFF_LSB, (uint8_t)(acc_off[0]));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_ACC_XOFF_MSB, (uint8_t)(acc_off[0] >> 8));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_ACC_YOFF_LSB, (uint8_t)(acc_off[1]));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_ACC_YOFF_MSB, (uint8_t)(acc_off[1] >> 8));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_ACC_ZOFF_LSB, (uint8_t)(acc_off[2]));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_ACC_ZOFF_MSB, (uint8_t)(acc_off[2] >> 8));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_MAG_XOFF_LSB, (uint8_t)(mag_off[0]));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_MAG_XOFF_MSB, (uint8_t)(mag_off[0] >> 8));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_MAG_YOFF_LSB, (uint8_t)(mag_off[1]));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_MAG_YOFF_MSB, (uint8_t)(mag_off[1] >> 8));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_MAG_ZOFF_LSB, (uint8_t)(mag_off[2]));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_MAG_ZOFF_MSB, (uint8_t)(mag_off[2] >> 8));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_GYR_XOFF_LSB, (uint8_t)(gyr_off[0]));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_GYR_XOFF_MSB, (uint8_t)(gyr_off[0] >> 8));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_GYR_YOFF_LSB, (uint8_t)(gyr_off[1]));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_GYR_YOFF_MSB, (uint8_t)(gyr_off[1] >> 8));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_GYR_ZOFF_LSB, (uint8_t)(gyr_off[2]));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_GYR_ZOFF_MSB, (uint8_t)(gyr_off[2] >> 8));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_ACC_RAD_LSB, (uint8_t)acc_rad);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_ACC_RAD_MSB, (uint8_t)(acc_rad >> 8));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_MAG_RAD_LSB, (uint8_t)mag_rad);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_write(num, BNO055_REG_MAG_RAD_MSB, (uint8_t)(mag_rad >> 8));
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to set sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    return 0;
}

static int _get_cal_stat (
    int num,
    uint8_t * sys_cal, uint8_t * acc_cal, uint8_t * mag_cal, uint8_t * gyr_cal
) {
    int ret;
    uint8_t sys_stat, cal_stat;

    LOG_INF("Getting sensor #%d calibration levels", num);

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d calibration levels", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to get sensor #%d calibration levels (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_CAL_STAT, &cal_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d calibration levels", num);
        return -1;
    }

    *sys_cal = BNO055_GET_BITS(cal_stat, BNO055_BIT_SYS_CAL);
    *acc_cal = BNO055_GET_BITS(cal_stat, BNO055_BIT_ACC_CAL);
    *mag_cal = BNO055_GET_BITS(cal_stat, BNO055_BIT_MAG_CAL);
    *gyr_cal = BNO055_GET_BITS(cal_stat, BNO055_BIT_GYR_CAL);

    LOG_INF(
        "Got sensor #%d calibration levels: "
        "Sys: %" PRIu8 ", Acc: %" PRIu8 ", Mag: %" PRIu8 ", Gyr: %" PRIu8,
        num, *sys_cal, *acc_cal, *mag_cal, *gyr_cal
    );

    return 0;
}

static int _get_cal_data (
    int num,
    int16_t * acc_rad, int16_t * mag_rad,
    int16_t * acc_off, int16_t * mag_off, int16_t * gyr_off
) {
    int ret;
    uint8_t sys_stat, cal_data[22];

    LOG_INF("Getting sensor #%d calibration parameters", num);

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d calibration parameters", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to get sensor #%d calibration parameters (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_CAL_DATA, cal_data, 22);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d calibration parameters");
        return -1;
    }

    acc_off[0] = cal_data[0] | (cal_data[1] << 8);
    acc_off[1] = cal_data[2] | (cal_data[3] << 8);
    acc_off[2] = cal_data[4] | (cal_data[5] << 8);

    mag_off[0] = cal_data[6] | (cal_data[7] << 8);
    mag_off[1] = cal_data[8] | (cal_data[9] << 8);
    mag_off[2] = cal_data[10] | (cal_data[11] << 8);

    gyr_off[0] = cal_data[12] | (cal_data[13] << 8);
    gyr_off[1] = cal_data[14] | (cal_data[15] << 8);
    gyr_off[2] = cal_data[16] | (cal_data[17] << 8);

    *acc_rad = cal_data[18] | (cal_data[19] << 8);
    *mag_rad = cal_data[20] | (cal_data[21] << 8);

    LOG_INF(
        "Got sensor #%d calibration parameters: "
        "Acc rad: %" PRId16 ", "
        "Mag rad: %" PRId16 ", "
        "Acc off: [%" PRId16 ", %" PRId16 ", %" PRId16 "], "
        "Mag off: [%" PRId16 ", %" PRId16 ", %" PRId16 "], "
        "Gyr off: [%" PRId16 ", %" PRId16 ", %" PRId16 "]",
        num,
        *acc_rad, *mag_rad,
        acc_off[0], acc_off[1], acc_off[2],
        mag_off[0], mag_off[1], mag_off[2],
        gyr_off[0], gyr_off[1], gyr_off[2]
    );

    return 0;
}

static int _get_gyr_data (int num, double * rot) {
    int ret;
    uint8_t sys_stat, gyr_data[6];

    LOG_INF("Getting sensor #%d angular velocity data", num);

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d angular velocity data", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to get sensor #%d angular velocity data (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_GYR_DATA, gyr_data, 6);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d angular velocity data", num);
        return -1;
    }

    rot[0] = (double)((int16_t)(gyr_data[0] | (gyr_data[1] << 8))) / 900.0;
    rot[1] = (double)((int16_t)(gyr_data[2] | (gyr_data[3] << 8))) / 900.0;
    rot[2] = (double)((int16_t)(gyr_data[4] | (gyr_data[5] << 8))) / 900.0;

    LOG_INF(
        "Got sensor #%d angular velocity data: [%.3e, %.3e, %.3e]rad/s",
        num, rot[0], rot[1], rot[2]
    );

    return 0;
}

static int _get_qua_data (int num, double * att) {
    int ret;
    uint8_t sys_stat, qua_data[8];

    LOG_INF("Getting sensor #%d attitude data", num);

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d attitude data", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR("Failed to get sensor #%d attitude data (Sensor error)", num);
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_QUA_DATA, qua_data, 8);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d attitude data", num);
        return -1;
    }

    att[0] = (double)((int16_t)(qua_data[0] | (qua_data[1] << 8))) / 16384.0;
    att[1] = (double)((int16_t)(qua_data[2] | (qua_data[3] << 8))) / 16384.0;
    att[2] = (double)((int16_t)(qua_data[4] | (qua_data[5] << 8))) / 16384.0;
    att[3] = (double)((int16_t)(qua_data[6] | (qua_data[7] << 8))) / 16384.0;

    LOG_INF(
        "Got sensor #%d attitude data: [%.3e, %.3e, %.3e, %.3e]",
        num, att[0], att[1], att[2], att[3]
    );

    return 0;
}

static int _get_lia_data (int num, double * acc) {
    int ret;
    uint8_t sys_stat, lia_data[6];

    LOG_INF("Getting sensor #%d acceleration data", num);

    ret = _reg_read(num, BNO055_REG_SYS_STAT, &sys_stat, 1);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d acceleration data", num);
        return -1;
    }

    if (sys_stat == BNO055_PRST_SYS_STAT_ERR) {
        LOG_ERR(
            "Failed to get sensor #%d acceleration data (Sensor error)",
            num
        );
        return -1;
    }

    ret = _reg_read(num, BNO055_REG_LIA_DATA, lia_data, 6);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d acceleration data", num);
        return -1;
    }

    acc[0] = (double)((int16_t)(lia_data[0] | (lia_data[1] << 8))) / 100.0;
    acc[1] = (double)((int16_t)(lia_data[2] | (lia_data[3] << 8))) / 100.0;
    acc[2] = (double)((int16_t)(lia_data[4] | (lia_data[5] << 8))) / 100.0;

    LOG_INF(
        "Got sensor #%d acceleration data: [%.3e, %.3e, %.3e]m/s²",
        num, acc[0], acc[1], acc[2]
    );

    return 0;
}

static int _init_uncal (int num, const bno055_axis_t * axes) {
    int ret;

    LOG_INF("Initializing sensor #%d in uncalibrated state", num);

    ret = _sys_reset(num);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d in uncalibrated state", num);
        return -1;
    }

    ret = _sys_wait(num);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d in uncalibrated state", num);
        return -1;
    }

    ret = _set_pwr_mode(num);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d in uncalibrated state", num);
        return -1;
    }

    ret = _set_opr_mode_cfg(num);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d in uncalibrated state", num);
        return -1;
    }

    ret = _set_unit_sel(num);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d in uncalibrated state", num);
        return -1;
    }

    ret = _set_axis_map(num, axes);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d in uncalibrated state", num);
        return -1;
    }

    ret = _set_axis_sgn(num, axes);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d in uncalibrated state", num);
        return -1;
    }

    ret = _set_opr_mode_ndof(num);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d in uncalibrated state", num);
        return -1;
    }

    return 0;
}

static int _init_cal (
    int num,
    const bno055_axis_t * axes,
    int16_t acc_rad, int16_t mag_rad,
    const int16_t * acc_off, const int16_t * mag_off, const int16_t * gyr_off
) {
    int ret;

    LOG_INF("Initializing sensor #%d in calibrated state", num);

    ret = _sys_reset(num);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d in calibrated state", num);
        return -1;
    }

    ret = _sys_wait(num);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d in calibrated state", num);
        return -1;
    }

    ret = _set_pwr_mode(num);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d in calibrated state", num);
        return -1;
    }

    ret = _set_opr_mode_cfg(num);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d in calibrated state", num);
        return -1;
    }

    ret = _set_unit_sel(num);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d in calibrated state", num);
        return -1;
    }

    ret = _set_axis_map(num, axes);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d in calibrated state", num);
        return -1;
    }

    ret = _set_axis_sgn(num, axes);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d in calibrated state", num);
        return -1;
    }

    ret = _set_cal_data(num, acc_rad, mag_rad, acc_off, mag_off, gyr_off);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d in calibrated state", num);
        return -1;
    }

    ret = _set_opr_mode_ndof(num);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensor #%d in calibrated state", num);
        return -1;
    }

    return 0;
}

static int _update (int num, double * rot, double * att, double * acc) {
    int ret;

    LOG_INF("Getting sensor #%d readings", num);

    ret = _get_gyr_data(num, rot);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d readings", num);
        for (int i = 0; i < 3; i++) {
            rot[i] = NAN;
        }
        for (int i = 0; i < 4; i++) {
            att[i] = NAN;
        }
        for (int i = 0; i < 3; i++) {
            acc[i] = NAN;
        }
        return -1;
    }

    ret = _get_qua_data(num, att);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d readings", num);
        for (int i = 0; i < 3; i++) {
            rot[i] = NAN;
        }
        for (int i = 0; i < 4; i++) {
            att[i] = NAN;
        }
        for (int i = 0; i < 3; i++) {
            acc[i] = NAN;
        }
        return -1;
    }

    ret = _get_lia_data(num, acc);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor #%d readings", num);
        for (int i = 0; i < 3; i++) {
            rot[i] = NAN;
        }
        for (int i = 0; i < 4; i++) {
            att[i] = NAN;
        }
        for (int i = 0; i < 3; i++) {
            acc[i] = NAN;
        }
        return -1;
    }

    return 1;
}

int bno055_init_uncal (
    const bno055_axis_t * axes1, const bno055_axis_t * axes2
) {
    int ret;

    LOG_INF("Initializing sensors in uncalibrated state");

    ret = _init_uncal(1, axes1);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensors in uncalibrated state");
        return -1;
    }

    ret = _init_uncal(2, axes2);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensors in uncalibrated state");
        return -1;
    }

    return 0;
}

int bno055_get_cal_stat (
    uint8_t * sys_cal1, uint8_t * sys_cal2,
    uint8_t * acc_cal1, uint8_t * acc_cal2,
    uint8_t * mag_cal1, uint8_t * mag_cal2,
    uint8_t * gyr_cal1, uint8_t * gyr_cal2
) {
    int ret;

    LOG_INF("Getting sensor calibration levels");

    ret = _get_cal_stat(1, sys_cal1, acc_cal1, mag_cal1, gyr_cal1);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor calibration levels");
        return -1;
    }

    ret = _get_cal_stat(2, sys_cal2, acc_cal2, mag_cal2, gyr_cal2);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor calibration levels");
        return -1;
    }

    return 0;
}

int bno055_get_cal_data(
    int16_t * acc_rad1, int16_t * acc_rad2,
    int16_t * mag_rad1, int16_t * mag_rad2,
    int16_t * acc_off1, int16_t * acc_off2,
    int16_t * mag_off1, int16_t * mag_off2,
    int16_t * gyr_off1, int16_t * gyr_off2
) {
    int ret;

    LOG_INF("Getting sensor calibration parameters");

    ret = _get_cal_data(1, acc_rad1, mag_rad1, acc_off1, mag_off1, gyr_off1);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor calibration parameters");
        return -1;
    }

    ret = _get_cal_data(2, acc_rad2, mag_rad2, acc_off2, mag_off2, gyr_off2);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor calibration parameters");
        return -1;
    }

    return 0;
}

int bno055_init_cal (
    const bno055_axis_t * axes1, const bno055_axis_t * axes2,
    int16_t acc_rad1, int16_t acc_rad2,
    int16_t mag_rad1, int16_t mag_rad2,
    const int16_t * acc_off1, const int16_t * acc_off2,
    const int16_t * mag_off1, const int16_t * mag_off2,
    const int16_t * gyr_off1, const int16_t * gyr_off2
) {
    int ret;

    LOG_INF("Initializing sensors in calibrated state");

    ret = _init_cal(1, axes1, acc_rad1, mag_rad1, acc_off1, mag_off1, gyr_off1);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensors in calibrated state");
        return -1;
    }

    ret = _init_cal(2, axes2, acc_rad2, mag_rad2, acc_off2, mag_off2, gyr_off2);
    if (ret < 0) {
        LOG_ERR("Failed to initialize sensors in calibrated state");
        return -1;
    }

    return 0;
}

int bno055_update (void) {
    int ret;
    bool fail1, fail2;
    bool avail1, avail2;

    LOG_INF("Getting sensor readings");

    ret = _update(1, _rot1, _att1, _acc1);
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

    ret = _update(2, _rot2, _att2, _acc2);
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

void bno055_get_rot (double * rot) {
    LOG_INF("Getting combined angular velocity reading");

    for (int i = 0; i < 3; i++) {
        if (!isnan(_rot1[i]) && !isnan(_rot2[i])) {
            rot[i] = 0.5 * (_rot1[i] + _rot2[i]);
        } else if (!isnan(_rot1[i])) {
            rot[i] = _rot1[i];
        } else if (!isnan(_rot2[i])) {
            rot[i] = _rot2[i];
        } else {
            rot[i] = NAN;
        }
    }

    LOG_INF(
        "Got combined angular velocity reading: [%.3e, %.3e, %.3e]rad/s",
        rot[0], rot[1], rot[2]
    );
}

void bno055_get_att (double * att) {
    LOG_INF("Getting combined attitude reading");

    for (int i = 0; i < 4; i++) {
        if (!isnan(_att1[i]) && !isnan(_att2[i])) {
            att[i] = 0.5 * (_att1[i] + _att2[i]);
        } else if (!isnan(_att1[i])) {
            att[i] = _att1[i];
        } else if (!isnan(_att2[i])) {
            att[i] = _att2[i];
        } else {
            att[i] = NAN;
        }
    }

    LOG_INF(
        "Got combined attitude reading: [%.3e, %.3e, %.3e, %.3e]",
        att[0], att[1], att[2], att[3]
    );
}

void bno055_get_acc (double * acc) {
    LOG_INF("Getting combined acceleration reading");

    for (int i = 0; i < 3; i++) {
        if (!isnan(_acc1[i]) && !isnan(_acc2[i])) {
            acc[i] = 0.5 * (_acc1[i] + _acc2[i]);
        } else if (!isnan(_acc1[i])) {
            acc[i] = _acc1[i];
        } else if (!isnan(_acc2[i])) {
            acc[i] = _acc2[i];
        } else {
            acc[i] = NAN;
        }
    }

    LOG_INF(
        "Got combined acceleration reading: [%.3e, %.3e, %.3e]m/s²",
        acc[0], acc[1], acc[2]
    );
}

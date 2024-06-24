#ifndef __DEV_BNO055_H__
#define __DEV_BNO055_H__

#include <stdint.h>

#define BNO055_ADDR1                0x28
#define BNO055_ADDR2                0x29

#define BNO055_REG_SYS_TRIG         0x3F
#define BNO055_REG_SYS_STAT         0x39
#define BNO055_REG_PWR_MODE         0x3E
#define BNO055_REG_OPR_MODE         0x3D
#define BNO055_REG_UNIT_SEL         0x3B
#define BNO055_REG_AXIS_MAP         0x41
#define BNO055_REG_AXIS_SGN         0x42
#define BNO055_REG_CAL_STAT         0x35
#define BNO055_REG_CAL_DATA         0x55
#define BNO055_REG_ACC_XOFF_LSB     0x55
#define BNO055_REG_ACC_XOFF_MSB     0x56
#define BNO055_REG_ACC_YOFF_LSB     0x57
#define BNO055_REG_ACC_YOFF_MSB     0x58
#define BNO055_REG_ACC_ZOFF_LSB     0x59
#define BNO055_REG_ACC_ZOFF_MSB     0x5A
#define BNO055_REG_MAG_XOFF_LSB     0x5B
#define BNO055_REG_MAG_XOFF_MSB     0x5C
#define BNO055_REG_MAG_YOFF_LSB     0x5D
#define BNO055_REG_MAG_YOFF_MSB     0x5E
#define BNO055_REG_MAG_ZOFF_LSB     0x5F
#define BNO055_REG_MAG_ZOFF_MSB     0x60
#define BNO055_REG_GYR_XOFF_LSB     0x61
#define BNO055_REG_GYR_XOFF_MSB     0x62
#define BNO055_REG_GYR_YOFF_LSB     0x63
#define BNO055_REG_GYR_YOFF_MSB     0x64
#define BNO055_REG_GYR_ZOFF_LSB     0x65
#define BNO055_REG_GYR_ZOFF_MSB     0x66
#define BNO055_REG_ACC_RAD_LSB      0x67
#define BNO055_REG_ACC_RAD_MSB      0x68
#define BNO055_REG_MAG_RAD_LSB      0x69
#define BNO055_REG_MAG_RAD_MSB      0x6A
#define BNO055_REG_GYR_DATA         0x14
#define BNO055_REG_QUA_DATA         0x20
#define BNO055_REG_LIA_DATA         0x28

#define BNO055_BIT_RST_SYS_MSK      0x20
#define BNO055_BIT_PWR_MODE_MSK     0x03
#define BNO055_BIT_OPR_MODE_MSK     0x0F
#define BNO055_BIT_ORI_MSK          0x80
#define BNO055_BIT_GYR_UNIT_MSK     0x02
#define BNO055_BIT_ACC_UNIT_MSK     0x01
#define BNO055_BIT_MAPX_MSK         0x03
#define BNO055_BIT_MAPY_MSK         0x0C
#define BNO055_BIT_MAPZ_MSK         0x30
#define BNO055_BIT_SGNX_MSK         0x04
#define BNO055_BIT_SGNY_MSK         0x02
#define BNO055_BIT_SGNZ_MSK         0x01
#define BNO055_BIT_SYS_CAL_MSK      0xC0
#define BNO055_BIT_GYR_CAL_MSK      0x30
#define BNO055_BIT_ACC_CAL_MSK      0x0C
#define BNO055_BIT_MAG_CAL_MSK      0x03

#define BNO055_BIT_RST_SYS_POS      5
#define BNO055_BIT_PWR_MODE_POS     0
#define BNO055_BIT_OPR_MODE_POS     0
#define BNO055_BIT_ORI_POS          7
#define BNO055_BIT_GYR_UNIT_POS     1
#define BNO055_BIT_ACC_UNIT_POS     0
#define BNO055_BIT_MAPX_POS         0
#define BNO055_BIT_MAPY_POS         2
#define BNO055_BIT_MAPZ_POS         4
#define BNO055_BIT_SGNX_POS         2
#define BNO055_BIT_SGNY_POS         1
#define BNO055_BIT_SGNZ_POS         0
#define BNO055_BIT_SYS_CAL_POS      6
#define BNO055_BIT_GYR_CAL_POS      4
#define BNO055_BIT_ACC_CAL_POS      2
#define BNO055_BIT_MAG_CAL_POS      0

#define BNO055_PRST_SYS_STAT_IDLE   0x00
#define BNO055_PRST_SYS_STAT_ERR    0x01
#define BNO055_PRST_PWR_MODE_NRM    0x00
#define BNO055_PRST_OPR_MODE_CFG    0x00
#define BNO055_PRST_OPR_MODE_NDOF   0x0C
#define BNO055_PRST_ORI_ANDR        0x01
#define BNO055_PRST_GYR_UNIT_RPS    0x01
#define BNO055_PRST_ACC_UNIT_MS2    0x00
#define BNO055_PRST_MAP_X           0x00
#define BNO055_PRST_MAP_Y           0x01
#define BNO055_PRST_MAP_Z           0x02
#define BNO055_PRST_SGN_P           0x00
#define BNO055_PRST_SGN_N           0x01

#define BNO055_GET_BITS(reg, bit) \
    (((reg) & (bit##_MSK)) >> (bit##_POS))

#define BNO055_SET_BITS(reg, bit, val) \
    (((reg) & ~(bit##_MSK)) | ((val) << (bit##_POS)))

typedef enum {
    BNO055_AXIS_PX = (BNO055_PRST_SGN_P << 8) | BNO055_PRST_MAP_X,
    BNO055_AXIS_NX = (BNO055_PRST_SGN_N << 8) | BNO055_PRST_MAP_X,
    BNO055_AXIS_PY = (BNO055_PRST_SGN_P << 8) | BNO055_PRST_MAP_Y,
    BNO055_AXIS_NY = (BNO055_PRST_SGN_N << 8) | BNO055_PRST_MAP_Y,
    BNO055_AXIS_PZ = (BNO055_PRST_SGN_P << 8) | BNO055_PRST_MAP_Z,
    BNO055_AXIS_NZ = (BNO055_PRST_SGN_N << 8) | BNO055_PRST_MAP_Z
} bno055_axis_t;

int bno055_init_uncal (
    const bno055_axis_t * axes1, const bno055_axis_t * axes2
);
int bno055_get_cal_stat (
    uint8_t * sys_cal1, uint8_t * sys_cal2,
    uint8_t * acc_cal1, uint8_t * acc_cal2,
    uint8_t * mag_cal1, uint8_t * mag_cal2,
    uint8_t * gyr_cal1, uint8_t * gyr_cal2
);
int bno055_get_cal_data(
    int16_t * acc_rad1, int16_t * acc_rad2,
    int16_t * mag_rad1, int16_t * mag_rad2,
    int16_t * acc_off1, int16_t * acc_off2,
    int16_t * mag_off1, int16_t * mag_off2,
    int16_t * gyr_off1, int16_t * gyr_off2
);
int bno055_init_cal (
    const bno055_axis_t * axes1, const bno055_axis_t * axes2,
    int16_t acc_rad1, int16_t acc_rad2,
    int16_t mag_rad1, int16_t mag_rad2,
    const int16_t * acc_off1, const int16_t * acc_off2,
    const int16_t * mag_off1, const int16_t * mag_off2,
    const int16_t * gyr_off1, const int16_t * gyr_off2
);
int bno055_update (void);
void bno055_get_rot (double * rot);
void bno055_get_att (double * att);
void bno055_get_acc (double * acc);

#endif

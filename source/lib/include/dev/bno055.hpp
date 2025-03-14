#ifndef __DEV_BNO055_HPP__
#define __DEV_BNO055_HPP__

#include <math/linalg.hpp>

#include <sys/logging.hpp>

#define BNO055_ADDR1                    0x28
#define BNO055_ADDR2                    0x29

#define BNO055_REG_TRG                  0x3F
#define BNO055_REG_PWR                  0x3E
#define BNO055_REG_OPR                  0x3D
#define BNO055_REG_UNT                  0x3B
#define BNO055_REG_AXM                  0x41
#define BNO055_REG_AXS                  0x42
#define BNO055_REG_STA                  0x39
#define BNO055_REG_ERR                  0x3A
#define BNO055_REG_CALLEV               0x35
#define BNO055_REG_CALDAT               0x55
#define BNO055_REG_OFACCXL              0x55
#define BNO055_REG_OFACCXM              0x56
#define BNO055_REG_OFACCYL              0x57
#define BNO055_REG_OFACCYM              0x58
#define BNO055_REG_OFACCZL              0x59
#define BNO055_REG_OFACCZM              0x5A
#define BNO055_REG_OFMAGXL              0x5B
#define BNO055_REG_OFMAGXM              0x5C
#define BNO055_REG_OFMAGYL              0x5D
#define BNO055_REG_OFMAGYM              0x5E
#define BNO055_REG_OFMAGZL              0x5F
#define BNO055_REG_OFMAGZM              0x60
#define BNO055_REG_OFGYRXL              0x61
#define BNO055_REG_OFGYRXM              0x62
#define BNO055_REG_OFGYRYL              0x63
#define BNO055_REG_OFGYRYM              0x64
#define BNO055_REG_OFGYRZL              0x65
#define BNO055_REG_OFGYRZM              0x66
#define BNO055_REG_RADACCL              0x67
#define BNO055_REG_RADACCM              0x68
#define BNO055_REG_RADMAGL              0x69
#define BNO055_REG_RADMAGM              0x6A
#define BNO055_REG_DATROT               0x14
#define BNO055_REG_DATQUA               0x20
#define BNO055_REG_DATLIA               0x28

#define BNO055_BIT_RST_MSK              0x20
#define BNO055_BIT_PWR_MSK              0x03
#define BNO055_BIT_OPR_MSK              0x0F
#define BNO055_BIT_ORI_MSK              0x80
#define BNO055_BIT_UNTACC_MSK           0x01
#define BNO055_BIT_UNTGYR_MSK           0x02
#define BNO055_BIT_MAPX_MSK             0x03
#define BNO055_BIT_MAPY_MSK             0x0C
#define BNO055_BIT_MAPZ_MSK             0x30
#define BNO055_BIT_SGNX_MSK             0x04
#define BNO055_BIT_SGNY_MSK             0x02
#define BNO055_BIT_SGNZ_MSK             0x01
#define BNO055_BIT_LEVSYS_MSK           0xC0
#define BNO055_BIT_LEVACC_MSK           0x0C
#define BNO055_BIT_LEVMAG_MSK           0x03
#define BNO055_BIT_LEVGYR_MSK           0x30

#define BNO055_BIT_RST_POS              5
#define BNO055_BIT_PWR_POS              0
#define BNO055_BIT_OPR_POS              0
#define BNO055_BIT_ORI_POS              7
#define BNO055_BIT_UNTACC_POS           0
#define BNO055_BIT_UNTGYR_POS           1
#define BNO055_BIT_MAPX_POS             0
#define BNO055_BIT_MAPY_POS             2
#define BNO055_BIT_MAPZ_POS             4
#define BNO055_BIT_SGNX_POS             2
#define BNO055_BIT_SGNY_POS             1
#define BNO055_BIT_SGNZ_POS             0
#define BNO055_BIT_LEVSYS_POS           6
#define BNO055_BIT_LEVACC_POS           2
#define BNO055_BIT_LEVMAG_POS           0
#define BNO055_BIT_LEVGYR_POS           4

#define BNO055_PRST_PWR_NRM             0x00
#define BNO055_PRST_OPR_CFG             0x00
#define BNO055_PRST_OPR_NDOF            0x0C
#define BNO055_PRST_ORI_ANDR            0x01
#define BNO055_PRST_UNTACC_MS2          0x00
#define BNO055_PRST_UNTGYR_RPS          0x01
#define BNO055_PRST_MAP_X               0x00
#define BNO055_PRST_MAP_Y               0x01
#define BNO055_PRST_MAP_Z               0x02
#define BNO055_PRST_SGN_P               0x00
#define BNO055_PRST_SGN_N               0x01
#define BNO055_PRST_STA_IDL             0x00
#define BNO055_PRST_STA_ERR             0x01
#define BNO055_PRST_STA_PERINIT         0x02
#define BNO055_PRST_STA_SYSINIT         0x03
#define BNO055_PRST_STA_STRUN           0x04
#define BNO055_PRST_STA_FUSRUN          0x05
#define BNO055_PRST_STA_NFUSRUN         0x06
#define BNO055_PRST_ERR_PERINIT         0x01
#define BNO055_PRST_ERR_SYSINIT         0x02
#define BNO055_PRST_ERR_STFAIL          0x03
#define BNO055_PRST_ERR_VALRNG          0x04
#define BNO055_PRST_ERR_ADRRNG          0x05
#define BNO055_PRST_ERR_WRFAIL          0x06
#define BNO055_PRST_ERR_LOWPWR          0x07
#define BNO055_PRST_ERR_ACCPWR          0x08
#define BNO055_PRST_ERR_FUSCFG          0x09
#define BNO055_PRST_ERR_SYSCFG          0x0A

#define BNO055_PUT_BITS(reg, bit, val)  (((reg) & ~(bit##_MSK)) | ((val) << (bit##_POS)))
#define BNO055_GET_BITS(reg, bit)       (((reg) & (bit##_MSK)) >> (bit##_POS))

namespace bno055 {

class axis {
    public:
        axis (int val, const char * text);

        operator int (void) const;
        operator logging::buffer (void) const;

    private:
        int val;
        const char * text;
};

extern const axis axis_px;
extern const axis axis_nx;
extern const axis axis_py;
extern const axis axis_ny;
extern const axis axis_pz;
extern const axis axis_nz;

bool fail (void);

void init (int id, axis axis_x, axis axis_y, axis axis_z);
void init (
    int id, axis axis_x, axis axis_y, axis axis_z,
    int rad_acc, int rad_mag, linalg::ivector off_acc, linalg::ivector off_mag, linalg::ivector off_gyr
);

int get_lev_sys (int id);
int get_lev_acc (int id);
int get_lev_mag (int id);
int get_lev_gyr (int id);

int get_rad_acc (int id);
int get_rad_mag (int id);

linalg::ivector get_off_acc (int id);
linalg::ivector get_off_mag (int id);
linalg::ivector get_off_gyr (int id);

linalg::fvector get_rot (int id);
linalg::fvector get_qua (int id);
linalg::fvector get_lia (int id);

void update (int id);

}

#endif

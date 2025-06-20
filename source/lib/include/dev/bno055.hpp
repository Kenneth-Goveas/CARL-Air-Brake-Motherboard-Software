#ifndef __DEV_BNO055_HPP__
#define __DEV_BNO055_HPP__

#include <cstdint>

#include <vector>

#include <math/linalg.hpp>
#include <sys/logging.hpp>
#include <dev/i2c.hpp>

#define BNO055_ADDR1            0x28
#define BNO055_ADDR2            0x29

#define BNO055_REG_TRG          0x3F
#define BNO055_REG_PWR          0x3E
#define BNO055_REG_OPR          0x3D
#define BNO055_REG_UNT          0x3B
#define BNO055_REG_AXM          0x41
#define BNO055_REG_AXS          0x42
#define BNO055_REG_STA          0x39
#define BNO055_REG_ERR          0x3A
#define BNO055_REG_CALLEV       0x35
#define BNO055_REG_CALDAT       0x55
#define BNO055_REG_CALOFFACCXL  0x55
#define BNO055_REG_CALOFFACCXM  0x56
#define BNO055_REG_CALOFFACCYL  0x57
#define BNO055_REG_CALOFFACCYM  0x58
#define BNO055_REG_CALOFFACCZL  0x59
#define BNO055_REG_CALOFFACCZM  0x5A
#define BNO055_REG_CALOFFMAGXL  0x5B
#define BNO055_REG_CALOFFMAGXM  0x5C
#define BNO055_REG_CALOFFMAGYL  0x5D
#define BNO055_REG_CALOFFMAGYM  0x5E
#define BNO055_REG_CALOFFMAGZL  0x5F
#define BNO055_REG_CALOFFMAGZM  0x60
#define BNO055_REG_CALOFFGYRXL  0x61
#define BNO055_REG_CALOFFGYRXM  0x62
#define BNO055_REG_CALOFFGYRYL  0x63
#define BNO055_REG_CALOFFGYRYM  0x64
#define BNO055_REG_CALOFFGYRZL  0x65
#define BNO055_REG_CALOFFGYRZM  0x66
#define BNO055_REG_CALRADACCL   0x67
#define BNO055_REG_CALRADACCM   0x68
#define BNO055_REG_CALRADMAGL   0x69
#define BNO055_REG_CALRADMAGM   0x6A
#define BNO055_REG_MEAROT       0x14
#define BNO055_REG_MEAQUA       0x20
#define BNO055_REG_MEALIA       0x28

#define BNO055_BIT_RST_MSK      0x20
#define BNO055_BIT_PWR_MSK      0x03
#define BNO055_BIT_OPR_MSK      0x0F
#define BNO055_BIT_ORI_MSK      0x80
#define BNO055_BIT_UNA_MSK      0x01
#define BNO055_BIT_UNG_MSK      0x02
#define BNO055_BIT_MAPX_MSK     0x03
#define BNO055_BIT_MAPY_MSK     0x0C
#define BNO055_BIT_MAPZ_MSK     0x30
#define BNO055_BIT_SGNX_MSK     0x04
#define BNO055_BIT_SGNY_MSK     0x02
#define BNO055_BIT_SGNZ_MSK     0x01
#define BNO055_BIT_LEVSYS_MSK   0xC0
#define BNO055_BIT_LEVACC_MSK   0x0C
#define BNO055_BIT_LEVMAG_MSK   0x03
#define BNO055_BIT_LEVGYR_MSK   0x30

#define BNO055_BIT_RST_POS      5
#define BNO055_BIT_PWR_POS      0
#define BNO055_BIT_OPR_POS      0
#define BNO055_BIT_ORI_POS      7
#define BNO055_BIT_UNA_POS      0
#define BNO055_BIT_UNG_POS      1
#define BNO055_BIT_MAPX_POS     0
#define BNO055_BIT_MAPY_POS     2
#define BNO055_BIT_MAPZ_POS     4
#define BNO055_BIT_SGNX_POS     2
#define BNO055_BIT_SGNY_POS     1
#define BNO055_BIT_SGNZ_POS     0
#define BNO055_BIT_LEVSYS_POS   6
#define BNO055_BIT_LEVACC_POS   2
#define BNO055_BIT_LEVMAG_POS   0
#define BNO055_BIT_LEVGYR_POS   4

#define BNO055_PRST_PWR_NRM     0x00
#define BNO055_PRST_OPR_CFG     0x00
#define BNO055_PRST_OPR_NDOF    0x0C
#define BNO055_PRST_ORI_ANDR    0x01
#define BNO055_PRST_UNA_MS2     0x00
#define BNO055_PRST_UNG_RPS     0x01
#define BNO055_PRST_MAP_X       0x00
#define BNO055_PRST_MAP_Y       0x01
#define BNO055_PRST_MAP_Z       0x02
#define BNO055_PRST_SGN_P       0x00
#define BNO055_PRST_SGN_N       0x01
#define BNO055_PRST_STA_IDL     0x00
#define BNO055_PRST_STA_ERR     0x01
#define BNO055_PRST_STA_PERINIT 0x02
#define BNO055_PRST_STA_SYSINIT 0x03
#define BNO055_PRST_STA_SLFTEST 0x04
#define BNO055_PRST_STA_FUS     0x05
#define BNO055_PRST_STA_NFUS    0x06
#define BNO055_PRST_ERR_PERINIT 0x01
#define BNO055_PRST_ERR_SYSINIT 0x02
#define BNO055_PRST_ERR_SLFTEST 0x03
#define BNO055_PRST_ERR_REGVAL  0x04
#define BNO055_PRST_ERR_REGADR  0x05
#define BNO055_PRST_ERR_REGWRI  0x06
#define BNO055_PRST_ERR_LOWPWR  0x07
#define BNO055_PRST_ERR_ACCPWR  0x08
#define BNO055_PRST_ERR_FUSCFG  0x09
#define BNO055_PRST_ERR_SYSCFG  0x0A

#define BNO055_PUT_BITS(reg, bit, val)  (((reg) & ~(bit##_MSK)) | ((val) << (bit##_POS)))
#define BNO055_GET_BITS(reg, bit)       (((reg) & (bit##_MSK)) >> (bit##_POS))

namespace bno055 {

enum class axis;

class tracker {
    protected:
        static bool exist1, exist2;
        static bool instantiate (int id);
        static bool validate (int id);
};

class instance : private tracker {
    public:
        instance (i2c::instance & i2c, int id, axis axis_x, axis axis_y, axis axis_z);
        instance (
            i2c::instance & i2c, int id, axis axis_x, axis axis_y, axis axis_z,
            int rad_acc, int rad_mag, const linalg::ivector & off_acc, const linalg::ivector & off_mag, const linalg::ivector & off_gyr
        );

        int get_lev_sys (void);
        int get_lev_acc (void);
        int get_lev_mag (void);
        int get_lev_gyr (void);

        int get_rad_acc (void);
        int get_rad_mag (void);

        linalg::ivector get_off_acc (void);
        linalg::ivector get_off_mag (void);
        linalg::ivector get_off_gyr (void);

        linalg::fvector get_rot (void);
        linalg::fvector get_qua (void);
        linalg::fvector get_lia (void);

        bool valid_rad_acc (void);
        bool valid_rad_mag (void);

        bool valid_off_acc (void);
        bool valid_off_mag (void);
        bool valid_off_gyr (void);

        void update (void);

        enum class except_ctor {fail};
        enum class except_get_lev_sys {fail};
        enum class except_get_lev_acc {fail};
        enum class except_get_lev_mag {fail};
        enum class except_get_lev_gyr {fail};
        enum class except_get_rad_acc {fail};
        enum class except_get_rad_mag {fail};
        enum class except_get_off_acc {fail};
        enum class except_get_off_mag {fail};
        enum class except_get_off_gyr {fail};
        enum class except_get_rot {fail};
        enum class except_get_qua {fail};
        enum class except_get_lia {fail};
        enum class except_valid_rad_acc {fail};
        enum class except_valid_rad_mag {fail};
        enum class except_valid_off_acc {fail};
        enum class except_valid_off_mag {fail};
        enum class except_valid_off_gyr {fail};
        enum class except_update {fail};

    private:
        logging::instance logging;
        bool init;
        int id;

        i2c::instance * i2c;
        std::uint8_t addr;

        bool sta_idl, sta_err, sta_perinit, sta_sysinit, sta_slftest, sta_fus, sta_nfus;
        bool err_perinit, err_sysinit, err_slftest, err_regval, err_regadr, err_regwri, err_lowpwr, err_accpwr, err_fuscfg, err_syscfg;

        int cal_lev_sys_data, cal_lev_acc_data, cal_lev_mag_data, cal_lev_gyr_data;
        int cal_rad_acc_data, cal_rad_mag_data;
        linalg::ivector cal_off_acc_data, cal_off_mag_data, cal_off_gyr_data;
        bool cal_lev_sys_valid, cal_lev_acc_valid, cal_lev_mag_valid, cal_lev_gyr_valid;
        bool cal_rad_acc_valid, cal_rad_mag_valid;
        bool cal_off_acc_valid, cal_off_mag_valid, cal_off_gyr_valid;

        linalg::fvector mea_rot_data, mea_qua_data, mea_lia_data;
        bool mea_rot_valid, mea_qua_valid, mea_lia_valid;

        bool mode_cal, mode_mea;

        void put_reg (std::uint8_t reg, std::uint8_t data);
        void get_reg (std::uint8_t reg, std::vector<std::uint8_t> & data);

        void put_trg_rst (void);
        void put_pwr_nrm (void);
        void put_opr_cfg (void);
        void put_opr_ndof (void);
        void put_unt_si (void);
        void put_axm (axis axis_x, axis axis_y, axis axis_z);
        void put_axs (axis axis_x, axis axis_y, axis axis_z);
        void put_caldat (
            int rad_acc, int rad_mag, const linalg::ivector & off_acc, const linalg::ivector & off_mag, const linalg::ivector & off_gyr
        );

        void get_sta (void);
        void get_err (void);
        void get_callev (void);
        void get_caldat (void);
        void get_mearot (void);
        void get_meaqua (void);
        void get_mealia (void);

        enum class except_put_reg {fail};
        enum class except_get_reg {fail};
        enum class except_put_trg_rst {fail};
        enum class except_put_pwr_nrm {fail};
        enum class except_put_opr_cfg {fail};
        enum class except_put_opr_ndof {fail};
        enum class except_put_unt_si {fail};
        enum class except_put_axm {fail};
        enum class except_put_axs {fail};
        enum class except_put_caldat {fail};
        enum class except_get_sta {fail};
        enum class except_get_err {fail};
        enum class except_get_callev {fail};
        enum class except_get_caldat {fail};
        enum class except_get_mearot {fail};
        enum class except_get_meaqua {fail};
        enum class except_get_mealia {fail};

        class except_intern {};
};

enum class axis {
    px = (BNO055_PRST_SGN_P << 8) | BNO055_PRST_MAP_X,
    nx = (BNO055_PRST_SGN_N << 8) | BNO055_PRST_MAP_X,
    py = (BNO055_PRST_SGN_P << 8) | BNO055_PRST_MAP_Y,
    ny = (BNO055_PRST_SGN_N << 8) | BNO055_PRST_MAP_Y,
    pz = (BNO055_PRST_SGN_P << 8) | BNO055_PRST_MAP_Z,
    nz = (BNO055_PRST_SGN_N << 8) | BNO055_PRST_MAP_Z
};

logging::stream & operator << (logging::stream & strm, axis axis);

}

#endif

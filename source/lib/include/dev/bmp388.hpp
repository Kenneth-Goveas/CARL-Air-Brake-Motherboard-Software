#ifndef __DEV_BMP388_HPP__
#define __DEV_BMP388_HPP__

#include <cstdint>

#include <vector>

#include <sys/logging.hpp>
#include <dev/i2c.hpp>

#define BMP388_ADDR1            0x76
#define BMP388_ADDR2            0x77

#define BMP388_REG_CMD          0x7E
#define BMP388_REG_PWR          0x1B
#define BMP388_REG_ODR          0x1D
#define BMP388_REG_OSR          0x1C
#define BMP388_REG_CFG          0x1F
#define BMP388_REG_STA          0x03
#define BMP388_REG_ERR          0x02
#define BMP388_REG_CAL          0x31
#define BMP388_REG_MEA          0x04

#define BMP388_BIT_ENABP_MSK    0x01
#define BMP388_BIT_ENABT_MSK    0x02
#define BMP388_BIT_MODE_MSK     0x30
#define BMP388_BIT_ODR_MSK      0x1F
#define BMP388_BIT_OSRP_MSK     0x07
#define BMP388_BIT_OSRT_MSK     0x38
#define BMP388_BIT_IIR_MSK      0x0E
#define BMP388_BIT_CRDY_MSK     0x10
#define BMP388_BIT_DRDYP_MSK    0x20
#define BMP388_BIT_DRDYT_MSK    0x40
#define BMP388_BIT_FTL_MSK      0x01
#define BMP388_BIT_CMD_MSK      0x02
#define BMP388_BIT_CFG_MSK      0x04

#define BMP388_BIT_ENABP_POS    0
#define BMP388_BIT_ENABT_POS    1
#define BMP388_BIT_MODE_POS     4
#define BMP388_BIT_ODR_POS      0
#define BMP388_BIT_OSRP_POS     0
#define BMP388_BIT_OSRT_POS     3
#define BMP388_BIT_IIR_POS      1
#define BMP388_BIT_CRDY_POS     4
#define BMP388_BIT_DRDYP_POS    5
#define BMP388_BIT_DRDYT_POS    6
#define BMP388_BIT_FTL_POS      0
#define BMP388_BIT_CMD_POS      1
#define BMP388_BIT_CFG_POS      2

#define BMP388_PRST_CMD_RST     0xB6
#define BMP388_PRST_MODE_SLP    0x00
#define BMP388_PRST_MODE_NRM    0x03

#define BMP388_PUT_BITS(reg, bit, val)  (((reg) & ~(bit##_MSK)) | ((val) << (bit##_POS)))
#define BMP388_GET_BITS(reg, bit)       (((reg) & (bit##_MSK)) >> (bit##_POS))

namespace bmp388 {

class tracker {
    protected:
        static bool exist1, exist2;
        static bool instantiate (int id);
        static bool validate (int id);
};

class instance : private tracker {
    public:
        instance (i2c::instance & i2c, int id, int drate, int ovsmp_pres, int ovsmp_temp, int iir);

        double get_pres (void);
        double get_temp (void);

        void update (void);

        enum class except_ctor {fail};
        enum class except_get_pres {fail};
        enum class except_get_temp {fail};
        enum class except_update {fail, blank};

    private:
        logging::instance logging;
        bool init;
        int id;

        i2c::instance * i2c;
        std::uint8_t addr;

        bool sta_crdy, sta_drdyp, sta_drdyt;
        bool err_ftl, err_cmd, err_cfg;

        std::vector<double> cal_pres_data, cal_temp_data;

        double mea_pres_data, mea_temp_data;
        bool mea_pres_valid, mea_temp_valid;

        void put_reg (std::uint8_t reg, std::uint8_t data);
        void get_reg (std::uint8_t reg, std::vector<std::uint8_t> & data);

        void put_cmd_rst (void);
        void put_pwr_slp (void);
        void put_pwr_nrm (void);
        void put_odr (int drate);
        void put_osr (int ovsmp_pres, int ovsmp_temp);
        void put_cfg (int iir);

        void get_sta (void);
        void get_err (void);
        void get_cal (void);
        void get_mea (void);

        enum class except_put_reg {fail};
        enum class except_get_reg {fail};
        enum class except_put_cmd_rst {fail};
        enum class except_put_pwr_slp {fail};
        enum class except_put_pwr_nrm {fail};
        enum class except_put_odr {fail};
        enum class except_put_osr {fail};
        enum class except_put_cfg {fail};
        enum class except_get_sta {fail};
        enum class except_get_err {fail};
        enum class except_get_cal {fail};
        enum class except_get_mea {fail};

        class except_intern {};
};

}

#endif

#ifndef __DEV_BMP388_HPP__
#define __DEV_BMP388_HPP__

#include <sys/logging.hpp>

#define BMP388_ADDR1                    0x76
#define BMP388_ADDR2                    0x77

#define BMP388_REG_CMD                  0x7E
#define BMP388_REG_PWR                  0x1B
#define BMP388_REG_ODR                  0x1D
#define BMP388_REG_OSR                  0x1C
#define BMP388_REG_CFG                  0x1F
#define BMP388_REG_STA                  0x03
#define BMP388_REG_ERR                  0x02
#define BMP388_REG_CAL                  0x31
#define BMP388_REG_DAT                  0x04

#define BMP388_BIT_ENABP_MSK            0x01
#define BMP388_BIT_ENABT_MSK            0x02
#define BMP388_BIT_MODE_MSK             0x30
#define BMP388_BIT_ODR_MSK              0x1F
#define BMP388_BIT_OSRP_MSK             0x07
#define BMP388_BIT_OSRT_MSK             0x38
#define BMP388_BIT_IIR_MSK              0x0E
#define BMP388_BIT_CRDY_MSK             0x10
#define BMP388_BIT_DRDYP_MSK            0x20
#define BMP388_BIT_DRDYT_MSK            0x40
#define BMP388_BIT_FTL_MSK              0x01
#define BMP388_BIT_CMD_MSK              0x02
#define BMP388_BIT_CFG_MSK              0x04

#define BMP388_BIT_ENABP_POS            0
#define BMP388_BIT_ENABT_POS            1
#define BMP388_BIT_MODE_POS             4
#define BMP388_BIT_ODR_POS              0
#define BMP388_BIT_OSRP_POS             0
#define BMP388_BIT_OSRT_POS             3
#define BMP388_BIT_IIR_POS              1
#define BMP388_BIT_CRDY_POS             4
#define BMP388_BIT_DRDYP_POS            5
#define BMP388_BIT_DRDYT_POS            6
#define BMP388_BIT_FTL_POS              0
#define BMP388_BIT_CMD_POS              1
#define BMP388_BIT_CFG_POS              2

#define BMP388_PRST_CMD_RST             0xB6
#define BMP388_PRST_MODE_SLP            0x00
#define BMP388_PRST_MODE_NRM            0x03
#define BMP388_PRST_ODR_200_HZ          0x00
#define BMP388_PRST_ODR_100_HZ          0x01
#define BMP388_PRST_ODR_50_HZ           0x02
#define BMP388_PRST_ODR_25_HZ           0x03
#define BMP388_PRST_ODR_25_2_HZ         0x04
#define BMP388_PRST_ODR_25_4_HZ         0x05
#define BMP388_PRST_ODR_25_8_HZ         0x06
#define BMP388_PRST_ODR_25_16_HZ        0x07
#define BMP388_PRST_ODR_25_32_HZ        0x08
#define BMP388_PRST_ODR_25_64_HZ        0x09
#define BMP388_PRST_ODR_25_128_HZ       0x0A
#define BMP388_PRST_ODR_25_256_HZ       0x0B
#define BMP388_PRST_ODR_25_512_HZ       0x0C
#define BMP388_PRST_ODR_25_1024_HZ      0x0D
#define BMP388_PRST_ODR_25_2048_HZ      0x0E
#define BMP388_PRST_ODR_25_4096_HZ      0x0F
#define BMP388_PRST_ODR_25_8192_HZ      0x10
#define BMP388_PRST_ODR_25_16384_HZ     0x11
#define BMP388_PRST_OSR_1X              0x00
#define BMP388_PRST_OSR_2X              0x01
#define BMP388_PRST_OSR_4X              0x02
#define BMP388_PRST_OSR_8X              0x03
#define BMP388_PRST_OSR_16X             0x04
#define BMP388_PRST_OSR_32X             0x05
#define BMP388_PRST_IIR_0               0x00
#define BMP388_PRST_IIR_1               0x01
#define BMP388_PRST_IIR_3               0x02
#define BMP388_PRST_IIR_7               0x03
#define BMP388_PRST_IIR_15              0x04
#define BMP388_PRST_IIR_31              0x05
#define BMP388_PRST_IIR_63              0x06
#define BMP388_PRST_IIR_127             0x07

#define BMP388_PUT_BITS(reg, bit, val)  (((reg) & ~(bit##_MSK)) | ((val) << (bit##_POS)))
#define BMP388_GET_BITS(reg, bit)       (((reg) & (bit##_MSK)) >> (bit##_POS))

namespace bmp388 {

class drate {
    public:
        drate (int val, const char * text);

        operator int (void) const;
        operator logging::buffer (void) const;

    private:
        int val;
        const char * text;
};

class ovsmp {
    public:
        ovsmp (int val, const char * text);

        operator int (void) const;
        operator logging::buffer (void) const;

    private:
        int val;
        const char * text;
};

class iir {
    public:
        iir (int val, const char * text);

        operator int (void) const;
        operator logging::buffer (void) const;

    private:
        int val;
        const char * text;
};

extern const drate drate_200_hz;
extern const drate drate_100_hz;
extern const drate drate_50_hz;
extern const drate drate_25_hz;
extern const drate drate_25_2_hz;
extern const drate drate_25_4_hz;
extern const drate drate_25_8_hz;
extern const drate drate_25_16_hz;
extern const drate drate_25_32_hz;
extern const drate drate_25_64_hz;
extern const drate drate_25_128_hz;
extern const drate drate_25_256_hz;
extern const drate drate_25_512_hz;
extern const drate drate_25_1024_hz;
extern const drate drate_25_2048_hz;
extern const drate drate_25_4096_hz;
extern const drate drate_25_8192_hz;
extern const drate drate_25_16384_hz;

extern const ovsmp ovsmp_1x;
extern const ovsmp ovsmp_2x;
extern const ovsmp ovsmp_4x;
extern const ovsmp ovsmp_8x;
extern const ovsmp ovsmp_16x;
extern const ovsmp ovsmp_32x;

extern const iir iir_0;
extern const iir iir_1;
extern const iir iir_3;
extern const iir iir_7;
extern const iir iir_15;
extern const iir iir_31;
extern const iir iir_63;
extern const iir iir_127;

bool fail (void);
bool incomp (void);

void init (int id, drate drate, ovsmp ovsmp_pres, ovsmp ovsmp_temp, iir iir);

double get_pres (int id);
double get_temp (int id);

void update (int id);

}

#endif

#ifndef __DEV_BMP388_HPP__
#define __DEV_BMP388_HPP__

#define BMP388_ADDR1                    0x76
#define BMP388_ADDR2                    0x77

#define BMP388_REG_CMD                  0x7E
#define BMP388_REG_PWRCTRL              0x1B
#define BMP388_REG_ODR                  0x1D
#define BMP388_REG_OSR                  0x1C
#define BMP388_REG_CONFIG               0x1F
#define BMP388_REG_STATUS               0x03
#define BMP388_REG_ERR                  0x02
#define BMP388_REG_CALIB                0x31
#define BMP388_REG_DATA                 0x04

#define BMP388_BIT_PRESEN_MSK           0x01
#define BMP388_BIT_TEMPEN_MSK           0x02
#define BMP388_BIT_MODE_MSK             0x30
#define BMP388_BIT_ODRSEL_MSK           0x1F
#define BMP388_BIT_OSRP_MSK             0x07
#define BMP388_BIT_OSRT_MSK             0x38
#define BMP388_BIT_IIRFILTER_MSK        0x0E
#define BMP388_BIT_CMDRDY_MSK           0x10
#define BMP388_BIT_DRDYPRES_MSK         0x20
#define BMP388_BIT_DRDYTEMP_MSK         0x40
#define BMP388_BIT_FATALERR_MSK         0x01
#define BMP388_BIT_CMDERR_MSK           0x02
#define BMP388_BIT_CONFERR_MSK          0x04

#define BMP388_BIT_PRESEN_POS           0
#define BMP388_BIT_TEMPEN_POS           1
#define BMP388_BIT_MODE_POS             4
#define BMP388_BIT_ODRSEL_POS           0
#define BMP388_BIT_OSRP_POS             0
#define BMP388_BIT_OSRT_POS             3
#define BMP388_BIT_IIRFILTER_POS        1
#define BMP388_BIT_CMDRDY_POS           4
#define BMP388_BIT_DRDYPRES_POS         5
#define BMP388_BIT_DRDYTEMP_POS         6
#define BMP388_BIT_FATALERR_POS         0
#define BMP388_BIT_CMDERR_POS           1
#define BMP388_BIT_CONFERR_POS          2

#define BMP388_PRST_CMD_SOFTRESET       0xB6
#define BMP388_PRST_MODE_SLEEP          0x00
#define BMP388_PRST_MODE_NORMAL         0x03
#define BMP388_PRST_ODRSEL_200_HZ       0x00
#define BMP388_PRST_ODRSEL_100_HZ       0x01
#define BMP388_PRST_ODRSEL_50_HZ        0x02
#define BMP388_PRST_ODRSEL_25_HZ        0x03
#define BMP388_PRST_ODRSEL_25_2_HZ      0x04
#define BMP388_PRST_ODRSEL_25_4_HZ      0x05
#define BMP388_PRST_ODRSEL_25_8_HZ      0x06
#define BMP388_PRST_ODRSEL_25_16_HZ     0x07
#define BMP388_PRST_ODRSEL_25_32_HZ     0x08
#define BMP388_PRST_ODRSEL_25_64_HZ     0x09
#define BMP388_PRST_ODRSEL_25_128_HZ    0x0A
#define BMP388_PRST_ODRSEL_25_256_HZ    0x0B
#define BMP388_PRST_ODRSEL_25_512_HZ    0x0C
#define BMP388_PRST_ODRSEL_25_1024_HZ   0x0D
#define BMP388_PRST_ODRSEL_25_2048_HZ   0x0E
#define BMP388_PRST_ODRSEL_25_4096_HZ   0x0F
#define BMP388_PRST_ODRSEL_25_8192_HZ   0x10
#define BMP388_PRST_ODRSEL_25_16384_HZ  0x11
#define BMP388_PRST_OSRP_OSRT_1X        0x00
#define BMP388_PRST_OSRP_OSRT_2X        0x01
#define BMP388_PRST_OSRP_OSRT_4X        0x02
#define BMP388_PRST_OSRP_OSRT_8X        0x03
#define BMP388_PRST_OSRP_OSRT_16X       0x04
#define BMP388_PRST_OSRP_OSRT_32X       0x05
#define BMP388_PRST_IIRFILTER_0         0x00
#define BMP388_PRST_IIRFILTER_1         0x01
#define BMP388_PRST_IIRFILTER_3         0x02
#define BMP388_PRST_IIRFILTER_7         0x03
#define BMP388_PRST_IIRFILTER_15        0x04
#define BMP388_PRST_IIRFILTER_31        0x05
#define BMP388_PRST_IIRFILTER_63        0x06
#define BMP388_PRST_IIRFILTER_127       0x07

#define BMP388_PUT_BITS(reg, bit, val) \
    (((reg) & ~(bit##_MSK)) | ((val) << (bit##_POS)))

#define BMP388_GET_BITS(reg, bit) \
    (((reg) & (bit##_MSK)) >> (bit##_POS))

namespace bmp388 {

typedef enum {
    drate_200_hz        = BMP388_PRST_ODRSEL_200_HZ,
    drate_100_hz        = BMP388_PRST_ODRSEL_100_HZ,
    drate_50_hz         = BMP388_PRST_ODRSEL_50_HZ,
    drate_25_hz         = BMP388_PRST_ODRSEL_25_HZ,
    drate_25_2_hz       = BMP388_PRST_ODRSEL_25_2_HZ,
    drate_25_4_hz       = BMP388_PRST_ODRSEL_25_4_HZ,
    drate_25_8_hz       = BMP388_PRST_ODRSEL_25_8_HZ,
    drate_25_16_hz      = BMP388_PRST_ODRSEL_25_16_HZ,
    drate_25_32_hz      = BMP388_PRST_ODRSEL_25_32_HZ,
    drate_25_64_hz      = BMP388_PRST_ODRSEL_25_64_HZ,
    drate_25_128_hz     = BMP388_PRST_ODRSEL_25_128_HZ,
    drate_25_256_hz     = BMP388_PRST_ODRSEL_25_256_HZ,
    drate_25_512_hz     = BMP388_PRST_ODRSEL_25_512_HZ,
    drate_25_1024_hz    = BMP388_PRST_ODRSEL_25_1024_HZ,
    drate_25_2048_hz    = BMP388_PRST_ODRSEL_25_2048_HZ,
    drate_25_4096_hz    = BMP388_PRST_ODRSEL_25_4096_HZ,
    drate_25_8192_hz    = BMP388_PRST_ODRSEL_25_8192_HZ,
    drate_25_16384_hz   = BMP388_PRST_ODRSEL_25_16384_HZ
} drate;

typedef enum {
    ovsmp_1x    = BMP388_PRST_OSRP_OSRT_1X,
    ovsmp_2x    = BMP388_PRST_OSRP_OSRT_2X,
    ovsmp_4x    = BMP388_PRST_OSRP_OSRT_4X,
    ovsmp_8x    = BMP388_PRST_OSRP_OSRT_8X,
    ovsmp_16x   = BMP388_PRST_OSRP_OSRT_16X,
    ovsmp_32x   = BMP388_PRST_OSRP_OSRT_32X
} ovsmp;

typedef enum {
    iir_0   = BMP388_PRST_IIRFILTER_0,
    iir_1   = BMP388_PRST_IIRFILTER_1,
    iir_3   = BMP388_PRST_IIRFILTER_3,
    iir_7   = BMP388_PRST_IIRFILTER_7,
    iir_15  = BMP388_PRST_IIRFILTER_15,
    iir_31  = BMP388_PRST_IIRFILTER_31,
    iir_63  = BMP388_PRST_IIRFILTER_63,
    iir_127 = BMP388_PRST_IIRFILTER_127
} iir;

bool fail (void);
bool incomp (void);

void init (
    drate drate1, ovsmp ovsmp_pres1, ovsmp ovsmp_temp1, iir iir1,
    drate drate2, ovsmp ovsmp_pres2, ovsmp ovsmp_temp2, iir iir2
);

double get_pres (void);
double get_temp (void);

void update (void);

}

#endif

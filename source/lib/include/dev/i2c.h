#ifndef __DEV_I2C_H__
#define __DEV_I2C_H__

#include <stddef.h>
#include <stdint.h>

int i2c_init (const char * path);
int i2c_deinit (void);
void i2c_send (uint8_t addr, const uint8_t * buf, size_t len);
void i2c_recv (uint8_t addr, uint8_t * buf, size_t len);
int i2c_exec (void);

#endif

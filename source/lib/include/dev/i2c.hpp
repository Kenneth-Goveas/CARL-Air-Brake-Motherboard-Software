#ifndef __DEV_I2C_HPP__
#define __DEV_I2C_HPP__

#include <cstdint>

#include <string>
#include <vector>

namespace i2c {

bool fail (void);

void init (std::string path);
void deinit (void);

void send (std::uint8_t addr, std::vector<std::uint8_t> buf);
void recv (std::uint8_t addr, std::vector<std::uint8_t> * buf);
void exec (void);

}

#endif

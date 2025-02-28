#ifndef __UI_OUTPUT_HPP__
#define __UI_OUTPUT_HPP__

#include <cstdint>

#include <string>
#include <vector>

#include <math/linalg.hpp>

namespace output {

void put (const char * val);

void put (std::string val);
void put (std::uint8_t val);
void put (bool val);
void put (int val);
void put (double val);
void put (linalg::ivector val);
void put (linalg::fvector val);
void put (linalg::imatrix val);
void put (linalg::fmatrix val);

void put (std::vector<std::string> val);
void put (std::vector<std::uint8_t> val);
void put (std::vector<bool> val);
void put (std::vector<int> val);
void put (std::vector<double> val);
void put (std::vector<linalg::ivector> val);
void put (std::vector<linalg::fvector> val);
void put (std::vector<linalg::imatrix> val);
void put (std::vector<linalg::fmatrix> val);

}

#endif

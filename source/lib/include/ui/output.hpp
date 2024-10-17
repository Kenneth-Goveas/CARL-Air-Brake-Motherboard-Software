#ifndef __UI_OUTPUT_HPP__
#define __UI_OUTPUT_HPP__

#include <string>

#include <math/linalg.hpp>

namespace output {

void put (bool val);
void put (std::string val);
void put (int val);
void put (double val);
void put (linalg::ivector val);
void put (linalg::fvector val);
void put (linalg::imatrix val);
void put (linalg::fmatrix val);
void put (const char * val);

}

#endif

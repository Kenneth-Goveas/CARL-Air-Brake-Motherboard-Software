#ifndef __UI_OUTPUT_HPP__
#define __UI_OUTPUT_HPP__

#include <string>

#include <math/linalg.hpp>

namespace output {

void print (bool val);
void print (std::string val);
void print (int val);
void print (double val);
void print (linalg::ivector val);
void print (linalg::fvector val);
void print (linalg::imatrix val);
void print (linalg::fmatrix val);
void print (const char * val);

}

#endif

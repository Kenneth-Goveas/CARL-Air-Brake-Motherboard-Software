#ifndef __MATH_RANDNUM_HPP__
#define __MATH_RANDNUM_HPP__

#include <math/linalg.hpp>

namespace randnum {

double unif (double lowr, double uppr);
linalg::fvector unif (linalg::fvector lowr, linalg::fvector uppr);
linalg::fmatrix unif (linalg::fmatrix lowr, linalg::fmatrix uppr);

double gauss (double mean, double var);
linalg::fvector gauss (linalg::fvector mean, linalg::fmatrix var);

}

#endif

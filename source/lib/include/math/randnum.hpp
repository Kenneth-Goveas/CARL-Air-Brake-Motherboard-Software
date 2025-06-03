#ifndef __MATH_RANDNUM_HPP__
#define __MATH_RANDNUM_HPP__

#include <math/linalg.hpp>

namespace randnum {

double unif (double lowr, double uppr);
linalg::fvector unif (const linalg::fvector & lowr, const linalg::fvector & uppr);
linalg::fmatrix unif (const linalg::fmatrix & lowr, const linalg::fmatrix & uppr);

double gauss (double mean, double var);
linalg::fvector gauss (const linalg::fvector & mean, const linalg::fmatrix & var);

}

#endif

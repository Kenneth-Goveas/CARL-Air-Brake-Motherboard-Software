#ifndef __MATH_LAPACK_HPP__
#define __MATH_LAPACK_HPP__

#include <vector>

namespace lapack {

void dgetrf (const std::vector<std::vector<double>> & data, std::vector<std::vector<double>> & fact, std::vector<int> & pivot);
void dgetri (const std::vector<std::vector<double>> & fact, const std::vector<int> & pivot, std::vector<std::vector<double>> & inv);
void dsyev (const std::vector<std::vector<double>> & data, std::vector<double> & eigval);
void dsyev (const std::vector<std::vector<double>> & data, std::vector<double> & eigval, std::vector<std::vector<double>> & eigvec);

}

#endif

#ifndef __MATH_LAPACK_HPP__
#define __MATH_LAPACK_HPP__

#include <vector>
#include <tuple>

namespace lapack {

std::tuple<std::vector<std::vector<double>>, std::vector<double>>
dgeqrf (std::vector<std::vector<double>> data);

std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>>
dorgqr (std::vector<std::vector<double>> fact, std::vector<double> coef);

std::tuple<std::vector<std::vector<double>>, std::vector<double>>
dgerqf (std::vector<std::vector<double>> data);

std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>>
dorgrq (std::vector<std::vector<double>> fact, std::vector<double> coef);

std::tuple<std::vector<std::vector<double>>, std::vector<double>>
dgeqlf (std::vector<std::vector<double>> data);

std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>>
dorgql (std::vector<std::vector<double>> fact, std::vector<double> coef);

std::tuple<std::vector<std::vector<double>>, std::vector<double>>
dgelqf (std::vector<std::vector<double>> data);

std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>>
dorglq (std::vector<std::vector<double>> fact, std::vector<double> coef);

std::tuple<std::vector<std::vector<double>>, std::vector<int>>
dgetrf (std::vector<std::vector<double>> data);

std::vector<std::vector<double>>
dgetri (std::vector<std::vector<double>> fact, std::vector<int> pivot);

std::tuple<std::vector<double>, std::vector<std::vector<double>>>
dsyev (std::vector<std::vector<double>> data);

}

#endif

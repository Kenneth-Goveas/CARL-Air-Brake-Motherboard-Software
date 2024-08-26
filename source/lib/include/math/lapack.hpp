#ifndef __MATH_LAPACK_HPP__
#define __MATH_LAPACK_HPP__

#include <vector>

namespace lapack {

void dgeqrf (
    std::vector<std::vector<double>> data,
    std::vector<std::vector<double>> * fact,
    std::vector<double> * coef
);
void dorgqr (
    std::vector<std::vector<double>> fact,
    std::vector<double> coef,
    std::vector<std::vector<double>> * orth,
    std::vector<std::vector<double>> * uppr
);
void dgerqf (
    std::vector<std::vector<double>> data,
    std::vector<std::vector<double>> * fact,
    std::vector<double> * coef
);
void dorgrq (
    std::vector<std::vector<double>> fact,
    std::vector<double> coef,
    std::vector<std::vector<double>> * uppr,
    std::vector<std::vector<double>> * orth
);
void dgeqlf (
    std::vector<std::vector<double>> data,
    std::vector<std::vector<double>> * fact,
    std::vector<double> * coef
);
void dorgql (
    std::vector<std::vector<double>> fact,
    std::vector<double> coef,
    std::vector<std::vector<double>> * orth,
    std::vector<std::vector<double>> * lowr
);
void dgelqf (
    std::vector<std::vector<double>> data,
    std::vector<std::vector<double>> * fact,
    std::vector<double> * coef
);
void dorglq (
    std::vector<std::vector<double>> fact,
    std::vector<double> coef,
    std::vector<std::vector<double>> * lowr,
    std::vector<std::vector<double>> * orth
);
void dgetrf (
    std::vector<std::vector<double>> data,
    std::vector<std::vector<double>> * perm,
    std::vector<std::vector<double>> * lowr,
    std::vector<std::vector<double>> * uppr
);
void dgetrf (
    std::vector<std::vector<double>> data,
    std::vector<double> * diag, int * sign
);
void dgetrf (
    std::vector<std::vector<double>> data,
    std::vector<std::vector<double>> * fact,
    std::vector<int> * pivot
);
void dgetri (
    std::vector<std::vector<double>> fact,
    std::vector<int> pivot,
    std::vector<std::vector<double>> * inv
);
void dsyev (
    std::vector<std::vector<double>> data,
    std::vector<double> * eigval
);
void dsyev (
    std::vector<std::vector<double>> data,
    std::vector<std::vector<double>> * eigval,
    std::vector<std::vector<double>> * eigvec
);

}

#endif

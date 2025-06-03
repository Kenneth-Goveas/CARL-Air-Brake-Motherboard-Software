#include <cstdint>
#include <cmath>
#include <csignal>

#include <random>
#include <chrono>
#include <tuple>

#include <math/linalg.hpp>
#include <math/randnum.hpp>

namespace randnum::intern {

std::uint_fast64_t seed (void);

std::mt19937_64 rng(seed());
std::uniform_real_distribution<double> unif(0, 1);
std::normal_distribution<double> gauss(0, 1);

}

namespace randnum {

double unif (double lowr, double uppr) {
    double res;

    if (lowr >= uppr) {
        std::raise(SIGFPE);
    }

    res = lowr + (uppr - lowr) * intern::unif(intern::rng);

    return res;
}

linalg::fvector unif (const linalg::fvector & lowr, const linalg::fvector & uppr) {
    linalg::fvector res;

    if (linalg::null(lowr) || linalg::null(uppr)) {
        std::raise(SIGFPE);
    } else if (linalg::dim(lowr) != linalg::dim(uppr)) {
        std::raise(SIGFPE);
    } else if (!(lowr < uppr)) {
        std::raise(SIGFPE);
    }

    linalg::resize(res, linalg::dim(lowr));
    for (int i = 0; i < linalg::dim(res); i++) {
        res[i] = unif(lowr[i], uppr[i]);
    }

    return res;
}

linalg::fmatrix unif (const linalg::fmatrix & lowr, const linalg::fmatrix & uppr) {
    linalg::fmatrix res;

    if (linalg::null(lowr) || linalg::null(uppr)) {
        std::raise(SIGFPE);
    } else if (linalg::rows(lowr) != linalg::rows(uppr) || linalg::cols(lowr) != linalg::cols(uppr)) {
        std::raise(SIGFPE);
    } else if (!(lowr < uppr)) {
        std::raise(SIGFPE);
    }

    linalg::resize(res, linalg::rows(lowr), linalg::cols(lowr));
    for (int i = 0; i < linalg::rows(res); i++) {
        for (int j = 0; j < linalg::cols(res); j++) {
            res[i][j] = unif(lowr[i][j], uppr[i][j]);
        }
    }

    return res;
}

double gauss (double mean, double var) {
    double res;

    if (var <= 0) {
        std::raise(SIGFPE);
    }

    res = mean + std::sqrt(var) * intern::gauss(intern::rng);

    return res;
}

linalg::fvector gauss (const linalg::fvector & mean, const linalg::fmatrix & var) {
    linalg::fvector res;

    linalg::fvector var_eigval;
    linalg::fmatrix var_eigvec;

    if (linalg::null(mean) || linalg::null(var)) {
        std::raise(SIGFPE);
    } else if (linalg::dim(mean) != linalg::rows(var) || linalg::dim(mean) != linalg::cols(var)) {
        std::raise(SIGFPE);
    } else if (!linalg::sym_pdf(var)) {
        std::raise(SIGFPE);
    }

    std::tie(var_eigval, var_eigvec) = linalg::eig(var);
    linalg::resize(res, linalg::dim(mean));
    for (int i = 0; i < linalg::dim(res); i++) {
        res[i] = gauss(0, var_eigval[i]);
    }
    res = mean + var_eigvec * res;

    return res;
}

}

namespace randnum::intern {

std::uint_fast64_t seed (void) {
    auto tstamp = std::chrono::steady_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(tstamp).count();
}

}

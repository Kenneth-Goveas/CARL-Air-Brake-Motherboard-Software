#include <cstdint>
#include <cmath>
#include <csignal>

#include <chrono>
#include <random>

#include <math/linalg.hpp>
#include <math/randnum.hpp>

namespace randnum :: intern {

std::uint_fast64_t seed (void);

std::mt19937_64 rng(seed());
std::uniform_real_distribution<double> unif(0, 1);
std::normal_distribution<double> gauss(0, 1);

}

namespace randnum {

double unif (double lowr, double uppr) {
    double res;

    if (!(linalg::is_finite(lowr) && linalg::is_finite(uppr))) {
        std::raise(SIGFPE);
    } else if (!linalg::is_lneq(lowr, uppr)) {
        std::raise(SIGFPE);
    }

    res = lowr + (uppr - lowr) * intern::unif(intern::rng);
    linalg::filter(&res);

    return res;
}

linalg::fvector unif (linalg::fvector lowr, linalg::fvector uppr) {
    linalg::fvector res;

    if (!(linalg::is_finite(lowr) && linalg::is_finite(uppr))) {
        std::raise(SIGFPE);
    } else if (!(linalg::dim(lowr) == linalg::dim(uppr))) {
        std::raise(SIGFPE);
    } else if (!(lowr < uppr)) {
        std::raise(SIGFPE);
    }

    linalg::resize(&res, linalg::dim(lowr));
    for (int i = 0; i < linalg::dim(res); i++) {
        res[i] = unif(lowr[i], uppr[i]);
    }
    linalg::filter(&res);

    return res;
}

linalg::fmatrix unif (linalg::fmatrix lowr, linalg::fmatrix uppr) {
    linalg::fmatrix res;

    if (!(linalg::is_finite(lowr) && linalg::is_finite(uppr))) {
        std::raise(SIGFPE);
    } else if (!(
           linalg::rows(lowr) == linalg::rows(uppr)
        && linalg::cols(lowr) == linalg::cols(uppr)
    )) {
        std::raise(SIGFPE);
    } else if (!(lowr < uppr)) {
        std::raise(SIGFPE);
    }

    linalg::resize(&res, linalg::rows(lowr), linalg::cols(lowr));
    for (int i = 0; i < linalg::rows(res); i++) {
        for (int j = 0; j < linalg::cols(res); j++) {
            res[i][j] = unif(lowr[i][j], uppr[i][j]);
        }
    }
    linalg::filter(&res);

    return res;
}

double gauss (double mean, double var) {
    double res;

    if (!(linalg::is_finite(mean) && linalg::is_finite(var))) {
        std::raise(SIGFPE);
    } else if (!linalg::is_gneq(var, 0)) {
        std::raise(SIGFPE);
    }

    res = mean + std::sqrt(var) * intern::gauss(intern::rng);
    linalg::filter(&res);

    return res;
}

linalg::fvector gauss (linalg::fvector mean, linalg::fmatrix var) {
    linalg::fvector res;
    linalg::fmatrix var_orth, var_diag;

    if (!(linalg::is_finite(mean) && linalg::is_finite(var))) {
        std::raise(SIGFPE);
    } else if (!(
           linalg::dim(mean) == linalg::rows(var)
        && linalg::dim(mean) == linalg::cols(var)
    )) {
        std::raise(SIGFPE);
    } else if (!linalg::is_sym_pdf(var)) {
        std::raise(SIGFPE);
    }

    linalg::qdq(var, &var_orth, &var_diag);
    linalg::resize(&res, linalg::dim(mean));
    for (int i = 0; i < linalg::dim(res); i++) {
        res[i] = gauss(0, var_diag[i][i]);
    }
    res = mean + var_orth * res;

    return res;
}

}

namespace randnum :: intern {

std::uint_fast64_t seed (void) {
    auto tstamp = std::chrono::steady_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(tstamp).count();
}

}
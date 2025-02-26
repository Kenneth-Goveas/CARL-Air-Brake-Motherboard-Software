#ifndef __SYS_LOGGING_HPP__
#define __SYS_LOGGING_HPP__

#include <cstdint>

#include <string>
#include <vector>

#include <math/linalg.hpp>

namespace logging {

class buffer {
    public:
        buffer (const char * val);

        buffer (std::string val);
        buffer (std::uint8_t val);
        buffer (bool val);
        buffer (int val);
        buffer (double val);
        buffer (linalg::ivector val);
        buffer (linalg::fvector val);
        buffer (linalg::imatrix val);
        buffer (linalg::fmatrix val);

        buffer (std::vector<std::string> val);
        buffer (std::vector<std::uint8_t> val);
        buffer (std::vector<bool> val);
        buffer (std::vector<int> val);
        buffer (std::vector<double> val);
        buffer (std::vector<linalg::ivector> val);
        buffer (std::vector<linalg::fvector> val);
        buffer (std::vector<linalg::imatrix> val);
        buffer (std::vector<linalg::fmatrix> val);

        operator std::string (void) const;

    private:
        std::string text;
};

bool fail (void);

void init (std::string path, std::string unit);
void deinit (void);

template <typename ... types>
void err (std::string mod, types ... args);

template <typename ... types>
void wrn (std::string mod, types ... args);

template <typename ... types>
void inf (std::string mod, types ... args);

}

#include <sys/logging.tpp>

#endif

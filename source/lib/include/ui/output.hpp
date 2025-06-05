#ifndef __UI_OUTPUT_HPP__
#define __UI_OUTPUT_HPP__

#include <cstdint>

#include <string>
#include <vector>

#include <math/linalg.hpp>
#include <sys/logging.hpp>

namespace output {

class tracker {
    protected:
        static bool exist;
        static bool instantiate (void);
};

class instance : private tracker {
    public:
        instance (void);

        void put (const char * val);

        void put (const std::string & val);
        void put (std::uint8_t val);
        void put (bool val);
        void put (int val);
        void put (double val);
        void put (const linalg::ivector & val);
        void put (const linalg::fvector & val);
        void put (const linalg::imatrix & val);
        void put (const linalg::fmatrix & val);

        void put (const std::vector<std::string> & val);
        void put (const std::vector<std::uint8_t> & val);
        void put (const std::vector<bool> & val);
        void put (const std::vector<int> & val);
        void put (const std::vector<double> & val);
        void put (const std::vector<linalg::ivector> & val);
        void put (const std::vector<linalg::fvector> & val);
        void put (const std::vector<linalg::imatrix> & val);
        void put (const std::vector<linalg::fmatrix> & val);

        enum class except_ctor {fail};
        enum class except_put {fail};

    private:
        logging::instance logging;
        bool init;
};

}

#endif

#ifndef __DAT_TXTWRITE_HPP__
#define __DAT_TXTWRITE_HPP__

#include <cstdint>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <math/linalg.hpp>
#include <sys/logging.hpp>

namespace txtwrite {

class tracker {
    protected:
        static int count;
        static int instantiate (void);
};

class instance : private tracker {
    public:
        instance (const std::string & path);
        ~instance (void);

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

        void newline (void);

        enum class except_ctor {fail};
        enum class except_put {fail};
        enum class except_newline {fail};

    private:
        logging::instance logging;
        bool init;
        int id;

        std::ofstream file;
};

}

#endif

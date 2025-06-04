#ifndef __SYS_LOGGING_HPP__
#define __SYS_LOGGING_HPP__

#include <cstdint>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <math/linalg.hpp>

namespace logging {

class stream;

class instance {
    public:
        static bool start (const std::string & path, const std::string & app);
        static void stop (void);

        instance (const std::string & lib);

        template <typename ... types>
        void inf (const types & ... args);

        template <typename ... types>
        void wrn (const types & ... args);

        template <typename ... types>
        void err (const types & ... args);

    private:
        static bool init;
        static int desc;
        static std::ofstream file;
        static std::string app;
        static stream strm;

        std::string lib;

        void head (const std::string & lev);
        void body (void);

        template <typename type, typename ... types>
        void body (const type & arg, const types & ... args);

        template <typename ... types>
        void msg (const std::string & lev, const types & ... args);
};

class stream {
    public:
        stream (std::ofstream & file);

        stream & operator << (const char * val);

        stream & operator << (const std::string & val);
        stream & operator << (std::uint8_t val);
        stream & operator << (bool val);
        stream & operator << (int val);
        stream & operator << (double val);
        stream & operator << (const linalg::ivector & val);
        stream & operator << (const linalg::fvector & val);
        stream & operator << (const linalg::imatrix & val);
        stream & operator << (const linalg::fmatrix & val);

        stream & operator << (const std::vector<std::string> & val);
        stream & operator << (const std::vector<std::uint8_t> & val);
        stream & operator << (const std::vector<bool> & val);
        stream & operator << (const std::vector<int> & val);
        stream & operator << (const std::vector<double> & val);
        stream & operator << (const std::vector<linalg::ivector> & val);
        stream & operator << (const std::vector<linalg::fvector> & val);
        stream & operator << (const std::vector<linalg::imatrix> & val);
        stream & operator << (const std::vector<linalg::fmatrix> & val);

    private:
        std::ofstream * file;
};

}

#include <sys/logging.tpp>

#endif

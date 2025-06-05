#ifndef __SYS_CONFIG_HPP__
#define __SYS_CONFIG_HPP__

#include <cstdint>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <math/linalg.hpp>
#include <sys/logging.hpp>

namespace config {

class tracker {
    protected:
        static bool exist;
        static bool instantiate (void);
};

class instance : private tracker {
    public:
        instance (const std::string & path);

        template <typename type>
        type get (const std::string & name);

        enum class except_ctor {fail};
        enum class except_get {fail};

    private:
        logging::instance logging;
        bool init;

        std::ifstream file;

        std::vector<std::string> name_all;

        std::vector<std::string> name_str;
        std::vector<std::string> val_str;

        std::vector<std::string> name_byte;
        std::vector<std::uint8_t> val_byte;

        std::vector<std::string> name_bool;
        std::vector<bool> val_bool;

        std::vector<std::string> name_iscl;
        std::vector<int> val_iscl;

        std::vector<std::string> name_fscl;
        std::vector<double> val_fscl;

        std::vector<std::string> name_ivec;
        std::vector<linalg::ivector> val_ivec;

        std::vector<std::string> name_fvec;
        std::vector<linalg::fvector> val_fvec;

        std::vector<std::string> name_imat;
        std::vector<linalg::imatrix> val_imat;

        std::vector<std::string> name_fmat;
        std::vector<linalg::fmatrix> val_fmat;

        std::vector<std::string> name_str_arr;
        std::vector<std::vector<std::string>> val_str_arr;

        std::vector<std::string> name_byte_arr;
        std::vector<std::vector<std::uint8_t>> val_byte_arr;

        std::vector<std::string> name_bool_arr;
        std::vector<std::vector<bool>> val_bool_arr;

        std::vector<std::string> name_iscl_arr;
        std::vector<std::vector<int>> val_iscl_arr;

        std::vector<std::string> name_fscl_arr;
        std::vector<std::vector<double>> val_fscl_arr;

        std::vector<std::string> name_ivec_arr;
        std::vector<std::vector<linalg::ivector>> val_ivec_arr;

        std::vector<std::string> name_fvec_arr;
        std::vector<std::vector<linalg::fvector>> val_fvec_arr;

        std::vector<std::string> name_imat_arr;
        std::vector<std::vector<linalg::imatrix>> val_imat_arr;

        std::vector<std::string> name_fmat_arr;
        std::vector<std::vector<linalg::fmatrix>> val_fmat_arr;

        void load (void);
        void read (std::string & stmt);
        void dissect (const std::string & str, std::string & name, std::string & repr);
        void split (const std::string & str, std::vector<std::string> & arr);
        void strip (std::string & str);

        void parse (const std::string & str, std::string & val);
        void parse (const std::string & str, std::uint8_t & val);
        void parse (const std::string & str, bool & val);
        void parse (const std::string & str, int & val);
        void parse (const std::string & str, double & val);
        void parse (const std::string & str, linalg::ivector & val);
        void parse (const std::string & str, linalg::fvector & val);
        void parse (const std::string & str, linalg::imatrix & val);
        void parse (const std::string & str, linalg::fmatrix & val);

        void parse (const std::string & str, std::vector<std::string> & val);
        void parse (const std::string & str, std::vector<std::uint8_t> & val);
        void parse (const std::string & str, std::vector<bool> & val);
        void parse (const std::string & str, std::vector<int> & val);
        void parse (const std::string & str, std::vector<double> & val);
        void parse (const std::string & str, std::vector<linalg::ivector> & val);
        void parse (const std::string & str, std::vector<linalg::fvector> & val);
        void parse (const std::string & str, std::vector<linalg::imatrix> & val);
        void parse (const std::string & str, std::vector<linalg::fmatrix> & val);

        enum class except_load {fail, eof};
        enum class except_read {fail, eof};
        enum class except_dissect {fail, blank};
        enum class except_split {fail};
        enum class except_parse {fail};

        class except_intern {};
};

template <>
std::string instance::get<std::string> (const std::string & name);

template <>
std::uint8_t instance::get<std::uint8_t> (const std::string & name);

template <>
bool instance::get<bool> (const std::string & name);

template <>
int instance::get<int> (const std::string & name);

template <>
double instance::get<double> (const std::string & name);

template <>
linalg::ivector instance::get<linalg::ivector> (const std::string & name);

template <>
linalg::fvector instance::get<linalg::fvector> (const std::string & name);

template <>
linalg::imatrix instance::get<linalg::imatrix> (const std::string & name);

template <>
linalg::fmatrix instance::get<linalg::fmatrix> (const std::string & name);

template <>
std::vector<std::string> instance::get<std::vector<std::string>> (const std::string & name);

template <>
std::vector<std::uint8_t> instance::get<std::vector<std::uint8_t>> (const std::string & name);

template <>
std::vector<bool> instance::get<std::vector<bool>> (const std::string & name);

template <>
std::vector<int> instance::get<std::vector<int>> (const std::string & name);

template <>
std::vector<double> instance::get<std::vector<double>> (const std::string & name);

template <>
std::vector<linalg::ivector> instance::get<std::vector<linalg::ivector>> (const std::string & name);

template <>
std::vector<linalg::fvector> instance::get<std::vector<linalg::fvector>> (const std::string & name);

template <>
std::vector<linalg::imatrix> instance::get<std::vector<linalg::imatrix>> (const std::string & name);

template <>
std::vector<linalg::fmatrix> instance::get<std::vector<linalg::fmatrix>> (const std::string & name);

}

#endif

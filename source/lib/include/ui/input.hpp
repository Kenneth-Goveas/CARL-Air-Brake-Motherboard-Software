#ifndef __UI_INPUT_HPP__
#define __UI_INPUT_HPP__

#include <cstdint>

#include <string>
#include <vector>

#include <math/linalg.hpp>
#include <sys/logging.hpp>

namespace input {

class tracker {
    protected:
        static bool exist;
        static bool instantiate (void);
};

class instance : private tracker {
    public:
        instance (void);

        template <typename type>
        type get (void);

        enum class except_ctor {fail};
        enum class except_get {fail, eof};

    private:
        logging::instance logging;
        bool init;

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

        enum class except_split {fail};
        enum class except_parse {fail};
};

template <>
std::string instance::get<std::string> (void);

template <>
std::uint8_t instance::get<std::uint8_t> (void);

template <>
bool instance::get<bool> (void);

template <>
int instance::get<int> (void);

template <>
double instance::get<double> (void);

template <>
linalg::ivector instance::get<linalg::ivector> (void);

template <>
linalg::fvector instance::get<linalg::fvector> (void);

template <>
linalg::imatrix instance::get<linalg::imatrix> (void);

template <>
linalg::fmatrix instance::get<linalg::fmatrix> (void);

template <>
std::vector<std::string> instance::get<std::vector<std::string>> (void);

template <>
std::vector<std::uint8_t> instance::get<std::vector<std::uint8_t>> (void);

template <>
std::vector<bool> instance::get<std::vector<bool>> (void);

template <>
std::vector<int> instance::get<std::vector<int>> (void);

template <>
std::vector<double> instance::get<std::vector<double>> (void);

template <>
std::vector<linalg::ivector> instance::get<std::vector<linalg::ivector>> (void);

template <>
std::vector<linalg::fvector> instance::get<std::vector<linalg::fvector>> (void);

template <>
std::vector<linalg::imatrix> instance::get<std::vector<linalg::imatrix>> (void);

template <>
std::vector<linalg::fmatrix> instance::get<std::vector<linalg::fmatrix>> (void);

}

#endif

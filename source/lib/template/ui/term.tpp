#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <sys/logging.hpp>

namespace term :: intern {

extern std::string mod;

extern bool fail;
extern bool eof;

template <typename type>
type conv_inpt_to_val (std::string inpt);

template <>
bool conv_inpt_to_val<bool> (std::string inpt);

template <>
std::string conv_inpt_to_val<std::string> (std::string inpt);

template <>
int conv_inpt_to_val<int> (std::string inpt);

template <>
double conv_inpt_to_val<double> (std::string inpt);

template <typename type>
std::string conv_val_to_oupt (type val);

template <>
std::string conv_val_to_oupt<bool> (bool val);

template <>
std::string conv_val_to_oupt<std::string> (std::string val);

template <>
std::string conv_val_to_oupt<int> (int val);

template <>
std::string conv_val_to_oupt<double> (double val);

std::string conv_color_to_oupt (color clr);

std::string strip (std::string inpt);

void append (std::ostringstream * ostr);

template <typename type, typename ... types>
void append (std::ostringstream * ostr, type arg, types ... args);

}

namespace term {

template <typename ... types>
void print (types ... args) {
    std::ostringstream ostr;
    std::string str;

    intern::append(&ostr, args ...);
    str = ostr.str();

    logging::inf(intern::mod,
        "Printing output: “", str, "”"
    );

    std::cout << str;
    std::cout.flush();

    intern::fail = false;
}

}

namespace term :: intern {

template <typename type, typename ... types>
void append (std::ostringstream * ostr, type arg, types ... args) {
    *ostr << arg;
    append(ostr, args ...);

    fail = false;
}

}

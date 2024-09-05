#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <math/linalg.hpp>

#include <sys/logging.hpp>

namespace term :: intern {

extern std::string mod;

extern bool fail;
extern bool eof;

bool vec_sntx_arbit_len (std::string inpt);
bool vec_sntx_fixed_len (std::string inpt, int len);

bool mat_sntx_arbit_rows_arbit_cols (std::string inpt);
bool mat_sntx_fixed_rows_arbit_cols (std::string inpt, int rows);
bool mat_sntx_arbit_rows_fixed_cols (std::string inpt, int cols);
bool mat_sntx_fixed_rows_fixed_cols (std::string inpt, int rows, int cols);

template <typename type>
type vec_parse (std::string inpt, int len);

template <>
linalg::ivector vec_parse<linalg::ivector> (std::string inpt, int len);

template <>
linalg::fvector vec_parse<linalg::fvector> (std::string inpt, int len);

template <typename type>
type mat_parse (std::string inpt, int rows, int cols);

template <>
linalg::imatrix mat_parse<linalg::imatrix> (
    std::string inpt, int rows, int cols
);

template <>
linalg::fmatrix mat_parse<linalg::fmatrix> (
    std::string inpt, int rows, int cols
);

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

template <>
linalg::ivector conv_inpt_to_val<linalg::ivector> (std::string inpt);

template <>
linalg::fvector conv_inpt_to_val<linalg::fvector> (std::string inpt);

template <>
linalg::imatrix conv_inpt_to_val<linalg::imatrix> (std::string inpt);

template <>
linalg::fmatrix conv_inpt_to_val<linalg::fmatrix> (std::string inpt);

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

template <>
std::string conv_val_to_oupt<linalg::ivector> (linalg::ivector val);

template <>
std::string conv_val_to_oupt<linalg::fvector> (linalg::fvector val);

template <>
std::string conv_val_to_oupt<linalg::imatrix> (linalg::imatrix val);

template <>
std::string conv_val_to_oupt<linalg::fmatrix> (linalg::fmatrix val);

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
}

}

namespace term :: intern {

template <typename type, typename ... types>
void append (std::ostringstream * ostr, type arg, types ... args) {
    *ostr << arg;
    append(ostr, args ...);
}

}

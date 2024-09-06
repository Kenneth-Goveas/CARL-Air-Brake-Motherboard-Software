#ifndef __UI_INPUT_HPP__
#define __UI_INPUT_HPP__

#include <string>

#include <math/linalg.hpp>

namespace input {

bool fail (void);
bool eof (void);

template <typename type>
type scan (void);

template <>
bool scan<bool> (void);

template <>
std::string scan<std::string> (void);

template <>
int scan<int> (void);

template <>
double scan<double> (void);

template <>
linalg::ivector scan<linalg::ivector> (void);

template <>
linalg::fvector scan<linalg::fvector> (void);

template <>
linalg::imatrix scan<linalg::imatrix> (void);

template <>
linalg::fmatrix scan<linalg::fmatrix> (void);

}

#endif

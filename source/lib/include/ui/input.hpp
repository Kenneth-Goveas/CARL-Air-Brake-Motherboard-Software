#ifndef __UI_INPUT_HPP__
#define __UI_INPUT_HPP__

#include <string>

#include <math/linalg.hpp>

namespace input {

bool fail (void);
bool eof (void);

template <typename type>
type get (void);

template <>
bool get<bool> (void);

template <>
std::string get<std::string> (void);

template <>
int get<int> (void);

template <>
double get<double> (void);

template <>
linalg::ivector get<linalg::ivector> (void);

template <>
linalg::fvector get<linalg::fvector> (void);

template <>
linalg::imatrix get<linalg::imatrix> (void);

template <>
linalg::fmatrix get<linalg::fmatrix> (void);

}

#endif

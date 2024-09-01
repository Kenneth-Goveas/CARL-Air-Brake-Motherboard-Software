#ifndef __SYS_CONFIG_HPP__
#define __SYS_CONFIG_HPP__

#include <string>

#include <math/linalg.hpp>

namespace config {

bool fail (void);

void init (std::string path);

template <typename type>
type get (std::string name);

template <>
bool get<bool> (std::string name);

template <>
std::string get<std::string> (std::string name);

template <>
int get<int> (std::string name);

template <>
double get<double> (std::string name);

template <>
linalg::ivector get<linalg::ivector> (std::string name);

template <>
linalg::fvector get<linalg::fvector> (std::string name);

template <>
linalg::imatrix get<linalg::imatrix> (std::string name);

template <>
linalg::fmatrix get<linalg::fmatrix> (std::string name);

}

#endif

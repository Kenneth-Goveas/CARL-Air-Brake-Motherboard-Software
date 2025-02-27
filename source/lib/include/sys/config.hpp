#ifndef __SYS_CONFIG_HPP__
#define __SYS_CONFIG_HPP__

#include <cstdint>

#include <string>
#include <vector>

#include <math/linalg.hpp>

namespace config {

bool fail (void);

void init (std::string path);

template <typename type>
type get (std::string name);

template <>
std::string get<std::string> (std::string name);

template <>
std::uint8_t get<std::uint8_t> (std::string name);

template <>
bool get<bool> (std::string name);

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

template <>
std::vector<std::string> get<std::vector<std::string>> (std::string name);

template <>
std::vector<std::uint8_t> get<std::vector<std::uint8_t>> (std::string name);

template <>
std::vector<bool> get<std::vector<bool>> (std::string name);

template <>
std::vector<int> get<std::vector<int>> (std::string name);

template <>
std::vector<double> get<std::vector<double>> (std::string name);

template <>
std::vector<linalg::ivector> get<std::vector<linalg::ivector>> (std::string name);

template <>
std::vector<linalg::fvector> get<std::vector<linalg::fvector>> (std::string name);

template <>
std::vector<linalg::imatrix> get<std::vector<linalg::imatrix>> (std::string name);

template <>
std::vector<linalg::fmatrix> get<std::vector<linalg::fmatrix>> (std::string name);

}

#endif

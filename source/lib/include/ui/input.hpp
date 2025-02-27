#ifndef __UI_INPUT_HPP__
#define __UI_INPUT_HPP__

#include <cstdint>

#include <string>
#include <vector>

#include <math/linalg.hpp>

namespace input {

bool fail (void);
bool eof (void);

template <typename type>
type get (void);

template <>
std::string get<std::string> (void);

template <>
std::uint8_t get<std::uint8_t> (void);

template <>
bool get<bool> (void);

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

template <>
std::vector<std::string> get<std::vector<std::string>> (void);

template <>
std::vector<std::uint8_t> get<std::vector<std::uint8_t>> (void);

template <>
std::vector<bool> get<std::vector<bool>> (void);

template <>
std::vector<int> get<std::vector<int>> (void);

template <>
std::vector<double> get<std::vector<double>> (void);

template <>
std::vector<linalg::ivector> get<std::vector<linalg::ivector>> (void);

template <>
std::vector<linalg::fvector> get<std::vector<linalg::fvector>> (void);

template <>
std::vector<linalg::imatrix> get<std::vector<linalg::imatrix>> (void);

template <>
std::vector<linalg::fmatrix> get<std::vector<linalg::fmatrix>> (void);

}

#endif

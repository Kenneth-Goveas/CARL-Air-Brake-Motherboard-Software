#ifndef __IPC_SHAREDMEM_HPP__
#define __IPC_SHAREDMEM_HPP__

#include <cstdint>

#include <string>

#include <math/linalg.hpp>

namespace sharedmem {

bool fail (void);

bool exists (std::string name);

template <typename type>
bool fits (std::string name);

template <typename type>
bool fits (std::string name, int dim);

template <typename type>
bool fits (std::string name, int rows, int cols);

template <>
bool fits<std::uint8_t> (std::string name);

template <>
bool fits<bool> (std::string name);

template <>
bool fits<int> (std::string name);

template <>
bool fits<double> (std::string name);

template <>
bool fits<linalg::ivector> (std::string name, int dim);

template <>
bool fits<linalg::fvector> (std::string name, int dim);

template <>
bool fits<linalg::imatrix> (std::string name, int rows, int cols);

template <>
bool fits<linalg::fmatrix> (std::string name, int rows, int cols);

template <typename type>
void create (std::string name);

template <typename type>
void create (std::string name, int dim);

template <typename type>
void create (std::string name, int rows, int cols);

template <>
void create<std::uint8_t> (std::string name);

template <>
void create<bool> (std::string name);

template <>
void create<int> (std::string name);

template <>
void create<double> (std::string name);

template <>
void create<linalg::ivector> (std::string name, int dim);

template <>
void create<linalg::fvector> (std::string name, int dim);

template <>
void create<linalg::imatrix> (std::string name, int rows, int cols);

template <>
void create<linalg::fmatrix> (std::string name, int rows, int cols);

void remove (std::string name);

int open (std::string name);
void close (void);

void put (int id, std::uint8_t val);
void put (int id, bool val);
void put (int id, int val);
void put (int id, double val);
void put (int id, linalg::ivector val);
void put (int id, linalg::fvector val);
void put (int id, linalg::imatrix val);
void put (int id, linalg::fmatrix val);

template <typename type>
type get (int id);

template <typename type>
type get (int id, int dim);

template <typename type>
type get (int id, int rows, int cols);

template <>
std::uint8_t get<std::uint8_t> (int id);

template <>
bool get<bool> (int id);

template <>
int get<int> (int id);

template <>
double get<double> (int id);

template <>
linalg::ivector get<linalg::ivector> (int id, int dim);

template <>
linalg::fvector get<linalg::fvector> (int id, int dim);

template <>
linalg::imatrix get<linalg::imatrix> (int id, int rows, int cols);

template <>
linalg::fmatrix get<linalg::fmatrix> (int id, int rows, int cols);

}

#endif

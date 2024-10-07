#ifndef __IPC_SHMEM_HPP__
#define __IPC_SHMEM_HPP__

#include <string>

#include <math/linalg.hpp>

namespace shmem {

bool fail (void);

bool exists (std::string name);

template <typename type>
bool fits (std::string name);

template <typename type>
bool fits (std::string name, int dim);

template <typename type>
bool fits (std::string name, int rows, int cols);

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

int map (std::string name);
void unmap (void);

void write (int id, bool val);
void write (int id, int val);
void write (int id, double val);
void write (int id, linalg::ivector val);
void write (int id, linalg::fvector val);
void write (int id, linalg::imatrix val);
void write (int id, linalg::fmatrix val);

template <typename type>
type read (int id);

template <typename type>
type read (int id, int dim);

template <typename type>
type read (int id, int rows, int cols);

template <>
bool read<bool> (int id);

template <>
int read<int> (int id);

template <>
double read<double> (int id);

template <>
linalg::ivector read<linalg::ivector> (int id, int dim);

template <>
linalg::fvector read<linalg::fvector> (int id, int dim);

template <>
linalg::imatrix read<linalg::imatrix> (int id, int rows, int cols);

template <>
linalg::fmatrix read<linalg::fmatrix> (int id, int rows, int cols);

}

#endif

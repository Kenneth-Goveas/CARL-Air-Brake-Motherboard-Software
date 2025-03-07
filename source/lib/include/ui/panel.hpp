#ifndef __UI_PANEL_HPP__
#define __UI_PANEL_HPP__

#include <cstdint>

#include <string>
#include <vector>

#include <math/linalg.hpp>

#include <ui/terminal.hpp>

namespace panel {

bool fail (void);
bool eof (void);

void init (void);
void deinit (void);

int open (terminal::color color, int row, int col, int hght, int wdth);
void close (void);

void put (terminal::color color, int id, int row, int col, const char * val);

void put (terminal::color color, int id, int row, int col, std::string val);
void put (terminal::color color, int id, int row, int col, std::uint8_t val);
void put (terminal::color color, int id, int row, int col, bool val);
void put (terminal::color color, int id, int row, int col, int val);
void put (terminal::color color, int id, int row, int col, double val);
void put (terminal::color color, int id, int row, int col, linalg::ivector val);
void put (terminal::color color, int id, int row, int col, linalg::fvector val);
void put (terminal::color color, int id, int row, int col, linalg::imatrix val);
void put (terminal::color color, int id, int row, int col, linalg::fmatrix val);

void put (terminal::color color, int id, int row, int col, std::vector<std::string> val);
void put (terminal::color color, int id, int row, int col, std::vector<std::uint8_t> val);
void put (terminal::color color, int id, int row, int col, std::vector<bool> val);
void put (terminal::color color, int id, int row, int col, std::vector<int> val);
void put (terminal::color color, int id, int row, int col, std::vector<double> val);
void put (terminal::color color, int id, int row, int col, std::vector<linalg::ivector> val);
void put (terminal::color color, int id, int row, int col, std::vector<linalg::fvector> val);
void put (terminal::color color, int id, int row, int col, std::vector<linalg::imatrix> val);
void put (terminal::color color, int id, int row, int col, std::vector<linalg::fmatrix> val);

template <typename type>
type get (terminal::color color, int id, int row, int col);

template <>
std::string get<std::string> (terminal::color color, int id, int row, int col);

template <>
std::uint8_t get<std::uint8_t> (terminal::color color, int id, int row, int col);

template <>
bool get<bool> (terminal::color color, int id, int row, int col);

template <>
int get<int> (terminal::color color, int id, int row, int col);

template <>
double get<double> (terminal::color color, int id, int row, int col);

template <>
linalg::ivector get<linalg::ivector> (terminal::color color, int id, int row, int col);

template <>
linalg::fvector get<linalg::fvector> (terminal::color color, int id, int row, int col);

template <>
linalg::imatrix get<linalg::imatrix> (terminal::color color, int id, int row, int col);

template <>
linalg::fmatrix get<linalg::fmatrix> (terminal::color color, int id, int row, int col);

template <>
std::vector<std::string> get<std::vector<std::string>> (terminal::color color, int id, int row, int col);

template <>
std::vector<std::uint8_t> get<std::vector<std::uint8_t>> (terminal::color color, int id, int row, int col);

template <>
std::vector<bool> get<std::vector<bool>> (terminal::color color, int id, int row, int col);

template <>
std::vector<int> get<std::vector<int>> (terminal::color color, int id, int row, int col);

template <>
std::vector<double> get<std::vector<double>> (terminal::color color, int id, int row, int col);

template <>
std::vector<linalg::ivector> get<std::vector<linalg::ivector>> (terminal::color color, int id, int row, int col);

template <>
std::vector<linalg::fvector> get<std::vector<linalg::fvector>> (terminal::color color, int id, int row, int col);

template <>
std::vector<linalg::imatrix> get<std::vector<linalg::imatrix>> (terminal::color color, int id, int row, int col);

template <>
std::vector<linalg::fmatrix> get<std::vector<linalg::fmatrix>> (terminal::color color, int id, int row, int col);

}

#endif

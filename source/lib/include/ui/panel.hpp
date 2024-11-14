#ifndef __UI_PANEL_HPP__
#define __UI_PANEL_HPP__

#include <string>

#include <math/linalg.hpp>

#include <ui/terminal.hpp>

namespace panel {

bool fail (void);
bool eof (void);

void init (void);
void deinit (void);

int open (terminal::color color, int row, int col, int hght, int wdth);
void close (void);

void put (terminal::color color, int id, int row, int col, bool val);
void put (terminal::color color, int id, int row, int col, std::string val);
void put (terminal::color color, int id, int row, int col, int val);
void put (terminal::color color, int id, int row, int col, double val);
void put (terminal::color color, int id, int row, int col, linalg::ivector val);
void put (terminal::color color, int id, int row, int col, linalg::fvector val);
void put (terminal::color color, int id, int row, int col, linalg::imatrix val);
void put (terminal::color color, int id, int row, int col, linalg::fmatrix val);
void put (terminal::color color, int id, int row, int col, const char * val);

template <typename type>
type get (terminal::color color, int id, int row, int col);

template <>
bool get<bool> (terminal::color color, int id, int row, int col);

template <>
std::string get<std::string> (terminal::color color, int id, int row, int col);

template <>
int get<int> (terminal::color color, int id, int row, int col);

template <>
double get<double> (terminal::color color, int id, int row, int col);

template <>
linalg::ivector get<linalg::ivector> (
    terminal::color color, int id, int row, int col
);

template <>
linalg::fvector get<linalg::fvector> (
    terminal::color color, int id, int row, int col
);

template <>
linalg::imatrix get<linalg::imatrix> (
    terminal::color color, int id, int row, int col
);

template <>
linalg::fmatrix get<linalg::fmatrix> (
    terminal::color color, int id, int row, int col
);

}

#endif

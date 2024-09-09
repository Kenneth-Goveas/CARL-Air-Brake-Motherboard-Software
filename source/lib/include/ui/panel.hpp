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

int draw (terminal::color clr, int row, int col, int hght, int wdth);
void erase (void);

void print (terminal::color clr, int id, int row, int col, bool val);
void print (terminal::color clr, int id, int row, int col, std::string val);
void print (terminal::color clr, int id, int row, int col, int val);
void print (terminal::color clr, int id, int row, int col, double val);
void print (terminal::color clr, int id, int row, int col, linalg::ivector val);
void print (terminal::color clr, int id, int row, int col, linalg::fvector val);
void print (terminal::color clr, int id, int row, int col, linalg::imatrix val);
void print (terminal::color clr, int id, int row, int col, linalg::fmatrix val);
void print (terminal::color clr, int id, int row, int col, const char * val);

template <typename type>
type scan (terminal::color clr, int id, int row, int col);

template <>
bool scan<bool> (terminal::color clr, int id, int row, int col);

template <>
std::string scan<std::string> (terminal::color clr, int id, int row, int col);

template <>
int scan<int> (terminal::color clr, int id, int row, int col);

template <>
double scan<double> (terminal::color clr, int id, int row, int col);

template <>
linalg::ivector scan<linalg::ivector> (
    terminal::color clr, int id, int row, int col
);

template <>
linalg::fvector scan<linalg::fvector> (
    terminal::color clr, int id, int row, int col
);

template <>
linalg::imatrix scan<linalg::imatrix> (
    terminal::color clr, int id, int row, int col
);

template <>
linalg::fmatrix scan<linalg::fmatrix> (
    terminal::color clr, int id, int row, int col
);

}

#endif

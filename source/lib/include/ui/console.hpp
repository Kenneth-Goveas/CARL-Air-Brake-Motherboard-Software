#ifndef __UI_CONSOLE_HPP__
#define __UI_CONSOLE_HPP__

#include <string>

#include <math/linalg.hpp>

#include <ui/terminal.hpp>

namespace console {

bool fail (void);
bool eof (void);

void put (terminal::color clr, int col, bool val);
void put (terminal::color clr, int col, std::string val);
void put (terminal::color clr, int col, int val);
void put (terminal::color clr, int col, double val);
void put (terminal::color clr, int col, linalg::ivector val);
void put (terminal::color clr, int col, linalg::fvector val);
void put (terminal::color clr, int col, linalg::imatrix val);
void put (terminal::color clr, int col, linalg::fmatrix val);
void put (terminal::color clr, int col, const char * val);

template <typename type>
type get (terminal::color clr, int col);

template <>
bool get<bool> (terminal::color clr, int col);

template <>
std::string get<std::string> (terminal::color clr, int col);

template <>
int get<int> (terminal::color clr, int col);

template <>
double get<double> (terminal::color clr, int col);

template <>
linalg::ivector get<linalg::ivector> (terminal::color clr, int col);

template <>
linalg::fvector get<linalg::fvector> (terminal::color clr, int col);

template <>
linalg::imatrix get<linalg::imatrix> (terminal::color clr, int col);

template <>
linalg::fmatrix get<linalg::fmatrix> (terminal::color clr, int col);

void newline (void);

}

#endif

#ifndef __UI_CONSOLE_HPP__
#define __UI_CONSOLE_HPP__

#include <string>

#include <math/linalg.hpp>

#include <ui/terminal.hpp>

namespace console {

bool fail (void);
bool eof (void);

void newline (void);

void print (terminal::color clr, int col, bool val);
void print (terminal::color clr, int col, std::string val);
void print (terminal::color clr, int col, int val);
void print (terminal::color clr, int col, double val);
void print (terminal::color clr, int col, linalg::ivector val);
void print (terminal::color clr, int col, linalg::fvector val);
void print (terminal::color clr, int col, linalg::imatrix val);
void print (terminal::color clr, int col, linalg::fmatrix val);
void print (terminal::color clr, int col, const char * val);

template <typename type>
type scan (terminal::color clr, int col);

template <>
bool scan<bool> (terminal::color clr, int col);

template <>
std::string scan<std::string> (terminal::color clr, int col);

template <>
int scan<int> (terminal::color clr, int col);

template <>
double scan<double> (terminal::color clr, int col);

template <>
linalg::ivector scan<linalg::ivector> (terminal::color clr, int col);

template <>
linalg::fvector scan<linalg::fvector> (terminal::color clr, int col);

template <>
linalg::imatrix scan<linalg::imatrix> (terminal::color clr, int col);

template <>
linalg::fmatrix scan<linalg::fmatrix> (terminal::color clr, int col);

}

#endif

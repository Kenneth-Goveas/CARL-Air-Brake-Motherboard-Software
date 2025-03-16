#ifndef __DAT_CSVREAD_HPP__
#define __DAT_CSVREAD_HPP__

#include <string>
#include <vector>

namespace csvread {

bool fail (void);
bool eof (void);

void open (std::string path);
void close (void);

std::vector<std::string> get_head (void);
std::vector<double> get_body (void);

}

#endif

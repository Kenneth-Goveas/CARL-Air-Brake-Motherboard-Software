#ifndef __DAT_CSVWRITE_HPP__
#define __DAT_CSVWRITE_HPP__

#include <string>
#include <vector>

namespace csvwrite {

bool fail (void);

void open (std::string path);
void close (void);

void put_head (std::vector<std::string> head);
void put_body (std::vector<double> body);

}

#endif

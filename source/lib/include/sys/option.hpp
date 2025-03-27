#ifndef __SYS_OPTION_HPP__
#define __SYS_OPTION_HPP__

#include <string>
#include <vector>

namespace option {

bool fail (void);

void init (std::vector<std::string> args, std::vector<std::string> names);

bool get (std::string name);

}

#endif

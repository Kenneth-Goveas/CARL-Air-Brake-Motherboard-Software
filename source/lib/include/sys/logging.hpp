#ifndef __SYS_LOGGING_HPP__
#define __SYS_LOGGING_HPP__

#include <string>

namespace logging {

bool fail (void);

void init (std::string unit, std::string path);
void deinit (void);

template <typename ... types>
void err (std::string mod, types ... args);

template <typename ... types>
void wrn (std::string mod, types ... args);

template <typename ... types>
void inf (std::string mod, types ... args);

}

#include <sys/logging.tpp>

#endif

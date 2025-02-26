#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>

#include <sys/file.h>

namespace logging :: intern {

extern bool fail;

extern std::string unit;
extern std::ofstream file;
extern int desc;

void head (std::string lev, std::string mod);
void body (void);

template <typename ... types>
void body (buffer buf, types ... args);

template <typename ... types>
void msg (std::string lev, std::string mod, types ... args);

}

namespace logging {

template <typename ... types>
void err (std::string mod, types ... args) {
    intern::msg("err", mod, args ...);
}

template <typename ... types>
void wrn (std::string mod, types ... args) {
    intern::msg("wrn", mod, args ...);
}

template <typename ... types>
void inf (std::string mod, types ... args) {
    intern::msg("inf", mod, args ...);
}

}

namespace logging :: intern {

template <typename ... types>
void body (buffer buf, types ... args) {
    file << std::string(buf);
    file.clear();
    body(args ...);
}

template <typename ... types>
void msg (std::string lev, std::string mod, types ... args) {
    int ret;

    ret = flock(desc, LOCK_EX);
    if (ret < 0) {
        return;
    }

    head(lev, mod);
    body(args ...);

    flock(desc, LOCK_UN);
}

}

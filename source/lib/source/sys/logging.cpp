#include <iomanip>
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/logging.hpp>

namespace logging :: intern {

bool fail;

std::string unit;
std::ofstream file;
int desc;

void head (std::string lev, std::string mod);
void body (void);

template <typename type, typename ... types>
void body (type arg, types ... args);

template <typename ... types>
void msg (std::string lev, std::string mod, types ... args);

}

namespace logging {

bool fail (void) {
    return intern::fail;
}

void init (std::string unit, std::string path) {
    intern::unit = unit;

    intern::file.open(path, std::ios::app);
    if (intern::file.fail()) {
        intern::file.clear();
        intern::fail = true;
        return;
    }

    intern::desc = open(path.c_str(), O_RDONLY);
    if (intern::desc < 0) {
        intern::file.close();
        intern::file.clear();
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

void deinit (void) {
    intern::file.close();
    intern::file.clear();
    close(intern::desc);
}

}

namespace logging :: intern {

void head (std::string lev, std::string mod) {
    auto tstamp = std::chrono::steady_clock::now().time_since_epoch();
    int hour, min, sec, msec, usec;

    hour = std::chrono::duration_cast<std::chrono::hours>(tstamp).count();
    min  = std::chrono::duration_cast<std::chrono::minutes>(tstamp).count()
           - 60 * hour;
    sec  = std::chrono::duration_cast<std::chrono::seconds>(tstamp).count()
           - 3600 * hour - 60 * min;
    msec = std::chrono::duration_cast<std::chrono::milliseconds>(tstamp).count()
           - 3600000 * hour - 60000 * min - 1000 * sec;
    usec = std::chrono::duration_cast<std::chrono::microseconds>(tstamp).count()
           - 3600000000 * hour - 60000000 * min - 1000000 * sec - 1000 * msec;

    file << "["
         << std::setfill('0') << std::setw(2) << hour
         << ":"
         << std::setfill('0') << std::setw(2) << min
         << ":"
         << std::setfill('0') << std::setw(2) << sec
         << "."
         << std::setfill('0') << std::setw(3) << msec
         << ","
         << std::setfill('0') << std::setw(3) << usec
         << "] "
         << "<" << lev << "> " << unit << "/" << mod << ": ";

    file.clear();
}

void body (void) {
    file << std::endl;
    file.clear();
}

}

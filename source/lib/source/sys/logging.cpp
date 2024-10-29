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
    std::chrono::steady_clock::time_point tpnt;
    std::chrono::steady_clock::duration tdur;
    std::chrono::hours hour;
    std::chrono::minutes min;
    std::chrono::seconds sec;
    std::chrono::milliseconds msec;
    std::chrono::microseconds usec;

    tpnt = std::chrono::steady_clock::now();
    tdur = tpnt.time_since_epoch();

    hour = std::chrono::duration_cast<std::chrono::hours>(tdur);
    min = std::chrono::duration_cast<std::chrono::minutes>(tdur);
    sec = std::chrono::duration_cast<std::chrono::seconds>(tdur);
    msec = std::chrono::duration_cast<std::chrono::milliseconds>(tdur);
    usec = std::chrono::duration_cast<std::chrono::microseconds>(tdur);

    usec -= msec;
    msec -= sec;
    sec -= min;
    min -= hour;

    file << "["
         << std::setfill('0') << std::setw(2) << hour.count()
         << ":"
         << std::setfill('0') << std::setw(2) << min.count()
         << ":"
         << std::setfill('0') << std::setw(2) << sec.count()
         << "."
         << std::setfill('0') << std::setw(3) << msec.count()
         << ","
         << std::setfill('0') << std::setw(3) << usec.count()
         << "] "
         << "<" << lev << "> " << unit << "/" << mod << ": ";

    file.clear();
}

void body (void) {
    file << std::endl;
    file.clear();
}

}

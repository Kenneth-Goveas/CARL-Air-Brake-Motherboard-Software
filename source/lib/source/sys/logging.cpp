#include <cstdint>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <math/linalg.hpp>
#include <sys/logging.hpp>

namespace logging {

bool instance::init = false;
int instance::desc;
std::ofstream instance::file;
std::string instance::app;
stream instance::strm(instance::file);

bool instance::start (const std::string & path, const std::string & app) {
    if (instance::init) {
        return false;
    }

    instance::file.clear();
    instance::file.open(path, std::ios::app);
    if (instance::file.fail()) {
        return false;
    }

    instance::desc = open(path.c_str(), O_RDONLY);
    if (instance::desc < 0) {
        instance::file.close();
        return false;
    }

    instance::app = app;

    instance::init = true;

    return true;
}

void instance::stop (void) {
    if (!instance::init) {
        return;
    }

    instance::file.close();
    close(instance::desc);

    instance::init = false;
}

instance::instance (const std::string & lib) : lib(lib) {}

void instance::head (const std::string & lev) {
    auto tstamp = std::chrono::steady_clock::now().time_since_epoch();
    auto hour = std::chrono::duration_cast<std::chrono::hours>(tstamp);
    auto min = std::chrono::duration_cast<std::chrono::minutes>(tstamp);
    auto sec = std::chrono::duration_cast<std::chrono::seconds>(tstamp);
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(tstamp);
    auto usec = std::chrono::duration_cast<std::chrono::microseconds>(tstamp);

    usec -= msec;
    msec -= sec;
    sec -= min;
    min -= hour;

    auto fill = instance::file.fill();
    auto width = instance::file.width();

    instance::file << "["
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
                   << "<" << lev << "> " << instance::app << "/" << this->lib << ": ";

    instance::file.fill(fill);
    instance::file.width(width);
}

void instance::body (void) {
    instance::file << std::endl;
}

stream::stream (std::ofstream & file) : file(&file) {}

stream & stream::operator << (const char * val) {
    *(this->file) << val;
    return *this;
}

stream & stream::operator << (const std::string & val) {
    *(this->file) << "“" << val << "”";
    return *this;
}

stream & stream::operator << (std::uint8_t val) {
    auto flags = this->file->flags();
    auto fill = this->file->fill();
    auto width = this->file->width();

    *(this->file) << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << int(val);

    this->file->flags(flags);
    this->file->fill(fill);
    this->file->width(width);

    return *this;
}

stream & stream::operator << (bool val) {
    *(this->file) << (val ? "True" : "False");
    return *this;
}

stream & stream::operator << (int val) {
    *(this->file) << val;
    return *this;
}

stream & stream::operator << (double val) {
    auto flags = this->file->flags();
    auto precision = this->file->precision();

    *(this->file) << std::scientific << std::setprecision(4) << val;

    this->file->flags(flags);
    this->file->precision(precision);

    return *this;
}

stream & stream::operator << (const linalg::ivector & val) {
    *this << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        *this << val[i] << (i < linalg::dim(val) - 1 ? ", " : "");
    }
    *this << "]";
    return *this;
}

stream & stream::operator << (const linalg::fvector & val) {
    *this << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        *this << val[i] << (i < linalg::dim(val) - 1 ? ", " : "");
    }
    *this << "]";
    return *this;
}

stream & stream::operator << (const linalg::imatrix & val) {
    *this << "[";
    for (int i = 0; i < linalg::rows(val); i++) {
        *this << "[";
        for (int j = 0; j < linalg::cols(val); j++) {
            *this << val[i][j] << (j < linalg::cols(val) - 1 ? ", " : "");
        }
        *this << "]" << (i < linalg::rows(val) - 1 ? ", " : "");
    }
    *this << "]";
    return *this;
}

stream & stream::operator << (const linalg::fmatrix & val) {
    *this << "[";
    for (int i = 0; i < linalg::rows(val); i++) {
        *this << "[";
        for (int j = 0; j < linalg::cols(val); j++) {
            *this << val[i][j] << (j < linalg::cols(val) - 1 ? ", " : "");
        }
        *this << "]" << (i < linalg::rows(val) - 1 ? ", " : "");
    }
    *this << "]";
    return *this;
}

stream & stream::operator << (const std::vector<std::string> & val) {
    *this << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        *this << *itr << (itr < val.end() - 1 ? ", " : "");
    }
    *this << "}";
    return *this;
}

stream & stream::operator << (const std::vector<std::uint8_t> & val) {
    *this << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        *this << *itr << (itr < val.end() - 1 ? ", " : "");
    }
    *this << "}";
    return *this;
}

stream & stream::operator << (const std::vector<bool> & val) {
    *this << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        *this << bool(*itr) << (itr < val.end() - 1 ? ", " : "");
    }
    *this << "}";
    return *this;
}

stream & stream::operator << (const std::vector<int> & val) {
    *this << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        *this << *itr << (itr < val.end() - 1 ? ", " : "");
    }
    *this << "}";
    return *this;
}

stream & stream::operator << (const std::vector<double> & val) {
    *this << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        *this << *itr << (itr < val.end() - 1 ? ", " : "");
    }
    *this << "}";
    return *this;
}

stream & stream::operator << (const std::vector<linalg::ivector> & val) {
    *this << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        *this << *itr << (itr < val.end() - 1 ? ", " : "");
    }
    *this << "}";
    return *this;
}

stream & stream::operator << (const std::vector<linalg::fvector> & val) {
    *this << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        *this << *itr << (itr < val.end() - 1 ? ", " : "");
    }
    *this << "}";
    return *this;
}

stream & stream::operator << (const std::vector<linalg::imatrix> & val) {
    *this << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        *this << *itr << (itr < val.end() - 1 ? ", " : "");
    }
    *this << "}";
    return *this;
}

stream & stream::operator << (const std::vector<linalg::fmatrix> & val) {
    *this << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        *this << *itr << (itr < val.end() - 1 ? ", " : "");
    }
    *this << "}";
    return *this;
}

}

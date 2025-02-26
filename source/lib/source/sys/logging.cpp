#include <cstdint>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include <vector>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <math/linalg.hpp>

#include <sys/logging.hpp>

namespace logging :: intern {

bool fail;

std::string unit;
std::ofstream file;
int desc;

void head (std::string lev, std::string mod);
void body (void);

template <typename ... types>
void body (buffer buf, types ... args);

template <typename ... types>
void msg (std::string lev, std::string mod, types ... args);

}

namespace logging {

buffer :: buffer (const char * val) {
    std::ostringstream ostr;
    ostr << val;
    this->text = ostr.str();
}

buffer :: buffer (std::string val) {
    std::ostringstream ostr;
    ostr << "“" << val << "”";
    this->text = ostr.str();
}

buffer :: buffer (std::uint8_t val) {
    std::ostringstream ostr;
    ostr << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << int(val);
    this->text = ostr.str();
}

buffer :: buffer (bool val) {
    std::ostringstream ostr;
    if (val) {
        ostr << "True";
    } else {
        ostr << "False";
    }
    this->text = ostr.str();
}

buffer :: buffer (int val) {
    std::ostringstream ostr;
    ostr << val;
    this->text = ostr.str();
}

buffer :: buffer (double val) {
    std::ostringstream ostr;
    ostr << std::scientific << std::setprecision(4) << val;
    this->text = ostr.str();
}

buffer :: buffer (linalg::ivector val) {
    std::ostringstream ostr;
    ostr << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        ostr << val[i];
        if (i < linalg::dim(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    this->text = ostr.str();
}

buffer :: buffer (linalg::fvector val) {
    std::ostringstream ostr;
    ostr << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        ostr << std::scientific << std::setprecision(4) << val[i];
        if (i < linalg::dim(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    this->text = ostr.str();
}

buffer :: buffer (linalg::imatrix val) {
    std::ostringstream ostr;
    ostr << "[";
    for (int i = 0; i < linalg::rows(val); i++) {
        ostr << "[";
        for (int j = 0; j < linalg::cols(val); j++) {
            ostr << val[i][j];
            if (j < linalg::cols(val) - 1) {
                ostr << ", ";
            }
        }
        ostr << "]";
        if (i < linalg::rows(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    this->text = ostr.str();
}

buffer :: buffer (linalg::fmatrix val) {
    std::ostringstream ostr;
    ostr << "[";
    for (int i = 0; i < linalg::rows(val); i++) {
        ostr << "[";
        for (int j = 0; j < linalg::cols(val); j++) {
            ostr << std::scientific << std::setprecision(4) << val[i][j];
            if (j < linalg::cols(val) - 1) {
                ostr << ", ";
            }
        }
        ostr << "]";
        if (i < linalg::rows(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    this->text = ostr.str();
}

buffer :: buffer (std::vector<std::string> val) {
    std::ostringstream ostr;
    ostr << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        ostr << "“" << *itr << "”";
        if (itr < val.end() - 1) {
            ostr << ", ";
        }
    }
    ostr << "}";
    this->text = ostr.str();
}

buffer :: buffer (std::vector<std::uint8_t> val) {
    std::ostringstream ostr;
    ostr << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        ostr << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << int(*itr);
        if (itr < val.end() - 1) {
            ostr << ", ";
        }
    }
    ostr << "}";
    this->text = ostr.str();
}

buffer :: buffer (std::vector<bool> val) {
    std::ostringstream ostr;
    ostr << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        if (*itr) {
            ostr << "True";
        } else {
            ostr << "False";
        }
        if (itr < val.end() - 1) {
            ostr << ", ";
        }
    }
    ostr << "}";
    this->text = ostr.str();
}

buffer :: buffer (std::vector<int> val) {
    std::ostringstream ostr;
    ostr << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        ostr << *itr;
        if (itr < val.end() - 1) {
            ostr << ", ";
        }
    }
    ostr << "}";
    this->text = ostr.str();
}

buffer :: buffer (std::vector<double> val) {
    std::ostringstream ostr;
    ostr << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        ostr << std::scientific << std::setprecision(4) << *itr;
        if (itr < val.end() - 1) {
            ostr << ", ";
        }
    }
    ostr << "}";
    this->text = ostr.str();
}

buffer :: buffer (std::vector<linalg::ivector> val) {
    std::ostringstream ostr;
    ostr << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        ostr << "[";
        for (int i = 0; i < linalg::dim(*itr); i++) {
            ostr << (*itr)[i];
            if (i < linalg::dim(*itr) - 1) {
                ostr << ", ";
            }
        }
        ostr << "]";
        if (itr < val.end() - 1) {
            ostr << ", ";
        }
    }
    ostr << "}";
    this->text = ostr.str();
}

buffer :: buffer (std::vector<linalg::fvector> val) {
    std::ostringstream ostr;
    ostr << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        ostr << "[";
        for (int i = 0; i < linalg::dim(*itr); i++) {
            ostr << std::scientific << std::setprecision(4) << (*itr)[i];
            if (i < linalg::dim(*itr) - 1) {
                ostr << ", ";
            }
        }
        ostr << "]";
        if (itr < val.end() - 1) {
            ostr << ", ";
        }
    }
    ostr << "}";
    this->text = ostr.str();
}

buffer :: buffer (std::vector<linalg::imatrix> val) {
    std::ostringstream ostr;
    ostr << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        ostr << "[";
        for (int i = 0; i < linalg::rows(*itr); i++) {
            ostr << "[";
            for (int j = 0; j < linalg::cols(*itr); j++) {
                ostr << (*itr)[i][j];
                if (j < linalg::cols(*itr) - 1) {
                    ostr << ", ";
                }
            }
            ostr << "]";
            if (i < linalg::rows(*itr) - 1) {
                ostr << ", ";
            }
        }
        ostr << "]";
        if (itr < val.end() - 1) {
            ostr << ", ";
        }
    }
    ostr << "}";
    this->text = ostr.str();
}

buffer :: buffer (std::vector<linalg::fmatrix> val) {
    std::ostringstream ostr;
    ostr << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        ostr << "[";
        for (int i = 0; i < linalg::rows(*itr); i++) {
            ostr << "[";
            for (int j = 0; j < linalg::cols(*itr); j++) {
                ostr << std::scientific << std::setprecision(4) << (*itr)[i][j];
                if (j < linalg::cols(*itr) - 1) {
                    ostr << ", ";
                }
            }
            ostr << "]";
            if (i < linalg::rows(*itr) - 1) {
                ostr << ", ";
            }
        }
        ostr << "]";
        if (itr < val.end() - 1) {
            ostr << ", ";
        }
    }
    ostr << "}";
    this->text = ostr.str();
}

buffer :: operator std::string (void) const {
    return this->text;
}

bool fail (void) {
    return intern::fail;
}

void init (std::string path, std::string unit) {
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
    auto tstmp = std::chrono::steady_clock::now().time_since_epoch();
    auto hour = std::chrono::duration_cast<std::chrono::hours>(tstmp);
    auto min = std::chrono::duration_cast<std::chrono::minutes>(tstmp);
    auto sec = std::chrono::duration_cast<std::chrono::seconds>(tstmp);
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(tstmp);
    auto usec = std::chrono::duration_cast<std::chrono::microseconds>(tstmp);

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

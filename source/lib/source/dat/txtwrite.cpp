#include <cstdint>
#include <cstring>
#include <cerrno>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <math/linalg.hpp>
#include <sys/logging.hpp>
#include <dat/txtwrite.hpp>

namespace txtwrite {

int tracker::count = 0;

int tracker::instantiate (void) {
    tracker::count++;
    return tracker::count;
}

instance::instance (const std::string & path) : logging("txtwrite"), init(false), id(instance::instantiate()) {
    this->logging.inf("Initializing instance #", this->id, ": Path: ", path);

    this->logging.inf("Opening TXT file #", this->id);
    this->file.clear();
    this->file.open(path);
    if (this->file.fail()) {
        this->logging.err("Failed to open TXT file #", this->id, " (", std::strerror(errno), ")");
        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    this->init = true;
}

instance::~instance (void) {
    if (this->init) {
        this->logging.inf("Closing TXT file #", this->id);
        this->file.clear();
        this->file.close();
        if (this->file.fail()) {
            this->logging.err("Failed to close TXT file #", this->id, " (", std::strerror(errno), ")");
        }
    }
}

void instance::put (const char * val) {
    this->logging.inf("Writing data to TXT file #", this->id, ": Type: Characters, Value: ", std::string(val));

    if (!this->init) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    this->file.clear();
    this->file << val;

    if (this->file.fail()) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_put::fail;
    }
}

void instance::put (const std::string & val) {
    this->logging.inf("Writing data to TXT file #", this->id, ": Type: String, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    this->file.clear();
    this->file << "“" << val << "”";

    if (this->file.fail()) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_put::fail;
    }
}

void instance::put (std::uint8_t val) {
    this->logging.inf("Writing data to TXT file #", this->id, ": Type: Byte, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = this->file.flags();
    auto fill = this->file.fill();
    auto width = this->file.width();

    this->file.clear();
    this->file << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << int(val);

    this->file.flags(flags);
    this->file.fill(fill);
    this->file.width(width);

    if (this->file.fail()) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_put::fail;
    }
}

void instance::put (bool val) {
    this->logging.inf("Writing data to TXT file #", this->id, ": Type: Boolean, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    this->file.clear();
    this->file << (val ? "Yes" : "No");

    if (this->file.fail()) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_put::fail;
    }
}

void instance::put (int val) {
    this->logging.inf("Writing data to TXT file #", this->id, ": Type: Integer scalar, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = this->file.flags();

    this->file.clear();
    this->file << std::showpos << val;

    this->file.flags(flags);

    if (this->file.fail()) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_put::fail;
    }
}

void instance::put (double val) {
    this->logging.inf("Writing data to TXT file #", this->id, ": Type: Real scalar, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = this->file.flags();
    auto precision = this->file.precision();

    this->file.clear();
    this->file << std::showpos << std::scientific << std::setprecision(10) << val;

    this->file.flags(flags);
    this->file.precision(precision);

    if (this->file.fail()) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_put::fail;
    }
}

void instance::put (const linalg::ivector & val) {
    this->logging.inf("Writing data to TXT file #", this->id, ": Type: Integer vector, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = this->file.flags();

    this->file.clear();
    this->file << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        this->file << std::showpos << val[i] << (i < linalg::dim(val) - 1 ? ", " : "");
    }
    this->file << "]";

    this->file.flags(flags);

    if (this->file.fail()) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_put::fail;
    }
}

void instance::put (const linalg::fvector & val) {
    this->logging.inf("Writing data to TXT file #", this->id, ": Type: Real vector, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = this->file.flags();
    auto precision = this->file.precision();

    this->file.clear();
    this->file << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        this->file << std::showpos << std::scientific << std::setprecision(10) << val[i] << (i < linalg::dim(val) - 1 ? ", " : "");
    }
    this->file << "]";

    this->file.flags(flags);
    this->file.precision(precision);

    if (this->file.fail()) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_put::fail;
    }
}

void instance::put (const linalg::imatrix & val) {
    this->logging.inf("Writing data to TXT file #", this->id, ": Type: Integer matrix, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = this->file.flags();

    this->file.clear();
    this->file << "[";
    for (int i = 0; i < linalg::rows(val); i++) {
        this->file << "[";
        for (int j = 0; j < linalg::cols(val); j++) {
            this->file << std::showpos << val[i][j] << (j < linalg::cols(val) - 1 ? ", " : "");
        }
        this->file << "]" << (i < linalg::rows(val) - 1 ? ", " : "");
    }
    this->file << "]";

    this->file.flags(flags);

    if (this->file.fail()) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_put::fail;
    }
}

void instance::put (const linalg::fmatrix & val) {
    this->logging.inf("Writing data to TXT file #", this->id, ": Type: Real matrix, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = this->file.flags();
    auto precision = this->file.precision();

    this->file.clear();
    this->file << "[";
    for (int i = 0; i < linalg::rows(val); i++) {
        this->file << "[";
        for (int j = 0; j < linalg::cols(val); j++) {
            this->file << std::showpos << std::scientific << std::setprecision(10) << val[i][j] << (j < linalg::cols(val) - 1 ? ", " : "");
        }
        this->file << "]" << (i < linalg::rows(val) - 1 ? ", " : "");
    }
    this->file << "]";

    this->file.flags(flags);
    this->file.precision(precision);

    if (this->file.fail()) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_put::fail;
    }
}

void instance::put (const std::vector<std::string> & val) {
    this->logging.inf("Writing data to TXT file #", this->id, ": Type: String array, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    this->file.clear();
    this->file << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        this->file << "“" << *itr << "”"
                   << (itr < val.end() - 1 ? ", " : "");
    }
    this->file << "}";

    if (this->file.fail()) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_put::fail;
    }
}

void instance::put (const std::vector<std::uint8_t> & val) {
    this->logging.inf("Writing data to TXT file #", this->id, ": Type: Byte array, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = this->file.flags();
    auto fill = this->file.fill();
    auto width = this->file.width();

    this->file.clear();
    this->file << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        this->file << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << int(*itr)
                   << (itr < val.end() - 1 ? ", " : "");
    }
    this->file << "}";

    this->file.flags(flags);
    this->file.fill(fill);
    this->file.width(width);

    if (this->file.fail()) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_put::fail;
    }
}

void instance::put (const std::vector<bool> & val) {
    this->logging.inf("Writing data to TXT file #", this->id, ": Type: Boolean array, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    this->file.clear();
    this->file << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        this->file << (*itr ? "Yes" : "No")
                   << (itr < val.end() - 1 ? ", " : "");
    }
    this->file << "}";

    if (this->file.fail()) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_put::fail;
    }
}

void instance::put (const std::vector<int> & val) {
    this->logging.inf("Writing data to TXT file #", this->id, ": Type: Integer scalar array, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = this->file.flags();

    this->file.clear();
    this->file << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        this->file << std::showpos << *itr
                   << (itr < val.end() - 1 ? ", " : "");
    }
    this->file << "}";

    this->file.flags(flags);

    if (this->file.fail()) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_put::fail;
    }
}

void instance::put (const std::vector<double> & val) {
    this->logging.inf("Writing data to TXT file #", this->id, ": Type: Real scalar array, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = this->file.flags();
    auto precision = this->file.precision();

    this->file.clear();
    this->file << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        this->file << std::showpos << std::scientific << std::setprecision(10) << *itr
                   << (itr < val.end() - 1 ? ", " : "");
    }
    this->file << "}";

    this->file.flags(flags);
    this->file.precision(precision);

    if (this->file.fail()) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_put::fail;
    }
}

void instance::put (const std::vector<linalg::ivector> & val) {
    this->logging.inf("Writing data to TXT file #", this->id, ": Type: Integer vector array, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = this->file.flags();

    this->file.clear();
    this->file << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        this->file << "[";
        for (int i = 0; i < linalg::dim(*itr); i++) {
            this->file << std::showpos << (*itr)[i]
                       << (i < linalg::dim(*itr) - 1 ? ", " : "");
        }
        this->file << "]" << (itr < val.end() - 1 ? ", " : "");
    }
    this->file << "}";

    this->file.flags(flags);

    if (this->file.fail()) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_put::fail;
    }
}

void instance::put (const std::vector<linalg::fvector> & val) {
    this->logging.inf("Writing data to TXT file #", this->id, ": Type: Real vector array, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = this->file.flags();
    auto precision = this->file.precision();

    this->file.clear();
    this->file << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        this->file << "[";
        for (int i = 0; i < linalg::dim(*itr); i++) {
            this->file << std::showpos << std::scientific << std::setprecision(10) << (*itr)[i]
                       << (i < linalg::dim(*itr) - 1 ? ", " : "");
        }
        this->file << "]" << (itr < val.end() - 1 ? ", " : "");
    }
    this->file << "}";

    this->file.flags(flags);
    this->file.precision(precision);

    if (this->file.fail()) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_put::fail;
    }
}

void instance::put (const std::vector<linalg::imatrix> & val) {
    this->logging.inf("Writing data to TXT file #", this->id, ": Type: Integer matrix array, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = this->file.flags();

    this->file.clear();
    this->file << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        this->file << "[";
        for (int i = 0; i < linalg::rows(*itr); i++) {
            this->file << "[";
            for (int j = 0; j < linalg::cols(*itr); j++) {
                this->file << std::showpos << (*itr)[i][j]
                           << (j < linalg::cols(*itr) - 1 ? ", " : "");
            }
            this->file << "]" << (i < linalg::rows(*itr) - 1 ? ", " : "");
        }
        this->file << "]" << (itr < val.end() - 1 ? ", " : "");
    }
    this->file << "}";

    this->file.flags(flags);

    if (this->file.fail()) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_put::fail;
    }
}

void instance::put (const std::vector<linalg::fmatrix> & val) {
    this->logging.inf("Writing data to TXT file #", this->id, ": Type: Real matrix array, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = this->file.flags();
    auto precision = this->file.precision();

    this->file.clear();
    this->file << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        this->file << "[";
        for (int i = 0; i < linalg::rows(*itr); i++) {
            this->file << "[";
            for (int j = 0; j < linalg::cols(*itr); j++) {
                this->file << std::showpos << std::scientific << std::setprecision(10) << (*itr)[i][j]
                           << (j < linalg::cols(*itr) - 1 ? ", " : "");
            }
            this->file << "]" << (i < linalg::rows(*itr) - 1 ? ", " : "");
        }
        this->file << "]" << (itr < val.end() - 1 ? ", " : "");
    }
    this->file << "}";

    this->file.flags(flags);
    this->file.precision(precision);

    if (this->file.fail()) {
        this->logging.err("Failed to write data to TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_put::fail;
    }
}

void instance::newline (void) {
    this->logging.inf("Entering new line in TXT file #", this->id);

    if (!this->init) {
        this->logging.err("Failed to enter new line in TXT file #", this->id, " (Instance not initialized)");
        throw except_newline::fail;
    }

    this->file.clear();
    this->file << std::endl;

    if (this->file.fail()) {
        this->logging.err("Failed to enter new line in TXT file #", this->id, " (", std::strerror(errno), ")");
        throw except_newline::fail;
    }
}

}

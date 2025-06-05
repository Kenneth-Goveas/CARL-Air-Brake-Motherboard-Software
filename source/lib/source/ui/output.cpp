#include <cstdint>

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <math/linalg.hpp>
#include <sys/logging.hpp>
#include <ui/output.hpp>

namespace output {

bool tracker::exist = false;

bool tracker::instantiate (void) {
    if (tracker::exist) {
        return false;
    } else {
        tracker::exist = true;
        return true;
    }
}

instance::instance (void) : logging("output"), init(false) {
    this->logging.inf("Initializing instance");

    if (!instance::instantiate()) {
        this->logging.err("Failed to initialize instance (Instance already exists)");
        throw except_ctor::fail;
    }

    this->init = true;
}

void instance::put (const char * val) {
    this->logging.inf("Printing output: Type: Characters, Value: ", std::string(val));

    if (!this->init) {
        this->logging.err("Failed to print output (Instance not initialized)");
        throw except_put::fail;
    }

    std::cout << val;
    std::cout.flush();
}

void instance::put (const std::string & val) {
    this->logging.inf("Printing output: Type: String, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to print output (Instance not initialized)");
        throw except_put::fail;
    }

    std::cout << "“" << val << "”";
    std::cout.flush();
}

void instance::put (std::uint8_t val) {
    this->logging.inf("Printing output: Type: Byte, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to print output (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = std::cout.flags();
    auto fill = std::cout.fill();
    auto width = std::cout.width();

    std::cout << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << int(val);
    std::cout.flush();

    std::cout.flags(flags);
    std::cout.fill(fill);
    std::cout.width(width);
}

void instance::put (bool val) {
    this->logging.inf("Printing output: Type: Boolean, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to print output (Instance not initialized)");
        throw except_put::fail;
    }

    std::cout << (val ? "Yes" : "No");
    std::cout.flush();
}

void instance::put (int val) {
    this->logging.inf("Printing output: Type: Integer scalar, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to print output (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = std::cout.flags();
    auto fill = std::cout.fill();
    auto width = std::cout.width();

    std::cout << std::showpos << std::setfill(' ') << std::setw(6) << val;
    std::cout.flush();

    std::cout.flags(flags);
    std::cout.fill(fill);
    std::cout.width(width);
}

void instance::put (double val) {
    this->logging.inf("Printing output: Type: Real scalar, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to print output (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = std::cout.flags();
    auto precision = std::cout.precision();

    std::cout << std::showpos << std::scientific << std::setprecision(4) << val;
    std::cout.flush();

    std::cout.flags(flags);
    std::cout.precision(precision);
}

void instance::put (const linalg::ivector & val) {
    this->logging.inf("Printing output: Type: Integer vector, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to print output (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = std::cout.flags();
    auto fill = std::cout.fill();
    auto width = std::cout.width();

    std::cout << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        std::cout << std::showpos << std::setfill(' ') << std::setw(6) << val[i] << (i < linalg::dim(val) - 1 ? ", " : "");
    }
    std::cout << "]";
    std::cout.flush();

    std::cout.flags(flags);
    std::cout.fill(fill);
    std::cout.width(width);
}

void instance::put (const linalg::fvector & val) {
    this->logging.inf("Printing output: Type: Real vector, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to print output (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = std::cout.flags();
    auto precision = std::cout.precision();

    std::cout << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        std::cout << std::showpos << std::scientific << std::setprecision(4) << val[i] << (i < linalg::dim(val) - 1 ? ", " : "");
    }
    std::cout << "]";
    std::cout.flush();

    std::cout.flags(flags);
    std::cout.precision(precision);
}

void instance::put (const linalg::imatrix & val) {
    this->logging.inf("Printing output: Type: Integer matrix, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to print output (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = std::cout.flags();
    auto fill = std::cout.fill();
    auto width = std::cout.width();

    std::cout << "[";
    for (int i = 0; i < linalg::rows(val); i++) {
        std::cout << "[";
        for (int j = 0; j < linalg::cols(val); j++) {
            std::cout << std::showpos << std::setfill(' ') << std::setw(6) << val[i][j] << (j < linalg::cols(val) - 1 ? ", " : "");
        }
        std::cout << "]" << (i < linalg::rows(val) - 1 ? ", " : "");
    }
    std::cout << "]";
    std::cout.flush();

    std::cout.flags(flags);
    std::cout.fill(fill);
    std::cout.width(width);
}

void instance::put (const linalg::fmatrix & val) {
    this->logging.inf("Printing output: Type: Real matrix, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to print output (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = std::cout.flags();
    auto precision = std::cout.precision();

    std::cout << "[";
    for (int i = 0; i < linalg::rows(val); i++) {
        std::cout << "[";
        for (int j = 0; j < linalg::cols(val); j++) {
            std::cout << std::showpos << std::scientific << std::setprecision(4) << val[i][j] << (j < linalg::cols(val) - 1 ? ", " : "");
        }
        std::cout << "]" << (i < linalg::rows(val) - 1 ? ", " : "");
    }
    std::cout << "]";
    std::cout.flush();

    std::cout.flags(flags);
    std::cout.precision(precision);
}

void instance::put (const std::vector<std::string> & val) {
    this->logging.inf("Printing output: Type: String array, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to print output (Instance not initialized)");
        throw except_put::fail;
    }

    std::cout << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        std::cout << "“" << *itr << "”"
                  << (itr < val.end() - 1 ? ", " : "");
    }
    std::cout << "}";
    std::cout.flush();
}

void instance::put (const std::vector<std::uint8_t> & val) {
    this->logging.inf("Printing output: Type: Byte array, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to print output (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = std::cout.flags();
    auto fill = std::cout.fill();
    auto width = std::cout.width();

    std::cout << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        std::cout << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << int(*itr)
                  << (itr < val.end() - 1 ? ", " : "");
    }
    std::cout << "}";
    std::cout.flush();

    std::cout.flags(flags);
    std::cout.fill(fill);
    std::cout.width(width);
}

void instance::put (const std::vector<bool> & val) {
    this->logging.inf("Printing output: Type: Boolean array, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to print output (Instance not initialized)");
        throw except_put::fail;
    }

    std::cout << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        std::cout << (*itr ? "Yes" : "No")
                  << (itr < val.end() - 1 ? ", " : "");
    }
    std::cout << "}";
    std::cout.flush();
}

void instance::put (const std::vector<int> & val) {
    this->logging.inf("Printing output: Type: Integer scalar array, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to print output (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = std::cout.flags();
    auto fill = std::cout.fill();
    auto width = std::cout.width();

    std::cout << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        std::cout << std::showpos << std::setfill(' ') << std::setw(6) << *itr
                  << (itr < val.end() - 1 ? ", " : "");
    }
    std::cout << "}";
    std::cout.flush();

    std::cout.flags(flags);
    std::cout.fill(fill);
    std::cout.width(width);
}

void instance::put (const std::vector<double> & val) {
    this->logging.inf("Printing output: Type: Real scalar array, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to print output (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = std::cout.flags();
    auto precision = std::cout.precision();

    std::cout << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        std::cout << std::showpos << std::scientific << std::setprecision(4) << *itr
                  << (itr < val.end() - 1 ? ", " : "");
    }
    std::cout << "}";
    std::cout.flush();

    std::cout.flags(flags);
    std::cout.precision(precision);
}

void instance::put (const std::vector<linalg::ivector> & val) {
    this->logging.inf("Printing output: Type: Integer vector array, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to print output (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = std::cout.flags();
    auto fill = std::cout.fill();
    auto width = std::cout.width();

    std::cout << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        std::cout << "[";
        for (int i = 0; i < linalg::dim(*itr); i++) {
            std::cout << std::showpos << std::setfill(' ') << std::setw(6) << (*itr)[i]
                      << (i < linalg::dim(*itr) - 1 ? ", " : "");
        }
        std::cout << "]" << (itr < val.end() - 1 ? ", " : "");
    }
    std::cout << "}";
    std::cout.flush();

    std::cout.flags(flags);
    std::cout.fill(fill);
    std::cout.width(width);
}

void instance::put (const std::vector<linalg::fvector> & val) {
    this->logging.inf("Printing output: Type: Real vector array, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to print output (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = std::cout.flags();
    auto precision = std::cout.precision();

    std::cout << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        std::cout << "[";
        for (int i = 0; i < linalg::dim(*itr); i++) {
            std::cout << std::showpos << std::scientific << std::setprecision(4) << (*itr)[i]
                      << (i < linalg::dim(*itr) - 1 ? ", " : "");
        }
        std::cout << "]" << (itr < val.end() - 1 ? ", " : "");
    }
    std::cout << "}";
    std::cout.flush();

    std::cout.flags(flags);
    std::cout.precision(precision);
}

void instance::put (const std::vector<linalg::imatrix> & val) {
    this->logging.inf("Printing output: Type: Integer matrix array, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to print output (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = std::cout.flags();
    auto fill = std::cout.fill();
    auto width = std::cout.width();

    std::cout << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        std::cout << "[";
        for (int i = 0; i < linalg::rows(*itr); i++) {
            std::cout << "[";
            for (int j = 0; j < linalg::cols(*itr); j++) {
                std::cout << std::showpos << std::setfill(' ') << std::setw(6) << (*itr)[i][j]
                          << (j < linalg::cols(*itr) - 1 ? ", " : "");
            }
            std::cout << "]" << (i < linalg::rows(*itr) - 1 ? ", " : "");
        }
        std::cout << "]" << (itr < val.end() - 1 ? ", " : "");
    }
    std::cout << "}";
    std::cout.flush();

    std::cout.flags(flags);
    std::cout.fill(fill);
    std::cout.width(width);
}

void instance::put (const std::vector<linalg::fmatrix> & val) {
    this->logging.inf("Printing output: Type: Real matrix array, Value: ", val);

    if (!this->init) {
        this->logging.err("Failed to print output (Instance not initialized)");
        throw except_put::fail;
    }

    auto flags = std::cout.flags();
    auto precision = std::cout.precision();

    std::cout << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        std::cout << "[";
        for (int i = 0; i < linalg::rows(*itr); i++) {
            std::cout << "[";
            for (int j = 0; j < linalg::cols(*itr); j++) {
                std::cout << std::showpos << std::scientific << std::setprecision(4) << (*itr)[i][j]
                          << (j < linalg::cols(*itr) - 1 ? ", " : "");
            }
            std::cout << "]" << (i < linalg::rows(*itr) - 1 ? ", " : "");
        }
        std::cout << "]" << (itr < val.end() - 1 ? ", " : "");
    }
    std::cout << "}";
    std::cout.flush();

    std::cout.flags(flags);
    std::cout.precision(precision);
}

}

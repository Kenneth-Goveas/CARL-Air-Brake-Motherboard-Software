#include <cstdint>
#include <cctype>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>

#include <math/linalg.hpp>
#include <sys/logging.hpp>
#include <ui/input.hpp>

namespace input {

bool tracker::exist = false;

bool tracker::instantiate (void) {
    if (tracker::exist) {
        return false;
    } else {
        tracker::exist = true;
        return true;
    }
}

instance::instance (void) : logging("input"), init(false) {
    this->logging.inf("Initializing instance");

    if (!instance::instantiate()) {
        this->logging.err("Failed to initialize instance (Instance already exists)");
        throw except_ctor::fail;
    }

    this->init = true;
}

template <>
std::string instance::get<std::string> (void) {
    std::string val;
    std::string repr;

    this->logging.inf("Scanning input: Type: String");

    if (!this->init) {
        this->logging.err("Failed to scan input (Instance not initialized)");
        throw except_get::fail;
    }

    this->logging.inf("Waiting for input");
    std::getline(std::cin, repr);
    if (std::cin.eof()) {
        this->logging.wrn("No input received");
        throw except_get::eof;
    }
    this->logging.inf("Received input: ", repr);

    this->strip(repr);

    this->logging.inf("Parsing input");
    try {
        this->parse(repr, val);
    } catch (except_parse) {
        this->logging.err("Failed to parse input (Syntax error)");
        this->logging.err("Failed to scan input");
        throw except_get::fail;
    }
    this->logging.inf("Parsed input: Value: ", val);

    return val;
}

template <>
std::uint8_t instance::get<std::uint8_t> (void) {
    std::uint8_t val;
    std::string repr;

    this->logging.inf("Scanning input: Type: Byte");

    if (!this->init) {
        this->logging.err("Failed to scan input (Instance not initialized)");
        throw except_get::fail;
    }

    this->logging.inf("Waiting for input");
    std::getline(std::cin, repr);
    if (std::cin.eof()) {
        this->logging.wrn("No input received");
        throw except_get::eof;
    }
    this->logging.inf("Received input: ", repr);

    this->strip(repr);

    this->logging.inf("Parsing input");
    try {
        this->parse(repr, val);
    } catch (except_parse) {
        this->logging.err("Failed to parse input (Syntax error)");
        this->logging.err("Failed to scan input");
        throw except_get::fail;
    }
    this->logging.inf("Parsed input: Value: ", val);

    return val;
}

template <>
bool instance::get<bool> (void) {
    bool val;
    std::string repr;

    this->logging.inf("Scanning input: Type: Boolean");

    if (!this->init) {
        this->logging.err("Failed to scan input (Instance not initialized)");
        throw except_get::fail;
    }

    this->logging.inf("Waiting for input");
    std::getline(std::cin, repr);
    if (std::cin.eof()) {
        this->logging.wrn("No input received");
        throw except_get::eof;
    }
    this->logging.inf("Received input: ", repr);

    this->strip(repr);

    this->logging.inf("Parsing input");
    try {
        this->parse(repr, val);
    } catch (except_parse) {
        this->logging.err("Failed to parse input (Syntax error)");
        this->logging.err("Failed to scan input");
        throw except_get::fail;
    }
    this->logging.inf("Parsed input: Value: ", val);

    return val;
}

template <>
int instance::get<int> (void) {
    int val;
    std::string repr;

    this->logging.inf("Scanning input: Type: Integer scalar");

    if (!this->init) {
        this->logging.err("Failed to scan input (Instance not initialized)");
        throw except_get::fail;
    }

    this->logging.inf("Waiting for input");
    std::getline(std::cin, repr);
    if (std::cin.eof()) {
        this->logging.wrn("No input received");
        throw except_get::eof;
    }
    this->logging.inf("Received input: ", repr);

    this->strip(repr);

    this->logging.inf("Parsing input");
    try {
        this->parse(repr, val);
    } catch (except_parse) {
        this->logging.err("Failed to parse input (Syntax error)");
        this->logging.err("Failed to scan input");
        throw except_get::fail;
    }
    this->logging.inf("Parsed input: Value: ", val);

    return val;
}

template <>
double instance::get<double> (void) {
    double val;
    std::string repr;

    this->logging.inf("Scanning input: Type: Real scalar");

    if (!this->init) {
        this->logging.err("Failed to scan input (Instance not initialized)");
        throw except_get::fail;
    }

    this->logging.inf("Waiting for input");
    std::getline(std::cin, repr);
    if (std::cin.eof()) {
        this->logging.wrn("No input received");
        throw except_get::eof;
    }
    this->logging.inf("Received input: ", repr);

    this->strip(repr);

    this->logging.inf("Parsing input");
    try {
        this->parse(repr, val);
    } catch (except_parse) {
        this->logging.err("Failed to parse input (Syntax error)");
        this->logging.err("Failed to scan input");
        throw except_get::fail;
    }
    this->logging.inf("Parsed input: Value: ", val);

    return val;
}

template <>
linalg::ivector instance::get<linalg::ivector> (void) {
    linalg::ivector val;
    std::string repr;

    this->logging.inf("Scanning input: Type: Integer vector");

    if (!this->init) {
        this->logging.err("Failed to scan input (Instance not initialized)");
        throw except_get::fail;
    }

    this->logging.inf("Waiting for input");
    std::getline(std::cin, repr);
    if (std::cin.eof()) {
        this->logging.wrn("No input received");
        throw except_get::eof;
    }
    this->logging.inf("Received input: ", repr);

    this->strip(repr);

    this->logging.inf("Parsing input");
    try {
        this->parse(repr, val);
    } catch (except_parse) {
        this->logging.err("Failed to parse input (Syntax error)");
        this->logging.err("Failed to scan input");
        throw except_get::fail;
    }
    this->logging.inf("Parsed input: Value: ", val);

    return val;
}

template <>
linalg::fvector instance::get<linalg::fvector> (void) {
    linalg::fvector val;
    std::string repr;

    this->logging.inf("Scanning input: Type: Real vector");

    if (!this->init) {
        this->logging.err("Failed to scan input (Instance not initialized)");
        throw except_get::fail;
    }

    this->logging.inf("Waiting for input");
    std::getline(std::cin, repr);
    if (std::cin.eof()) {
        this->logging.wrn("No input received");
        throw except_get::eof;
    }
    this->logging.inf("Received input: ", repr);

    this->strip(repr);

    this->logging.inf("Parsing input");
    try {
        this->parse(repr, val);
    } catch (except_parse) {
        this->logging.err("Failed to parse input (Syntax error)");
        this->logging.err("Failed to scan input");
        throw except_get::fail;
    }
    this->logging.inf("Parsed input: Value: ", val);

    return val;
}

template <>
linalg::imatrix instance::get<linalg::imatrix> (void) {
    linalg::imatrix val;
    std::string repr;

    this->logging.inf("Scanning input: Type: Integer matrix");

    if (!this->init) {
        this->logging.err("Failed to scan input (Instance not initialized)");
        throw except_get::fail;
    }

    this->logging.inf("Waiting for input");
    std::getline(std::cin, repr);
    if (std::cin.eof()) {
        this->logging.wrn("No input received");
        throw except_get::eof;
    }
    this->logging.inf("Received input: ", repr);

    this->strip(repr);

    this->logging.inf("Parsing input");
    try {
        this->parse(repr, val);
    } catch (except_parse) {
        this->logging.err("Failed to parse input (Syntax error)");
        this->logging.err("Failed to scan input");
        throw except_get::fail;
    }
    this->logging.inf("Parsed input: Value: ", val);

    return val;
}

template <>
linalg::fmatrix instance::get<linalg::fmatrix> (void) {
    linalg::fmatrix val;
    std::string repr;

    this->logging.inf("Scanning input: Type: Real matrix");

    if (!this->init) {
        this->logging.err("Failed to scan input (Instance not initialized)");
        throw except_get::fail;
    }

    this->logging.inf("Waiting for input");
    std::getline(std::cin, repr);
    if (std::cin.eof()) {
        this->logging.wrn("No input received");
        throw except_get::eof;
    }
    this->logging.inf("Received input: ", repr);

    this->strip(repr);

    this->logging.inf("Parsing input");
    try {
        this->parse(repr, val);
    } catch (except_parse) {
        this->logging.err("Failed to parse input (Syntax error)");
        this->logging.err("Failed to scan input");
        throw except_get::fail;
    }
    this->logging.inf("Parsed input: Value: ", val);

    return val;
}

template <>
std::vector<std::string> instance::get<std::vector<std::string>> (void) {
    std::vector<std::string> val;
    std::string repr;

    this->logging.inf("Scanning input: Type: String array");

    if (!this->init) {
        this->logging.err("Failed to scan input (Instance not initialized)");
        throw except_get::fail;
    }

    this->logging.inf("Waiting for input");
    std::getline(std::cin, repr);
    if (std::cin.eof()) {
        this->logging.wrn("No input received");
        throw except_get::eof;
    }
    this->logging.inf("Received input: ", repr);

    this->strip(repr);

    this->logging.inf("Parsing input");
    try {
        this->parse(repr, val);
    } catch (except_parse) {
        this->logging.err("Failed to parse input (Syntax error)");
        this->logging.err("Failed to scan input");
        throw except_get::fail;
    }
    this->logging.inf("Parsed input: Value: ", val);

    return val;
}

template <>
std::vector<std::uint8_t> instance::get<std::vector<std::uint8_t>> (void) {
    std::vector<std::uint8_t> val;
    std::string repr;

    this->logging.inf("Scanning input: Type: Byte array");

    if (!this->init) {
        this->logging.err("Failed to scan input (Instance not initialized)");
        throw except_get::fail;
    }

    this->logging.inf("Waiting for input");
    std::getline(std::cin, repr);
    if (std::cin.eof()) {
        this->logging.wrn("No input received");
        throw except_get::eof;
    }
    this->logging.inf("Received input: ", repr);

    this->strip(repr);

    this->logging.inf("Parsing input");
    try {
        this->parse(repr, val);
    } catch (except_parse) {
        this->logging.err("Failed to parse input (Syntax error)");
        this->logging.err("Failed to scan input");
        throw except_get::fail;
    }
    this->logging.inf("Parsed input: Value: ", val);

    return val;
}

template <>
std::vector<bool> instance::get<std::vector<bool>> (void) {
    std::vector<bool> val;
    std::string repr;

    this->logging.inf("Scanning input: Type: Boolean array");

    if (!this->init) {
        this->logging.err("Failed to scan input (Instance not initialized)");
        throw except_get::fail;
    }

    this->logging.inf("Waiting for input");
    std::getline(std::cin, repr);
    if (std::cin.eof()) {
        this->logging.wrn("No input received");
        throw except_get::eof;
    }
    this->logging.inf("Received input: ", repr);

    this->strip(repr);

    this->logging.inf("Parsing input");
    try {
        this->parse(repr, val);
    } catch (except_parse) {
        this->logging.err("Failed to parse input (Syntax error)");
        this->logging.err("Failed to scan input");
        throw except_get::fail;
    }
    this->logging.inf("Parsed input: Value: ", val);

    return val;
}

template <>
std::vector<int> instance::get<std::vector<int>> (void) {
    std::vector<int> val;
    std::string repr;

    this->logging.inf("Scanning input: Type: Integer scalar array");

    if (!this->init) {
        this->logging.err("Failed to scan input (Instance not initialized)");
        throw except_get::fail;
    }

    this->logging.inf("Waiting for input");
    std::getline(std::cin, repr);
    if (std::cin.eof()) {
        this->logging.wrn("No input received");
        throw except_get::eof;
    }
    this->logging.inf("Received input: ", repr);

    this->strip(repr);

    this->logging.inf("Parsing input");
    try {
        this->parse(repr, val);
    } catch (except_parse) {
        this->logging.err("Failed to parse input (Syntax error)");
        this->logging.err("Failed to scan input");
        throw except_get::fail;
    }
    this->logging.inf("Parsed input: Value: ", val);

    return val;
}

template <>
std::vector<double> instance::get<std::vector<double>> (void) {
    std::vector<double> val;
    std::string repr;

    this->logging.inf("Scanning input: Type: Real scalar array");

    if (!this->init) {
        this->logging.err("Failed to scan input (Instance not initialized)");
        throw except_get::fail;
    }

    this->logging.inf("Waiting for input");
    std::getline(std::cin, repr);
    if (std::cin.eof()) {
        this->logging.wrn("No input received");
        throw except_get::eof;
    }
    this->logging.inf("Received input: ", repr);

    this->strip(repr);

    this->logging.inf("Parsing input");
    try {
        this->parse(repr, val);
    } catch (except_parse) {
        this->logging.err("Failed to parse input (Syntax error)");
        this->logging.err("Failed to scan input");
        throw except_get::fail;
    }
    this->logging.inf("Parsed input: Value: ", val);

    return val;
}

template <>
std::vector<linalg::ivector> instance::get<std::vector<linalg::ivector>> (void) {
    std::vector<linalg::ivector> val;
    std::string repr;

    this->logging.inf("Scanning input: Type: Integer vector array");

    if (!this->init) {
        this->logging.err("Failed to scan input (Instance not initialized)");
        throw except_get::fail;
    }

    this->logging.inf("Waiting for input");
    std::getline(std::cin, repr);
    if (std::cin.eof()) {
        this->logging.wrn("No input received");
        throw except_get::eof;
    }
    this->logging.inf("Received input: ", repr);

    this->strip(repr);

    this->logging.inf("Parsing input");
    try {
        this->parse(repr, val);
    } catch (except_parse) {
        this->logging.err("Failed to parse input (Syntax error)");
        this->logging.err("Failed to scan input");
        throw except_get::fail;
    }
    this->logging.inf("Parsed input: Value: ", val);

    return val;
}

template <>
std::vector<linalg::fvector> instance::get<std::vector<linalg::fvector>> (void) {
    std::vector<linalg::fvector> val;
    std::string repr;

    this->logging.inf("Scanning input: Type: Real vector array");

    if (!this->init) {
        this->logging.err("Failed to scan input (Instance not initialized)");
        throw except_get::fail;
    }

    this->logging.inf("Waiting for input");
    std::getline(std::cin, repr);
    if (std::cin.eof()) {
        this->logging.wrn("No input received");
        throw except_get::eof;
    }
    this->logging.inf("Received input: ", repr);

    this->strip(repr);

    this->logging.inf("Parsing input");
    try {
        this->parse(repr, val);
    } catch (except_parse) {
        this->logging.err("Failed to parse input (Syntax error)");
        this->logging.err("Failed to scan input");
        throw except_get::fail;
    }
    this->logging.inf("Parsed input: Value: ", val);

    return val;
}

template <>
std::vector<linalg::imatrix> instance::get<std::vector<linalg::imatrix>> (void) {
    std::vector<linalg::imatrix> val;
    std::string repr;

    this->logging.inf("Scanning input: Type: Integer matrix array");

    if (!this->init) {
        this->logging.err("Failed to scan input (Instance not initialized)");
        throw except_get::fail;
    }

    this->logging.inf("Waiting for input");
    std::getline(std::cin, repr);
    if (std::cin.eof()) {
        this->logging.wrn("No input received");
        throw except_get::eof;
    }
    this->logging.inf("Received input: ", repr);

    this->strip(repr);

    this->logging.inf("Parsing input");
    try {
        this->parse(repr, val);
    } catch (except_parse) {
        this->logging.err("Failed to parse input (Syntax error)");
        this->logging.err("Failed to scan input");
        throw except_get::fail;
    }
    this->logging.inf("Parsed input: Value: ", val);

    return val;
}

template <>
std::vector<linalg::fmatrix> instance::get<std::vector<linalg::fmatrix>> (void) {
    std::vector<linalg::fmatrix> val;
    std::string repr;

    this->logging.inf("Scanning input: Type: Real matrix array");

    if (!this->init) {
        this->logging.err("Failed to scan input (Instance not initialized)");
        throw except_get::fail;
    }

    this->logging.inf("Waiting for input");
    std::getline(std::cin, repr);
    if (std::cin.eof()) {
        this->logging.wrn("No input received");
        throw except_get::eof;
    }
    this->logging.inf("Received input: ", repr);

    this->strip(repr);

    this->logging.inf("Parsing input");
    try {
        this->parse(repr, val);
    } catch (except_parse) {
        this->logging.err("Failed to parse input (Syntax error)");
        this->logging.err("Failed to scan input");
        throw except_get::fail;
    }
    this->logging.inf("Parsed input: Value: ", val);

    return val;
}

void instance::split (const std::string & str, std::vector<std::string> & arr) {
    bool quot = false, escp = false;
    std::string nest;

    arr = std::vector<std::string>(1);

    if (str.empty()) {
        throw except_split::fail;
    }

    if (!(str.front() == '[' && str.back() == ']') && !(str.front() == '{' && str.back() == '}')) {
        throw except_split::fail;
    }

    for (auto itr = str.begin() + 1; itr < str.end() - 1; itr++) {
        if (quot) {
            arr.back().push_back(*itr);
            if (escp) {
                escp = false;
            } else {
                if (*itr == '\\') {
                    escp = true;
                } else if (*itr == '"') {
                    quot = false;
                }
            }
        } else if (!nest.empty()) {
            arr.back().push_back(*itr);
            if (*itr == ']') {
                if (nest.back() != '[') {
                    throw except_split::fail;
                } else {
                    nest.pop_back();
                }
            } else if (*itr == '}') {
                if (nest.back() != '{') {
                    throw except_split::fail;
                } else {
                    nest.pop_back();
                }
            } else if (*itr == '[') {
                nest.push_back('[');
            } else if (*itr == '{') {
                nest.push_back('{');
            } else if (*itr == '"') {
                quot = true;
            }
        } else {
            if (*itr != ',') {
                arr.back().push_back(*itr);
                if (*itr == ']') {
                    throw except_split::fail;
                } else if (*itr == '}') {
                    throw except_split::fail;
                } else if (*itr == '[') {
                    nest.push_back('[');
                } else if (*itr == '{') {
                    nest.push_back('{');
                } else if (*itr == '"') {
                    quot = true;
                }
            } else {
                arr.push_back("");
            }
        }
    }

    if (quot || !nest.empty()) {
        throw except_split::fail;
    }

    for (auto itr = arr.begin(); itr < arr.end(); itr++) {
        this->strip(*itr);
    }
}

void instance::strip (std::string & str) {
    while (!str.empty()) {
        if (std::isspace(str.front())) {
            str.erase(str.begin());
        } else if (std::isspace(str.back())) {
            str.erase(str.end() - 1);
        } else {
            return;
        }
    }
}

void instance::parse (const std::string & str, std::string & val) {
    bool escp = false;

    if (str.size() < 2) {
        throw except_parse::fail;
    }

    if (str.front() != '"' || str.back() != '"') {
        throw except_parse::fail;
    }

    val.clear();
    for (auto itr = str.begin() + 1; itr < str.end() - 1; itr++) {
        if (escp) {
            if (*itr != '\\' && *itr != '"') {
                throw except_parse::fail;
            } else {
                escp = false;
                val.push_back(*itr);
            }
        } else {
            if (*itr == '\n' || *itr == '"') {
                throw except_parse::fail;
            } else if (*itr == '\\') {
                escp = true;
            } else {
                val.push_back(*itr);
            }
        }
    }

    if (escp) {
        throw except_parse::fail;
    }
}

void instance::parse (const std::string & str, std::uint8_t & val) {
    int buf;
    std::istringstream istr(str);

    if (str.substr(0, 2) != "0x") {
        throw except_parse::fail;
    }

    istr >> std::hex >> buf;
    if (istr.fail() || !istr.eof()) {
        throw except_parse::fail;
    }

    if (buf < 0 || buf > 0xFF) {
        throw except_parse::fail;
    }

    val = buf;
}

void instance::parse (const std::string & str, bool & val) {
    if (str == "yes") {
        val = true;
    } else if (str == "no") {
        val = false;
    } else {
        throw except_parse::fail;
    }
}

void instance::parse (const std::string & str, int & val) {
    std::istringstream istr(str);
    istr >> val;
    if (istr.fail() || !istr.eof()) {
        throw except_parse::fail;
    }
}

void instance::parse (const std::string & str, double & val) {
    std::istringstream istr(str);
    istr >> val;
    if (istr.fail() || !istr.eof()) {
        throw except_parse::fail;
    }
}

void instance::parse (const std::string & str, linalg::ivector & val) {
    std::vector<std::string> arr;
    int dim = 0;

    if (str.empty()) {
        throw except_parse::fail;
    }

    if (str.front() != '[' || str.back() != ']') {
        throw except_parse::fail;
    }

    try {
        this->split(str, arr);
        dim = arr.size();
    } catch (except_split) {
        throw except_parse::fail;
    }

    try {
        linalg::resize(val, dim);
        for (int i = 0; i < dim; i++) {
            this->parse(arr[i], val[i]);
        }
    } catch (except_parse) {
        throw except_parse::fail;
    }
}

void instance::parse (const std::string & str, linalg::fvector & val) {
    std::vector<std::string> arr;
    int dim = 0;

    if (str.empty()) {
        throw except_parse::fail;
    }

    if (str.front() != '[' || str.back() != ']') {
        throw except_parse::fail;
    }

    try {
        this->split(str, arr);
        dim = arr.size();
    } catch (except_split) {
        throw except_parse::fail;
    }

    try {
        linalg::resize(val, dim);
        for (int i = 0; i < dim; i++) {
            this->parse(arr[i], val[i]);
        }
    } catch (except_parse) {
        throw except_parse::fail;
    }
}

void instance::parse (const std::string & str, linalg::imatrix & val) {
    std::vector<std::string> buf;
    std::vector<std::vector<std::string>> arr;
    int rows = 0, cols = 0;

    if (str.empty()) {
        throw except_parse::fail;
    }

    if (str.front() != '[' || str.back() != ']') {
        throw except_parse::fail;
    }

    try {
        this->split(str, buf);
        rows = buf.size();
        arr.resize(rows);
    } catch (except_split) {
        throw except_parse::fail;
    }

    for (int i = 0; i < rows; i++) {
        if (buf[i].empty()) {
            throw except_parse::fail;
        }

        if (buf[i].front() != '[' || buf[i].back() != ']') {
            throw except_parse::fail;
        }

        try {
            this->split(buf[i], arr[i]);
            cols = arr[i].size();
        } catch (except_split) {
            throw except_parse::fail;
        }
    }

    for (auto elem : arr) {
        if (int(elem.size()) != cols) {
            throw except_parse::fail;
        }
    }

    try {
        linalg::resize(val, rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                this->parse(arr[i][j], val[i][j]);
            }
        }
    } catch (except_parse) {
        throw except_parse::fail;
    }
}

void instance::parse (const std::string & str, linalg::fmatrix & val) {
    std::vector<std::string> buf;
    std::vector<std::vector<std::string>> arr;
    int rows = 0, cols = 0;

    if (str.empty()) {
        throw except_parse::fail;
    }

    if (str.front() != '[' || str.back() != ']') {
        throw except_parse::fail;
    }

    try {
        this->split(str, buf);
        rows = buf.size();
        arr.resize(rows);
    } catch (except_split) {
        throw except_parse::fail;
    }

    for (int i = 0; i < rows; i++) {
        if (buf[i].empty()) {
            throw except_parse::fail;
        }

        if (buf[i].front() != '[' || buf[i].back() != ']') {
            throw except_parse::fail;
        }

        try {
            this->split(buf[i], arr[i]);
            cols = arr[i].size();
        } catch (except_split) {
            throw except_parse::fail;
        }
    }

    for (auto elem : arr) {
        if (int(elem.size()) != cols) {
            throw except_parse::fail;
        }
    }

    try {
        linalg::resize(val, rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                this->parse(arr[i][j], val[i][j]);
            }
        }
    } catch (except_parse) {
        throw except_parse::fail;
    }
}

void instance::parse (const std::string & str, std::vector<std::string> & val) {
    std::vector<std::string> arr;

    if (str.empty()) {
        throw except_parse::fail;
    }

    if (str.front() != '{' || str.back() != '}') {
        throw except_parse::fail;
    }

    try {
        std::string buf;

        this->split(str, arr);
        val.resize(arr.size());

        for (auto itr = arr.begin(); itr < arr.end(); itr++) {
            this->parse(*itr, buf);
            *(val.begin() + std::distance(arr.begin(), itr)) = buf;
        }
    } catch (...) {
        throw except_parse::fail;
    }
}

void instance::parse (const std::string & str, std::vector<std::uint8_t> & val) {
    std::vector<std::string> arr;

    if (str.empty()) {
        throw except_parse::fail;
    }

    if (str.front() != '{' || str.back() != '}') {
        throw except_parse::fail;
    }

    try {
        std::uint8_t buf;

        this->split(str, arr);
        val.resize(arr.size());

        for (auto itr = arr.begin(); itr < arr.end(); itr++) {
            this->parse(*itr, buf);
            *(val.begin() + std::distance(arr.begin(), itr)) = buf;
        }
    } catch (...) {
        throw except_parse::fail;
    }
}

void instance::parse (const std::string & str, std::vector<bool> & val) {
    std::vector<std::string> arr;

    if (str.empty()) {
        throw except_parse::fail;
    }

    if (str.front() != '{' || str.back() != '}') {
        throw except_parse::fail;
    }

    try {
        bool buf;

        this->split(str, arr);
        val.resize(arr.size());

        for (auto itr = arr.begin(); itr < arr.end(); itr++) {
            this->parse(*itr, buf);
            *(val.begin() + std::distance(arr.begin(), itr)) = buf;
        }
    } catch (...) {
        throw except_parse::fail;
    }
}

void instance::parse (const std::string & str, std::vector<int> & val) {
    std::vector<std::string> arr;

    if (str.empty()) {
        throw except_parse::fail;
    }

    if (str.front() != '{' || str.back() != '}') {
        throw except_parse::fail;
    }

    try {
        int buf;

        this->split(str, arr);
        val.resize(arr.size());

        for (auto itr = arr.begin(); itr < arr.end(); itr++) {
            this->parse(*itr, buf);
            *(val.begin() + std::distance(arr.begin(), itr)) = buf;
        }
    } catch (...) {
        throw except_parse::fail;
    }
}

void instance::parse (const std::string & str, std::vector<double> & val) {
    std::vector<std::string> arr;

    if (str.empty()) {
        throw except_parse::fail;
    }

    if (str.front() != '{' || str.back() != '}') {
        throw except_parse::fail;
    }

    try {
        double buf;

        this->split(str, arr);
        val.resize(arr.size());

        for (auto itr = arr.begin(); itr < arr.end(); itr++) {
            this->parse(*itr, buf);
            *(val.begin() + std::distance(arr.begin(), itr)) = buf;
        }
    } catch (...) {
        throw except_parse::fail;
    }
}

void instance::parse (const std::string & str, std::vector<linalg::ivector> & val) {
    std::vector<std::string> arr;

    if (str.empty()) {
        throw except_parse::fail;
    }

    if (str.front() != '{' || str.back() != '}') {
        throw except_parse::fail;
    }

    try {
        linalg::ivector buf;

        this->split(str, arr);
        val.resize(arr.size());

        for (auto itr = arr.begin(); itr < arr.end(); itr++) {
            this->parse(*itr, buf);
            *(val.begin() + std::distance(arr.begin(), itr)) = buf;
        }
    } catch (...) {
        throw except_parse::fail;
    }
}

void instance::parse (const std::string & str, std::vector<linalg::fvector> & val) {
    std::vector<std::string> arr;

    if (str.empty()) {
        throw except_parse::fail;
    }

    if (str.front() != '{' || str.back() != '}') {
        throw except_parse::fail;
    }

    try {
        linalg::fvector buf;

        this->split(str, arr);
        val.resize(arr.size());

        for (auto itr = arr.begin(); itr < arr.end(); itr++) {
            this->parse(*itr, buf);
            *(val.begin() + std::distance(arr.begin(), itr)) = buf;
        }
    } catch (...) {
        throw except_parse::fail;
    }
}

void instance::parse (const std::string & str, std::vector<linalg::imatrix> & val) {
    std::vector<std::string> arr;

    if (str.empty()) {
        throw except_parse::fail;
    }

    if (str.front() != '{' || str.back() != '}') {
        throw except_parse::fail;
    }

    try {
        linalg::imatrix buf;

        this->split(str, arr);
        val.resize(arr.size());

        for (auto itr = arr.begin(); itr < arr.end(); itr++) {
            this->parse(*itr, buf);
            *(val.begin() + std::distance(arr.begin(), itr)) = buf;
        }
    } catch (...) {
        throw except_parse::fail;
    }
}

void instance::parse (const std::string & str, std::vector<linalg::fmatrix> & val) {
    std::vector<std::string> arr;

    if (str.empty()) {
        throw except_parse::fail;
    }

    if (str.front() != '{' || str.back() != '}') {
        throw except_parse::fail;
    }

    try {
        linalg::fmatrix buf;

        this->split(str, arr);
        val.resize(arr.size());

        for (auto itr = arr.begin(); itr < arr.end(); itr++) {
            this->parse(*itr, buf);
            *(val.begin() + std::distance(arr.begin(), itr)) = buf;
        }
    } catch (...) {
        throw except_parse::fail;
    }
}

}

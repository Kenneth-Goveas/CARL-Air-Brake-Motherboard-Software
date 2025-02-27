#include <cstdint>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <regex>
#include <string>
#include <vector>

#include <math/linalg.hpp>

#include <sys/logging.hpp>

#include <ui/input.hpp>

namespace input :: intern {

std::string mod = "input";

bool fail;
bool eof;

std::string strip (std::string inpt);
std::vector<std::string> split (std::string inpt);

bool is_vec_arb_dim (std::string inpt);
bool is_vec_fix_dim (std::string inpt, int dim);

bool is_mat_arb_rows_arb_cols (std::string inpt);
bool is_mat_fix_rows_arb_cols (std::string inpt, int rows);
bool is_mat_arb_rows_fix_cols (std::string inpt, int cols);
bool is_mat_fix_rows_fix_cols (std::string inpt, int rows, int cols);

template <typename type>
type to_vec (std::string inpt, int dim);

template <>
linalg::ivector to_vec<linalg::ivector> (std::string inpt, int dim);

template <>
linalg::fvector to_vec<linalg::fvector> (std::string inpt, int dim);

template <typename type>
type to_mat (std::string inpt, int rows, int cols);

template <>
linalg::imatrix to_mat<linalg::imatrix> (std::string inpt, int rows, int cols);

template <>
linalg::fmatrix to_mat<linalg::fmatrix> (std::string inpt, int rows, int cols);

template <typename type>
type parse (std::string inpt);

template <>
std::string parse<std::string> (std::string inpt);

template <>
std::uint8_t parse<std::uint8_t> (std::string inpt);

template <>
bool parse<bool> (std::string inpt);

template <>
int parse<int> (std::string inpt);

template <>
double parse<double> (std::string inpt);

template <>
linalg::ivector parse<linalg::ivector> (std::string inpt);

template <>
linalg::fvector parse<linalg::fvector> (std::string inpt);

template <>
linalg::imatrix parse<linalg::imatrix> (std::string inpt);

template <>
linalg::fmatrix parse<linalg::fmatrix> (std::string inpt);

template <>
std::vector<std::string> parse<std::vector<std::string>> (std::string inpt);

template <>
std::vector<std::uint8_t> parse<std::vector<std::uint8_t>> (std::string inpt);

template <>
std::vector<bool> parse<std::vector<bool>> (std::string inpt);

template <>
std::vector<int> parse<std::vector<int>> (std::string inpt);

template <>
std::vector<double> parse<std::vector<double>> (std::string inpt);

template <>
std::vector<linalg::ivector> parse<std::vector<linalg::ivector>> (std::string inpt);

template <>
std::vector<linalg::fvector> parse<std::vector<linalg::fvector>> (std::string inpt);

template <>
std::vector<linalg::imatrix> parse<std::vector<linalg::imatrix>> (std::string inpt);

template <>
std::vector<linalg::fmatrix> parse<std::vector<linalg::fmatrix>> (std::string inpt);

}

namespace input {

bool fail (void) {
    return intern::fail;
}

bool eof (void) {
    return intern::eof;
}

template <>
std::string get<std::string> (void) {
    std::string val;
    std::string inpt;

    logging::inf(intern::mod,
        "Scanning input: Type: String"
    );

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: ", inpt
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    val = intern::parse<std::string>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input (Syntax error)"
        );
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    logging::inf(intern::mod,
        "Parsed input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::uint8_t get<std::uint8_t> (void) {
    std::uint8_t val;
    std::string inpt;

    logging::inf(intern::mod,
        "Scanning input: Type: Byte"
    );

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: ", inpt
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    val = intern::parse<std::uint8_t>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input (Syntax error)"
        );
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    logging::inf(intern::mod,
        "Parsed input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
bool get<bool> (void) {
    bool val;
    std::string inpt;

    logging::inf(intern::mod,
        "Scanning input: Type: Boolean"
    );

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: ", inpt
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    val = intern::parse<bool>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input (Syntax error)"
        );
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    logging::inf(intern::mod,
        "Parsed input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
int get<int> (void) {
    int val;
    std::string inpt;

    logging::inf(intern::mod,
        "Scanning input: Type: Integer scalar"
    );

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: ", inpt
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    val = intern::parse<int>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input (Syntax error)"
        );
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    logging::inf(intern::mod,
        "Parsed input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
double get<double> (void) {
    double val;
    std::string inpt;

    logging::inf(intern::mod,
        "Scanning input: Type: Real scalar"
    );

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: ", inpt
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    val = intern::parse<double>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input (Syntax error)"
        );
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    logging::inf(intern::mod,
        "Parsed input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::ivector get<linalg::ivector> (void) {
    linalg::ivector val;
    std::string inpt;

    logging::inf(intern::mod,
        "Scanning input: Type: Integer vector"
    );

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: ", inpt
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    val = intern::parse<linalg::ivector>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input (Syntax error)"
        );
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    logging::inf(intern::mod,
        "Parsed input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::fvector get<linalg::fvector> (void) {
    linalg::fvector val;
    std::string inpt;

    logging::inf(intern::mod,
        "Scanning input: Type: Real vector"
    );

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: ", inpt
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    val = intern::parse<linalg::fvector>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input (Syntax error)"
        );
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    logging::inf(intern::mod,
        "Parsed input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::imatrix get<linalg::imatrix> (void) {
    linalg::imatrix val;
    std::string inpt;

    logging::inf(intern::mod,
        "Scanning input: Type: Integer matrix"
    );

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: ", inpt
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    val = intern::parse<linalg::imatrix>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input (Syntax error)"
        );
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    logging::inf(intern::mod,
        "Parsed input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::fmatrix get<linalg::fmatrix> (void) {
    linalg::fmatrix val;
    std::string inpt;

    logging::inf(intern::mod,
        "Scanning input: Type: Real matrix"
    );

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: ", inpt
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    val = intern::parse<linalg::fmatrix>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input (Syntax error)"
        );
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    logging::inf(intern::mod,
        "Parsed input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<std::string> get<std::vector<std::string>> (void) {
    std::vector<std::string> val;
    std::string inpt;

    logging::inf(intern::mod,
        "Scanning input: Type: String array"
    );

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: ", inpt
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    val = intern::parse<std::vector<std::string>>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input (Syntax error)"
        );
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    logging::inf(intern::mod,
        "Parsed input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<std::uint8_t> get<std::vector<std::uint8_t>> (void) {
    std::vector<std::uint8_t> val;
    std::string inpt;

    logging::inf(intern::mod,
        "Scanning input: Type: Byte array"
    );

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: ", inpt
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    val = intern::parse<std::vector<std::uint8_t>>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input (Syntax error)"
        );
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    logging::inf(intern::mod,
        "Parsed input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<bool> get<std::vector<bool>> (void) {
    std::vector<bool> val;
    std::string inpt;

    logging::inf(intern::mod,
        "Scanning input: Type: Boolean array"
    );

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: ", inpt
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    val = intern::parse<std::vector<bool>>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input (Syntax error)"
        );
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    logging::inf(intern::mod,
        "Parsed input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<int> get<std::vector<int>> (void) {
    std::vector<int> val;
    std::string inpt;

    logging::inf(intern::mod,
        "Scanning input: Type: Integer scalar array"
    );

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: ", inpt
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    val = intern::parse<std::vector<int>>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input (Syntax error)"
        );
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    logging::inf(intern::mod,
        "Parsed input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<double> get<std::vector<double>> (void) {
    std::vector<double> val;
    std::string inpt;

    logging::inf(intern::mod,
        "Scanning input: Type: Real scalar array"
    );

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: ", inpt
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    val = intern::parse<std::vector<double>>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input (Syntax error)"
        );
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    logging::inf(intern::mod,
        "Parsed input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<linalg::ivector> get<std::vector<linalg::ivector>> (void) {
    std::vector<linalg::ivector> val;
    std::string inpt;

    logging::inf(intern::mod,
        "Scanning input: Type: Integer vector array"
    );

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: ", inpt
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    val = intern::parse<std::vector<linalg::ivector>>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input (Syntax error)"
        );
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    logging::inf(intern::mod,
        "Parsed input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<linalg::fvector> get<std::vector<linalg::fvector>> (void) {
    std::vector<linalg::fvector> val;
    std::string inpt;

    logging::inf(intern::mod,
        "Scanning input: Type: Real vector array"
    );

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: ", inpt
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    val = intern::parse<std::vector<linalg::fvector>>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input (Syntax error)"
        );
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    logging::inf(intern::mod,
        "Parsed input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<linalg::imatrix> get<std::vector<linalg::imatrix>> (void) {
    std::vector<linalg::imatrix> val;
    std::string inpt;

    logging::inf(intern::mod,
        "Scanning input: Type: Integer matrix array"
    );

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: ", inpt
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    val = intern::parse<std::vector<linalg::imatrix>>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input (Syntax error)"
        );
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    logging::inf(intern::mod,
        "Parsed input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<linalg::fmatrix> get<std::vector<linalg::fmatrix>> (void) {
    std::vector<linalg::fmatrix> val;
    std::string inpt;

    logging::inf(intern::mod,
        "Scanning input: Type: Real matrix array"
    );

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: ", inpt
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    val = intern::parse<std::vector<linalg::fmatrix>>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input (Syntax error)"
        );
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    logging::inf(intern::mod,
        "Parsed input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

}

namespace input :: intern {

std::string strip (std::string inpt) {
    while (!inpt.empty()) {
        if (inpt.front() == ' ' || inpt.front() == '\t') {
            inpt.erase(inpt.begin());
        } else {
            break;
        }
    }
    while (!inpt.empty()) {
        if (inpt.back() == ' ' || inpt.back() == '\t') {
            inpt.erase(inpt.end() - 1);
        } else {
            break;
        }
    }
    return inpt;
}

std::vector<std::string> split (std::string inpt) {
    std::vector<std::string> arr(1, "");
    bool quot = false, escap = false;
    int brac = 0;

    inpt = strip(inpt);

    if (inpt.front() != '{' || inpt.back() != '}') {
        fail = true;
        return arr;
    }

    for (auto itr = inpt.begin() + 1; itr < inpt.end() - 1; itr++) {
        if (quot) {
            arr.back() += *itr;
            if (escap) {
                escap = false;
            } else {
                if (*itr == '\\') {
                    escap = true;
                } else if (*itr == '"') {
                    quot = false;
                }
            }
        } else if (brac > 0) {
            arr.back() += *itr;
            if (*itr == '"') {
                quot = true;
            } else if (*itr == '[') {
                brac++;
            } else if (*itr == ']') {
                brac--;
            }
        } else {
            if (*itr == ',') {
                arr.push_back("");
            } else {
                arr.back() += *itr;
                if (*itr == '"') {
                    quot = true;
                } else if (*itr == '[') {
                    brac++;
                } else if (*itr == ']') {
                    fail = true;
                    return arr;
                }
            }
        }
    }

    if (quot || brac > 0) {
        fail = true;
        return arr;
    }

    fail = false;
    return arr;
}

bool is_vec_arb_dim (std::string inpt) {
    std::string pat;
    std::regex reg;

    pat = "^"
          "\\["
              "("
                  "[[:space:]]*"
                  "[^][[:space:],]+"
                  "[[:space:]]*"
              ",)*"
              "[[:space:]]*"
              "[^][[:space:],]+"
              "[[:space:]]*"
          "\\]"
          "$";

    reg.assign(pat, std::regex_constants::extended);

    return std::regex_match(inpt, reg);
}

bool is_vec_fix_dim (std::string inpt, int dim) {
    std::string pat = "";
    std::regex reg;

    pat += "^"
           "\\[";
    for (int i = 0; i < dim; i++) {
        pat += "[[:space:]]*"
               "[^][[:space:],]+"
               "[[:space:]]*";
        if (i < dim - 1) {
            pat += ",";
        }
    }
    pat += "\\]"
           "$";

    reg.assign(pat, std::regex_constants::extended);

    return std::regex_match(inpt, reg);
}

bool is_mat_arb_rows_arb_cols (std::string inpt) {
    std::string pat;
    std::regex reg;

    pat = "^"
          "\\["
              "("
                  "[[:space:]]*"
                  "\\["
                      "("
                          "[[:space:]]*"
                          "[^][[:space:],]+"
                          "[[:space:]]*"
                      ",)*"
                      "[[:space:]]*"
                      "[^][[:space:],]+"
                      "[[:space:]]*"
                  "\\]"
                  "[[:space:]]*"
              ",)*"
              "[[:space:]]*"
              "\\["
                  "("
                      "[[:space:]]*"
                      "[^][[:space:],]+"
                      "[[:space:]]*"
                  ",)*"
                  "[[:space:]]*"
                  "[^][[:space:],]+"
                  "[[:space:]]*"
              "\\]"
              "[[:space:]]*"
          "\\]"
          "$";

    reg.assign(pat, std::regex_constants::extended);

    return std::regex_match(inpt, reg);
}

bool is_mat_fix_rows_arb_cols (std::string inpt, int rows) {
    std::string pat = "";
    std::regex reg;

    pat += "^"
           "\\[";
    for (int i = 0; i < rows; i++) {
        pat += "[[:space:]]*"
               "\\["
                   "("
                       "[[:space:]]*"
                       "[^][[:space:],]+"
                       "[[:space:]]*"
                   ",)*"
                   "[[:space:]]*"
                   "[^][[:space:],]+"
                   "[[:space:]]*"
               "\\]"
               "[[:space:]]*";
        if (i < rows - 1) {
            pat += ",";
        }
    }
    pat += "\\]"
           "$";

    reg.assign(pat, std::regex_constants::extended);

    return std::regex_match(inpt, reg);
}

bool is_mat_arb_rows_fix_cols (std::string inpt, int cols) {
    std::string pat = "";
    std::regex reg;

    pat += "^"
           "\\[";
    pat += "("
               "[[:space:]]*"
               "\\["
                   "("
                       "[[:space:]]*"
                       "[^][[:space:],]+"
                       "[[:space:]]*"
                   ",)*"
                   "[[:space:]]*"
                   "[^][[:space:],]+"
                   "[[:space:]]*"
               "\\]"
               "[[:space:]]*"
           ",)*";
    pat += "[[:space:]]*"
           "\\[";
    for (int i = 0; i < cols; i++) {
        pat += "[[:space:]]*"
               "[^][[:space:],]+"
               "[[:space:]]*";
        if (i < cols - 1) {
            pat += ",";
        }
    }
    pat += "\\]"
           "[[:space:]]*";
    pat += "\\]"
           "$";

    reg.assign(pat, std::regex_constants::extended);

    return std::regex_match(inpt, reg);
}

bool is_mat_fix_rows_fix_cols (std::string inpt, int rows, int cols) {
    std::string pat = "";
    std::regex reg;

    pat += "^"
           "\\[";
    for (int i = 0; i < rows; i++) {
        pat += "[[:space:]]*"
               "\\[";
        for (int j = 0; j < cols; j++) {
            pat += "[[:space:]]*"
                   "[^][[:space:],]+"
                   "[[:space:]]*";
            if (j < cols - 1) {
                pat += ",";
            }
        }
        pat += "\\]"
               "[[:space:]]*";
        if (i < rows - 1) {
            pat += ",";
        }
    }
    pat += "\\]"
           "$";

    reg.assign(pat, std::regex_constants::extended);

    return std::regex_match(inpt, reg);
}

template <>
linalg::ivector to_vec<linalg::ivector> (std::string inpt, int dim) {
    linalg::ivector val;
    std::string pat = "";
    std::regex reg;
    std::smatch match;
    std::istringstream istr;

    pat += "^"
           "\\[";
    for (int i = 0; i < dim; i++) {
        pat += "[[:space:]]*"
               "([^][[:space:],]+)"
               "[[:space:]]*";
        if (i < dim - 1) {
            pat += ",";
        }
    }
    pat += "\\]"
           "$";

    reg.assign(pat, std::regex_constants::extended);
    if (!std::regex_match(inpt, match, reg)) {
        fail = true;
        return val;
    }

    linalg::resize(val, dim);
    for (int i = 0; i < dim; i++) {
        istr = std::istringstream(match[i + 1]);
        istr >> val[i];
        if (istr.fail() || !istr.eof()) {
            fail = true;
            return val;
        }
    }

    fail = false;
    return val;
}

template <>
linalg::fvector to_vec<linalg::fvector> (std::string inpt, int dim) {
    linalg::fvector val;
    std::string pat = "";
    std::regex reg;
    std::smatch match;
    std::istringstream istr;

    pat += "^"
           "\\[";
    for (int i = 0; i < dim; i++) {
        pat += "[[:space:]]*"
               "([^][[:space:],]+)"
               "[[:space:]]*";
        if (i < dim - 1) {
            pat += ",";
        }
    }
    pat += "\\]"
           "$";

    reg.assign(pat, std::regex_constants::extended);
    if (!std::regex_match(inpt, match, reg)) {
        fail = true;
        return val;
    }

    linalg::resize(val, dim);
    for (int i = 0; i < dim; i++) {
        istr = std::istringstream(match[i + 1]);
        istr >> val[i];
        if (istr.fail() || !istr.eof()) {
            fail = true;
            return val;
        }
    }

    fail = false;
    return val;
}

template <>
linalg::imatrix to_mat<linalg::imatrix> (std::string inpt, int rows, int cols) {
    linalg::imatrix val;
    std::string pat = "";
    std::regex reg;
    std::smatch match;
    std::istringstream istr;

    pat += "^"
           "\\[";
    for (int i = 0; i < rows; i++) {
        pat += "[[:space:]]*"
               "\\[";
        for (int j = 0; j < cols; j++) {
            pat += "[[:space:]]*"
                   "([^][[:space:],]+)"
                   "[[:space:]]*";
            if (j < cols - 1) {
                pat += ",";
            }
        }
        pat += "\\]"
               "[[:space:]]*";
        if (i < rows - 1) {
            pat += ",";
        }
    }
    pat += "\\]"
           "$";

    reg.assign(pat, std::regex_constants::extended);
    if (!std::regex_match(inpt, match, reg)) {
        fail = true;
        return val;
    }

    linalg::resize(val, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            istr = std::istringstream(match[i * cols + j + 1]);
            istr >> val[i][j];
            if (istr.fail() || !istr.eof()) {
                fail = true;
                return val;
            }
        }
    }

    fail = false;
    return val;
}

template <>
linalg::fmatrix to_mat<linalg::fmatrix> (std::string inpt, int rows, int cols) {
    linalg::fmatrix val;
    std::string pat = "";
    std::regex reg;
    std::smatch match;
    std::istringstream istr;

    pat += "^"
           "\\[";
    for (int i = 0; i < rows; i++) {
        pat += "[[:space:]]*"
               "\\[";
        for (int j = 0; j < cols; j++) {
            pat += "[[:space:]]*"
                   "([^][[:space:],]+)"
                   "[[:space:]]*";
            if (j < cols - 1) {
                pat += ",";
            }
        }
        pat += "\\]"
               "[[:space:]]*";
        if (i < rows - 1) {
            pat += ",";
        }
    }
    pat += "\\]"
           "$";

    reg.assign(pat, std::regex_constants::extended);
    if (!std::regex_match(inpt, match, reg)) {
        fail = true;
        return val;
    }

    linalg::resize(val, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            istr = std::istringstream(match[i * cols + j + 1]);
            istr >> val[i][j];
            if (istr.fail() || !istr.eof()) {
                fail = true;
                return val;
            }
        }
    }

    fail = false;
    return val;
}

template <>
std::string parse<std::string> (std::string inpt) {
    std::string val;
    bool escap = false;

    inpt = strip(inpt);

    if (inpt.size() < 2) {
        fail = true;
        return val;
    }

    if (inpt.front() != '"' || inpt.back() != '"') {
        fail = true;
        return val;
    }

    val = "";
    for (auto itr = inpt.begin() + 1; itr < inpt.end() - 1; itr++) {
        if (escap) {
            escap = false;
            if (*itr == '\\' || *itr == '"') {
                val += *itr;
            } else {
                fail = true;
                return val;
            }
        } else {
            if (*itr == '\\') {
                escap = true;
            } else if (*itr == '"') {
                fail = true;
                return val;
            } else {
                val += *itr;
            }
        }
    }

    if (escap) {
        fail = true;
        return val;
    }

    fail = false;
    return val;
}

template <>
std::uint8_t parse<std::uint8_t> (std::string inpt) {
    int val;
    std::istringstream istr;

    inpt = strip(inpt);
    istr = std::istringstream(inpt);

    if (inpt.substr(0, 2) != "0x") {
        fail = true;
        return std::uint8_t(val);
    }

    istr >> std::hex >> val;
    if (istr.fail() || !istr.eof()) {
        fail = true;
        return std::uint8_t(val);
    }

    if (val < 0 || val > 0xFF) {
        fail = true;
        return std::uint8_t(val);
    }

    fail = false;
    return std::uint8_t(val);
}

template <>
bool parse<bool> (std::string inpt) {
    bool val;

    inpt = strip(inpt);

    if (inpt == "yes") {
        val = true;
    } else if (inpt == "no") {
        val = false;
    } else {
        fail = true;
        return val;
    }

    fail = false;
    return val;
}

template <>
int parse<int> (std::string inpt) {
    int val;
    std::istringstream istr;

    inpt = strip(inpt);
    istr = std::istringstream(inpt);

    istr >> val;
    if (istr.fail() || !istr.eof()) {
        fail = true;
        return val;
    }

    fail = false;
    return val;
}

template <>
double parse<double> (std::string inpt) {
    double val;
    std::istringstream istr;

    inpt = strip(inpt);
    istr = std::istringstream(inpt);

    istr >> val;
    if (istr.fail() || !istr.eof()) {
        fail = true;
        return val;
    }

    fail = false;
    return val;
}

template <>
linalg::ivector parse<linalg::ivector> (std::string inpt) {
    linalg::ivector val;
    int dim;

    inpt = strip(inpt);

    if (!is_vec_arb_dim(inpt)) {
        fail = true;
        return val;
    }

    for (dim = 1; !is_vec_fix_dim(inpt, dim); dim++);

    val = to_vec<linalg::ivector>(inpt, dim);
    if (fail) {
        fail = true;
        return val;
    }

    fail = false;
    return val;
}

template <>
linalg::fvector parse<linalg::fvector> (std::string inpt) {
    linalg::fvector val;
    int dim;

    inpt = strip(inpt);

    if (!is_vec_arb_dim(inpt)) {
        fail = true;
        return val;
    }

    for (dim = 1; !is_vec_fix_dim(inpt, dim); dim++);

    val = to_vec<linalg::fvector>(inpt, dim);
    if (fail) {
        fail = true;
        return val;
    }

    fail = false;
    return val;
}

template <>
linalg::imatrix parse<linalg::imatrix> (std::string inpt) {
    linalg::imatrix val;
    int rows, cols;

    inpt = strip(inpt);

    if (!is_mat_arb_rows_arb_cols(inpt)) {
        fail = true;
        return val;
    }

    for (rows = 1; !is_mat_fix_rows_arb_cols(inpt, rows); rows++);
    for (cols = 1; !is_mat_arb_rows_fix_cols(inpt, cols); cols++);

    if (!is_mat_fix_rows_fix_cols(inpt, rows, cols)) {
        fail = true;
        return val;
    }

    val = to_mat<linalg::imatrix>(inpt, rows, cols);
    if (fail) {
        fail = true;
        return val;
    }

    fail = false;
    return val;
}

template <>
linalg::fmatrix parse<linalg::fmatrix> (std::string inpt) {
    linalg::fmatrix val;
    int rows, cols;

    inpt = strip(inpt);

    if (!is_mat_arb_rows_arb_cols(inpt)) {
        fail = true;
        return val;
    }

    for (rows = 1; !is_mat_fix_rows_arb_cols(inpt, rows); rows++);
    for (cols = 1; !is_mat_arb_rows_fix_cols(inpt, cols); cols++);

    if (!is_mat_fix_rows_fix_cols(inpt, rows, cols)) {
        fail = true;
        return val;
    }

    val = to_mat<linalg::fmatrix>(inpt, rows, cols);
    if (fail) {
        fail = true;
        return val;
    }

    fail = false;
    return val;
}

template <>
std::vector<std::string> parse<std::vector<std::string>> (std::string inpt) {
    std::vector<std::string> val;
    std::vector<std::string> arr;

    arr = split(inpt);
    if (fail) {
        fail = true;
        return val;
    }

    for (auto elem : arr) {
        val.push_back(parse<std::string>(elem));
        if (fail) {
            fail = true;
            return val;
        }
    }

    fail = false;
    return val;
}

template <>
std::vector<std::uint8_t> parse<std::vector<std::uint8_t>> (std::string inpt) {
    std::vector<std::uint8_t> val;
    std::vector<std::string> arr;

    arr = split(inpt);
    if (fail) {
        fail = true;
        return val;
    }

    for (auto elem : arr) {
        val.push_back(parse<std::uint8_t>(elem));
        if (fail) {
            fail = true;
            return val;
        }
    }

    fail = false;
    return val;
}

template <>
std::vector<bool> parse<std::vector<bool>> (std::string inpt) {
    std::vector<bool> val;
    std::vector<std::string> arr;

    arr = split(inpt);
    if (fail) {
        fail = true;
        return val;
    }

    for (auto elem : arr) {
        val.push_back(parse<bool>(elem));
        if (fail) {
            fail = true;
            return val;
        }
    }

    fail = false;
    return val;
}

template <>
std::vector<int> parse<std::vector<int>> (std::string inpt) {
    std::vector<int> val;
    std::vector<std::string> arr;

    arr = split(inpt);
    if (fail) {
        fail = true;
        return val;
    }

    for (auto elem : arr) {
        val.push_back(parse<int>(elem));
        if (fail) {
            fail = true;
            return val;
        }
    }

    fail = false;
    return val;
}

template <>
std::vector<double> parse<std::vector<double>> (std::string inpt) {
    std::vector<double> val;
    std::vector<std::string> arr;

    arr = split(inpt);
    if (fail) {
        fail = true;
        return val;
    }

    for (auto elem : arr) {
        val.push_back(parse<double>(elem));
        if (fail) {
            fail = true;
            return val;
        }
    }

    fail = false;
    return val;
}

template <>
std::vector<linalg::ivector> parse<std::vector<linalg::ivector>> (std::string inpt) {
    std::vector<linalg::ivector> val;
    std::vector<std::string> arr;

    arr = split(inpt);
    if (fail) {
        fail = true;
        return val;
    }

    for (auto elem : arr) {
        val.push_back(parse<linalg::ivector>(elem));
        if (fail) {
            fail = true;
            return val;
        }
    }

    fail = false;
    return val;
}

template <>
std::vector<linalg::fvector> parse<std::vector<linalg::fvector>> (std::string inpt) {
    std::vector<linalg::fvector> val;
    std::vector<std::string> arr;

    arr = split(inpt);
    if (fail) {
        fail = true;
        return val;
    }

    for (auto elem : arr) {
        val.push_back(parse<linalg::fvector>(elem));
        if (fail) {
            fail = true;
            return val;
        }
    }

    fail = false;
    return val;
}

template <>
std::vector<linalg::imatrix> parse<std::vector<linalg::imatrix>> (std::string inpt) {
    std::vector<linalg::imatrix> val;
    std::vector<std::string> arr;

    arr = split(inpt);
    if (fail) {
        fail = true;
        return val;
    }

    for (auto elem : arr) {
        val.push_back(parse<linalg::imatrix>(elem));
        if (fail) {
            fail = true;
            return val;
        }
    }

    fail = false;
    return val;
}

template <>
std::vector<linalg::fmatrix> parse<std::vector<linalg::fmatrix>> (std::string inpt) {
    std::vector<linalg::fmatrix> val;
    std::vector<std::string> arr;

    arr = split(inpt);
    if (fail) {
        fail = true;
        return val;
    }

    for (auto elem : arr) {
        val.push_back(parse<linalg::fmatrix>(elem));
        if (fail) {
            fail = true;
            return val;
        }
    }

    fail = false;
    return val;
}

}

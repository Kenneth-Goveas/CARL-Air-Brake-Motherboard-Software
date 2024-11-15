#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <regex>

#include <math/linalg.hpp>

#include <sys/logging.hpp>

#include <ui/input.hpp>

namespace input :: intern {

std::string mod = "input";

bool fail;
bool eof;

bool vec_sntx_arbit_len (std::string inpt);
bool vec_sntx_fixed_len (std::string inpt, int len);

bool mat_sntx_arbit_rows_arbit_cols (std::string inpt);
bool mat_sntx_fixed_rows_arbit_cols (std::string inpt, int rows);
bool mat_sntx_arbit_rows_fixed_cols (std::string inpt, int cols);
bool mat_sntx_fixed_rows_fixed_cols (std::string inpt, int rows, int cols);

template <typename type>
type vec_parse (std::string inpt, int len);

template <>
linalg::ivector vec_parse<linalg::ivector> (std::string inpt, int len);

template <>
linalg::fvector vec_parse<linalg::fvector> (std::string inpt, int len);

template <typename type>
type mat_parse (std::string inpt, int rows, int cols);

template <>
linalg::imatrix mat_parse<linalg::imatrix> (
    std::string inpt, int rows, int cols
);

template <>
linalg::fmatrix mat_parse<linalg::fmatrix> (
    std::string inpt, int rows, int cols
);

template <typename type>
type conv_inpt_to_val (std::string inpt);

template <>
bool conv_inpt_to_val<bool> (std::string inpt);

template <>
std::string conv_inpt_to_val<std::string> (std::string inpt);

template <>
int conv_inpt_to_val<int> (std::string inpt);

template <>
double conv_inpt_to_val<double> (std::string inpt);

template <>
linalg::ivector conv_inpt_to_val<linalg::ivector> (std::string inpt);

template <>
linalg::fvector conv_inpt_to_val<linalg::fvector> (std::string inpt);

template <>
linalg::imatrix conv_inpt_to_val<linalg::imatrix> (std::string inpt);

template <>
linalg::fmatrix conv_inpt_to_val<linalg::fmatrix> (std::string inpt);

std::string conv_val_to_oupt (bool val);
std::string conv_val_to_oupt (std::string val);
std::string conv_val_to_oupt (int val);
std::string conv_val_to_oupt (double val);
std::string conv_val_to_oupt (linalg::ivector val);
std::string conv_val_to_oupt (linalg::fvector val);
std::string conv_val_to_oupt (linalg::imatrix val);
std::string conv_val_to_oupt (linalg::fmatrix val);

std::string strip (std::string inpt);

}

namespace input {

bool fail (void) {
    return intern::fail;
}

bool eof (void) {
    return intern::eof;
}

template <>
bool get<bool> (void) {
    bool val;
    std::string inpt, oupt;

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
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    inpt = intern::strip(inpt);
    val = intern::conv_inpt_to_val<bool>(inpt);
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

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::string get<std::string> (void) {
    std::string val;
    std::string inpt, oupt;

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
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    inpt = intern::strip(inpt);
    val = intern::conv_inpt_to_val<std::string>(inpt);
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

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
int get<int> (void) {
    int val;
    std::string inpt, oupt;

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
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    inpt = intern::strip(inpt);
    val = intern::conv_inpt_to_val<int>(inpt);
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

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
double get<double> (void) {
    double val;
    std::string inpt, oupt;

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
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    inpt = intern::strip(inpt);
    val = intern::conv_inpt_to_val<double>(inpt);
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

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::ivector get<linalg::ivector> (void) {
    linalg::ivector val;
    std::string inpt, oupt;

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
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    inpt = intern::strip(inpt);
    val = intern::conv_inpt_to_val<linalg::ivector>(inpt);
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

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::fvector get<linalg::fvector> (void) {
    linalg::fvector val;
    std::string inpt, oupt;

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
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    inpt = intern::strip(inpt);
    val = intern::conv_inpt_to_val<linalg::fvector>(inpt);
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

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::imatrix get<linalg::imatrix> (void) {
    linalg::imatrix val;
    std::string inpt, oupt;

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
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    inpt = intern::strip(inpt);
    val = intern::conv_inpt_to_val<linalg::imatrix>(inpt);
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

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::fmatrix get<linalg::fmatrix> (void) {
    linalg::fmatrix val;
    std::string inpt, oupt;

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
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input"
    );

    inpt = intern::strip(inpt);
    val = intern::conv_inpt_to_val<linalg::fmatrix>(inpt);
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

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

}

namespace input :: intern {

bool vec_sntx_arbit_len (std::string inpt) {
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
    if (std::regex_match(inpt, reg)) {
        return true;
    } else {
        return false;
    }
}

bool vec_sntx_fixed_len (std::string inpt, int len) {
    std::string pat = "";
    std::regex reg;

    pat += "^"
           "\\[";
    for (int i = 0; i < len; i++) {
        pat += "[[:space:]]*"
               "[^][[:space:],]+"
               "[[:space:]]*";
        if (i < len - 1) {
            pat += ",";
        }
    }
    pat += "\\]"
           "$";

    reg.assign(pat, std::regex_constants::extended);
    if (std::regex_match(inpt, reg)) {
        return true;
    } else {
        return false;
    }
}

bool mat_sntx_arbit_rows_arbit_cols (std::string inpt) {
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
    if (std::regex_match(inpt, reg)) {
        return true;
    } else {
        return false;
    }
}

bool mat_sntx_fixed_rows_arbit_cols (std::string inpt, int rows) {
    std::string pat = "";
    std::regex reg;

    pat += "^"
           "\\[";
    for (int i = 0; i < rows; i++) {
        pat +=  "[[:space:]]*"
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
    if (std::regex_match(inpt, reg)) {
        return true;
    } else {
        return false;
    }
}

bool mat_sntx_arbit_rows_fixed_cols (std::string inpt, int cols) {
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
    if (std::regex_match(inpt, reg)) {
        return true;
    } else {
        return false;
    }
}

bool mat_sntx_fixed_rows_fixed_cols (std::string inpt, int rows, int cols) {
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
    if (std::regex_match(inpt, reg)) {
        return true;
    } else {
        return false;
    }
}

template <>
linalg::ivector vec_parse<linalg::ivector> (std::string inpt, int len) {
    std::string pat = "";
    std::regex reg;
    std::smatch match;
    std::istringstream istr;
    linalg::ivector val;

    pat += "^"
           "\\[";
    for (int i = 0; i < len; i++) {
        pat += "[[:space:]]*"
               "([^][[:space:],]+)"
               "[[:space:]]*";
        if (i < len - 1) {
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

    linalg::resize(&val, len);
    for (int i = 0; i < len; i++) {
        istr = std::istringstream(match[i + 1]);
        istr >> std::noskipws >> val[i];
        if (istr.fail()) {
            fail = true;
            return val;
        } else if (!istr.eof()) {
            fail = true;
            return val;
        }
    }

    fail = false;
    return val;
}

template <>
linalg::fvector vec_parse<linalg::fvector> (std::string inpt, int len) {
    std::string pat = "";
    std::regex reg;
    std::smatch match;
    std::istringstream istr;
    linalg::fvector val;

    pat += "^"
           "\\[";
    for (int i = 0; i < len; i++) {
        pat += "[[:space:]]*"
               "([^][[:space:],]+)"
               "[[:space:]]*";
        if (i < len - 1) {
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

    linalg::resize(&val, len);
    for (int i = 0; i < len; i++) {
        istr = std::istringstream(match[i + 1]);
        istr >> std::noskipws >> val[i];
        if (istr.fail()) {
            fail = true;
            return val;
        } else if (!istr.eof()) {
            fail = true;
            return val;
        }
    }

    fail = false;
    return val;
}

template <>
linalg::imatrix mat_parse<linalg::imatrix> (
    std::string inpt, int rows, int cols
) {
    std::string pat = "";
    std::regex reg;
    std::smatch match;
    std::istringstream istr;
    linalg::imatrix val;

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

    linalg::resize(&val, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            istr = std::istringstream(match[i * cols + j + 1]);
            istr >> std::noskipws >> val[i][j];
            if (istr.fail()) {
                fail = true;
                return val;
            } else if (!istr.eof()) {
                fail = true;
                return val;
            }
        }
    }

    fail = false;
    return val;
}

template <>
linalg::fmatrix mat_parse<linalg::fmatrix> (
    std::string inpt, int rows, int cols
) {
    std::string pat = "";
    std::regex reg;
    std::smatch match;
    std::istringstream istr;
    linalg::fmatrix val;

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

    linalg::resize(&val, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            istr = std::istringstream(match[i * cols + j + 1]);
            istr >> std::noskipws >> val[i][j];
            if (istr.fail()) {
                fail = true;
                return val;
            } else if (!istr.eof()) {
                fail = true;
                return val;
            }
        }
    }

    fail = false;
    return val;
}

template <>
bool conv_inpt_to_val<bool> (std::string inpt) {
    bool val;
    if (inpt == "yes") {
        val = true;
        fail = false;
        return val;
    } else if (inpt == "no") {
        val = false;
        fail = false;
        return val;
    } else {
        fail = true;
        return val;
    }
}

template <>
std::string conv_inpt_to_val<std::string> (std::string inpt) {
    bool escap = false;
    std::string val;

    if (inpt.size() < 2) {
        fail = true;
        return val;
    }

    if (inpt[0] != '"' || inpt[inpt.size() - 1] != '"') {
        fail = true;
        return val;
    }

    val = "";
    for (auto i = 1; i < inpt.size() - 1; i++) {
        if (escap) {
            escap = false;
            if (inpt[i] == '\\' || inpt[i] == '"') {
                val += inpt[i];
            } else {
                fail = true;
                return val;
            }
        } else {
            if (inpt[i] == '\\') {
                escap = true;
            } else if (inpt[i] == '"') {
                fail = true;
                return val;
            } else {
                val += inpt[i];
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
int conv_inpt_to_val<int> (std::string inpt) {
    std::istringstream istr(inpt);
    int val;

    istr >> std::noskipws >> val;
    if (istr.fail()) {
        fail = true;
        return val;
    } else if (!istr.eof()) {
        fail = true;
        return val;
    }

    fail = false;
    return val;
}

template <>
double conv_inpt_to_val<double> (std::string inpt) {
    std::istringstream istr(inpt);
    double val;

    istr >> std::noskipws >> val;
    if (istr.fail()) {
        fail = true;
        return val;
    } else if (!istr.eof()) {
        fail = true;
        return val;
    }

    fail = false;
    return val;
}

template <>
linalg::ivector conv_inpt_to_val<linalg::ivector> (std::string inpt) {
    int len;
    linalg::ivector val;

    if (!vec_sntx_arbit_len(inpt)) {
        fail = true;
        return val;
    }

    for (len = 1; !vec_sntx_fixed_len(inpt, len); len++);

    val = vec_parse<linalg::ivector>(inpt, len);
    if (fail) {
        fail = true;
        return val;
    } else {
        fail = false;
        return val;
    }
}

template <>
linalg::fvector conv_inpt_to_val<linalg::fvector> (std::string inpt) {
    int len;
    linalg::fvector val;

    if (!vec_sntx_arbit_len(inpt)) {
        fail = true;
        return val;
    }

    for (len = 1; !vec_sntx_fixed_len(inpt, len); len++);

    val = vec_parse<linalg::fvector>(inpt, len);
    if (fail) {
        fail = true;
        return val;
    } else {
        fail = false;
        return val;
    }
}

template <>
linalg::imatrix conv_inpt_to_val<linalg::imatrix> (std::string inpt) {
    int rows, cols;
    linalg::imatrix val;

    if (!mat_sntx_arbit_rows_arbit_cols(inpt)) {
        fail = true;
        return val;
    }

    for (rows = 1; !mat_sntx_fixed_rows_arbit_cols(inpt, rows); rows++);
    for (cols = 1; !mat_sntx_arbit_rows_fixed_cols(inpt, cols); cols++);

    if (!mat_sntx_fixed_rows_fixed_cols(inpt, rows, cols)) {
        fail = true;
        return val;
    }

    val = mat_parse<linalg::imatrix>(inpt, rows, cols);
    if (fail) {
        fail = true;
        return val;
    } else {
        fail = false;
        return val;
    }
}

template <>
linalg::fmatrix conv_inpt_to_val<linalg::fmatrix> (std::string inpt) {
    int rows, cols;
    linalg::fmatrix val;

    if (!mat_sntx_arbit_rows_arbit_cols(inpt)) {
        fail = true;
        return val;
    }

    for (rows = 1; !mat_sntx_fixed_rows_arbit_cols(inpt, rows); rows++);
    for (cols = 1; !mat_sntx_arbit_rows_fixed_cols(inpt, cols); cols++);

    if (!mat_sntx_fixed_rows_fixed_cols(inpt, rows, cols)) {
        fail = true;
        return val;
    }

    val = mat_parse<linalg::fmatrix>(inpt, rows, cols);
    if (fail) {
        fail = true;
        return val;
    } else {
        fail = false;
        return val;
    }
}

std::string conv_val_to_oupt (bool val) {
    std::string oupt;
    if (val) {
        oupt = "True";
    } else {
        oupt = "False";
    }
    return oupt;
}

std::string conv_val_to_oupt (std::string val) {
    std::string oupt;
    oupt = std::string("“") + val + std::string("”");
    return oupt;
}

std::string conv_val_to_oupt (int val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << std::showpos << val;
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (double val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << std::showpos << std::scientific << std::setprecision(2) << val;
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (linalg::ivector val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        ostr << std::showpos << val[i];
        if (i < linalg::dim(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (linalg::fvector val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        ostr << std::showpos << std::scientific << std::setprecision(2)
             << val[i];
        if (i < linalg::dim(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (linalg::imatrix val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (int i = 0; i < linalg::rows(val); i++) {
        ostr << "[";
        for (int j = 0; j < linalg::cols(val); j++) {
            ostr << std::showpos << val[i][j];
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
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (linalg::fmatrix val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (int i = 0; i < linalg::rows(val); i++) {
        ostr << "[";
        for (int j = 0; j < linalg::cols(val); j++) {
            ostr << std::showpos << std::scientific << std::setprecision(2)
                 << val[i][j];
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
    oupt = ostr.str();

    return oupt;
}

std::string strip (std::string inpt) {
    while (inpt.size() > 0) {
        if (*(inpt.begin()) == ' ' || *(inpt.begin()) == '\t') {
            inpt.erase(inpt.begin());
        } else {
            break;
        }
    }

    while (inpt.size() > 0) {
        if (*(inpt.end() - 1) == ' ' || *(inpt.end() - 1) == '\t') {
            inpt.erase(inpt.end() - 1);
        } else {
            break;
        }
    }

    return inpt;
}

}

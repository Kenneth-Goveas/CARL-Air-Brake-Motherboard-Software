#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <regex>

#include <math/linalg.hpp>

#include <sys/logging.hpp>

#include <ui/term.hpp>

namespace term :: intern {

std::string mod = "term";

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

template <typename type>
std::string conv_val_to_oupt (type val);

template <>
std::string conv_val_to_oupt<bool> (bool val);

template <>
std::string conv_val_to_oupt<std::string> (std::string val);

template <>
std::string conv_val_to_oupt<int> (int val);

template <>
std::string conv_val_to_oupt<double> (double val);

template <>
std::string conv_val_to_oupt<linalg::ivector> (linalg::ivector val);

template <>
std::string conv_val_to_oupt<linalg::fvector> (linalg::fvector val);

template <>
std::string conv_val_to_oupt<linalg::imatrix> (linalg::imatrix val);

template <>
std::string conv_val_to_oupt<linalg::fmatrix> (linalg::fmatrix val);

std::string conv_color_to_oupt (color clr);

std::string strip (std::string inpt);

void append (std::ostringstream * ostr);

template <typename type, typename ... types>
void append (std::ostringstream * ostr, type arg, types ... args);

}

namespace term {

bool fail (void) {
    return intern::fail;
}

bool eof (void) {
    return intern::eof;
}

void erase_line (void) {
    logging::inf(intern::mod,
        "Erasing line"
    );

    std::cout << "\e[2K";
    std::cout.flush();
}

void erase_screen (void) {
    logging::inf(intern::mod,
        "Erasing screen"
    );

    std::cout << "\e[2J";
    std::cout.flush();
}

void color_default (void) {
    logging::inf(intern::mod,
        "Setting default colors"
    );

    std::cout << "\e[m";
    std::cout.flush();
}

void color_fore (color clr) {
    std::string oupt;

    oupt = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Setting foreground color: ", oupt
    );

    std::cout << "\e[38;5;" << clr << "m";
    std::cout.flush();
}

void color_back (color clr) {
    std::string oupt;

    oupt = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Setting background color: ", oupt
    );

    std::cout << "\e[48;5;" << clr << "m";
    std::cout.flush();
}

void cursor_hide (void) {
    logging::inf(intern::mod,
        "Hiding cursor"
    );

    std::cout << "\e[?25l";
    std::cout.flush();
}

void cursor_show (void) {
    logging::inf(intern::mod,
        "Revealing cursor"
    );

    std::cout << "\e[?25h";
    std::cout.flush();
}

void cursor_row (int row) {
    logging::inf(intern::mod,
        "Moving cursor: Row: ", row
    );

    if (!(row > 0)) {
        logging::err(intern::mod,
            "Failed to move cursor (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    std::cout << "\e[" << row << "d";
    std::cout.flush();

    intern::fail = false;
}

void cursor_col (int col) {
    logging::inf(intern::mod,
        "Moving cursor: Col: ", col
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to move cursor (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    std::cout << "\e[" << col << "G";
    std::cout.flush();

    intern::fail = false;
}

void cursor_pos (int row, int col) {
    logging::inf(intern::mod,
        "Moving cursor: Row: ", row, ", Col: ", col
    );

    if (!(row > 0 && col > 0)) {
        logging::err(intern::mod,
            "Failed to move cursor (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    std::cout << "\e[" << row << ";" << col << "H";
    std::cout.flush();

    intern::fail = false;
}

void newline (void) {
    logging::inf(intern::mod,
        "Printing newline"
    );

    std::cout << std::endl;
}

template <>
bool scan<bool> (void) {
    std::string inpt, oupt;
    bool val;

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input supplied"
        );
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    inpt = intern::strip(inpt);
    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input as boolean"
    );

    val = intern::conv_inpt_to_val<bool>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input as boolean (Syntax error)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input as boolean: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::string scan<std::string> (void) {
    std::string inpt, oupt;
    std::string val;

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input supplied"
        );
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    inpt = intern::strip(inpt);
    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input as string"
    );

    val = intern::conv_inpt_to_val<std::string>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input as string (Syntax error)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input as string: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
int scan<int> (void) {
    std::string inpt, oupt;
    int val;

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input supplied"
        );
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    inpt = intern::strip(inpt);
    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input as integer scalar"
    );

    val = intern::conv_inpt_to_val<int>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input as integer scalar (Syntax error)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input as integer scalar: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
double scan<double> (void) {
    std::string inpt, oupt;
    double val;

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input supplied"
        );
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    inpt = intern::strip(inpt);
    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input as floating point scalar"
    );

    val = intern::conv_inpt_to_val<double>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input as floating point scalar (Syntax error)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input as floating point scalar: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::ivector scan<linalg::ivector> (void) {
    std::string inpt, oupt;
    linalg::ivector val;

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input supplied"
        );
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    inpt = intern::strip(inpt);
    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input as integer vector"
    );

    val = intern::conv_inpt_to_val<linalg::ivector>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input as integer vector (Syntax error)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input as integer vector: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::fvector scan<linalg::fvector> (void) {
    std::string inpt, oupt;
    linalg::fvector val;

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input supplied"
        );
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    inpt = intern::strip(inpt);
    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input as floating point vector"
    );

    val = intern::conv_inpt_to_val<linalg::fvector>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input as floating point vector (Syntax error)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input as floating point vector: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::imatrix scan<linalg::imatrix> (void) {
    std::string inpt, oupt;
    linalg::imatrix val;

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input supplied"
        );
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    inpt = intern::strip(inpt);
    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input as integer matrix"
    );

    val = intern::conv_inpt_to_val<linalg::imatrix>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input as integer matrix (Syntax error)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input as integer matrix: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::fmatrix scan<linalg::fmatrix> (void) {
    std::string inpt, oupt;
    linalg::fmatrix val;

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);
    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input supplied"
        );
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    inpt = intern::strip(inpt);
    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input as floating point matrix"
    );

    val = intern::conv_inpt_to_val<linalg::fmatrix>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input as floating point matrix (Syntax error)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input as floating point matrix: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

}

namespace term :: intern {

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

template <>
std::string conv_val_to_oupt<bool> (bool val) {
    std::string oupt;
    if (val) {
        oupt = "True";
    } else {
        oupt = "False";
    }
    return oupt;
}

template <>
std::string conv_val_to_oupt<std::string> (std::string val) {
    std::string oupt;
    oupt = std::string("“") + val + std::string("”");
    return oupt;
}

template <>
std::string conv_val_to_oupt<int> (int val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << std::showpos << val;
    oupt = ostr.str();

    return oupt;
}

template <>
std::string conv_val_to_oupt<double> (double val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << std::showpos << std::scientific << std::setprecision(2) << val;
    oupt = ostr.str();

    return oupt;
}

template <>
std::string conv_val_to_oupt<linalg::ivector> (linalg::ivector val) {
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

template <>
std::string conv_val_to_oupt<linalg::fvector> (linalg::fvector val) {
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

template <>
std::string conv_val_to_oupt<linalg::imatrix> (linalg::imatrix val) {
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

template <>
std::string conv_val_to_oupt<linalg::fmatrix> (linalg::fmatrix val) {
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

std::string conv_color_to_oupt (color clr) {
    std::string oupt;

    switch (clr) {
        case black:
            oupt = "Black";
            break;
        case darkred:
            oupt = "Dark red";
            break;
        case darkgreen:
            oupt = "Dark green";
            break;
        case darkyellow:
            oupt = "Dark yellow";
            break;
        case darkblue:
            oupt = "Dark blue";
            break;
        case darkmagenta:
            oupt = "Dark magenta";
            break;
        case darkcyan:
            oupt = "Dark cyan";
            break;
        case lightgray:
            oupt = "Light gray";
            break;
        case darkgray:
            oupt = "Dark gray";
            break;
        case lightred:
            oupt = "Light red";
            break;
        case lightgreen:
            oupt = "Light green";
            break;
        case lightyellow:
            oupt = "Light yellow";
            break;
        case lightblue:
            oupt = "Light blue";
            break;
        case lightmagenta:
            oupt = "Light magenta";
            break;
        case lightcyan:
            oupt = "Light cyan";
            break;
        case white:
            oupt = "White";
            break;
    }

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

void append (std::ostringstream * ostr) {}

}

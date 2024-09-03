#include <cstring>
#include <cerrno>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

#include <math/linalg.hpp>

#include <sys/logging.hpp>
#include <sys/config.hpp>

namespace config :: intern {

std::string mod = "config";

bool fail;

std::vector<std::string> name_all;

std::vector<std::string> name_bool;
std::vector<bool> val_bool;

std::vector<std::string> name_str;
std::vector<std::string> val_str;

std::vector<std::string> name_iscl;
std::vector<int> val_iscl;

std::vector<std::string> name_fscl;
std::vector<double> val_fscl;

std::vector<std::string> name_ivec;
std::vector<linalg::ivector> val_ivec;

std::vector<std::string> name_fvec;
std::vector<linalg::fvector> val_fvec;

std::vector<std::string> name_imat;
std::vector<linalg::imatrix> val_imat;

std::vector<std::string> name_fmat;
std::vector<linalg::fmatrix> val_fmat;

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

template <typename type>
std::string param (std::string name, std::string inpt);

template <>
std::string param<bool> (std::string name, std::string inpt);

template <>
std::string param<std::string> (std::string name, std::string inpt);

template <>
std::string param<int> (std::string name, std::string inpt);

template <>
std::string param<double> (std::string name, std::string inpt);

template <>
std::string param<linalg::ivector> (std::string name, std::string inpt);

template <>
std::string param<linalg::fvector> (std::string name, std::string inpt);

template <>
std::string param<linalg::imatrix> (std::string name, std::string inpt);

template <>
std::string param<linalg::fmatrix> (std::string name, std::string inpt);

std::string strip (std::string line);
bool blank (std::string line);
std::vector<std::string> split (std::string line);
bool exists (std::string name);

void parse (std::string line);

}

namespace config {

bool fail (void) {
    return intern::fail;
}

void init (std::string path) {
    std::ifstream file;
    std::string line;

    logging::inf(intern::mod,
        "Opening config file: ", path
    );

    file.open(path);
    if (file.fail()) {
        logging::err(intern::mod,
            "Failed to open config file ",
            "(", std::strerror(errno), ")"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Parsing config file"
    );

    while (true) {
        std::getline(file, line);
        if (file.fail()) {
            if (file.eof()) {
                file.clear();
                break;
            } else {
                logging::err(intern::mod,
                    "Failed to read config file ",
                    "(", std::strerror(errno), ")"
                );
                logging::wrn(intern::mod,
                    "Closing config file"
                );
                file.clear();
                file.close();
                if (file.fail()) {
                    logging::err(intern::mod,
                        "Failed to close config file ",
                        "(", std::strerror(errno), ")"
                    );
                }
                intern::fail = true;
                return;
            }
        } else {
            intern::parse(line);
            if (intern::fail) {
                logging::err(intern::mod,
                    "Failed to parse config file"
                );
                logging::wrn(intern::mod,
                    "Closing config file"
                );
                file.close();
                if (file.fail()) {
                    logging::err(intern::mod,
                        "Failed to close config file ",
                        "(", std::strerror(errno), ")"
                    );
                }
                intern::fail = true;
                return;
            }
        }
    }

    logging::inf(intern::mod,
        "Closing config file"
    );

    file.close();
    if (file.fail()) {
        logging::err(intern::mod,
            "Failed to close config file ",
            "(", std::strerror(errno), ")"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

template <>
bool get<bool> (std::string name) {
    bool val;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting boolean parameter: Name: “", name, "”"
    );

    for (auto i = 0; i < intern::name_bool.size(); i++) {
        if (name == intern::name_bool[i]) {
            val = intern::val_bool[i];
            oupt = intern::conv_val_to_oupt(val);
            logging::inf(intern::mod,
                "Got boolean parameter: Value: ", oupt
            );
            intern::fail = false;
            return val;
        }
    }

    logging::err(intern::mod,
        "Failed to get boolean parameter (Name not found)"
    );

    intern::fail = true;
    return val;
}

template <>
std::string get<std::string> (std::string name) {
    std::string val;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting string parameter: Name: “", name, "”"
    );

    for (auto i = 0; i < intern::name_str.size(); i++) {
        if (name == intern::name_str[i]) {
            val = intern::val_str[i];
            oupt = intern::conv_val_to_oupt(val);
            logging::inf(intern::mod,
                "Got string parameter: Value: ", oupt
            );
            intern::fail = false;
            return val;
        }
    }

    logging::err(intern::mod,
        "Failed to get string parameter (Name not found)"
    );

    intern::fail = true;
    return val;
}

template <>
int get<int> (std::string name) {
    int val;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting integer scalar parameter: Name: “", name, "”"
    );

    for (auto i = 0; i < intern::name_iscl.size(); i++) {
        if (name == intern::name_iscl[i]) {
            val = intern::val_iscl[i];
            oupt = intern::conv_val_to_oupt(val);
            logging::inf(intern::mod,
                "Got integer scalar parameter: Value: ", oupt
            );
            intern::fail = false;
            return val;
        }
    }

    logging::err(intern::mod,
        "Failed to get integer scalar parameter (Name not found)"
    );

    intern::fail = true;
    return val;
}

template <>
double get<double> (std::string name) {
    double val;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting floating point scalar parameter: Name: “", name, "”"
    );

    for (auto i = 0; i < intern::name_fscl.size(); i++) {
        if (name == intern::name_fscl[i]) {
            val = intern::val_fscl[i];
            oupt = intern::conv_val_to_oupt(val);
            logging::inf(intern::mod,
                "Got floating point scalar parameter: Value: ", oupt
            );
            intern::fail = false;
            return val;
        }
    }

    logging::err(intern::mod,
        "Failed to get floating point scalar parameter (Name not found)"
    );

    intern::fail = true;
    return val;
}

template <>
linalg::ivector get<linalg::ivector> (std::string name) {
    linalg::ivector val;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting integer vector parameter: Name: “", name, "”"
    );

    for (auto i = 0; i < intern::name_ivec.size(); i++) {
        if (name == intern::name_ivec[i]) {
            val = intern::val_ivec[i];
            oupt = intern::conv_val_to_oupt(val);
            logging::inf(intern::mod,
                "Got integer vector parameter: Value: ", oupt
            );
            intern::fail = false;
            return val;
        }
    }

    logging::err(intern::mod,
        "Failed to get integer vector parameter (Name not found)"
    );

    intern::fail = true;
    return val;
}

template <>
linalg::fvector get<linalg::fvector> (std::string name) {
    linalg::fvector val;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting floating point vector parameter: Name: “", name, "”"
    );

    for (auto i = 0; i < intern::name_fvec.size(); i++) {
        if (name == intern::name_fvec[i]) {
            val = intern::val_fvec[i];
            oupt = intern::conv_val_to_oupt(val);
            logging::inf(intern::mod,
                "Got floating point vector parameter: Value: ", oupt
            );
            intern::fail = false;
            return val;
        }
    }

    logging::err(intern::mod,
        "Failed to get floating point vector parameter (Name not found)"
    );

    intern::fail = true;
    return val;
}

template <>
linalg::imatrix get<linalg::imatrix> (std::string name) {
    linalg::imatrix val;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting integer matrix parameter: Name: “", name, "”"
    );

    for (auto i = 0; i < intern::name_imat.size(); i++) {
        if (name == intern::name_imat[i]) {
            val = intern::val_imat[i];
            oupt = intern::conv_val_to_oupt(val);
            logging::inf(intern::mod,
                "Got integer matrix parameter: Value: ", oupt
            );
            intern::fail = false;
            return val;
        }
    }

    logging::err(intern::mod,
        "Failed to get integer matrix parameter (Name not found)"
    );

    intern::fail = true;
    return val;
}

template <>
linalg::fmatrix get<linalg::fmatrix> (std::string name) {
    linalg::fmatrix val;
    std::string oupt;

    logging::inf(intern::mod,
        "Getting floating point matrix parameter: Name: “", name, "”"
    );

    for (auto i = 0; i < intern::name_fmat.size(); i++) {
        if (name == intern::name_fmat[i]) {
            val = intern::val_fmat[i];
            oupt = intern::conv_val_to_oupt(val);
            logging::inf(intern::mod,
                "Got floating point matrix parameter: Value: ", oupt
            );
            intern::fail = false;
            return val;
        }
    }

    logging::err(intern::mod,
        "Failed to get floating point matrix parameter (Name not found)"
    );

    intern::fail = true;
    return val;
}

}

namespace config :: intern {

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
    if (inpt == "true") {
        val = true;
        fail = false;
        return val;
    } else if (inpt == "false") {
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

template <>
std::string param<bool> (std::string name, std::string inpt) {
    bool val;
    std::string oupt;

    val = conv_inpt_to_val<bool>(inpt);
    if (fail) {
        fail = true;
        return oupt;
    }
    oupt = conv_val_to_oupt(val);

    name_all.push_back(name);
    name_bool.push_back(name);
    val_bool.push_back(val);

    fail = false;
    return oupt;
}

template <>
std::string param<std::string> (std::string name, std::string inpt) {
    std::string val;
    std::string oupt;

    val = conv_inpt_to_val<std::string>(inpt);
    if (fail) {
        fail = true;
        return oupt;
    }
    oupt = conv_val_to_oupt(val);

    name_all.push_back(name);
    name_str.push_back(name);
    val_str.push_back(val);

    fail = false;
    return oupt;
}

template <>
std::string param<int> (std::string name, std::string inpt) {
    int val;
    std::string oupt;

    val = conv_inpt_to_val<int>(inpt);
    if (fail) {
        fail = true;
        return oupt;
    }
    oupt = conv_val_to_oupt(val);

    name_all.push_back(name);
    name_iscl.push_back(name);
    val_iscl.push_back(val);

    fail = false;
    return oupt;
}

template <>
std::string param<double> (std::string name, std::string inpt) {
    double val;
    std::string oupt;

    val = conv_inpt_to_val<double>(inpt);
    if (fail) {
        fail = true;
        return oupt;
    }
    oupt = conv_val_to_oupt(val);

    name_all.push_back(name);
    name_fscl.push_back(name);
    val_fscl.push_back(val);

    fail = false;
    return oupt;
}

template <>
std::string param<linalg::ivector> (std::string name, std::string inpt) {
    linalg::ivector val;
    std::string oupt;

    val = conv_inpt_to_val<linalg::ivector>(inpt);
    if (fail) {
        fail = true;
        return oupt;
    }
    oupt = conv_val_to_oupt(val);

    name_all.push_back(name);
    name_ivec.push_back(name);
    val_ivec.push_back(val);

    fail = false;
    return oupt;
}

template <>
std::string param<linalg::fvector> (std::string name, std::string inpt) {
    linalg::fvector val;
    std::string oupt;

    val = conv_inpt_to_val<linalg::fvector>(inpt);
    if (fail) {
        fail = true;
        return oupt;
    }
    oupt = conv_val_to_oupt(val);

    name_all.push_back(name);
    name_fvec.push_back(name);
    val_fvec.push_back(val);

    fail = false;
    return oupt;
}

template <>
std::string param<linalg::imatrix> (std::string name, std::string inpt) {
    linalg::imatrix val;
    std::string oupt;

    val = conv_inpt_to_val<linalg::imatrix>(inpt);
    if (fail) {
        fail = true;
        return oupt;
    }
    oupt = conv_val_to_oupt(val);

    name_all.push_back(name);
    name_imat.push_back(name);
    val_imat.push_back(val);

    fail = false;
    return oupt;
}

template <>
std::string param<linalg::fmatrix> (std::string name, std::string inpt) {
    linalg::fmatrix val;
    std::string oupt;

    val = conv_inpt_to_val<linalg::fmatrix>(inpt);
    if (fail) {
        fail = true;
        return oupt;
    }
    oupt = conv_val_to_oupt(val);

    name_all.push_back(name);
    name_fmat.push_back(name);
    val_fmat.push_back(val);

    fail = false;
    return oupt;
}

std::string strip (std::string line) {
    bool quot = false, escap = false;

    for (auto i = 0; i < line.size(); i++) {
        if (quot) {
            if (escap) {
                escap = false;
            } else {
                if (line[i] == '\\') {
                    escap = true;
                } else if (line[i] == '"') {
                    quot = false;
                }
            }
        } else {
            if (line[i] == '"') {
                quot = true;
            } else if (line[i] == '#') {
                line.resize(i);
                break;
            }
        }
    }

    return line;
}

bool blank (std::string line) {
    for (auto i = 0; i < line.size(); i++) {
        if (line[i] != ' ' && line[i] != '\t') {
            return false;
        }
    }
    return true;
}

std::vector<std::string> split (std::string line) {
    std::string pat;
    std::regex reg;
    std::smatch match;
    std::vector<std::string> name_inpt;

    pat = "^"
          "[[:space:]]*"
          "([[:alpha:]][[:alnum:]_]*)"
          "[[:space:]]*"
          "="
          "[[:space:]]*"
          "([^[:space:]]|[^[:space:]].*[^[:space:]])"
          "[[:space:]]*"
          "$";

    reg.assign(pat, std::regex_constants::extended);
    if (std::regex_match(line, match, reg)) {
        name_inpt.push_back(match[1]);
        name_inpt.push_back(match[2]);
        fail = false;
        return name_inpt;
    } else {
        fail = true;
        return name_inpt;
    }
}

bool exists (std::string name) {
    for (auto i = 0; i < name_all.size(); i++) {
        if (name == name_all[i]) {
            return true;
        }
    }
    return false;
}

void parse (std::string line) {
    std::vector<std::string> name_inpt;
    std::string name, inpt, oupt;

    logging::inf(mod,
        "Parsing line: “", line, "”"
    );

    line = strip(line);

    if (blank(line)) {
        logging::inf(mod,
            "Parsed line as blank"
        );
        fail = false;
        return;
    }

    name_inpt = split(line);
    if (fail) {
        logging::err(mod,
            "Failed to parse line (Syntax error)"
        );
        fail = true;
        return;
    }

    name = name_inpt[0];
    inpt = name_inpt[1];

    if (exists(name)) {
        logging::err(mod,
            "Failed to parse line (Repeated name)"
        );
        fail = true;
        return;
    }

    oupt = param<bool>(name, inpt);
    if (!fail) {
        logging::inf(mod,
            "Parsed line as boolean parameter: ",
            "Name: “", name, "”, Value: ", oupt
        );
        fail = false;
        return;
    }

    oupt = param<std::string>(name, inpt);
    if (!fail) {
        logging::inf(mod,
            "Parsed line as string parameter: ",
            "Name: “", name, "”, Value: ", oupt
        );
        fail = false;
        return;
    }

    oupt = param<int>(name, inpt);
    if (!fail) {
        logging::inf(mod,
            "Parsed line as integer scalar parameter: ",
            "Name: “", name, "”, Value: ", oupt
        );
        fail = false;
        return;
    }

    oupt = param<double>(name, inpt);
    if (!fail) {
        logging::inf(mod,
            "Parsed line as floating point scalar parameter: ",
            "Name: “", name, "”, Value: ", oupt
        );
        fail = false;
        return;
    }

    oupt = param<linalg::ivector>(name, inpt);
    if (!fail) {
        logging::inf(mod,
            "Parsed line as integer vector parameter: ",
            "Name: “", name, "”, Value: ", oupt
        );
        fail = false;
        return;
    }

    oupt = param<linalg::fvector>(name, inpt);
    if (!fail) {
        logging::inf(mod,
            "Parsed line as floating point vector parameter: ",
            "Name: “", name, "”, Value: ", oupt
        );
        fail = false;
        return;
    }

    oupt = param<linalg::imatrix>(name, inpt);
    if (!fail) {
        logging::inf(mod,
            "Parsed line as integer matrix parameter: ",
            "Name: “", name, "”, Value: ", oupt
        );
        fail = false;
        return;
    }

    oupt = param<linalg::fmatrix>(name, inpt);
    if (!fail) {
        logging::inf(mod,
            "Parsed line as floating point matrix parameter: ",
            "Name: “", name, "”, Value: ", oupt
        );
        fail = false;
        return;
    }

    logging::err(mod,
        "Failed to parse line (Syntax error)"
    );

    fail = true;
    return;
}

}

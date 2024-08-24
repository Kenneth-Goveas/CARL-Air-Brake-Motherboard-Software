#include <cstring>
#include <cerrno>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

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
std::vector<std::vector<int>> val_ivec;

std::vector<std::string> name_fvec;
std::vector<std::vector<double>> val_fvec;

std::vector<std::string> name_imat;
std::vector<std::vector<std::vector<int>>> val_imat;

std::vector<std::string> name_fmat;
std::vector<std::vector<std::vector<double>>> val_fmat;

bool vec_sntx_arbit_len (std::string inpt);
bool vec_sntx_fixed_len (std::string inpt, int len);

bool mat_sntx_arbit_rows_arbit_cols (std::string inpt);
bool mat_sntx_fixed_rows_arbit_cols (std::string inpt, int rows);
bool mat_sntx_arbit_rows_fixed_cols (std::string inpt, int cols);
bool mat_sntx_fixed_rows_fixed_cols (std::string inpt, int rows, int cols);

template <typename type>
type vec_parse (std::string inpt, int len);

template <>
std::vector<int>
vec_parse<std::vector<int>> (std::string inpt, int len);

template <>
std::vector<double>
vec_parse<std::vector<double>> (std::string inpt, int len);

template <typename type>
type mat_parse (std::string inpt, int rows, int cols);

template <>
std::vector<std::vector<int>>
mat_parse<std::vector<std::vector<int>>> (
    std::string inpt, int rows, int cols
);

template <>
std::vector<std::vector<double>>
mat_parse<std::vector<std::vector<double>>> (
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
std::vector<int>
conv_inpt_to_val<std::vector<int>> (std::string inpt);

template <>
std::vector<double>
conv_inpt_to_val<std::vector<double>> (std::string inpt);

template <>
std::vector<std::vector<int>>
conv_inpt_to_val<std::vector<std::vector<int>>> (std::string inpt);

template <>
std::vector<std::vector<double>>
conv_inpt_to_val<std::vector<std::vector<double>>> (std::string inpt);

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
std::string conv_val_to_oupt<std::vector<int>> (
    std::vector<int> val
);

template <>
std::string conv_val_to_oupt<std::vector<double>> (
    std::vector<double> val
);

template <>
std::string conv_val_to_oupt<std::vector<std::vector<int>>> (
    std::vector<std::vector<int>> val
);

template <>
std::string conv_val_to_oupt<std::vector<std::vector<double>>> (
    std::vector<std::vector<double>> val
);

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
std::string param<std::vector<int>> (
    std::string name, std::string inpt
);

template <>
std::string param<std::vector<double>> (
    std::string name, std::string inpt
);

template <>
std::string param<std::vector<std::vector<int>>> (
    std::string name, std::string inpt
);

template <>
std::string param<std::vector<std::vector<double>>> (
    std::string name, std::string inpt
);

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
                    "Failed to read config file",
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
std::vector<int>
get<std::vector<int>> (std::string name) {
    std::vector<int> val;
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
std::vector<double>
get<std::vector<double>> (std::string name) {
    std::vector<double> val;
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
std::vector<std::vector<int>>
get<std::vector<std::vector<int>>> (std::string name) {
    std::vector<std::vector<int>> val;
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
std::vector<std::vector<double>>
get<std::vector<std::vector<double>>> (std::string name) {
    std::vector<std::vector<double>> val;
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
        fail = false;
        return true;
    } else {
        fail = false;
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
        fail = false;
        return true;
    } else {
        fail = false;
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
        fail = false;
        return true;
    } else {
        fail = false;
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
        fail = false;
        return true;
    } else {
        fail = false;
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
        fail = false;
        return true;
    } else {
        fail = false;
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
        fail = false;
        return true;
    } else {
        fail = false;
        return false;
    }
}

template <>
std::vector<int>
vec_parse<std::vector<int>> (std::string inpt, int len) {
    std::string pat = "";
    std::regex reg;
    std::smatch match;
    std::istringstream istr;
    std::vector<int> val;

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

    val.resize(len);
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
std::vector<double>
vec_parse<std::vector<double>> (std::string inpt, int len) {
    std::string pat = "";
    std::regex reg;
    std::smatch match;
    std::istringstream istr;
    std::vector<double> val;

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

    val.resize(len);
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
std::vector<std::vector<int>>
mat_parse<std::vector<std::vector<int>>> (
    std::string inpt, int rows, int cols
) {
    std::string pat = "";
    std::regex reg;
    std::smatch match;
    std::istringstream istr;
    std::vector<std::vector<int>> val;

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

    val.resize(rows);
    for (int i = 0; i < rows; i++) {
        val[i].resize(cols);
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
std::vector<std::vector<double>>
mat_parse<std::vector<std::vector<double>>> (
    std::string inpt, int rows, int cols
) {
    std::string pat = "";
    std::regex reg;
    std::smatch match;
    std::istringstream istr;
    std::vector<std::vector<double>> val;

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

    val.resize(rows);
    for (int i = 0; i < rows; i++) {
        val[i].resize(cols);
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
std::vector<int>
conv_inpt_to_val<std::vector<int>> (std::string inpt) {
    int len;
    std::vector<int> val;

    if (!vec_sntx_arbit_len(inpt)) {
        fail = true;
        return val;
    }

    for (len = 1; !vec_sntx_fixed_len(inpt, len); len++);

    val = vec_parse<std::vector<int>>(inpt, len);
    if (fail) {
        fail = true;
        return val;
    } else {
        fail = false;
        return val;
    }
}

template <>
std::vector<double>
conv_inpt_to_val<std::vector<double>> (std::string inpt) {
    int len;
    std::vector<double> val;

    if (!vec_sntx_arbit_len(inpt)) {
        fail = true;
        return val;
    }

    for (len = 1; !vec_sntx_fixed_len(inpt, len); len++);

    val = vec_parse<std::vector<double>>(inpt, len);
    if (fail) {
        fail = true;
        return val;
    } else {
        fail = false;
        return val;
    }
}

template <>
std::vector<std::vector<int>>
conv_inpt_to_val<std::vector<std::vector<int>>> (std::string inpt) {
    int rows, cols;
    std::vector<std::vector<int>> val;

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

    val = mat_parse<std::vector<std::vector<int>>>(inpt, rows, cols);
    if (fail) {
        fail = true;
        return val;
    } else {
        fail = false;
        return val;
    }
}

template <>
std::vector<std::vector<double>>
conv_inpt_to_val<std::vector<std::vector<double>>> (std::string inpt) {
    int rows, cols;
    std::vector<std::vector<double>> val;

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

    val = mat_parse<std::vector<std::vector<double>>>(inpt, rows, cols);
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

    fail = false;
    return oupt;
}

template <>
std::string conv_val_to_oupt<std::string> (std::string val) {
    std::string oupt;

    oupt = std::string("“") + val + std::string("”");

    fail = false;
    return oupt;
}

template <>
std::string conv_val_to_oupt<int> (int val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << std::showpos << val;
    oupt = ostr.str();

    fail = false;
    return oupt;
}

template <>
std::string conv_val_to_oupt<double> (double val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << std::showpos << std::scientific << std::setprecision(2) << val;
    oupt = ostr.str();

    fail = false;
    return oupt;
}

template <>
std::string conv_val_to_oupt<std::vector<int>> (
    std::vector<int> val
) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (auto i = 0; i < val.size(); i++) {
        ostr << std::showpos << val[i];
        if (i < val.size() - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";

    oupt = ostr.str();

    fail = false;
    return oupt;
}

template <>
std::string conv_val_to_oupt<std::vector<double>> (
    std::vector<double> val
) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (auto i = 0; i < val.size(); i++) {
        ostr << std::showpos << std::scientific << std::setprecision(2)
             << val[i];
        if (i < val.size() - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";

    oupt = ostr.str();

    fail = false;
    return oupt;
}

template <>
std::string conv_val_to_oupt<std::vector<std::vector<int>>> (
    std::vector<std::vector<int>> val
) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (auto i = 0; i < val.size(); i++) {
        ostr << "[";
        for (auto j = 0; j < val[i].size(); j++) {
            ostr << std::showpos << val[i][j];
            if (j < val[i].size() - 1) {
                ostr << ", ";
            }
        }
        ostr << "]";
        if (i < val.size() - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";

    oupt = ostr.str();

    fail = false;
    return oupt;
}

template <>
std::string conv_val_to_oupt<std::vector<std::vector<double>>> (
    std::vector<std::vector<double>> val
) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (auto i = 0; i < val.size(); i++) {
        ostr << "[";
        for (auto j = 0; j < val[i].size(); j++) {
            ostr << std::showpos << std::scientific << std::setprecision(2)
                 << val[i][j];
            if (j < val[i].size() - 1) {
                ostr << ", ";
            }
        }
        ostr << "]";
        if (i < val.size() - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";

    oupt = ostr.str();

    fail = false;
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
std::string param<std::vector<int>> (
    std::string name, std::string inpt
) {
    std::vector<int> val;
    std::string oupt;

    val = conv_inpt_to_val<std::vector<int>>(inpt);
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
std::string param<std::vector<double>> (
    std::string name, std::string inpt
) {
    std::vector<double> val;
    std::string oupt;

    val = conv_inpt_to_val<std::vector<double>>(inpt);
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
std::string param<std::vector<std::vector<int>>> (
    std::string name, std::string inpt
) {
    std::vector<std::vector<int>> val;
    std::string oupt;

    val = conv_inpt_to_val<std::vector<std::vector<int>>>(inpt);
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
std::string param<std::vector<std::vector<double>>> (
    std::string name, std::string inpt
) {
    std::vector<std::vector<double>> val;
    std::string oupt;

    val = conv_inpt_to_val<std::vector<std::vector<double>>>(inpt);
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

    fail = false;
    return line;
}

bool blank (std::string line) {
    for (auto i = 0; i < line.size(); i++) {
        if (line[i] != ' ' && line[i] != '\t') {
            fail = false;
            return false;
        }
    }
    fail = false;
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
            fail = false;
            return true;
        }
    }
    fail = false;
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

    oupt = param<std::vector<int>>(name, inpt);
    if (!fail) {
        logging::inf(mod,
            "Parsed line as integer vector parameter: ",
            "Name: “", name, "”, Value: ", oupt
        );
        fail = false;
        return;
    }

    oupt = param<std::vector<double>>(name, inpt);
    if (!fail) {
        logging::inf(mod,
            "Parsed line as floating point vector parameter: ",
            "Name: “", name, "”, Value: ", oupt
        );
        fail = false;
        return;
    }

    oupt = param<std::vector<std::vector<int>>>(name, inpt);
    if (!fail) {
        logging::inf(mod,
            "Parsed line as integer matrix parameter: ",
            "Name: “", name, "”, Value: ", oupt
        );
        fail = false;
        return;
    }

    oupt = param<std::vector<std::vector<double>>>(name, inpt);
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

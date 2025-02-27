#include <cstdint>
#include <cstring>
#include <cerrno>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <regex>
#include <string>
#include <vector>
#include <tuple>

#include <math/linalg.hpp>

#include <sys/logging.hpp>
#include <sys/config.hpp>

namespace config :: intern {

std::string mod = "config";

bool fail;

std::vector<std::string> name_all;

std::vector<std::string> name_str;
std::vector<std::string> val_str;

std::vector<std::string> name_byte;
std::vector<std::uint8_t> val_byte;

std::vector<std::string> name_bool;
std::vector<bool> val_bool;

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

std::vector<std::string> name_str_arr;
std::vector<std::vector<std::string>> val_str_arr;

std::vector<std::string> name_byte_arr;
std::vector<std::vector<std::uint8_t>> val_byte_arr;

std::vector<std::string> name_bool_arr;
std::vector<std::vector<bool>> val_bool_arr;

std::vector<std::string> name_iscl_arr;
std::vector<std::vector<int>> val_iscl_arr;

std::vector<std::string> name_fscl_arr;
std::vector<std::vector<double>> val_fscl_arr;

std::vector<std::string> name_ivec_arr;
std::vector<std::vector<linalg::ivector>> val_ivec_arr;

std::vector<std::string> name_fvec_arr;
std::vector<std::vector<linalg::fvector>> val_fvec_arr;

std::vector<std::string> name_imat_arr;
std::vector<std::vector<linalg::imatrix>> val_imat_arr;

std::vector<std::string> name_fmat_arr;
std::vector<std::vector<linalg::fmatrix>> val_fmat_arr;

void load (std::string line);

std::string uncomment (std::string line);
bool blank (std::string line);
std::tuple<std::string, std::string> pair (std::string line);
bool exists (std::string name);
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

namespace config {

bool fail (void) {
    return intern::fail;
}

void init (std::string path) {
    std::ifstream file;
    std::string line;

    logging::inf(intern::mod,
        "Loading config file: Path: ", path
    );

    logging::inf(intern::mod,
        "Opening config file"
    );

    file.open(path);
    if (file.fail()) {
        logging::err(intern::mod,
            "Failed to open config file (", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to load config file"
        );
        intern::fail = true;
        return;
    }

    while (true) {
        logging::inf(intern::mod,
            "Reading line from config file"
        );
        std::getline(file, line);
        if (file.fail()) {
            if (file.eof()) {
                logging::inf(intern::mod,
                    "No more lines in config file"
                );
                file.clear();
                break;
            } else {
                logging::err(intern::mod,
                    "Failed to read line from config file (", std::strerror(errno), ")"
                );
                logging::wrn(intern::mod,
                    "Closing config file"
                );
                file.clear();
                file.close();
                if (file.fail()) {
                    logging::err(intern::mod,
                        "Failed to close config file (", std::strerror(errno), ")"
                    );
                }
                logging::err(intern::mod,
                    "Failed to load config file"
                );
                intern::fail = true;
                return;
            }
        } else {
            logging::inf(intern::mod,
                "Read line from config file: Line: ", line
            );
            intern::load(line);
            if (intern::fail) {
                logging::wrn(intern::mod,
                    "Closing config file"
                );
                file.close();
                if (file.fail()) {
                    logging::err(intern::mod,
                        "Failed to close config file (", std::strerror(errno), ")"
                    );
                }
                logging::err(intern::mod,
                    "Failed to load config file"
                );
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
            "Failed to close config file (", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to load config file"
        );
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

template <>
std::string get<std::string> (std::string name) {
    std::string val;

    logging::inf(intern::mod,
        "Getting parameter: Type: String, Name: ", name
    );

    auto name_beg = intern::name_str.begin();
    auto name_end = intern::name_str.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = intern::val_str.begin();
    auto val_end = intern::val_str.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        logging::err(intern::mod,
            "Failed to get parameter (Name not found)"
        );
        intern::fail = true;
        return val;
    }

    val = *val_itr;
    logging::inf(intern::mod,
        "Got parameter: Value: ", val
    );

    intern::fail = false;
    return val;
}

template <>
std::uint8_t get<std::uint8_t> (std::string name) {
    std::uint8_t val;

    logging::inf(intern::mod,
        "Getting parameter: Type: Byte, Name: ", name
    );

    auto name_beg = intern::name_byte.begin();
    auto name_end = intern::name_byte.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = intern::val_byte.begin();
    auto val_end = intern::val_byte.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        logging::err(intern::mod,
            "Failed to get parameter (Name not found)"
        );
        intern::fail = true;
        return val;
    }

    val = *val_itr;
    logging::inf(intern::mod,
        "Got parameter: Value: ", val
    );

    intern::fail = false;
    return val;
}

template <>
bool get<bool> (std::string name) {
    bool val;

    logging::inf(intern::mod,
        "Getting parameter: Type: Boolean, Name: ", name
    );

    auto name_beg = intern::name_bool.begin();
    auto name_end = intern::name_bool.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = intern::val_bool.begin();
    auto val_end = intern::val_bool.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        logging::err(intern::mod,
            "Failed to get parameter (Name not found)"
        );
        intern::fail = true;
        return val;
    }

    val = *val_itr;
    logging::inf(intern::mod,
        "Got parameter: Value: ", val
    );

    intern::fail = false;
    return val;
}

template <>
int get<int> (std::string name) {
    int val;

    logging::inf(intern::mod,
        "Getting parameter: Type: Integer scalar, Name: ", name
    );

    auto name_beg = intern::name_iscl.begin();
    auto name_end = intern::name_iscl.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = intern::val_iscl.begin();
    auto val_end = intern::val_iscl.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        logging::err(intern::mod,
            "Failed to get parameter (Name not found)"
        );
        intern::fail = true;
        return val;
    }

    val = *val_itr;
    logging::inf(intern::mod,
        "Got parameter: Value: ", val
    );

    intern::fail = false;
    return val;
}

template <>
double get<double> (std::string name) {
    double val;

    logging::inf(intern::mod,
        "Getting parameter: Type: Real scalar, Name: ", name
    );

    auto name_beg = intern::name_fscl.begin();
    auto name_end = intern::name_fscl.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = intern::val_fscl.begin();
    auto val_end = intern::val_fscl.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        logging::err(intern::mod,
            "Failed to get parameter (Name not found)"
        );
        intern::fail = true;
        return val;
    }

    val = *val_itr;
    logging::inf(intern::mod,
        "Got parameter: Value: ", val
    );

    intern::fail = false;
    return val;
}

template <>
linalg::ivector get<linalg::ivector> (std::string name) {
    linalg::ivector val;

    logging::inf(intern::mod,
        "Getting parameter: Type: Integer vector, Name: ", name
    );

    auto name_beg = intern::name_ivec.begin();
    auto name_end = intern::name_ivec.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = intern::val_ivec.begin();
    auto val_end = intern::val_ivec.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        logging::err(intern::mod,
            "Failed to get parameter (Name not found)"
        );
        intern::fail = true;
        return val;
    }

    val = *val_itr;
    logging::inf(intern::mod,
        "Got parameter: Value: ", val
    );

    intern::fail = false;
    return val;
}

template <>
linalg::fvector get<linalg::fvector> (std::string name) {
    linalg::fvector val;

    logging::inf(intern::mod,
        "Getting parameter: Type: Real vector, Name: ", name
    );

    auto name_beg = intern::name_fvec.begin();
    auto name_end = intern::name_fvec.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = intern::val_fvec.begin();
    auto val_end = intern::val_fvec.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        logging::err(intern::mod,
            "Failed to get parameter (Name not found)"
        );
        intern::fail = true;
        return val;
    }

    val = *val_itr;
    logging::inf(intern::mod,
        "Got parameter: Value: ", val
    );

    intern::fail = false;
    return val;
}

template <>
linalg::imatrix get<linalg::imatrix> (std::string name) {
    linalg::imatrix val;

    logging::inf(intern::mod,
        "Getting parameter: Type: Integer matrix, Name: ", name
    );

    auto name_beg = intern::name_imat.begin();
    auto name_end = intern::name_imat.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = intern::val_imat.begin();
    auto val_end = intern::val_imat.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        logging::err(intern::mod,
            "Failed to get parameter (Name not found)"
        );
        intern::fail = true;
        return val;
    }

    val = *val_itr;
    logging::inf(intern::mod,
        "Got parameter: Value: ", val
    );

    intern::fail = false;
    return val;
}

template <>
linalg::fmatrix get<linalg::fmatrix> (std::string name) {
    linalg::fmatrix val;

    logging::inf(intern::mod,
        "Getting parameter: Type: Real matrix, Name: ", name
    );

    auto name_beg = intern::name_fmat.begin();
    auto name_end = intern::name_fmat.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = intern::val_fmat.begin();
    auto val_end = intern::val_fmat.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        logging::err(intern::mod,
            "Failed to get parameter (Name not found)"
        );
        intern::fail = true;
        return val;
    }

    val = *val_itr;
    logging::inf(intern::mod,
        "Got parameter: Value: ", val
    );

    intern::fail = false;
    return val;
}

template <>
std::vector<std::string> get<std::vector<std::string>> (std::string name) {
    std::vector<std::string> val;

    logging::inf(intern::mod,
        "Getting parameter: Type: String array, Name: ", name
    );

    auto name_beg = intern::name_str_arr.begin();
    auto name_end = intern::name_str_arr.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = intern::val_str_arr.begin();
    auto val_end = intern::val_str_arr.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        logging::err(intern::mod,
            "Failed to get parameter (Name not found)"
        );
        intern::fail = true;
        return val;
    }

    val = *val_itr;
    logging::inf(intern::mod,
        "Got parameter: Value: ", val
    );

    intern::fail = false;
    return val;
}

template <>
std::vector<std::uint8_t> get<std::vector<std::uint8_t>> (std::string name) {
    std::vector<std::uint8_t> val;

    logging::inf(intern::mod,
        "Getting parameter: Type: Byte array, Name: ", name
    );

    auto name_beg = intern::name_byte_arr.begin();
    auto name_end = intern::name_byte_arr.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = intern::val_byte_arr.begin();
    auto val_end = intern::val_byte_arr.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        logging::err(intern::mod,
            "Failed to get parameter (Name not found)"
        );
        intern::fail = true;
        return val;
    }

    val = *val_itr;
    logging::inf(intern::mod,
        "Got parameter: Value: ", val
    );

    intern::fail = false;
    return val;
}

template <>
std::vector<bool> get<std::vector<bool>> (std::string name) {
    std::vector<bool> val;

    logging::inf(intern::mod,
        "Getting parameter: Type: Boolean array, Name: ", name
    );

    auto name_beg = intern::name_bool_arr.begin();
    auto name_end = intern::name_bool_arr.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = intern::val_bool_arr.begin();
    auto val_end = intern::val_bool_arr.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        logging::err(intern::mod,
            "Failed to get parameter (Name not found)"
        );
        intern::fail = true;
        return val;
    }

    val = *val_itr;
    logging::inf(intern::mod,
        "Got parameter: Value: ", val
    );

    intern::fail = false;
    return val;
}

template <>
std::vector<int> get<std::vector<int>> (std::string name) {
    std::vector<int> val;

    logging::inf(intern::mod,
        "Getting parameter: Type: Integer scalar array, Name: ", name
    );

    auto name_beg = intern::name_iscl_arr.begin();
    auto name_end = intern::name_iscl_arr.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = intern::val_iscl_arr.begin();
    auto val_end = intern::val_iscl_arr.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        logging::err(intern::mod,
            "Failed to get parameter (Name not found)"
        );
        intern::fail = true;
        return val;
    }

    val = *val_itr;
    logging::inf(intern::mod,
        "Got parameter: Value: ", val
    );

    intern::fail = false;
    return val;
}

template <>
std::vector<double> get<std::vector<double>> (std::string name) {
    std::vector<double> val;

    logging::inf(intern::mod,
        "Getting parameter: Type: Real scalar array, Name: ", name
    );

    auto name_beg = intern::name_fscl_arr.begin();
    auto name_end = intern::name_fscl_arr.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = intern::val_fscl_arr.begin();
    auto val_end = intern::val_fscl_arr.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        logging::err(intern::mod,
            "Failed to get parameter (Name not found)"
        );
        intern::fail = true;
        return val;
    }

    val = *val_itr;
    logging::inf(intern::mod,
        "Got parameter: Value: ", val
    );

    intern::fail = false;
    return val;
}

template <>
std::vector<linalg::ivector> get<std::vector<linalg::ivector>> (std::string name) {
    std::vector<linalg::ivector> val;

    logging::inf(intern::mod,
        "Getting parameter: Type: Integer vector array, Name: ", name
    );

    auto name_beg = intern::name_ivec_arr.begin();
    auto name_end = intern::name_ivec_arr.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = intern::val_ivec_arr.begin();
    auto val_end = intern::val_ivec_arr.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        logging::err(intern::mod,
            "Failed to get parameter (Name not found)"
        );
        intern::fail = true;
        return val;
    }

    val = *val_itr;
    logging::inf(intern::mod,
        "Got parameter: Value: ", val
    );

    intern::fail = false;
    return val;
}

template <>
std::vector<linalg::fvector> get<std::vector<linalg::fvector>> (std::string name) {
    std::vector<linalg::fvector> val;

    logging::inf(intern::mod,
        "Getting parameter: Type: Real vector array, Name: ", name
    );

    auto name_beg = intern::name_fvec_arr.begin();
    auto name_end = intern::name_fvec_arr.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = intern::val_fvec_arr.begin();
    auto val_end = intern::val_fvec_arr.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        logging::err(intern::mod,
            "Failed to get parameter (Name not found)"
        );
        intern::fail = true;
        return val;
    }

    val = *val_itr;
    logging::inf(intern::mod,
        "Got parameter: Value: ", val
    );

    intern::fail = false;
    return val;
}

template <>
std::vector<linalg::imatrix> get<std::vector<linalg::imatrix>> (std::string name) {
    std::vector<linalg::imatrix> val;

    logging::inf(intern::mod,
        "Getting parameter: Type: Integer matrix array, Name: ", name
    );

    auto name_beg = intern::name_imat_arr.begin();
    auto name_end = intern::name_imat_arr.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = intern::val_imat_arr.begin();
    auto val_end = intern::val_imat_arr.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        logging::err(intern::mod,
            "Failed to get parameter (Name not found)"
        );
        intern::fail = true;
        return val;
    }

    val = *val_itr;
    logging::inf(intern::mod,
        "Got parameter: Value: ", val
    );

    intern::fail = false;
    return val;
}

template <>
std::vector<linalg::fmatrix> get<std::vector<linalg::fmatrix>> (std::string name) {
    std::vector<linalg::fmatrix> val;

    logging::inf(intern::mod,
        "Getting parameter: Type: Real matrix array, Name: ", name
    );

    auto name_beg = intern::name_fmat_arr.begin();
    auto name_end = intern::name_fmat_arr.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = intern::val_fmat_arr.begin();
    auto val_end = intern::val_fmat_arr.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        logging::err(intern::mod,
            "Failed to get parameter (Name not found)"
        );
        intern::fail = true;
        return val;
    }

    val = *val_itr;
    logging::inf(intern::mod,
        "Got parameter: Value: ", val
    );

    intern::fail = false;
    return val;
}

}

namespace config :: intern {

void load (std::string line) {
    std::string name, inpt;

    logging::inf(mod,
        "Parsing line"
    );

    line = uncomment(line);

    if (blank(line)) {
        logging::inf(mod,
            "Parsed line as blank"
        );
        fail = false;
        return;
    }

    std::tie(name, inpt) = pair(line);
    if (fail) {
        logging::err(mod,
            "Failed to parse line (Syntax error)"
        );
        fail = true;
        return;
    }

    if (exists(name)) {
        logging::err(mod,
            "Failed to parse line (Repeated name)"
        );
        fail = true;
        return;
    }

    val_str.push_back(parse<std::string>(inpt));
    if (!fail) {
        name_all.push_back(name);
        name_str.push_back(name);
        logging::inf(mod,
            "Parsed line as parameter: Type: String, Name: ", name_str.back(), ", Value: ", val_str.back()
        );
        fail = false;
        return;
    } else {
        val_str.pop_back();
    }

    val_byte.push_back(parse<std::uint8_t>(inpt));
    if (!fail) {
        name_all.push_back(name);
        name_byte.push_back(name);
        logging::inf(mod,
            "Parsed line as parameter: Type: Byte, Name: ", name_byte.back(), ", Value: ", val_byte.back()
        );
        fail = false;
        return;
    } else {
        val_byte.pop_back();
    }

    val_bool.push_back(parse<bool>(inpt));
    if (!fail) {
        name_all.push_back(name);
        name_bool.push_back(name);
        logging::inf(mod,
            "Parsed line as parameter: Type: Boolean, Name: ", name_bool.back(), ", Value: ", bool(val_bool.back())
        );
        fail = false;
        return;
    } else {
        val_bool.pop_back();
    }

    val_iscl.push_back(parse<int>(inpt));
    if (!fail) {
        name_all.push_back(name);
        name_iscl.push_back(name);
        logging::inf(mod,
            "Parsed line as parameter: Type: Integer scalar, Name: ", name_iscl.back(), ", Value: ", val_iscl.back()
        );
        fail = false;
        return;
    } else {
        val_iscl.pop_back();
    }

    val_fscl.push_back(parse<double>(inpt));
    if (!fail) {
        name_all.push_back(name);
        name_fscl.push_back(name);
        logging::inf(mod,
            "Parsed line as parameter: Type: Real scalar, Name: ", name_fscl.back(), ", Value: ", val_fscl.back()
        );
        fail = false;
        return;
    } else {
        val_fscl.pop_back();
    }

    val_ivec.push_back(parse<linalg::ivector>(inpt));
    if (!fail) {
        name_all.push_back(name);
        name_ivec.push_back(name);
        logging::inf(mod,
            "Parsed line as parameter: Type: Integer vector, Name: ", name_ivec.back(), ", Value: ", val_ivec.back()
        );
        fail = false;
        return;
    } else {
        val_ivec.pop_back();
    }

    val_fvec.push_back(parse<linalg::fvector>(inpt));
    if (!fail) {
        name_all.push_back(name);
        name_fvec.push_back(name);
        logging::inf(mod,
            "Parsed line as parameter: Type: Real vector, Name: ", name_fvec.back(), ", Value: ", val_fvec.back()
        );
        fail = false;
        return;
    } else {
        val_fvec.pop_back();
    }

    val_imat.push_back(parse<linalg::imatrix>(inpt));
    if (!fail) {
        name_all.push_back(name);
        name_imat.push_back(name);
        logging::inf(mod,
            "Parsed line as parameter: Type: Integer matrix, Name: ", name_imat.back(), ", Value: ", val_imat.back()
        );
        fail = false;
        return;
    } else {
        val_imat.pop_back();
    }

    val_fmat.push_back(parse<linalg::fmatrix>(inpt));
    if (!fail) {
        name_all.push_back(name);
        name_fmat.push_back(name);
        logging::inf(mod,
            "Parsed line as parameter: Type: Real matrix, Name: ", name_fmat.back(), ", Value: ", val_fmat.back()
        );
        fail = false;
        return;
    } else {
        val_fmat.pop_back();
    }

    val_str_arr.push_back(parse<std::vector<std::string>>(inpt));
    if (!fail) {
        name_all.push_back(name);
        name_str_arr.push_back(name);
        logging::inf(mod,
            "Parsed line as parameter: Type: String array, Name: ", name_str_arr.back(), ", Value: ", val_str_arr.back()
        );
        fail = false;
        return;
    } else {
        val_str_arr.pop_back();
    }

    val_byte_arr.push_back(parse<std::vector<std::uint8_t>>(inpt));
    if (!fail) {
        name_all.push_back(name);
        name_byte_arr.push_back(name);
        logging::inf(mod,
            "Parsed line as parameter: Type: Byte array, Name: ", name_byte_arr.back(), ", Value: ", val_byte_arr.back()
        );
        fail = false;
        return;
    } else {
        val_byte_arr.pop_back();
    }

    val_bool_arr.push_back(parse<std::vector<bool>>(inpt));
    if (!fail) {
        name_all.push_back(name);
        name_bool_arr.push_back(name);
        logging::inf(mod,
            "Parsed line as parameter: Type: Boolean array, Name: ", name_bool_arr.back(), ", Value: ", val_bool_arr.back()
        );
        fail = false;
        return;
    } else {
        val_bool_arr.pop_back();
    }

    val_iscl_arr.push_back(parse<std::vector<int>>(inpt));
    if (!fail) {
        name_all.push_back(name);
        name_iscl_arr.push_back(name);
        logging::inf(mod,
            "Parsed line as parameter: Type: Integer scalar array, Name: ", name_iscl_arr.back(), ", Value: ", val_iscl_arr.back()
        );
        fail = false;
        return;
    } else {
        val_iscl_arr.pop_back();
    }

    val_fscl_arr.push_back(parse<std::vector<double>>(inpt));
    if (!fail) {
        name_all.push_back(name);
        name_fscl_arr.push_back(name);
        logging::inf(mod,
            "Parsed line as parameter: Type: Real scalar array, Name: ", name_fscl_arr.back(), ", Value: ", val_fscl_arr.back()
        );
        fail = false;
        return;
    } else {
        val_fscl_arr.pop_back();
    }

    val_ivec_arr.push_back(parse<std::vector<linalg::ivector>>(inpt));
    if (!fail) {
        name_all.push_back(name);
        name_ivec_arr.push_back(name);
        logging::inf(mod,
            "Parsed line as parameter: Type: Integer vector array, Name: ", name_ivec_arr.back(), ", Value: ", val_ivec_arr.back()
        );
        fail = false;
        return;
    } else {
        val_ivec_arr.pop_back();
    }

    val_fvec_arr.push_back(parse<std::vector<linalg::fvector>>(inpt));
    if (!fail) {
        name_all.push_back(name);
        name_fvec_arr.push_back(name);
        logging::inf(mod,
            "Parsed line as parameter: Type: Real vector array, Name: ", name_fvec_arr.back(), ", Value: ", val_fvec_arr.back()
        );
        fail = false;
        return;
    } else {
        val_fvec_arr.pop_back();
    }

    val_imat_arr.push_back(parse<std::vector<linalg::imatrix>>(inpt));
    if (!fail) {
        name_all.push_back(name);
        name_imat_arr.push_back(name);
        logging::inf(mod,
            "Parsed line as parameter: Type: Integer matrix array, Name: ", name_imat_arr.back(), ", Value: ", val_imat_arr.back()
        );
        fail = false;
        return;
    } else {
        val_imat_arr.pop_back();
    }

    val_fmat_arr.push_back(parse<std::vector<linalg::fmatrix>>(inpt));
    if (!fail) {
        name_all.push_back(name);
        name_fmat_arr.push_back(name);
        logging::inf(mod,
            "Parsed line as parameter: Type: Real matrix array, Name: ", name_fmat_arr.back(), ", Value: ", val_fmat_arr.back()
        );
        fail = false;
        return;
    } else {
        val_fmat_arr.pop_back();
    }

    logging::err(mod,
        "Failed to parse line (Syntax error)"
    );

    fail = true;
}

std::string uncomment (std::string line) {
    bool quot = false, escap = false;

    for (auto itr = line.begin(); itr < line.end(); itr++) {
        if (quot) {
            if (escap) {
                escap = false;
            } else {
                if (*itr == '\\') {
                    escap = true;
                } else if (*itr == '"') {
                    quot = false;
                }
            }
        } else {
            if (*itr == '"') {
                quot = true;
            } else if (*itr == '#') {
                line.erase(itr, line.end());
                break;
            }
        }
    }

    return line;
}

bool blank (std::string line) {
    auto pred = [] (auto arg) {
        return (arg == ' ' || arg == '\t');
    };
    return std::all_of(line.begin(), line.end(), pred);
}

std::tuple<std::string, std::string> pair (std::string line) {
    std::string name, inpt;
    std::string pat;
    std::regex reg;
    std::smatch match;

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
        name = match[1];
        inpt = match[2];
        fail = false;
        return {name, inpt};
    } else {
        fail = true;
        return {name, inpt};
    }
}

bool exists (std::string name) {
    auto pred = [name] (auto arg) {
        return (arg == name);
    };
    return std::any_of(name_all.begin(), name_all.end(), pred);
}

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

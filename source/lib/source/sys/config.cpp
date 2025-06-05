#include <cstdint>
#include <cctype>
#include <cstring>
#include <cerrno>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <string>
#include <vector>

#include <math/linalg.hpp>
#include <sys/logging.hpp>
#include <sys/config.hpp>

namespace config {

bool tracker::exist = false;

bool tracker::instantiate (void) {
    if (tracker::exist) {
        return false;
    } else {
        tracker::exist = true;
        return true;
    }
}

instance::instance (const std::string & path) : logging("config"), init(false) {
    bool file_open = false;

    this->logging.inf("Initializing instance: Path: ", path);

    if (!instance::instantiate()) {
        this->logging.err("Failed to initialize instance (Instance already exists)");
        throw except_ctor::fail;
    }

    try {
        this->logging.inf("Opening config file");
        this->file.clear();
        this->file.open(path);
        if (this->file.fail()) {
            this->logging.err("Failed to open config file (", std::strerror(errno), ")");
            throw except_intern{};
        }

        file_open = true;

        try {
            while (true) {
                this->load();
            }
        } catch (except_load except) {
            switch (except) {
                case except_load::fail:
                    throw except_intern{};
                case except_load::eof:
                    break;
            }
        }

        file_open = false;

        this->logging.inf("Closing config file");
        this->file.clear();
        this->file.close();
        if (this->file.fail()) {
            this->logging.err("Failed to close config file (", std::strerror(errno), ")");
            throw except_intern{};
        }
    } catch (except_intern) {
        if (file_open) {
            this->logging.wrn("Closing config file");
            this->file.clear();
            this->file.close();
            if (this->file.fail()) {
                this->logging.err("Failed to close config file (", std::strerror(errno), ")");
            }
        }

        this->logging.err("Failed to initialize instance");
        throw except_ctor::fail;
    }

    this->init = true;
}

template <>
std::string instance::get<std::string> (const std::string & name) {
    std::string val;

    this->logging.inf("Getting parameter from config file: Type: String, Name: ", name);

    if (!this->init) {
        this->logging.err("Failed to get parameter from config file (Instance not initialized)");
        throw except_get::fail;
    }

    auto name_beg = this->name_str.begin();
    auto name_end = this->name_str.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = this->val_str.begin();
    auto val_end = this->val_str.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        this->logging.err("Failed to get parameter from config file (Name not found)");
        throw except_get::fail;
    }

    val = *val_itr;
    this->logging.inf("Got parameter from config file: Value: ", val);

    return val;
}

template <>
std::uint8_t instance::get<std::uint8_t> (const std::string & name) {
    std::uint8_t val;

    this->logging.inf("Getting parameter from config file: Type: Byte, Name: ", name);

    if (!this->init) {
        this->logging.err("Failed to get parameter from config file (Instance not initialized)");
        throw except_get::fail;
    }

    auto name_beg = this->name_byte.begin();
    auto name_end = this->name_byte.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = this->val_byte.begin();
    auto val_end = this->val_byte.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        this->logging.err("Failed to get parameter from config file (Name not found)");
        throw except_get::fail;
    }

    val = *val_itr;
    this->logging.inf("Got parameter from config file: Value: ", val);

    return val;
}

template <>
bool instance::get<bool> (const std::string & name) {
    bool val;

    this->logging.inf("Getting parameter from config file: Type: Boolean, Name: ", name);

    if (!this->init) {
        this->logging.err("Failed to get parameter from config file (Instance not initialized)");
        throw except_get::fail;
    }

    auto name_beg = this->name_bool.begin();
    auto name_end = this->name_bool.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = this->val_bool.begin();
    auto val_end = this->val_bool.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        this->logging.err("Failed to get parameter from config file (Name not found)");
        throw except_get::fail;
    }

    val = *val_itr;
    this->logging.inf("Got parameter from config file: Value: ", val);

    return val;
}

template <>
int instance::get<int> (const std::string & name) {
    int val;

    this->logging.inf("Getting parameter from config file: Type: Integer scalar, Name: ", name);

    if (!this->init) {
        this->logging.err("Failed to get parameter from config file (Instance not initialized)");
        throw except_get::fail;
    }

    auto name_beg = this->name_iscl.begin();
    auto name_end = this->name_iscl.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = this->val_iscl.begin();
    auto val_end = this->val_iscl.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        this->logging.err("Failed to get parameter from config file (Name not found)");
        throw except_get::fail;
    }

    val = *val_itr;
    this->logging.inf("Got parameter from config file: Value: ", val);

    return val;
}

template <>
double instance::get<double> (const std::string & name) {
    double val;

    this->logging.inf("Getting parameter from config file: Type: Real scalar, Name: ", name);

    if (!this->init) {
        this->logging.err("Failed to get parameter from config file (Instance not initialized)");
        throw except_get::fail;
    }

    auto name_beg = this->name_fscl.begin();
    auto name_end = this->name_fscl.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = this->val_fscl.begin();
    auto val_end = this->val_fscl.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        this->logging.err("Failed to get parameter from config file (Name not found)");
        throw except_get::fail;
    }

    val = *val_itr;
    this->logging.inf("Got parameter from config file: Value: ", val);

    return val;
}

template <>
linalg::ivector instance::get<linalg::ivector> (const std::string & name) {
    linalg::ivector val;

    this->logging.inf("Getting parameter from config file: Type: Integer vector, Name: ", name);

    if (!this->init) {
        this->logging.err("Failed to get parameter from config file (Instance not initialized)");
        throw except_get::fail;
    }

    auto name_beg = this->name_ivec.begin();
    auto name_end = this->name_ivec.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = this->val_ivec.begin();
    auto val_end = this->val_ivec.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        this->logging.err("Failed to get parameter from config file (Name not found)");
        throw except_get::fail;
    }

    val = *val_itr;
    this->logging.inf("Got parameter from config file: Value: ", val);

    return val;
}

template <>
linalg::fvector instance::get<linalg::fvector> (const std::string & name) {
    linalg::fvector val;

    this->logging.inf("Getting parameter from config file: Type: Real vector, Name: ", name);

    if (!this->init) {
        this->logging.err("Failed to get parameter from config file (Instance not initialized)");
        throw except_get::fail;
    }

    auto name_beg = this->name_fvec.begin();
    auto name_end = this->name_fvec.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = this->val_fvec.begin();
    auto val_end = this->val_fvec.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        this->logging.err("Failed to get parameter from config file (Name not found)");
        throw except_get::fail;
    }

    val = *val_itr;
    this->logging.inf("Got parameter from config file: Value: ", val);

    return val;
}

template <>
linalg::imatrix instance::get<linalg::imatrix> (const std::string & name) {
    linalg::imatrix val;

    this->logging.inf("Getting parameter from config file: Type: Integer matrix, Name: ", name);

    if (!this->init) {
        this->logging.err("Failed to get parameter from config file (Instance not initialized)");
        throw except_get::fail;
    }

    auto name_beg = this->name_imat.begin();
    auto name_end = this->name_imat.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = this->val_imat.begin();
    auto val_end = this->val_imat.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        this->logging.err("Failed to get parameter from config file (Name not found)");
        throw except_get::fail;
    }

    val = *val_itr;
    this->logging.inf("Got parameter from config file: Value: ", val);

    return val;
}

template <>
linalg::fmatrix instance::get<linalg::fmatrix> (const std::string & name) {
    linalg::fmatrix val;

    this->logging.inf("Getting parameter from config file: Type: Real matrix, Name: ", name);

    if (!this->init) {
        this->logging.err("Failed to get parameter from config file (Instance not initialized)");
        throw except_get::fail;
    }

    auto name_beg = this->name_fmat.begin();
    auto name_end = this->name_fmat.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = this->val_fmat.begin();
    auto val_end = this->val_fmat.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        this->logging.err("Failed to get parameter from config file (Name not found)");
        throw except_get::fail;
    }

    val = *val_itr;
    this->logging.inf("Got parameter from config file: Value: ", val);

    return val;
}

template <>
std::vector<std::string> instance::get<std::vector<std::string>> (const std::string & name) {
    std::vector<std::string> val;

    this->logging.inf("Getting parameter from config file: Type: String array, Name: ", name);

    if (!this->init) {
        this->logging.err("Failed to get parameter from config file (Instance not initialized)");
        throw except_get::fail;
    }

    auto name_beg = this->name_str_arr.begin();
    auto name_end = this->name_str_arr.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = this->val_str_arr.begin();
    auto val_end = this->val_str_arr.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        this->logging.err("Failed to get parameter from config file (Name not found)");
        throw except_get::fail;
    }

    val = *val_itr;
    this->logging.inf("Got parameter from config file: Value: ", val);

    return val;
}

template <>
std::vector<std::uint8_t> instance::get<std::vector<std::uint8_t>> (const std::string & name) {
    std::vector<std::uint8_t> val;

    this->logging.inf("Getting parameter from config file: Type: Byte array, Name: ", name);

    if (!this->init) {
        this->logging.err("Failed to get parameter from config file (Instance not initialized)");
        throw except_get::fail;
    }

    auto name_beg = this->name_byte_arr.begin();
    auto name_end = this->name_byte_arr.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = this->val_byte_arr.begin();
    auto val_end = this->val_byte_arr.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        this->logging.err("Failed to get parameter from config file (Name not found)");
        throw except_get::fail;
    }

    val = *val_itr;
    this->logging.inf("Got parameter from config file: Value: ", val);

    return val;
}

template <>
std::vector<bool> instance::get<std::vector<bool>> (const std::string & name) {
    std::vector<bool> val;

    this->logging.inf("Getting parameter from config file: Type: Boolean array, Name: ", name);

    if (!this->init) {
        this->logging.err("Failed to get parameter from config file (Instance not initialized)");
        throw except_get::fail;
    }

    auto name_beg = this->name_bool_arr.begin();
    auto name_end = this->name_bool_arr.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = this->val_bool_arr.begin();
    auto val_end = this->val_bool_arr.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        this->logging.err("Failed to get parameter from config file (Name not found)");
        throw except_get::fail;
    }

    val = *val_itr;
    this->logging.inf("Got parameter from config file: Value: ", val);

    return val;
}

template <>
std::vector<int> instance::get<std::vector<int>> (const std::string & name) {
    std::vector<int> val;

    this->logging.inf("Getting parameter from config file: Type: Integer scalar array, Name: ", name);

    if (!this->init) {
        this->logging.err("Failed to get parameter from config file (Instance not initialized)");
        throw except_get::fail;
    }

    auto name_beg = this->name_iscl_arr.begin();
    auto name_end = this->name_iscl_arr.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = this->val_iscl_arr.begin();
    auto val_end = this->val_iscl_arr.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        this->logging.err("Failed to get parameter from config file (Name not found)");
        throw except_get::fail;
    }

    val = *val_itr;
    this->logging.inf("Got parameter from config file: Value: ", val);

    return val;
}

template <>
std::vector<double> instance::get<std::vector<double>> (const std::string & name) {
    std::vector<double> val;

    this->logging.inf("Getting parameter from config file: Type: Real scalar array, Name: ", name);

    if (!this->init) {
        this->logging.err("Failed to get parameter from config file (Instance not initialized)");
        throw except_get::fail;
    }

    auto name_beg = this->name_fscl_arr.begin();
    auto name_end = this->name_fscl_arr.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = this->val_fscl_arr.begin();
    auto val_end = this->val_fscl_arr.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        this->logging.err("Failed to get parameter from config file (Name not found)");
        throw except_get::fail;
    }

    val = *val_itr;
    this->logging.inf("Got parameter from config file: Value: ", val);

    return val;
}

template <>
std::vector<linalg::ivector> instance::get<std::vector<linalg::ivector>> (const std::string & name) {
    std::vector<linalg::ivector> val;

    this->logging.inf("Getting parameter from config file: Type: Integer vector array, Name: ", name);

    if (!this->init) {
        this->logging.err("Failed to get parameter from config file (Instance not initialized)");
        throw except_get::fail;
    }

    auto name_beg = this->name_ivec_arr.begin();
    auto name_end = this->name_ivec_arr.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = this->val_ivec_arr.begin();
    auto val_end = this->val_ivec_arr.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        this->logging.err("Failed to get parameter from config file (Name not found)");
        throw except_get::fail;
    }

    val = *val_itr;
    this->logging.inf("Got parameter from config file: Value: ", val);

    return val;
}

template <>
std::vector<linalg::fvector> instance::get<std::vector<linalg::fvector>> (const std::string & name) {
    std::vector<linalg::fvector> val;

    this->logging.inf("Getting parameter from config file: Type: Real vector array, Name: ", name);

    if (!this->init) {
        this->logging.err("Failed to get parameter from config file (Instance not initialized)");
        throw except_get::fail;
    }

    auto name_beg = this->name_fvec_arr.begin();
    auto name_end = this->name_fvec_arr.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = this->val_fvec_arr.begin();
    auto val_end = this->val_fvec_arr.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        this->logging.err("Failed to get parameter from config file (Name not found)");
        throw except_get::fail;
    }

    val = *val_itr;
    this->logging.inf("Got parameter from config file: Value: ", val);

    return val;
}

template <>
std::vector<linalg::imatrix> instance::get<std::vector<linalg::imatrix>> (const std::string & name) {
    std::vector<linalg::imatrix> val;

    this->logging.inf("Getting parameter from config file: Type: Integer matrix array, Name: ", name);

    if (!this->init) {
        this->logging.err("Failed to get parameter from config file (Instance not initialized)");
        throw except_get::fail;
    }

    auto name_beg = this->name_imat_arr.begin();
    auto name_end = this->name_imat_arr.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = this->val_imat_arr.begin();
    auto val_end = this->val_imat_arr.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        this->logging.err("Failed to get parameter from config file (Name not found)");
        throw except_get::fail;
    }

    val = *val_itr;
    this->logging.inf("Got parameter from config file: Value: ", val);

    return val;
}

template <>
std::vector<linalg::fmatrix> instance::get<std::vector<linalg::fmatrix>> (const std::string & name) {
    std::vector<linalg::fmatrix> val;

    this->logging.inf("Getting parameter from config file: Type: Real matrix array, Name: ", name);

    if (!this->init) {
        this->logging.err("Failed to get parameter from config file (Instance not initialized)");
        throw except_get::fail;
    }

    auto name_beg = this->name_fmat_arr.begin();
    auto name_end = this->name_fmat_arr.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = this->val_fmat_arr.begin();
    auto val_end = this->val_fmat_arr.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        this->logging.err("Failed to get parameter from config file (Name not found)");
        throw except_get::fail;
    }

    val = *val_itr;
    this->logging.inf("Got parameter from config file: Value: ", val);

    return val;
}

void instance::load (void) {
    std::string stmt, name, repr;

    this->logging.inf("Loading parameter from config file");

    try {
        this->read(stmt);
        this->dissect(stmt, name, repr);
    } catch (except_read except) {
        switch (except) {
            case except_read::fail:
                this->logging.err("Failed to load parameter from config file (", std::strerror(errno), ")");
                throw except_load::fail;
            case except_read::eof:
                this->logging.inf("No more parameters in config file");
                throw except_load::eof;
        }
    } catch (except_dissect except) {
        switch (except) {
            case except_dissect::fail:
                this->logging.err("Failed to load parameter from config file (Syntax error)");
                throw except_load::fail;
            case except_dissect::blank:
                this->logging.inf("No more parameters in config file");
                throw except_load::eof;
        }
    }

    auto pred = [name] (auto arg) {
        return (arg == name);
    };

    if (std::any_of(this->name_all.begin(), this->name_all.end(), pred)) {
        this->logging.err("Failed to load parameter from config file (Repeated name)");
        throw except_load::fail;
    }

    try {
        std::string val;
        this->parse(repr, val);
        this->name_all.push_back(name);
        this->name_str.push_back(name);
        this->val_str.push_back(val);
        this->logging.inf("Loaded parameter from config file: Type: String, Name: ", name, ", Value: ", val);
        return;
    } catch (except_parse) {}

    try {
        std::uint8_t val;
        this->parse(repr, val);
        this->name_all.push_back(name);
        this->name_byte.push_back(name);
        this->val_byte.push_back(val);
        this->logging.inf("Loaded parameter from config file: Type: Byte, Name: ", name, ", Value: ", val);
        return;
    } catch (except_parse) {}

    try {
        bool val;
        this->parse(repr, val);
        this->name_all.push_back(name);
        this->name_bool.push_back(name);
        this->val_bool.push_back(val);
        this->logging.inf("Loaded parameter from config file: Type: Boolean, Name: ", name, ", Value: ", val);
        return;
    } catch (except_parse) {}

    try {
        int val;
        this->parse(repr, val);
        this->name_all.push_back(name);
        this->name_iscl.push_back(name);
        this->val_iscl.push_back(val);
        this->logging.inf("Loaded parameter from config file: Type: Integer scalar, Name: ", name, ", Value: ", val);
        return;
    } catch (except_parse) {}

    try {
        double val;
        this->parse(repr, val);
        this->name_all.push_back(name);
        this->name_fscl.push_back(name);
        this->val_fscl.push_back(val);
        this->logging.inf("Loaded parameter from config file: Type: Real scalar, Name: ", name, ", Value: ", val);
        return;
    } catch (except_parse) {}

    try {
        linalg::ivector val;
        this->parse(repr, val);
        this->name_all.push_back(name);
        this->name_ivec.push_back(name);
        this->val_ivec.push_back(val);
        this->logging.inf("Loaded parameter from config file: Type: Integer vector, Name: ", name, ", Value: ", val);
        return;
    } catch (except_parse) {}

    try {
        linalg::fvector val;
        this->parse(repr, val);
        this->name_all.push_back(name);
        this->name_fvec.push_back(name);
        this->val_fvec.push_back(val);
        this->logging.inf("Loaded parameter from config file: Type: Real vector, Name: ", name, ", Value: ", val);
        return;
    } catch (except_parse) {}

    try {
        linalg::imatrix val;
        this->parse(repr, val);
        this->name_all.push_back(name);
        this->name_imat.push_back(name);
        this->val_imat.push_back(val);
        this->logging.inf("Loaded parameter from config file: Type: Integer matrix, Name: ", name, ", Value: ", val);
        return;
    } catch (except_parse) {}

    try {
        linalg::fmatrix val;
        this->parse(repr, val);
        this->name_all.push_back(name);
        this->name_fmat.push_back(name);
        this->val_fmat.push_back(val);
        this->logging.inf("Loaded parameter from config file: Type: Real matrix, Name: ", name, ", Value: ", val);
        return;
    } catch (except_parse) {}

    try {
        std::vector<std::string> val;
        this->parse(repr, val);
        this->name_all.push_back(name);
        this->name_str_arr.push_back(name);
        this->val_str_arr.push_back(val);
        this->logging.inf("Loaded parameter from config file: Type: String array, Name: ", name, ", Value: ", val);
        return;
    } catch (except_parse) {}

    try {
        std::vector<std::uint8_t> val;
        this->parse(repr, val);
        this->name_all.push_back(name);
        this->name_byte_arr.push_back(name);
        this->val_byte_arr.push_back(val);
        this->logging.inf("Loaded parameter from config file: Type: Byte array, Name: ", name, ", Value: ", val);
        return;
    } catch (except_parse) {}

    try {
        std::vector<bool> val;
        this->parse(repr, val);
        this->name_all.push_back(name);
        this->name_bool_arr.push_back(name);
        this->val_bool_arr.push_back(val);
        this->logging.inf("Loaded parameter from config file: Type: Boolean array, Name: ", name, ", Value: ", val);
        return;
    } catch (except_parse) {}

    try {
        std::vector<int> val;
        this->parse(repr, val);
        this->name_all.push_back(name);
        this->name_iscl_arr.push_back(name);
        this->val_iscl_arr.push_back(val);
        this->logging.inf("Loaded parameter from config file: Type: Integer scalar array, Name: ", name, ", Value: ", val);
        return;
    } catch (except_parse) {}

    try {
        std::vector<double> val;
        this->parse(repr, val);
        this->name_all.push_back(name);
        this->name_fscl_arr.push_back(name);
        this->val_fscl_arr.push_back(val);
        this->logging.inf("Loaded parameter from config file: Type: Real scalar array, Name: ", name, ", Value: ", val);
        return;
    } catch (except_parse) {}

    try {
        std::vector<linalg::ivector> val;
        this->parse(repr, val);
        this->name_all.push_back(name);
        this->name_ivec_arr.push_back(name);
        this->val_ivec_arr.push_back(val);
        this->logging.inf("Loaded parameter from config file: Type: Integer vector array, Name: ", name, ", Value: ", val);
        return;
    } catch (except_parse) {}

    try {
        std::vector<linalg::fvector> val;
        this->parse(repr, val);
        this->name_all.push_back(name);
        this->name_fvec_arr.push_back(name);
        this->val_fvec_arr.push_back(val);
        this->logging.inf("Loaded parameter from config file: Type: Real vector array, Name: ", name, ", Value: ", val);
        return;
    } catch (except_parse) {}

    try {
        std::vector<linalg::imatrix> val;
        this->parse(repr, val);
        this->name_all.push_back(name);
        this->name_imat_arr.push_back(name);
        this->val_imat_arr.push_back(val);
        this->logging.inf("Loaded parameter from config file: Type: Integer matrix array, Name: ", name, ", Value: ", val);
        return;
    } catch (except_parse) {}

    try {
        std::vector<linalg::fmatrix> val;
        this->parse(repr, val);
        this->name_all.push_back(name);
        this->name_fmat_arr.push_back(name);
        this->val_fmat_arr.push_back(val);
        this->logging.inf("Loaded parameter from config file: Type: Real matrix array, Name: ", name, ", Value: ", val);
        return;
    } catch (except_parse) {}

    this->logging.err("Failed to load parameter from config file (Syntax error)");
    throw except_load::fail;
}

void instance::read (std::string & stmt) {
    bool cmmt = false, quot = false, escp = false, delm = false;
    char chr;

    this->file.clear();
    this->file.get();
    if (this->file.fail()) {
        if (this->file.eof()) {
            throw except_read::eof;
        } else {
            throw except_read::fail;
        }
    }
    this->file.unget();

    stmt.clear();
    while (true) {
        this->file.clear();
        chr = this->file.get();
        if (this->file.fail()) {
            if (this->file.eof()) {
                this->strip(stmt);
                return;
            } else {
                throw except_read::fail;
            }
        }

        if (cmmt) {
            if (chr == '\n') {
                cmmt = false;
            }
        } else if (quot) {
            if (escp) {
                escp = false;
            } else {
                if (chr == '\\') {
                    escp = true;
                } else if (chr == '"') {
                    quot = false;
                }
            }
        } else {
            if (chr == '#') {
                cmmt = true;
            } else if (chr == '"') {
                quot = true;
            } else if (chr == ';') {
                delm = true;
            }
        }

        if (!cmmt) {
            stmt.push_back(chr);
        }

        if (delm) {
            this->strip(stmt);
            return;
        }
    }
}

void instance::dissect (const std::string & str, std::string & name, std::string & repr) {
    if (str.empty()) {
        throw except_dissect::blank;
    }

    if (str.back() != ';') {
        throw except_dissect::fail;
    }

    auto str_beg = str.begin();
    auto str_end = str.end();
    auto str_eql = std::find(str_beg, str_end, '=');

    if (str_eql == str_end) {
        throw except_dissect::fail;
    }

    name.clear();
    for (auto itr = str_beg; itr < str_eql; itr++) {
        name.push_back(*itr);
    }

    repr.clear();
    for (auto itr = str_eql + 1; itr < str_end - 1; itr++) {
        repr.push_back(*itr);
    }

    this->strip(name);
    this->strip(repr);

    if (name.empty() || repr.empty()) {
        throw except_dissect::fail;
    }

    auto pred = [] (auto arg) {
        return (std::isalnum(arg) || arg == '_');
    };

    if (!std::isalpha(name.front()) || !std::all_of(name.begin(), name.end(), pred)) {
        throw except_dissect::fail;
    }
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

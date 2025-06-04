#include <cctype>

#include <iterator>
#include <algorithm>
#include <string>
#include <vector>

#include <sys/logging.hpp>
#include <sys/option.hpp>

namespace option {

bool tracker::exist = false;

bool tracker::instantiate (void) {
    if (tracker::exist) {
        return false;
    } else {
        tracker::exist = true;
        return true;
    }
}

instance::instance (int argc, char ** argv, const std::vector<std::string> & names) : logging("option"), init(false) {
    std::vector<std::string> args;
    for (int i = 1; i < argc; i++) {
        args.push_back(argv[i]);
    }

    this->logging.inf("Initializing instance: Args: ", args, ", Names: ", names);

    if (!instance::instantiate()) {
        this->logging.err("Failed to initialize instance (Instance already exists)");
        throw except_ctor::fail;
    }

    this->logging.inf("Parsing options");

    for (auto itr1 = names.begin(); itr1 < names.end() - 1; itr1++) {
        for (auto itr2 = itr1 + 1; itr2 < names.end(); itr2++) {
            if (*itr1 == *itr2) {
                this->logging.err("Failed to parse options (Repeated name)");
                this->logging.err("Failed to initialize instance");
                throw except_ctor::fail;
            }
        }
    }

    for (auto elem : names) {
        auto pred = [] (auto arg) {
            return (std::isalnum(arg) || arg == '-');
        };

        if (elem.empty()) {
            this->logging.err("Failed to parse options (Invalid name)");
            this->logging.err("Failed to initialize instance");
            throw except_ctor::fail;
        }

        if (!std::isalpha(elem.front()) || !std::all_of(elem.begin(), elem.end(), pred)) {
            this->logging.err("Failed to parse options (Invalid name)");
            this->logging.err("Failed to initialize instance");
            throw except_ctor::fail;
        }
    }

    for (auto elem : args) {
        auto pred = [elem] (auto arg) {
            return (elem == "--" + arg);
        };

        if (std::none_of(names.begin(), names.end(), pred)) {
            this->logging.err("Failed to parse options (Unrecognized argument)");
            this->logging.err("Failed to initialize instance");
            throw except_ctor::fail;
        }
    }

    for (auto elem : names) {
        this->logging.inf("Parsing option: Name: ", elem);

        auto pred = [elem] (auto arg) {
            return (arg == "--" + elem);
        };

        this->name.push_back(elem);
        this->val.push_back(std::any_of(args.begin(), args.end(), pred));

        this->logging.inf("Parsed option: Value: ", bool(this->val.back()));
    }

    this->init = true;
}

bool instance::get (const std::string & name) {
    bool val;

    this->logging.inf("Getting option: Name: ", name);

    if (!this->init) {
        this->logging.err("Failed to get option (Instance not initialized)");
        throw except_get::fail;
    }

    auto name_beg = this->name.begin();
    auto name_end = this->name.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = this->val.begin();
    auto val_end = this->val.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        this->logging.err("Failed to get option (Name not found)");
        throw except_get::fail;
    }

    val = *val_itr;
    this->logging.inf("Got option: Value: ", val);

    return val;
}

}

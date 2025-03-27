#include <iterator>
#include <algorithm>
#include <string>
#include <vector>

#include <sys/logging.hpp>
#include <sys/option.hpp>

namespace option :: intern {

std::string mod = "option";

bool fail;

std::vector<std::string> name;
std::vector<bool> val;

}

namespace option {

bool fail (void) {
    return intern::fail;
}

void init (std::vector<std::string> args, std::vector<std::string> names) {
    logging::inf(intern::mod,
        "Parsing options: Args: ", args, ", Names: ", names
    );

    for (auto itr1 = names.begin(); itr1 < names.end() - 1; itr1++) {
        for (auto itr2 = itr1 + 1; itr2 < names.end(); itr2++) {
            if (*itr1 == *itr2) {
                logging::err(intern::mod,
                    "Failed to parse options (Repeated name)"
                );
                intern::fail = true;
                return;
            }
        }
    }

    for (auto elem : names) {
        auto pred = [elem] (auto arg) {
            return (arg == "--" + elem);
        };

        intern::name.push_back(elem);
        intern::val.push_back(std::any_of(args.begin(), args.end(), pred));

        logging::inf(intern::mod,
            "Parsed option: Name: ", intern::name.back(), ", Value: ", bool(intern::val.back())
        );
    }

    intern::fail = false;
}

bool get (std::string name) {
    bool val;

    logging::inf(intern::mod,
        "Getting option: Name: ", name
    );

    auto name_beg = intern::name.begin();
    auto name_end = intern::name.end();
    auto name_itr = std::find(name_beg, name_end, name);

    auto val_beg = intern::val.begin();
    auto val_end = intern::val.end();
    auto val_itr = val_beg + std::distance(name_beg, name_itr);

    if (val_itr == val_end) {
        logging::err(intern::mod,
            "Failed to get option (Name not found)"
        );
        intern::fail = true;
        return val;
    }

    val = *val_itr;
    logging::inf(intern::mod,
        "Got option: Value: ", val
    );

    intern::fail = false;
    return val;
}

}

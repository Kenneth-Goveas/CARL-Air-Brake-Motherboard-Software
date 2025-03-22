#include <cstring>
#include <cerrno>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <sys/logging.hpp>

#include <dat/csvwrite.hpp>

namespace csvwrite :: intern {

std::string mod = "csvwrite";

bool fail;

std::ofstream file;
int count;

std::string strip (std::string word);

}

namespace csvwrite {

bool fail (void) {
    return intern::fail;
}

void open (std::string path) {
    logging::inf(intern::mod,
        "Opening CSV file: Path: ", path
    );

    intern::file.open(path);
    if (intern::file.fail()) {
        logging::err(intern::mod,
            "Failed to open CSV file (", std::strerror(errno), ")"
        );
        intern::file.clear();
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

void close (void) {
    logging::inf(intern::mod,
        "Closing CSV file"
    );

    intern::file.close();
    if (intern::file.fail()) {
        logging::err(intern::mod,
            "Failed to close CSV file (", std::strerror(errno), ")"
        );
        intern::file.clear();
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

void put_head (std::vector<std::string> head) {
    logging::inf(intern::mod,
        "Writing dataset head to CSV file: Head: ", head
    );

    for (auto itr = head.begin(); itr < head.end(); itr++) {
        *itr = intern::strip(*itr);
    }

    if (head.size() == 0) {
        logging::err(intern::mod,
            "Failed to write dataset head to CSV file (Invalid dimension)"
        );
        intern::fail = true;
        return;
    }

    for (auto elem : head) {
        if (elem.size() == 0) {
            logging::err(intern::mod,
                "Failed to write dataset head to CSV file (Syntax error)"
            );
            intern::fail = true;
            return;
        }
    }

    for (auto itr1 = head.begin(); itr1 < head.end() - 1; itr1++) {
        for (auto itr2 = itr1 + 1; itr2 < head.end(); itr2++) {
            if (*itr1 == *itr2) {
                logging::err(intern::mod,
                    "Failed to write dataset head to CSV file (Repeated name)"
                );
                intern::fail = true;
                return;
            }
        }
    }

    for (auto itr = head.begin(); itr < head.end(); itr++) {
        intern::file << *itr;
        if (itr < head.end() - 1) {
            intern::file << ",";
        } else {
            intern::file << std::endl;
        }
    }

    if (intern::file.fail()) {
        logging::err(intern::mod,
            "Failed to write dataset head to CSV file (", std::strerror(errno), ")"
        );
        intern::file.clear();
        intern::fail = true;
        return;
    }

    intern::count = head.size();

    intern::fail = false;
}

void put_body (std::vector<double> body) {
    logging::inf(intern::mod,
        "Writing dataset body entry to CSV file: Body: ", body
    );

    if (body.size() != intern::count) {
        logging::err(intern::mod,
            "Failed to write dataset body entry to CSV file (Inconsistent dimension)"
        );
        intern::fail = true;
        return;
    }

    for (auto itr = body.begin(); itr < body.end(); itr++) {
        intern::file << std::showpos << std::scientific << std::setprecision(10) << *itr;
        if (itr < body.end() - 1) {
            intern::file << ",";
        } else {
            intern::file << std::endl;
        }
    }

    if (intern::file.fail()) {
        logging::err(intern::mod,
            "Failed to write dataset body entry to CSV file (", std::strerror(errno), ")"
        );
        intern::file.clear();
        intern::fail = true;
        return;
    }

    intern::fail = false;
}

}

namespace csvwrite :: intern {

std::string strip (std::string word) {
    while (!word.empty()) {
        if (word.front() == ' ' || word.front() == '\t') {
            word.erase(word.begin());
        } else {
            break;
        }
    }
    while (!word.empty()) {
        if (word.back() == ' ' || word.back() == '\t') {
            word.erase(word.end() - 1);
        } else {
            break;
        }
    }
    return word;
}

}

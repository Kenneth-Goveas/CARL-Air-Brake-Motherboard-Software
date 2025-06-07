#include <cctype>
#include <cstring>
#include <cerrno>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>

#include <sys/logging.hpp>
#include <dat/csvread.hpp>

namespace csvread {

int tracker::count = 0;

int tracker::instantiate (void) {
    tracker::count++;
    return tracker::count;
}

instance::instance (const std::string & path) : logging("csvread"), init(false), id(instance::instantiate()), idx(0) {
    bool file_open = false;
    std::ifstream file;
    std::string line;

    this->logging.inf("Initializing instance #", this->id, ": Path: ", path);

    try {
        this->logging.inf("Opening CSV file #", this->id);
        file.clear();
        file.open(path);
        if (file.fail()) {
            this->logging.err("Failed to open CSV file #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }

        file_open = true;

        this->logging.inf("Reading line from CSV file #", this->id);
        file.clear();
        std::getline(file, line);
        if (file.fail()) {
            if (file.eof()) {
                this->logging.err("Failed to read line from CSV file #", this->id, " (End of file)");
                throw except_intern{};
            } else {
                this->logging.err("Failed to read line from CSV file #", this->id, " (", std::strerror(errno), ")");
                throw except_intern{};
            }
        }
        this->logging.inf("Read line from CSV file #", this->id, ": Line: ", line);

        try {
            this->parse_head(line, this->head);
        } catch (except_parse_head) {
            throw except_intern{};
        }

        while (true) {
            this->logging.inf("Reading line from CSV file #", this->id);
            file.clear();
            std::getline(file, line);
            if (file.fail()) {
                if (file.eof()) {
                    this->logging.inf("No more lines in CSV file #", this->id);
                    break;
                } else {
                    this->logging.err("Failed to read line from CSV file #", this->id, " (", std::strerror(errno), ")");
                    throw except_intern{};
                }
            }
            this->logging.inf("Read line from CSV file #", this->id, ": Line: ", line);

            try {
                this->body.resize(this->body.size() + 1);
                this->parse_body(line, this->body.back());
            } catch (except_parse_body) {
                throw except_intern{};
            }
        }

        file_open = false;

        this->logging.inf("Closing CSV file #", this->id);
        file.clear();
        file.close();
        if (file.fail()) {
            this->logging.err("Failed to close CSV file #", this->id, " (", std::strerror(errno), ")");
            throw except_intern{};
        }
    } catch (except_intern) {
        if (file_open) {
            this->logging.wrn("Closing CSV file #", this->id);
            file.clear();
            file.close();
            if (file.fail()) {
                this->logging.err("Failed to close CSV file #", this->id, " (", std::strerror(errno), ")");
            }
        }

        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    this->init = true;
}

std::vector<std::string> instance::get_head (void) {
    this->logging.inf("Getting head from CSV file #", this->id);

    if (!this->init) {
        this->logging.err("Failed to get head from CSV file #", this->id, " (Instance not initialized)");
        throw except_get_head::fail;
    }

    this->logging.inf("Got head from CSV file #", this->id, ": Head: ", this->head);

    return this->head;
}

std::vector<double> instance::get_body (void) {
    this->logging.inf("Getting body from CSV file #", this->id);

    if (!this->init) {
        this->logging.err("Failed to get body from CSV file #", this->id, " (Instance not initialized)");
        throw except_get_body::fail;
    }

    if (this->idx == int(this->body.size())) {
        this->logging.wrn("No more body in CSV file #", this->id);
        throw except_get_body::eof;
    }

    this->idx++;

    this->logging.inf("Got body from CSV file #", this->id, ": Body: ", this->body[this->idx - 1]);

    return this->body[this->idx - 1];
}

void instance::split (const std::string & str, std::vector<std::string> & arr) {
    arr = std::vector<std::string>(1);

    for (auto elem : str) {
        if (elem == ',') {
            arr.push_back("");
        } else {
            arr.back().push_back(elem);
        }
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

void instance::parse_head (const std::string & str, std::vector<std::string> & head) {
    this->logging.inf("Parsing line as head");

    this->split(str, head);

    for (auto elem : head) {
        if (elem.empty()) {
            this->logging.err("Failed to parse line as head (Syntax error)");
            throw except_parse_head::fail;
        }
    }

    for (auto itr1 = head.begin(); itr1 < head.end() - 1; itr1++) {
        for (auto itr2 = itr1 + 1; itr2 < head.end(); itr2++) {
            if (*itr1 == *itr2) {
                this->logging.err("Failed to parse line as head (Repeated name)");
                throw except_parse_head::fail;
            }
        }
    }

    this->logging.inf("Parsed line as head: ", head);
}

void instance::parse_body (const std::string & str, std::vector<double> & body) {
    std::vector<std::string> arr;

    this->logging.inf("Parsing line as body");

    this->split(str, arr);

    if (arr.size() != this->head.size()) {
        this->logging.err("Failed to parse line as body (Inconsistent dimension)");
        throw except_parse_body::fail;
    }

    body.resize(arr.size());
    for (auto itr = arr.begin(); itr < arr.end(); itr++) {
        std::istringstream istr(*itr);
        istr >> *(body.begin() + std::distance(arr.begin(), itr));
        if (istr.fail() || !istr.eof()) {
            this->logging.err("Failed to parse line as body (Syntax error)");
            throw except_parse_body::fail;
        }
    }

    this->logging.inf("Parsed line as body: ", body);
}

}

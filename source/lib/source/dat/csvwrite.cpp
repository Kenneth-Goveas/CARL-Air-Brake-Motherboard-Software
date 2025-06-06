#include <cctype>
#include <cstring>
#include <cerrno>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <sys/logging.hpp>
#include <dat/csvwrite.hpp>

namespace csvwrite {

int tracker::count = 0;

int tracker::instantiate (void) {
    tracker::count++;
    return tracker::count;
}

instance::instance (const std::string & path) : logging("csvwrite"), init(false), id(instance::instantiate()), dim(0) {
    this->logging.inf("Initializing instance #", this->id, ": Path: ", path);

    this->logging.inf("Opening CSV file #", this->id);
    this->file.clear();
    this->file.open(path);
    if (this->file.fail()) {
        this->logging.err("Failed to open CSV file #", this->id, " (", std::strerror(errno), ")");
        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    this->init = true;
}

instance::~instance (void) {
    if (this->init) {
        this->logging.inf("Closing CSV file #", this->id);
        this->file.clear();
        this->file.close();
        if (this->file.fail()) {
            this->logging.err("Failed to close CSV file #", this->id, " (", std::strerror(errno), ")");
        }
    }
}

void instance::put_head (const std::vector<std::string> & head) {
    this->logging.inf("Writing head to CSV file #", this->id, ": Head: ", head);

    if (!this->init) {
        this->logging.err("Failed to write head to CSV file #", this->id, " (Instance not initialized)");
        throw except_put_head::fail;
    }

    if (this->dim > 0) {
        this->logging.err("Failed to write head to CSV file #", this->id, " (Head already written)");
        throw except_put_head::fail;
    }

    if (head.empty()) {
        this->logging.err("Failed to write head to CSV file #", this->id, " (Invalid dimension)");
        throw except_put_head::fail;
    }

    for (auto elem : head) {
        if (elem.empty()) {
            this->logging.err("Failed to write head to CSV file #", this->id, " (Invalid name)");
            throw except_put_head::fail;
        } else if (std::isspace(elem.front()) || std::isspace(elem.back())) {
            this->logging.err("Failed to write head to CSV file #", this->id, " (Invalid name)");
            throw except_put_head::fail;
        }
    }

    for (auto itr1 = head.begin(); itr1 < head.end() - 1; itr1++) {
        for (auto itr2 = itr1 + 1; itr2 < head.end(); itr2++) {
            if (*itr1 == *itr2) {
                this->logging.err("Failed to write head to CSV file #", this->id, " (Repeated name)");
                throw except_put_head::fail;
            }
        }
    }

    this->file.clear();
    for (auto itr = head.begin(); itr < head.end(); itr++) {
        this->file << *itr << (itr < head.end() - 1 ? "," : "");
    }
    this->file << std::endl;

    if (this->file.fail()) {
        this->logging.err("Failed to write head to CSV file #", this->id, " (", std::strerror(errno), ")");
        throw except_put_head::fail;
    }

    this->dim = head.size();
}

void instance::put_body (const std::vector<double> & body) {
    this->logging.inf("Writing body to CSV file #", this->id, ": Body: ", body);

    if (!this->init) {
        this->logging.err("Failed to write body to CSV file #", this->id, " (Instance not initialized)");
        throw except_put_body::fail;
    }

    if (this->dim == 0) {
        this->logging.err("Failed to write body to CSV file #", this->id, " (Head not written)");
        throw except_put_body::fail;
    }

    if (int(body.size()) != this->dim) {
        this->logging.err("Failed to write body to CSV file #", this->id, " (Inconsistent dimension)");
        throw except_put_body::fail;
    }

    auto flags = this->file.flags();
    auto precision = this->file.precision();

    this->file.clear();
    for (auto itr = body.begin(); itr < body.end(); itr++) {
        this->file << std::showpos << std::scientific << std::setprecision(10) << *itr << (itr < body.end() - 1 ? "," : "");
    }
    this->file << std::endl;

    this->file.flags(flags);
    this->file.precision(precision);

    if (this->file.fail()) {
        this->logging.err("Failed to write body to CSV file #", this->id, " (", std::strerror(errno), ")");
        throw except_put_body::fail;
    }
}

}

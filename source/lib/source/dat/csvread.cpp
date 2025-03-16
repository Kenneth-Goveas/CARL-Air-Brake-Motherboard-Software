#include <cstring>
#include <cerrno>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <sys/logging.hpp>

#include <dat/csvread.hpp>

namespace csvread :: intern {

std::string mod = "csvread";

bool fail;
bool eof;

std::vector<std::string> head;
std::vector<std::vector<double>> body;
int index;

std::vector<std::string> split (std::string line);
std::string strip (std::string word);

std::vector<std::string> parse_head (std::string line);
std::vector<double> parse_body (std::string line);

}

namespace csvread {

bool fail (void) {
    return intern::fail;
}

bool eof (void) {
    return intern::eof;
}

void open (std::string path) {
    std::ifstream file;
    std::string line;
    std::vector<std::string> head;
    std::vector<std::vector<double>> body;

    logging::inf(intern::mod,
        "Loading dataset: Path: ", path
    );

    logging::inf(intern::mod,
        "Opening CSV file"
    );

    file.open(path);
    if (file.fail()) {
        logging::err(intern::mod,
            "Failed to open CSV file (", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to load dataset"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Reading line from CSV file"
    );

    std::getline(file, line);
    if (file.fail()) {
        logging::err(intern::mod,
            "Failed to read line from CSV file (", std::strerror(errno), ")"
        );
        logging::wrn(intern::mod,
            "Closing CSV file"
        );
        file.clear();
        file.close();
        if (file.fail()) {
            logging::err(intern::mod,
                "Failed to close CSV file (", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to load dataset"
        );
        intern::fail = true;
        return;
    }

    logging::inf(intern::mod,
        "Read line from CSV file: Line: ", line
    );

    head = intern::parse_head(line);
    if (intern::fail) {
        logging::wrn(intern::mod,
            "Closing CSV file"
        );
        file.close();
        if (file.fail()) {
            logging::err(intern::mod,
                "Failed to close CSV file (", std::strerror(errno), ")"
            );
        }
        logging::err(intern::mod,
            "Failed to load dataset"
        );
        intern::fail = true;
        return;
    }

    while (true) {
        logging::inf(intern::mod,
            "Reading line from CSV file"
        );
        std::getline(file, line);
        if (file.fail()) {
            if (file.eof()) {
                logging::inf(intern::mod,
                    "No more lines in CSV file"
                );
                file.clear();
                break;
            } else {
                logging::err(intern::mod,
                    "Failed to read line from CSV file (", std::strerror(errno), ")"
                );
                logging::wrn(intern::mod,
                    "Closing CSV file"
                );
                file.clear();
                file.close();
                if (file.fail()) {
                    logging::err(intern::mod,
                        "Failed to close CSV file (", std::strerror(errno), ")"
                    );
                }
                logging::err(intern::mod,
                    "Failed to load dataset"
                );
                intern::fail = true;
                return;
            }
        } else {
            logging::inf(intern::mod,
                "Read line from CSV file: Line: ", line
            );
            body.push_back(intern::parse_body(line));
            if (intern::fail) {
                logging::wrn(intern::mod,
                    "Closing CSV file"
                );
                file.close();
                if (file.fail()) {
                    logging::err(intern::mod,
                        "Failed to close CSV file (", std::strerror(errno), ")"
                    );
                }
                logging::err(intern::mod,
                    "Failed to load dataset"
                );
                intern::fail = true;
                return;
            }
        }
    }

    logging::inf(intern::mod,
        "Closing CSV file"
    );

    file.close();
    if (file.fail()) {
        logging::err(intern::mod,
            "Failed to close CSV file (", std::strerror(errno), ")"
        );
        logging::err(intern::mod,
            "Failed to load dataset"
        );
        intern::fail = true;
        return;
    }

    for (auto elem : body) {
        if (elem.size() != head.size()) {
            logging::err(intern::mod,
                "Failed to load dataset (Inconsistent dimension)"
            );
            intern::fail = true;
            return;
        }
    }

    intern::head = head;
    intern::body = body;
    intern::index = 0;

    intern::fail = false;
}

void close (void) {
    logging::inf(intern::mod,
        "Clearing dataset"
    );

    intern::head.clear();
    intern::body.clear();
    intern::index = 0;
}

std::vector<std::string> get_head (void) {
    std::vector<std::string> head;

    logging::inf(intern::mod,
        "Getting dataset head"
    );

    head = intern::head;

    logging::inf(intern::mod,
        "Got dataset head: ", head
    );

    return head;
}

std::vector<double> get_body (void) {
    std::vector<double> body;

    logging::inf(intern::mod,
        "Getting dataset body entry"
    );

    if (intern::index == intern::body.size()) {
        logging::inf(intern::mod,
            "No more entries in dataset body"
        );
        intern::eof = true;
        return body;
    }

    body = intern::body[intern::index];
    intern::index++;

    logging::inf(intern::mod,
        "Got dataset body entry: ", body
    );

    intern::eof = false;
    return body;
}

}

namespace csvread :: intern {

std::vector<std::string> split (std::string line) {
    std::vector<std::string> words = {""};

    for (auto elem : line) {
        if (elem == ',') {
            words.push_back("");
        } else {
            words.back() += elem;
        }
    }

    return words;
}

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

std::vector<std::string> parse_head (std::string line) {
    std::vector<std::string> head;

    logging::inf(mod,
        "Parsing line as dataset head"
    );

    head = split(line);
    for (auto itr = head.begin(); itr < head.end(); itr++) {
        *itr = strip(*itr);
    }

    for (auto elem : head) {
        if (elem.size() == 0) {
            logging::err(mod,
                "Failed to parse line as dataset head (Syntax error)"
            );
            fail = true;
            return head;
        }
    }

    for (auto itr1 = head.begin(); itr1 < head.end() - 1; itr1++) {
        for (auto itr2 = itr1 + 1; itr2 < head.end(); itr2++) {
            if (*itr1 == *itr2) {
                logging::err(mod,
                    "Failed to parse line as dataset head (Repeated name)"
                );
                fail = true;
                return head;
            }
        }
    }

    logging::inf(mod,
        "Parsed line as dataset head: ", head
    );

    fail = false;
    return head;
}

std::vector<double> parse_body (std::string line) {
    std::vector<double> body;
    std::vector<std::string> words;
    std::istringstream istr;

    logging::inf(mod,
        "Parsing line as dataset body entry"
    );

    words = split(line);
    for (auto itr = words.begin(); itr < words.end(); itr++) {
        *itr = strip(*itr);
    }

    for (auto elem : words) {
        if (elem.size() == 0) {
            logging::err(mod,
                "Failed to parse line as dataset body entry (Syntax error)"
            );
            fail = true;
            return body;
        }
    }

    for (auto elem : words) {
        body.resize(body.size() + 1);
        istr = std::istringstream(elem);
        istr >> body.back();
        if (istr.fail() || !istr.eof()) {
            logging::err(mod,
                "Failed to parse line as dataset body entry (Syntax error)"
            );
            fail = true;
            return body;
        }
    }

    logging::inf(mod,
        "Parsed line as dataset body entry: ", body
    );

    fail = false;
    return body;
}

}

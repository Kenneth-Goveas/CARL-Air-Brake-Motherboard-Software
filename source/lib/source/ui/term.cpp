#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <sys/logging.hpp>

#include <ui/term.hpp>

namespace term :: intern {

std::string mod = "term";

bool fail;
bool eof;

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

std::string conv_color_to_oupt (color clr);

std::string strip (std::string inpt);

void append (std::ostringstream * ostr);

template <typename type, typename ... types>
void append (std::ostringstream * ostr, type arg, types ... args);

}

namespace term {

bool fail (void) {
    return intern::fail;
}

bool eof (void) {
    return intern::eof;
}

void erase_line (void) {
    logging::inf(intern::mod,
        "Erasing line"
    );

    std::cout << "\e[2K";
    std::cout.flush();
}

void erase_screen (void) {
    logging::inf(intern::mod,
        "Erasing screen"
    );

    std::cout << "\e[2J";
    std::cout.flush();
}

void color_default (void) {
    logging::inf(intern::mod,
        "Setting default colors"
    );

    std::cout << "\e[m";
    std::cout.flush();
}

void color_fore (color clr) {
    std::string oupt;

    oupt = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Setting foreground color: ", oupt
    );

    std::cout << "\e[38;5;" << clr << "m";
    std::cout.flush();
}

void color_back (color clr) {
    std::string oupt;

    oupt = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Setting background color: ", oupt
    );

    std::cout << "\e[48;5;" << clr << "m";
    std::cout.flush();
}

void cursor_hide (void) {
    logging::inf(intern::mod,
        "Hiding cursor"
    );

    std::cout << "\e[?25l";
    std::cout.flush();
}

void cursor_show (void) {
    logging::inf(intern::mod,
        "Revealing cursor"
    );

    std::cout << "\e[?25h";
    std::cout.flush();
}

void cursor_row (int row) {
    logging::inf(intern::mod,
        "Moving cursor: Row: ", row
    );

    if (!(row > 0)) {
        logging::err(intern::mod,
            "Failed to move cursor (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    std::cout << "\e[" << row << "d";
    std::cout.flush();

    intern::fail = false;
}

void cursor_col (int col) {
    logging::inf(intern::mod,
        "Moving cursor: Col: ", col
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to move cursor (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    std::cout << "\e[" << col << "G";
    std::cout.flush();

    intern::fail = false;
}

void cursor_pos (int row, int col) {
    logging::inf(intern::mod,
        "Moving cursor: Row: ", row, ", Col: ", col
    );

    if (!(row > 0 && col > 0)) {
        logging::err(intern::mod,
            "Failed to move cursor (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    std::cout << "\e[" << row << ";" << col << "H";
    std::cout.flush();

    intern::fail = false;
}

void newline (void) {
    logging::inf(intern::mod,
        "Printing newline"
    );

    std::cout << std::endl;
}

template <>
bool scan<bool> (void) {
    std::string inpt, oupt;
    bool val;

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);

    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input supplied"
        );
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    inpt = intern::strip(inpt);

    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input as boolean"
    );

    val = intern::conv_inpt_to_val<bool>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input as boolean (Syntax error)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input as boolean: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::string scan<std::string> (void) {
    std::string inpt, oupt;
    std::string val;

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);

    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input supplied"
        );
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    inpt = intern::strip(inpt);

    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input as string"
    );

    val = intern::conv_inpt_to_val<std::string>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input as string (Syntax error)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input as string: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
int scan<int> (void) {
    std::string inpt, oupt;
    int val;

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);

    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input supplied"
        );
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    inpt = intern::strip(inpt);

    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input as integer"
    );

    val = intern::conv_inpt_to_val<int>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input as integer (Syntax error)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input as integer: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
double scan<double> (void) {
    std::string inpt, oupt;
    double val;

    logging::inf(intern::mod,
        "Waiting for input"
    );

    std::getline(std::cin, inpt);

    if (std::cin.eof()) {
        logging::wrn(intern::mod,
            "No input supplied"
        );
        std::cin.clear();
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    inpt = intern::strip(inpt);

    logging::inf(intern::mod,
        "Received input: “", inpt, "”"
    );

    logging::inf(intern::mod,
        "Parsing input as floating point"
    );

    val = intern::conv_inpt_to_val<double>(inpt);
    if (intern::fail) {
        logging::err(intern::mod,
            "Failed to parse input as floating point (Syntax error)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Parsed input as floating point: Value: ", oupt
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

}

namespace term :: intern {

template <>
bool conv_inpt_to_val<bool> (std::string inpt) {
    bool val;
    if (inpt == "yes") {
        val = true;
        fail = false;
        return val;
    } else if (inpt == "no") {
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
    std::string val = inpt;
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

std::string conv_color_to_oupt (color clr) {
    std::string oupt;

    switch (clr) {
        case black:
            oupt = "Black";
            break;
        case darkred:
            oupt = "Dark red";
            break;
        case darkgreen:
            oupt = "Dark green";
            break;
        case darkyellow:
            oupt = "Dark yellow";
            break;
        case darkblue:
            oupt = "Dark blue";
            break;
        case darkmagenta:
            oupt = "Dark magenta";
            break;
        case darkcyan:
            oupt = "Dark cyan";
            break;
        case lightgray:
            oupt = "Light gray";
            break;
        case darkgray:
            oupt = "Dark gray";
            break;
        case lightred:
            oupt = "Light red";
            break;
        case lightgreen:
            oupt = "Light green";
            break;
        case lightyellow:
            oupt = "Light yellow";
            break;
        case lightblue:
            oupt = "Light blue";
            break;
        case lightmagenta:
            oupt = "Light magenta";
            break;
        case lightcyan:
            oupt = "Light cyan";
            break;
        case white:
            oupt = "White";
            break;
    }

    return oupt;
}

std::string strip (std::string inpt) {
    while (inpt.size() > 0) {
        if (*(inpt.begin()) == ' ' || *(inpt.begin()) == '\t') {
            inpt.erase(inpt.begin());
        } else {
            break;
        }
    }

    while (inpt.size() > 0) {
        if (*(inpt.end() - 1) == ' ' || *(inpt.end() - 1) == '\t') {
            inpt.erase(inpt.end() - 1);
        } else {
            break;
        }
    }

    return inpt;
}

void append (std::ostringstream * ostr) {}

}

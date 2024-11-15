#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <math/linalg.hpp>

#include <sys/logging.hpp>

#include <ui/terminal.hpp>
#include <ui/input.hpp>
#include <ui/output.hpp>
#include <ui/console.hpp>

namespace console :: intern {

std::string mod = "console";

bool fail;
bool eof;

std::string conv_val_to_oupt (bool val);
std::string conv_val_to_oupt (std::string val);
std::string conv_val_to_oupt (int val);
std::string conv_val_to_oupt (double val);
std::string conv_val_to_oupt (linalg::ivector val);
std::string conv_val_to_oupt (linalg::fvector val);
std::string conv_val_to_oupt (linalg::imatrix val);
std::string conv_val_to_oupt (linalg::fmatrix val);
std::string conv_color_to_oupt (terminal::color color);

}

namespace console {

bool fail (void) {
    return intern::fail;
}

bool eof (void) {
    return intern::eof;
}

void put (terminal::color color, int col, bool val) {
    std::string oupt_color, oupt_val;

    oupt_color = intern::conv_color_to_oupt(color);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing output: Color: ", oupt_color, ", Col: ", col, ", ",
        "Type: Boolean, Value: ", oupt_val
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to print output (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int col, std::string val) {
    std::string oupt_color, oupt_val;

    oupt_color = intern::conv_color_to_oupt(color);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing output: Color: ", oupt_color, ", Col: ", col, ", ",
        "Type: String, Value: ", oupt_val
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to print output (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int col, int val) {
    std::string oupt_color, oupt_val;

    oupt_color = intern::conv_color_to_oupt(color);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing output: Color: ", oupt_color, ", Col: ", col, ", ",
        "Type: Integer scalar, Value: ", oupt_val
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to print output (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int col, double val) {
    std::string oupt_color, oupt_val;

    oupt_color = intern::conv_color_to_oupt(color);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing output: Color: ", oupt_color, ", Col: ", col, ", ",
        "Type: Real scalar, Value: ", oupt_val
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to print output (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int col, linalg::ivector val) {
    std::string oupt_color, oupt_val;

    oupt_color = intern::conv_color_to_oupt(color);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing output: Color: ", oupt_color, ", Col: ", col, ", ",
        "Type: Integer vector, Value: ", oupt_val
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to print output (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int col, linalg::fvector val) {
    std::string oupt_color, oupt_val;

    oupt_color = intern::conv_color_to_oupt(color);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing output: Color: ", oupt_color, ", Col: ", col, ", ",
        "Type: Real vector, Value: ", oupt_val
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to print output (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int col, linalg::imatrix val) {
    std::string oupt_color, oupt_val;

    oupt_color = intern::conv_color_to_oupt(color);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing output: Color: ", oupt_color, ", Col: ", col, ", ",
        "Type: Integer matrix, Value: ", oupt_val
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to print output (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int col, linalg::fmatrix val) {
    std::string oupt_color, oupt_val;

    oupt_color = intern::conv_color_to_oupt(color);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing output: Color: ", oupt_color, ", Col: ", col, ", ",
        "Type: Real matrix, Value: ", oupt_val
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to print output (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int col, const char * val) {
    std::string oupt_color, oupt_val;

    oupt_color = intern::conv_color_to_oupt(color);
    oupt_val = intern::conv_val_to_oupt(std::string(val));
    logging::inf(intern::mod,
        "Printing output: Color: ", oupt_color, ", Col: ", col, ", ",
        "Type: String, Value: ", oupt_val
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to print output (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    output::put(val);

    intern::fail = false;
}

template <>
bool get<bool> (terminal::color color, int col) {
    bool val;
    std::string oupt_color, oupt_val;

    oupt_color = intern::conv_color_to_oupt(color);
    logging::inf(intern::mod,
        "Scanning input: Color: ", oupt_color, ", Col: ", col, ", ",
        "Type: Boolean"
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to scan input (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    val = input::get<bool>();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Scanned input: Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::string get<std::string> (terminal::color color, int col) {
    std::string val;
    std::string oupt_color, oupt_val;

    oupt_color = intern::conv_color_to_oupt(color);
    logging::inf(intern::mod,
        "Scanning input: Color: ", oupt_color, ", Col: ", col, ", ",
        "Type: String"
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to scan input (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    val = input::get<std::string>();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Scanned input: Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
int get<int> (terminal::color color, int col) {
    int val;
    std::string oupt_color, oupt_val;

    oupt_color = intern::conv_color_to_oupt(color);
    logging::inf(intern::mod,
        "Scanning input: Color: ", oupt_color, ", Col: ", col, ", ",
        "Type: Integer scalar"
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to scan input (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    val = input::get<int>();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Scanned input: Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
double get<double> (terminal::color color, int col) {
    double val;
    std::string oupt_color, oupt_val;

    oupt_color = intern::conv_color_to_oupt(color);
    logging::inf(intern::mod,
        "Scanning input: Color: ", oupt_color, ", Col: ", col, ", ",
        "Type: Real scalar"
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to scan input (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    val = input::get<double>();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Scanned input: Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::ivector get<linalg::ivector> (terminal::color color, int col) {
    linalg::ivector val;
    std::string oupt_color, oupt_val;

    oupt_color = intern::conv_color_to_oupt(color);
    logging::inf(intern::mod,
        "Scanning input: Color: ", oupt_color, ", Col: ", col, ", ",
        "Type: Integer vector"
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to scan input (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    val = input::get<linalg::ivector>();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Scanned input: Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::fvector get<linalg::fvector> (terminal::color color, int col) {
    linalg::fvector val;
    std::string oupt_color, oupt_val;

    oupt_color = intern::conv_color_to_oupt(color);
    logging::inf(intern::mod,
        "Scanning input: Color: ", oupt_color, ", Col: ", col, ", ",
        "Type: Real vector"
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to scan input (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    val = input::get<linalg::fvector>();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Scanned input: Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::imatrix get<linalg::imatrix> (terminal::color color, int col) {
    linalg::imatrix val;
    std::string oupt_color, oupt_val;

    oupt_color = intern::conv_color_to_oupt(color);
    logging::inf(intern::mod,
        "Scanning input: Color: ", oupt_color, ", Col: ", col, ", ",
        "Type: Integer matrix"
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to scan input (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    val = input::get<linalg::imatrix>();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Scanned input: Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::fmatrix get<linalg::fmatrix> (terminal::color color, int col) {
    linalg::fmatrix val;
    std::string oupt_color, oupt_val;

    oupt_color = intern::conv_color_to_oupt(color);
    logging::inf(intern::mod,
        "Scanning input: Color: ", oupt_color, ", Col: ", col, ", ",
        "Type: Real matrix"
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to scan input (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    val = input::get<linalg::fmatrix>();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Scanned input: Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

void newline (void) {
    logging::inf(intern::mod,
        "Entering new line"
    );

    terminal::color_default();
    terminal::newline();
    terminal::erase_line();
}

}

namespace console :: intern {

std::string conv_val_to_oupt (bool val) {
    std::string oupt;
    if (val) {
        oupt = "True";
    } else {
        oupt = "False";
    }
    return oupt;
}

std::string conv_val_to_oupt (std::string val) {
    std::string oupt;
    oupt = std::string("“") + val + std::string("”");
    return oupt;
}

std::string conv_val_to_oupt (int val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << std::showpos << val;
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (double val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << std::showpos << std::scientific << std::setprecision(2) << val;
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (linalg::ivector val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        ostr << std::showpos << val[i];
        if (i < linalg::dim(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (linalg::fvector val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        ostr << std::showpos << std::scientific << std::setprecision(2)
             << val[i];
        if (i < linalg::dim(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (linalg::imatrix val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (int i = 0; i < linalg::rows(val); i++) {
        ostr << "[";
        for (int j = 0; j < linalg::cols(val); j++) {
            ostr << std::showpos << val[i][j];
            if (j < linalg::cols(val) - 1) {
                ostr << ", ";
            }
        }
        ostr << "]";
        if (i < linalg::rows(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (linalg::fmatrix val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (int i = 0; i < linalg::rows(val); i++) {
        ostr << "[";
        for (int j = 0; j < linalg::cols(val); j++) {
            ostr << std::showpos << std::scientific << std::setprecision(2)
                 << val[i][j];
            if (j < linalg::cols(val) - 1) {
                ostr << ", ";
            }
        }
        ostr << "]";
        if (i < linalg::rows(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    oupt = ostr.str();

    return oupt;
}

std::string conv_color_to_oupt (terminal::color color) {
    switch (color) {
        case terminal::black:
            return std::string("Black");
        case terminal::darkred:
            return std::string("Dark red");
        case terminal::darkgreen:
            return std::string("Dark green");
        case terminal::darkyellow:
            return std::string("Dark yellow");
        case terminal::darkblue:
            return std::string("Dark blue");
        case terminal::darkmagenta:
            return std::string("Dark magenta");
        case terminal::darkcyan:
            return std::string("Dark cyan");
        case terminal::lightgray:
            return std::string("Light gray");
        case terminal::darkgray:
            return std::string("Dark gray");
        case terminal::lightred:
            return std::string("Light red");
        case terminal::lightgreen:
            return std::string("Light green");
        case terminal::lightyellow:
            return std::string("Light yellow");
        case terminal::lightblue:
            return std::string("Light blue");
        case terminal::lightmagenta:
            return std::string("Light magenta");
        case terminal::lightcyan:
            return std::string("Light cyan");
        case terminal::white:
            return std::string("White");
        default:
            return std::string("");
    }
}

}

#include <cstdint>

#include <string>
#include <vector>

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

}

namespace console {

bool fail (void) {
    return intern::fail;
}

bool eof (void) {
    return intern::eof;
}

void put (terminal::color color, int col, const char * val) {
    logging::inf(intern::mod,
        "Printing output: Color: ", color, ", Col: ", col, ", Type: Characters, Value: ", std::string(val)
    );

    if (col <= 0) {
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
    logging::inf(intern::mod,
        "Printing output: Color: ", color, ", Col: ", col, ", Type: String, Value: ", val
    );

    if (col <= 0) {
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

void put (terminal::color color, int col, std::uint8_t val) {
    logging::inf(intern::mod,
        "Printing output: Color: ", color, ", Col: ", col, ", Type: Byte, Value: ", val
    );

    if (col <= 0) {
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

void put (terminal::color color, int col, bool val) {
    logging::inf(intern::mod,
        "Printing output: Color: ", color, ", Col: ", col, ", Type: Boolean, Value: ", val
    );

    if (col <= 0) {
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
    logging::inf(intern::mod,
        "Printing output: Color: ", color, ", Col: ", col, ", Type: Integer scalar, Value: ", val
    );

    if (col <= 0) {
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
    logging::inf(intern::mod,
        "Printing output: Color: ", color, ", Col: ", col, ", Type: Real scalar, Value: ", val
    );

    if (col <= 0) {
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
    logging::inf(intern::mod,
        "Printing output: Color: ", color, ", Col: ", col, ", Type: Integer vector, Value: ", val
    );

    if (col <= 0) {
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
    logging::inf(intern::mod,
        "Printing output: Color: ", color, ", Col: ", col, ", Type: Real vector, Value: ", val
    );

    if (col <= 0) {
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
    logging::inf(intern::mod,
        "Printing output: Color: ", color, ", Col: ", col, ", Type: Integer matrix, Value: ", val
    );

    if (col <= 0) {
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
    logging::inf(intern::mod,
        "Printing output: Color: ", color, ", Col: ", col, ", Type: Real matrix, Value: ", val
    );

    if (col <= 0) {
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

void put (terminal::color color, int col, std::vector<std::string> val) {
    logging::inf(intern::mod,
        "Printing output: Color: ", color, ", Col: ", col, ", Type: String array, Value: ", val
    );

    if (col <= 0) {
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

void put (terminal::color color, int col, std::vector<std::uint8_t> val) {
    logging::inf(intern::mod,
        "Printing output: Color: ", color, ", Col: ", col, ", Type: Byte array, Value: ", val
    );

    if (col <= 0) {
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

void put (terminal::color color, int col, std::vector<bool> val) {
    logging::inf(intern::mod,
        "Printing output: Color: ", color, ", Col: ", col, ", Type: Boolean array, Value: ", val
    );

    if (col <= 0) {
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

void put (terminal::color color, int col, std::vector<int> val) {
    logging::inf(intern::mod,
        "Printing output: Color: ", color, ", Col: ", col, ", Type: Integer scalar array, Value: ", val
    );

    if (col <= 0) {
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

void put (terminal::color color, int col, std::vector<double> val) {
    logging::inf(intern::mod,
        "Printing output: Color: ", color, ", Col: ", col, ", Type: Real scalar array, Value: ", val
    );

    if (col <= 0) {
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

void put (terminal::color color, int col, std::vector<linalg::ivector> val) {
    logging::inf(intern::mod,
        "Printing output: Color: ", color, ", Col: ", col, ", Type: Integer vector array, Value: ", val
    );

    if (col <= 0) {
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

void put (terminal::color color, int col, std::vector<linalg::fvector> val) {
    logging::inf(intern::mod,
        "Printing output: Color: ", color, ", Col: ", col, ", Type: Real vector array, Value: ", val
    );

    if (col <= 0) {
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

void put (terminal::color color, int col, std::vector<linalg::imatrix> val) {
    logging::inf(intern::mod,
        "Printing output: Color: ", color, ", Col: ", col, ", Type: Integer matrix array, Value: ", val
    );

    if (col <= 0) {
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

void put (terminal::color color, int col, std::vector<linalg::fmatrix> val) {
    logging::inf(intern::mod,
        "Printing output: Color: ", color, ", Col: ", col, ", Type: Real matrix array, Value: ", val
    );

    if (col <= 0) {
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
std::string get<std::string> (terminal::color color, int col) {
    std::string val;

    logging::inf(intern::mod,
        "Scanning input: Color: ", color, ", Col: ", col, ", Type: String"
    );

    if (col <= 0) {
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

    logging::inf(intern::mod,
        "Scanned input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::uint8_t get<std::uint8_t> (terminal::color color, int col) {
    std::uint8_t val;

    logging::inf(intern::mod,
        "Scanning input: Color: ", color, ", Col: ", col, ", Type: Byte"
    );

    if (col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    val = input::get<std::uint8_t>();

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

    logging::inf(intern::mod,
        "Scanned input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
bool get<bool> (terminal::color color, int col) {
    bool val;

    logging::inf(intern::mod,
        "Scanning input: Color: ", color, ", Col: ", col, ", Type: Boolean"
    );

    if (col <= 0) {
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

    logging::inf(intern::mod,
        "Scanned input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
int get<int> (terminal::color color, int col) {
    int val;

    logging::inf(intern::mod,
        "Scanning input: Color: ", color, ", Col: ", col, ", Type: Integer scalar"
    );

    if (col <= 0) {
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

    logging::inf(intern::mod,
        "Scanned input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
double get<double> (terminal::color color, int col) {
    double val;

    logging::inf(intern::mod,
        "Scanning input: Color: ", color, ", Col: ", col, ", Type: Real scalar"
    );

    if (col <= 0) {
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

    logging::inf(intern::mod,
        "Scanned input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::ivector get<linalg::ivector> (terminal::color color, int col) {
    linalg::ivector val;

    logging::inf(intern::mod,
        "Scanning input: Color: ", color, ", Col: ", col, ", Type: Integer vector"
    );

    if (col <= 0) {
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

    logging::inf(intern::mod,
        "Scanned input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::fvector get<linalg::fvector> (terminal::color color, int col) {
    linalg::fvector val;

    logging::inf(intern::mod,
        "Scanning input: Color: ", color, ", Col: ", col, ", Type: Real vector"
    );

    if (col <= 0) {
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

    logging::inf(intern::mod,
        "Scanned input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::imatrix get<linalg::imatrix> (terminal::color color, int col) {
    linalg::imatrix val;

    logging::inf(intern::mod,
        "Scanning input: Color: ", color, ", Col: ", col, ", Type: Integer matrix"
    );

    if (col <= 0) {
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

    logging::inf(intern::mod,
        "Scanned input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::fmatrix get<linalg::fmatrix> (terminal::color color, int col) {
    linalg::fmatrix val;

    logging::inf(intern::mod,
        "Scanning input: Color: ", color, ", Col: ", col, ", Type: Real matrix"
    );

    if (col <= 0) {
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

    logging::inf(intern::mod,
        "Scanned input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<std::string> get<std::vector<std::string>> (terminal::color color, int col) {
    std::vector<std::string> val;

    logging::inf(intern::mod,
        "Scanning input: Color: ", color, ", Col: ", col, ", Type: String array"
    );

    if (col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    val = input::get<std::vector<std::string>>();

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

    logging::inf(intern::mod,
        "Scanned input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<std::uint8_t> get<std::vector<std::uint8_t>> (terminal::color color, int col) {
    std::vector<std::uint8_t> val;

    logging::inf(intern::mod,
        "Scanning input: Color: ", color, ", Col: ", col, ", Type: Byte array"
    );

    if (col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    val = input::get<std::vector<std::uint8_t>>();

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

    logging::inf(intern::mod,
        "Scanned input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<bool> get<std::vector<bool>> (terminal::color color, int col) {
    std::vector<bool> val;

    logging::inf(intern::mod,
        "Scanning input: Color: ", color, ", Col: ", col, ", Type: Boolean array"
    );

    if (col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    val = input::get<std::vector<bool>>();

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

    logging::inf(intern::mod,
        "Scanned input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<int> get<std::vector<int>> (terminal::color color, int col) {
    std::vector<int> val;

    logging::inf(intern::mod,
        "Scanning input: Color: ", color, ", Col: ", col, ", Type: Integer scalar array"
    );

    if (col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    val = input::get<std::vector<int>>();

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

    logging::inf(intern::mod,
        "Scanned input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<double> get<std::vector<double>> (terminal::color color, int col) {
    std::vector<double> val;

    logging::inf(intern::mod,
        "Scanning input: Color: ", color, ", Col: ", col, ", Type: Real scalar array"
    );

    if (col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    val = input::get<std::vector<double>>();

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

    logging::inf(intern::mod,
        "Scanned input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<linalg::ivector> get<std::vector<linalg::ivector>> (terminal::color color, int col) {
    std::vector<linalg::ivector> val;

    logging::inf(intern::mod,
        "Scanning input: Color: ", color, ", Col: ", col, ", Type: Integer vector array"
    );

    if (col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    val = input::get<std::vector<linalg::ivector>>();

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

    logging::inf(intern::mod,
        "Scanned input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<linalg::fvector> get<std::vector<linalg::fvector>> (terminal::color color, int col) {
    std::vector<linalg::fvector> val;

    logging::inf(intern::mod,
        "Scanning input: Color: ", color, ", Col: ", col, ", Type: Real vector array"
    );

    if (col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    val = input::get<std::vector<linalg::fvector>>();

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

    logging::inf(intern::mod,
        "Scanned input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<linalg::imatrix> get<std::vector<linalg::imatrix>> (terminal::color color, int col) {
    std::vector<linalg::imatrix> val;

    logging::inf(intern::mod,
        "Scanning input: Color: ", color, ", Col: ", col, ", Type: Integer matrix array"
    );

    if (col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    val = input::get<std::vector<linalg::imatrix>>();

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

    logging::inf(intern::mod,
        "Scanned input: Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<linalg::fmatrix> get<std::vector<linalg::fmatrix>> (terminal::color color, int col) {
    std::vector<linalg::fmatrix> val;

    logging::inf(intern::mod,
        "Scanning input: Color: ", color, ", Col: ", col, ", Type: Real matrix array"
    );

    if (col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::cursor_col(col);
    val = input::get<std::vector<linalg::fmatrix>>();

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

    logging::inf(intern::mod,
        "Scanned input: Value: ", val
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

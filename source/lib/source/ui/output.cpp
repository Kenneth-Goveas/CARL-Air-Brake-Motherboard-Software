#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <math/linalg.hpp>

#include <sys/logging.hpp>

#include <ui/output.hpp>

namespace output :: intern {

std::string mod = "output";

std::string conv_val_to_oupt (bool val);
std::string conv_val_to_oupt (std::string val);
std::string conv_val_to_oupt (int val);
std::string conv_val_to_oupt (double val);
std::string conv_val_to_oupt (linalg::ivector val);
std::string conv_val_to_oupt (linalg::fvector val);
std::string conv_val_to_oupt (linalg::imatrix val);
std::string conv_val_to_oupt (linalg::fmatrix val);

std::string conv_val_to_disp (bool val);
std::string conv_val_to_disp (std::string val);
std::string conv_val_to_disp (int val);
std::string conv_val_to_disp (double val);
std::string conv_val_to_disp (linalg::ivector val);
std::string conv_val_to_disp (linalg::fvector val);
std::string conv_val_to_disp (linalg::imatrix val);
std::string conv_val_to_disp (linalg::fmatrix val);

}

namespace output {

void put (bool val) {
    std::string oupt, disp;

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing output: Type: Boolean, Value: ", oupt
    );

    disp = intern::conv_val_to_disp(val);
    std::cout << disp;
    std::cout.flush();
}

void put (std::string val) {
    std::string oupt, disp;

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing output: Type: String, Value: ", oupt
    );

    disp = intern::conv_val_to_disp(val);
    std::cout << disp;
    std::cout.flush();
}

void put (int val) {
    std::string oupt, disp;

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing output: Type: Integer scalar, Value: ", oupt
    );

    disp = intern::conv_val_to_disp(val);
    std::cout << disp;
    std::cout.flush();
}

void put (double val) {
    std::string oupt, disp;

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing output: Type: Real scalar, Value: ", oupt
    );

    disp = intern::conv_val_to_disp(val);
    std::cout << disp;
    std::cout.flush();
}

void put (linalg::ivector val) {
    std::string oupt, disp;

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing output: Type: Integer vector, Value: ", oupt
    );

    disp = intern::conv_val_to_disp(val);
    std::cout << disp;
    std::cout.flush();
}

void put (linalg::fvector val) {
    std::string oupt, disp;

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing output: Type: Real vector, Value: ", oupt
    );

    disp = intern::conv_val_to_disp(val);
    std::cout << disp;
    std::cout.flush();
}

void put (linalg::imatrix val) {
    std::string oupt, disp;

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing output: Type: Integer matrix, Value: ", oupt
    );

    disp = intern::conv_val_to_disp(val);
    std::cout << disp;
    std::cout.flush();
}

void put (linalg::fmatrix val) {
    std::string oupt, disp;

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing output: Type: Real matrix, Value: ", oupt
    );

    disp = intern::conv_val_to_disp(val);
    std::cout << disp;
    std::cout.flush();
}

void put (const char * val) {
    std::string oupt, disp;

    oupt = intern::conv_val_to_oupt(std::string(val));
    logging::inf(intern::mod,
        "Printing output: Type: String, Value: ", oupt
    );

    disp = intern::conv_val_to_disp(std::string(val));
    std::cout << disp;
    std::cout.flush();
}

}

namespace output :: intern {

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

std::string conv_val_to_disp (bool val) {
    std::string disp;
    if (val) {
        disp = "Yes";
    } else {
        disp = "No";
    }
    return disp;
}

std::string conv_val_to_disp (std::string val) {
    std::string disp;
    disp = val;
    return disp;
}

std::string conv_val_to_disp (int val) {
    std::ostringstream ostr;
    std::string disp;

    ostr << std::showpos << val;
    disp = ostr.str();

    return disp;
}

std::string conv_val_to_disp (double val) {
    std::ostringstream ostr;
    std::string disp;

    ostr << std::showpos << std::scientific << std::setprecision(2) << val;
    disp = ostr.str();

    return disp;
}

std::string conv_val_to_disp (linalg::ivector val) {
    std::ostringstream ostr;
    std::string disp;

    ostr << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        ostr << std::showpos << val[i];
        if (i < linalg::dim(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    disp = ostr.str();

    return disp;
}

std::string conv_val_to_disp (linalg::fvector val) {
    std::ostringstream ostr;
    std::string disp;

    ostr << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        ostr << std::showpos << std::scientific << std::setprecision(2)
             << val[i];
        if (i < linalg::dim(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    disp = ostr.str();

    return disp;
}

std::string conv_val_to_disp (linalg::imatrix val) {
    std::ostringstream ostr;
    std::string disp;

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
    disp = ostr.str();

    return disp;
}

std::string conv_val_to_disp (linalg::fmatrix val) {
    std::ostringstream ostr;
    std::string disp;

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
    disp = ostr.str();

    return disp;
}

}

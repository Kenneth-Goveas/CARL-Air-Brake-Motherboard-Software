#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <math/linalg.hpp>

#include <sys/logging.hpp>

#include <ui/output.hpp>

namespace output :: intern {

std::string mod = "output";

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

template <>
std::string conv_val_to_oupt<linalg::ivector> (linalg::ivector val);

template <>
std::string conv_val_to_oupt<linalg::fvector> (linalg::fvector val);

template <>
std::string conv_val_to_oupt<linalg::imatrix> (linalg::imatrix val);

template <>
std::string conv_val_to_oupt<linalg::fmatrix> (linalg::fmatrix val);

template <typename type>
std::string conv_val_to_disp (type val);

template <>
std::string conv_val_to_disp<bool> (bool val);

template <>
std::string conv_val_to_disp<std::string> (std::string val);

template <>
std::string conv_val_to_disp<int> (int val);

template <>
std::string conv_val_to_disp<double> (double val);

template <>
std::string conv_val_to_disp<linalg::ivector> (linalg::ivector val);

template <>
std::string conv_val_to_disp<linalg::fvector> (linalg::fvector val);

template <>
std::string conv_val_to_disp<linalg::imatrix> (linalg::imatrix val);

template <>
std::string conv_val_to_disp<linalg::fmatrix> (linalg::fmatrix val);

}

namespace output {

void print (bool val) {
    std::string oupt, disp;

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing boolean: Value: ", oupt
    );

    disp = intern::conv_val_to_disp(val);
    std::cout << disp;
    std::cout.flush();
}

void print (std::string val) {
    std::string oupt, disp;

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing string: Value: ", oupt
    );

    disp = intern::conv_val_to_disp(val);
    std::cout << disp;
    std::cout.flush();
}

void print (int val) {
    std::string oupt, disp;

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing integer scalar: Value: ", oupt
    );

    disp = intern::conv_val_to_disp(val);
    std::cout << disp;
    std::cout.flush();
}

void print (double val) {
    std::string oupt, disp;

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing floating point scalar: Value: ", oupt
    );

    disp = intern::conv_val_to_disp(val);
    std::cout << disp;
    std::cout.flush();
}

void print (linalg::ivector val) {
    std::string oupt, disp;

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing integer vector: Value: ", oupt
    );

    disp = intern::conv_val_to_disp(val);
    std::cout << disp;
    std::cout.flush();
}

void print (linalg::fvector val) {
    std::string oupt, disp;

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing floating point vector: Value: ", oupt
    );

    disp = intern::conv_val_to_disp(val);
    std::cout << disp;
    std::cout.flush();
}

void print (linalg::imatrix val) {
    std::string oupt, disp;

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing integer matrix: Value: ", oupt
    );

    disp = intern::conv_val_to_disp(val);
    std::cout << disp;
    std::cout.flush();
}

void print (linalg::fmatrix val) {
    std::string oupt, disp;

    oupt = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing floating point matrix: Value: ", oupt
    );

    disp = intern::conv_val_to_disp(val);
    std::cout << disp;
    std::cout.flush();
}

}

namespace output :: intern {

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

template <>
std::string conv_val_to_oupt<linalg::ivector> (linalg::ivector val) {
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

template <>
std::string conv_val_to_oupt<linalg::fvector> (linalg::fvector val) {
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

template <>
std::string conv_val_to_oupt<linalg::imatrix> (linalg::imatrix val) {
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

template <>
std::string conv_val_to_oupt<linalg::fmatrix> (linalg::fmatrix val) {
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

template <>
std::string conv_val_to_disp<bool> (bool val) {
    std::string disp;
    if (val) {
        disp = "Yes";
    } else {
        disp = "No";
    }
    return disp;
}

template <>
std::string conv_val_to_disp<std::string> (std::string val) {
    std::string disp;
    disp = val;
    return disp;
}

template <>
std::string conv_val_to_disp<int> (int val) {
    std::ostringstream ostr;
    std::string disp;

    ostr << std::showpos << val;
    disp = ostr.str();

    return disp;
}

template <>
std::string conv_val_to_disp<double> (double val) {
    std::ostringstream ostr;
    std::string disp;

    ostr << std::showpos << std::scientific << std::setprecision(2) << val;
    disp = ostr.str();

    return disp;
}

template <>
std::string conv_val_to_disp<linalg::ivector> (linalg::ivector val) {
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

template <>
std::string conv_val_to_disp<linalg::fvector> (linalg::fvector val) {
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

template <>
std::string conv_val_to_disp<linalg::imatrix> (linalg::imatrix val) {
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

template <>
std::string conv_val_to_disp<linalg::fmatrix> (linalg::fmatrix val) {
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

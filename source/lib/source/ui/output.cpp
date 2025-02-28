#include <cstdint>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <math/linalg.hpp>

#include <sys/logging.hpp>

#include <ui/output.hpp>

namespace output :: intern {

std::string mod = "output";

}

namespace output {

void put (const char * val) {
    std::ostringstream ostr;

    logging::inf(intern::mod,
        "Printing output: Type: Characters, Value: ", std::string(val)
    );

    ostr << val;

    std::cout << ostr.str();
    std::cout.flush();
}

void put (std::string val) {
    std::ostringstream ostr;

    logging::inf(intern::mod,
        "Printing output: Type: String, Value: ", val
    );

    ostr << "“" << val << "”";

    std::cout << ostr.str();
    std::cout.flush();
}

void put (std::uint8_t val) {
    std::ostringstream ostr;

    logging::inf(intern::mod,
        "Printing output: Type: Byte, Value: ", val
    );

    ostr << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << int(val);

    std::cout << ostr.str();
    std::cout.flush();
}

void put (bool val) {
    std::ostringstream ostr;

    logging::inf(intern::mod,
        "Printing output: Type: Boolean, Value: ", val
    );

    if (val) {
        ostr << "Yes";
    } else {
        ostr << "No";
    }

    std::cout << ostr.str();
    std::cout.flush();
}

void put (int val) {
    std::ostringstream ostr;

    logging::inf(intern::mod,
        "Printing output: Type: Integer scalar, Value: ", val
    );

    ostr << std::showpos << val;

    std::cout << ostr.str();
    std::cout.flush();
}

void put (double val) {
    std::ostringstream ostr;

    logging::inf(intern::mod,
        "Printing output: Type: Real scalar, Value: ", val
    );

    ostr << std::showpos << std::scientific << std::setprecision(4) << val;

    std::cout << ostr.str();
    std::cout.flush();
}

void put (linalg::ivector val) {
    std::ostringstream ostr;

    logging::inf(intern::mod,
        "Printing output: Type: Integer vector, Value: ", val
    );

    ostr << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        ostr << std::showpos << val[i];
        if (i < linalg::dim(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";

    std::cout << ostr.str();
    std::cout.flush();
}

void put (linalg::fvector val) {
    std::ostringstream ostr;

    logging::inf(intern::mod,
        "Printing output: Type: Real vector, Value: ", val
    );

    ostr << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        ostr << std::showpos << std::scientific << std::setprecision(4) << val[i];
        if (i < linalg::dim(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";

    std::cout << ostr.str();
    std::cout.flush();
}

void put (linalg::imatrix val) {
    std::ostringstream ostr;

    logging::inf(intern::mod,
        "Printing output: Type: Integer matrix, Value: ", val
    );

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

    std::cout << ostr.str();
    std::cout.flush();
}

void put (linalg::fmatrix val) {
    std::ostringstream ostr;

    logging::inf(intern::mod,
        "Printing output: Type: Real matrix, Value: ", val
    );

    ostr << "[";
    for (int i = 0; i < linalg::rows(val); i++) {
        ostr << "[";
        for (int j = 0; j < linalg::cols(val); j++) {
            ostr << std::showpos << std::scientific << std::setprecision(4) << val[i][j];
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

    std::cout << ostr.str();
    std::cout.flush();
}

void put (std::vector<std::string> val) {
    std::ostringstream ostr;

    logging::inf(intern::mod,
        "Printing output: Type: String array, Value: ", val
    );

    ostr << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        ostr << "“" << *itr << "”";
        if (itr < val.end() - 1) {
            ostr << ", ";
        }
    }
    ostr << "}";

    std::cout << ostr.str();
    std::cout.flush();
}

void put (std::vector<std::uint8_t> val) {
    std::ostringstream ostr;

    logging::inf(intern::mod,
        "Printing output: Type: Byte array, Value: ", val
    );

    ostr << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        ostr << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << int(*itr);
        if (itr < val.end() - 1) {
            ostr << ", ";
        }
    }
    ostr << "}";

    std::cout << ostr.str();
    std::cout.flush();
}

void put (std::vector<bool> val) {
    std::ostringstream ostr;

    logging::inf(intern::mod,
        "Printing output: Type: Boolean array, Value: ", val
    );

    ostr << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        if (*itr) {
            ostr << "Yes";
        } else {
            ostr << "No";
        }
        if (itr < val.end() - 1) {
            ostr << ", ";
        }
    }
    ostr << "}";

    std::cout << ostr.str();
    std::cout.flush();
}

void put (std::vector<int> val) {
    std::ostringstream ostr;

    logging::inf(intern::mod,
        "Printing output: Type: Integer scalar array, Value: ", val
    );

    ostr << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        ostr << std::showpos << *itr;
        if (itr < val.end() - 1) {
            ostr << ", ";
        }
    }
    ostr << "}";

    std::cout << ostr.str();
    std::cout.flush();
}

void put (std::vector<double> val) {
    std::ostringstream ostr;

    logging::inf(intern::mod,
        "Printing output: Type: Real scalar array, Value: ", val
    );

    ostr << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        ostr << std::showpos << std::scientific << std::setprecision(4) << *itr;
        if (itr < val.end() - 1) {
            ostr << ", ";
        }
    }
    ostr << "}";

    std::cout << ostr.str();
    std::cout.flush();
}

void put (std::vector<linalg::ivector> val) {
    std::ostringstream ostr;

    logging::inf(intern::mod,
        "Printing output: Type: Integer vector array, Value: ", val
    );

    ostr << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        ostr << "[";
        for (int i = 0; i < linalg::dim(*itr); i++) {
            ostr << std::showpos << (*itr)[i];
            if (i < linalg::dim(*itr) - 1) {
                ostr << ", ";
            }
        }
        ostr << "]";
        if (itr < val.end() - 1) {
            ostr << ", ";
        }
    }
    ostr << "}";

    std::cout << ostr.str();
    std::cout.flush();
}

void put (std::vector<linalg::fvector> val) {
    std::ostringstream ostr;

    logging::inf(intern::mod,
        "Printing output: Type: Real vector array, Value: ", val
    );

    ostr << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        ostr << "[";
        for (int i = 0; i < linalg::dim(*itr); i++) {
            ostr << std::showpos << std::scientific << std::setprecision(4) << (*itr)[i];
            if (i < linalg::dim(*itr) - 1) {
                ostr << ", ";
            }
        }
        ostr << "]";
        if (itr < val.end() - 1) {
            ostr << ", ";
        }
    }
    ostr << "}";

    std::cout << ostr.str();
    std::cout.flush();
}

void put (std::vector<linalg::imatrix> val) {
    std::ostringstream ostr;

    logging::inf(intern::mod,
        "Printing output: Type: Integer matrix array, Value: ", val
    );

    ostr << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        ostr << "[";
        for (int i = 0; i < linalg::rows(*itr); i++) {
            ostr << "[";
            for (int j = 0; j < linalg::cols(*itr); j++) {
                ostr << std::showpos << (*itr)[i][j];
                if (j < linalg::cols(*itr) - 1) {
                    ostr << ", ";
                }
            }
            ostr << "]";
            if (i < linalg::rows(*itr) - 1) {
                ostr << ", ";
            }
        }
        ostr << "]";
        if (itr < val.end() - 1) {
            ostr << ", ";
        }
    }
    ostr << "}";

    std::cout << ostr.str();
    std::cout.flush();
}

void put (std::vector<linalg::fmatrix> val) {
    std::ostringstream ostr;

    logging::inf(intern::mod,
        "Printing output: Type: Real matrix array, Value: ", val
    );

    ostr << "{";
    for (auto itr = val.begin(); itr < val.end(); itr++) {
        ostr << "[";
        for (int i = 0; i < linalg::rows(*itr); i++) {
            ostr << "[";
            for (int j = 0; j < linalg::cols(*itr); j++) {
                ostr << std::showpos << std::scientific << std::setprecision(4) << (*itr)[i][j];
                if (j < linalg::cols(*itr) - 1) {
                    ostr << ", ";
                }
            }
            ostr << "]";
            if (i < linalg::rows(*itr) - 1) {
                ostr << ", ";
            }
        }
        ostr << "]";
        if (itr < val.end() - 1) {
            ostr << ", ";
        }
    }
    ostr << "}";

    std::cout << ostr.str();
    std::cout.flush();
}

}

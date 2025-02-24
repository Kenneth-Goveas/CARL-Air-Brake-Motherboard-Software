#include <csignal>

#include <utility>
#include <algorithm>
#include <vector>
#include <tuple>

#include <math/lapack.hpp>

extern "C" {

void dgeqrf_ (int * rows, int * cols, double * data, int * lead, double * coef, double * work, int * len, int * ret);
void dorgqr_ (int * rows, int * cols, int * count, double * data, int * lead, double * coef, double * work, int * len, int * ret);
void dgerqf_ (int * rows, int * cols, double * data, int * lead, double * coef, double * work, int * len, int * ret);
void dorgrq_ (int * rows, int * cols, int * count, double * data, int * lead, double * coef, double * work, int * len, int * ret);
void dgeqlf_ (int * rows, int * cols, double * data, int * lead, double * coef, double * work, int * len, int * ret);
void dorgql_ (int * rows, int * cols, int * count, double * data, int * lead, double * coef, double * work, int * len, int * ret);
void dgelqf_ (int * rows, int * cols, double * data, int * lead, double * coef, double * work, int * len, int * ret);
void dorglq_ (int * rows, int * cols, int * count, double * data, int * lead, double * coef, double * work, int * len, int * ret);
void dgetrf_ (int * rows, int * cols, double * data, int * lead, int * pivot, int * ret);
void dgetri_ (int * dim, double * data, int * lead, int * pivot, double * work, int * len, int * ret);
void dsyev_ (char * task, char * type, int * dim, double * data, int * lead, double * eig, double * work, int * len, int * ret);

}

namespace lapack :: intern {

bool valid_dim (int dim);
bool valid_dim (int rows, int cols);

int get_dim (std::vector<int> vec);
int get_dim (std::vector<double> vec);
std::tuple<int, int> get_dim (std::vector<std::vector<int>> mat);
std::tuple<int, int> get_dim (std::vector<std::vector<double>> mat);

bool is_sqr (std::vector<std::vector<int>> mat);
bool is_sqr (std::vector<std::vector<double>> mat);

}

namespace lapack {

std::tuple<std::vector<std::vector<double>>, std::vector<double>>
dgeqrf (std::vector<std::vector<double>> data) {
    std::vector<std::vector<double>> fact;
    std::vector<double> coef;
    int ret, rows, cols, comm, len;
    double * data_buf, * coef_buf, * work_buf;

    std::tie(rows, cols) = intern::get_dim(data);
    comm = std::min(rows, cols);

    data_buf = new double [rows * cols];
    coef_buf = new double [comm];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            data_buf[i + j * rows] = data[i][j];
        }
    }

    len = -1;
    work_buf = new double;

    dgeqrf_(&rows, &cols, data_buf, &rows, coef_buf, work_buf, &len, &ret);

    len = int(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dgeqrf_(&rows, &cols, data_buf, &rows, coef_buf, work_buf, &len, &ret);

    fact = std::vector<std::vector<double>>(rows, std::vector<double>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fact[i][j] = data_buf[i + j * rows];
        }
    }

    coef = std::vector<double>(comm);
    for (int i = 0; i < comm; i++) {
        coef[i] = coef_buf[i];
    }

    delete [] data_buf;
    delete [] coef_buf;
    delete [] work_buf;

    return {fact, coef};
}

std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>>
dorgqr (std::vector<std::vector<double>> fact, std::vector<double> coef) {
    std::vector<std::vector<double>> orth, uppr;
    int ret, rows, cols, comm, len;
    double * fact_buf, * coef_buf, * work_buf;

    std::tie(rows, cols) = intern::get_dim(fact);
    comm = intern::get_dim(coef);

    if (comm != std::min(rows, cols)) {
        std::raise(SIGFPE);
    }

    fact_buf = new double [rows * comm];
    coef_buf = new double [comm];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < comm; j++) {
            fact_buf[i + j * rows] = fact[i][j];
        }
    }

    for (int i = 0; i < comm; i++) {
        coef_buf[i] = coef[i];
    }

    len = -1;
    work_buf = new double;

    dorgqr_(&rows, &comm, &comm, fact_buf, &rows, coef_buf, work_buf, &len, &ret);

    len = int(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dorgqr_(&rows, &comm, &comm, fact_buf, &rows, coef_buf, work_buf, &len, &ret);

    orth = std::vector<std::vector<double>>(rows, std::vector<double>(comm));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < comm; j++) {
            orth[i][j] = fact_buf[i + j * rows];
        }
    }

    uppr = std::vector<std::vector<double>>(comm, std::vector<double>(cols));
    for (int i = 0; i < comm; i++) {
        for (int j = 0; j < cols; j++) {
            if (i > j) {
                uppr[i][j] = 0;
            } else {
                uppr[i][j] = fact[i][j];
            }
        }
    }

    delete [] fact_buf;
    delete [] coef_buf;
    delete [] work_buf;

    return {orth, uppr};
}

std::tuple<std::vector<std::vector<double>>, std::vector<double>>
dgerqf (std::vector<std::vector<double>> data) {
    std::vector<std::vector<double>> fact;
    std::vector<double> coef;
    int ret, rows, cols, comm, len;
    double * data_buf, * coef_buf, * work_buf;

    std::tie(rows, cols) = intern::get_dim(data);
    comm = std::min(rows, cols);

    data_buf = new double [rows * cols];
    coef_buf = new double [comm];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            data_buf[i + j * rows] = data[i][j];
        }
    }

    len = -1;
    work_buf = new double;

    dgerqf_(&rows, &cols, data_buf, &rows, coef_buf, work_buf, &len, &ret);

    len = int(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dgerqf_(&rows, &cols, data_buf, &rows, coef_buf, work_buf, &len, &ret);

    fact = std::vector<std::vector<double>>(rows, std::vector<double>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fact[i][j] = data_buf[i + j * rows];
        }
    }

    coef = std::vector<double>(comm);
    for (int i = 0; i < comm; i++) {
        coef[i] = coef_buf[i];
    }

    delete [] data_buf;
    delete [] coef_buf;
    delete [] work_buf;

    return {fact, coef};
}

std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>>
dorgrq (std::vector<std::vector<double>> fact, std::vector<double> coef) {
    std::vector<std::vector<double>> uppr, orth;
    int ret, rows, cols, comm, len;
    double * fact_buf, * coef_buf, * work_buf;

    std::tie(rows, cols) = intern::get_dim(fact);
    comm = intern::get_dim(coef);

    if (comm != std::min(rows, cols)) {
        std::raise(SIGFPE);
    }

    fact_buf = new double [comm * cols];
    coef_buf = new double [comm];

    for (int i = 0; i < comm; i++) {
        for (int j = 0; j < cols; j++) {
            fact_buf[i + j * comm] = fact[rows - comm + i][j];
        }
    }

    for (int i = 0; i < comm; i++) {
        coef_buf[i] = coef[i];
    }

    len = -1;
    work_buf = new double;

    dorgrq_(&comm, &cols, &comm, fact_buf, &comm, coef_buf, work_buf, &len, &ret);

    len = int(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dorgrq_(&comm, &cols, &comm, fact_buf, &comm, coef_buf, work_buf, &len, &ret);

    orth = std::vector<std::vector<double>>(comm, std::vector<double>(cols));
    for (int i = 0; i < comm; i++) {
        for (int j = 0; j < cols; j++) {
            orth[i][j] = fact_buf[i + j * comm];
        }
    }

    uppr = std::vector<std::vector<double>>(rows, std::vector<double>(comm));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < comm; j++) {
            if (i - j > rows - comm) {
                uppr[i][j] = 0;
            } else {
                uppr[i][j] = fact[i][cols - comm + j];
            }
        }
    }

    delete [] fact_buf;
    delete [] coef_buf;
    delete [] work_buf;

    return {uppr, orth};
}

std::tuple<std::vector<std::vector<double>>, std::vector<double>>
dgeqlf (std::vector<std::vector<double>> data) {
    std::vector<std::vector<double>> fact;
    std::vector<double> coef;
    int ret, rows, cols, comm, len;
    double * data_buf, * coef_buf, * work_buf;

    std::tie(rows, cols) = intern::get_dim(data);
    comm = std::min(rows, cols);

    data_buf = new double [rows * cols];
    coef_buf = new double [comm];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            data_buf[i + j * rows] = data[i][j];
        }
    }

    len = -1;
    work_buf = new double;

    dgeqlf_(&rows, &cols, data_buf, &rows, coef_buf, work_buf, &len, &ret);

    len = int(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dgeqlf_(&rows, &cols, data_buf, &rows, coef_buf, work_buf, &len, &ret);

    fact = std::vector<std::vector<double>>(rows, std::vector<double>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fact[i][j] = data_buf[i + j * rows];
        }
    }

    coef = std::vector<double>(comm);
    for (int i = 0; i < comm; i++) {
        coef[i] = coef_buf[i];
    }

    delete [] data_buf;
    delete [] coef_buf;
    delete [] work_buf;

    return {fact, coef};
}

std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>>
dorgql (std::vector<std::vector<double>> fact, std::vector<double> coef) {
    std::vector<std::vector<double>> orth, lowr;
    int ret, rows, cols, comm, len;
    double * fact_buf, * coef_buf, * work_buf;

    std::tie(rows, cols) = intern::get_dim(fact);
    comm = intern::get_dim(coef);

    if (comm != std::min(rows, cols)) {
        std::raise(SIGFPE);
    }

    fact_buf = new double [rows * comm];
    coef_buf = new double [comm];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < comm; j++) {
            fact_buf[i + j * rows] = fact[i][cols - comm + j];
        }
    }

    for (int i = 0; i < comm; i++) {
        coef_buf[i] = coef[i];
    }

    len = -1;
    work_buf = new double;

    dorgql_(&rows, &comm, &comm, fact_buf, &rows, coef_buf, work_buf, &len, &ret);

    len = int(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dorgql_(&rows, &comm, &comm, fact_buf, &rows, coef_buf, work_buf, &len, &ret);

    orth = std::vector<std::vector<double>>(rows, std::vector<double>(comm));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < comm; j++) {
            orth[i][j] = fact_buf[i + j * rows];
        }
    }

    lowr = std::vector<std::vector<double>>(comm, std::vector<double>(cols));
    for (int i = 0; i < comm; i++) {
        for (int j = 0; j < cols; j++) {
            if (j - i > cols - comm) {
                lowr[i][j] = 0;
            } else {
                lowr[i][j] = fact[rows - comm + i][j];
            }
        }
    }

    delete [] fact_buf;
    delete [] coef_buf;
    delete [] work_buf;

    return {orth, lowr};
}

std::tuple<std::vector<std::vector<double>>, std::vector<double>>
dgelqf (std::vector<std::vector<double>> data) {
    std::vector<std::vector<double>> fact;
    std::vector<double> coef;
    int ret, rows, cols, comm, len;
    double * data_buf, * coef_buf, * work_buf;

    std::tie(rows, cols) = intern::get_dim(data);
    comm = std::min(rows, cols);

    data_buf = new double [rows * cols];
    coef_buf = new double [comm];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            data_buf[i + j * rows] = data[i][j];
        }
    }

    len = -1;
    work_buf = new double;

    dgelqf_(&rows, &cols, data_buf, &rows, coef_buf, work_buf, &len, &ret);

    len = int(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dgelqf_(&rows, &cols, data_buf, &rows, coef_buf, work_buf, &len, &ret);

    fact = std::vector<std::vector<double>>(rows, std::vector<double>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fact[i][j] = data_buf[i + j * rows];
        }
    }

    coef = std::vector<double>(comm);
    for (int i = 0; i < comm; i++) {
        coef[i] = coef_buf[i];
    }

    delete [] data_buf;
    delete [] coef_buf;
    delete [] work_buf;

    return {fact, coef};
}

std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>>
dorglq (std::vector<std::vector<double>> fact, std::vector<double> coef) {
    std::vector<std::vector<double>> lowr, orth;
    int ret, rows, cols, comm, len;
    double * fact_buf, * coef_buf, * work_buf;

    std::tie(rows, cols) = intern::get_dim(fact);
    comm = intern::get_dim(coef);

    if (comm != std::min(rows, cols)) {
        std::raise(SIGFPE);
    }

    fact_buf = new double [comm * cols];
    coef_buf = new double [comm];

    for (int i = 0; i < comm; i++) {
        for (int j = 0; j < cols; j++) {
            fact_buf[i + j * comm] = fact[i][j];
        }
    }

    for (int i = 0; i < comm; i++) {
        coef_buf[i] = coef[i];
    }

    len = -1;
    work_buf = new double;

    dorglq_(&comm, &cols, &comm, fact_buf, &comm, coef_buf, work_buf, &len, &ret);

    len = int(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dorglq_(&comm, &cols, &comm, fact_buf, &comm, coef_buf, work_buf, &len, &ret);

    orth = std::vector<std::vector<double>>(comm, std::vector<double>(cols));
    for (int i = 0; i < comm; i++) {
        for (int j = 0; j < cols; j++) {
            orth[i][j] = fact_buf[i + j * comm];
        }
    }

    lowr = std::vector<std::vector<double>>(rows, std::vector<double>(comm));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < comm; j++) {
            if (j > i) {
                lowr[i][j] = 0;
            } else {
                lowr[i][j] = fact[i][j];
            }
        }
    }

    delete [] fact_buf;
    delete [] coef_buf;
    delete [] work_buf;

    return {lowr, orth};
}

std::tuple<std::vector<std::vector<double>>, std::vector<int>>
dgetrf (std::vector<std::vector<double>> data) {
    std::vector<std::vector<double>> fact;
    std::vector<int> pivot;
    int ret, rows, cols, comm;
    double * data_buf;
    int * pivot_buf;

    std::tie(rows, cols) = intern::get_dim(data);
    comm = std::min(rows, cols);

    data_buf = new double [rows * cols];
    pivot_buf = new int [comm];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            data_buf[i + j * rows] = data[i][j];
        }
    }

    dgetrf_(&rows, &cols, data_buf, &rows, pivot_buf, &ret);

    fact = std::vector<std::vector<double>>(rows, std::vector<double>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fact[i][j] = data_buf[i + j * rows];
        }
    }

    pivot = std::vector<int>(comm);
    for (int i = 0; i < comm; i++) {
        pivot[i] = pivot_buf[i];
    }

    delete [] data_buf;
    delete [] pivot_buf;

    return {fact, pivot};
}

std::vector<std::vector<double>>
dgetri (std::vector<std::vector<double>> fact, std::vector<int> pivot) {
    std::vector<std::vector<double>> inv;
    int ret, dim, comm, len;
    double * fact_buf, * work_buf;
    int * pivot_buf;

    if (!intern::is_sqr(fact)) {
        std::raise(SIGFPE);
    }

    std::tie(dim, std::ignore) = intern::get_dim(fact);
    comm = intern::get_dim(pivot);

    if (comm != dim) {
        std::raise(SIGFPE);
    }

    fact_buf = new double [dim * dim];
    pivot_buf = new int [dim];

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            fact_buf[i + j * dim] = fact[i][j];
        }
    }

    for (int i = 0; i < dim; i++) {
        pivot_buf[i] = pivot[i];
    }

    len = -1;
    work_buf = new double;

    dgetri_(&dim, fact_buf, &dim, pivot_buf, work_buf, &len, &ret);
    if (ret != 0) {
        delete [] fact_buf;
        delete [] pivot_buf;
        delete work_buf;
        std::raise(SIGFPE);
    }

    len = int(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dgetri_(&dim, fact_buf, &dim, pivot_buf, work_buf, &len, &ret);
    if (ret != 0) {
        delete [] fact_buf;
        delete [] pivot_buf;
        delete [] work_buf;
        std::raise(SIGFPE);
    }

    inv = std::vector<std::vector<double>>(dim, std::vector<double>(dim));
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            inv[i][j] = fact_buf[i + j * dim];
        }
    }

    delete [] fact_buf;
    delete [] pivot_buf;
    delete [] work_buf;

    return inv;
}

std::tuple<std::vector<double>, std::vector<std::vector<double>>>
dsyev (std::vector<std::vector<double>> data) {
    std::vector<double> eigval;
    std::vector<std::vector<double>> eigvec;
    int ret, dim, len;
    double * data_buf, * eig_buf, * work_buf;
    char task = 'V', type = 'L';

    if (!intern::is_sqr(data)) {
        std::raise(SIGFPE);
    }

    std::tie(dim, std::ignore) = intern::get_dim(data);

    data_buf = new double [dim * dim];
    eig_buf = new double [dim];

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            data_buf[i + j * dim] = data[i][j];
        }
    }

    len = -1;
    work_buf = new double;

    dsyev_(&task, &type, &dim, data_buf, &dim, eig_buf, work_buf, &len, &ret);
    if (ret != 0) {
        delete [] data_buf;
        delete [] eig_buf;
        delete work_buf;
        std::raise(SIGFPE);
    }

    len = int(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dsyev_(&task, &type, &dim, data_buf, &dim, eig_buf, work_buf, &len, &ret);
    if (ret != 0) {
        delete [] data_buf;
        delete [] eig_buf;
        delete [] work_buf;
        std::raise(SIGFPE);
    }

    eigval = std::vector<double>(dim);
    for (int i = 0; i < dim; i++) {
        eigval[i] = eig_buf[i];
    }

    eigvec = std::vector<std::vector<double>>(dim, std::vector<double>(dim));
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            eigvec[i][j] = data_buf[i + j * dim];
        }
    }

    delete [] data_buf;
    delete [] eig_buf;
    delete [] work_buf;

    return {eigval, eigvec};
}

}

namespace lapack :: intern {

bool valid_dim (int dim) {
    return (dim > 0);
}

bool valid_dim (int rows, int cols) {
    return (rows > 0 && cols > 0);
}

int get_dim (std::vector<int> vec) {
    int dim = vec.size();

    if (!valid_dim(dim)) {
        std::raise(SIGFPE);
    }

    return dim;
}

int get_dim (std::vector<double> vec) {
    int dim = vec.size();

    if (!valid_dim(dim)) {
        std::raise(SIGFPE);
    }

    return dim;
}

std::tuple<int, int> get_dim (std::vector<std::vector<int>> mat) {
    int rows = 0, cols = 0;

    if (!mat.empty()) {
        rows = mat.size();
        cols = mat.front().size();
    }

    for (auto elem : mat) {
        if (elem.size() != cols) {
            std::raise(SIGFPE);
        }
    }

    if (!valid_dim(rows, cols)) {
        std::raise(SIGFPE);
    }

    return {rows, cols};
}

std::tuple<int, int> get_dim (std::vector<std::vector<double>> mat) {
    int rows = 0, cols = 0;

    if (!mat.empty()) {
        rows = mat.size();
        cols = mat.front().size();
    }

    for (auto elem : mat) {
        if (elem.size() != cols) {
            std::raise(SIGFPE);
        }
    }

    if (!valid_dim(rows, cols)) {
        std::raise(SIGFPE);
    }

    return {rows, cols};
}

bool is_sqr (std::vector<std::vector<int>> mat) {
    int rows, cols;
    std::tie(rows, cols) = get_dim(mat);
    return (rows == cols);
}

bool is_sqr (std::vector<std::vector<double>> mat) {
    int rows, cols;
    std::tie(rows, cols) = get_dim(mat);
    return (rows == cols);
}

}

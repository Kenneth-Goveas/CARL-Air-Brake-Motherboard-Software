#include <csignal>

#include <algorithm>
#include <vector>

#include <math/lapack.hpp>

extern "C" {

void dgetrf_ (int * rows, int * cols, double * data, int * lead, int * pivot, int * ret);
void dgetri_ (int * dim, double * data, int * lead, int * pivot, double * work, int * len, int * ret);
void dsyev_ (char * task, char * type, int * dim, double * data, int * lead, double * eig, double * work, int * len, int * ret);

}

namespace lapack::intern {

bool valid_dim (int dim);
bool valid_dim (int rows, int cols);

void get_dim (const std::vector<int> & vec, int & dim);
void get_dim (const std::vector<double> & vec, int & dim);

void get_dim (const std::vector<std::vector<int>> & mat, int & rows, int & cols);
void get_dim (const std::vector<std::vector<double>> & mat, int & rows, int & cols);

void get_dim (const std::vector<std::vector<int>> & mat, int & dim);
void get_dim (const std::vector<std::vector<double>> & mat, int & dim);

}

namespace lapack {

void dgetrf (const std::vector<std::vector<double>> & data, std::vector<std::vector<double>> & fact, std::vector<int> & pivot) {
    int rows, cols, comm, ret;
    double * data_buf;
    int * pivot_buf;

    intern::get_dim(data, rows, cols);
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
}

void dgetri (const std::vector<std::vector<double>> & fact, const std::vector<int> & pivot, std::vector<std::vector<double>> & inv) {
    int dim, comm, len, ret;
    double * data_buf, * work_buf;
    int * pivot_buf;

    intern::get_dim(fact, dim);
    intern::get_dim(pivot, comm);

    if (dim != comm) {
        std::raise(SIGFPE);
    }

    data_buf = new double [dim * dim];
    pivot_buf = new int [dim];

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            data_buf[i + j * dim] = fact[i][j];
        }
    }

    for (int i = 0; i < dim; i++) {
        pivot_buf[i] = pivot[i];
    }

    len = -1;
    work_buf = new double;

    dgetri_(&dim, data_buf, &dim, pivot_buf, work_buf, &len, &ret);
    if (ret != 0) {
        delete [] data_buf;
        delete [] pivot_buf;
        delete work_buf;
        std::raise(SIGFPE);
    }

    len = int(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dgetri_(&dim, data_buf, &dim, pivot_buf, work_buf, &len, &ret);
    if (ret != 0) {
        delete [] data_buf;
        delete [] pivot_buf;
        delete [] work_buf;
        std::raise(SIGFPE);
    }

    inv = std::vector<std::vector<double>>(dim, std::vector<double>(dim));
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            inv[i][j] = data_buf[i + j * dim];
        }
    }

    delete [] data_buf;
    delete [] pivot_buf;
    delete [] work_buf;
}

void dsyev (const std::vector<std::vector<double>> & data, std::vector<double> & eigval) {
    int dim, len, ret;
    double * data_buf, * eig_buf, * work_buf;
    char task = 'N', type = 'L';

    intern::get_dim(data, dim);

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

    delete [] data_buf;
    delete [] eig_buf;
    delete [] work_buf;
}

void dsyev (const std::vector<std::vector<double>> & data, std::vector<double> & eigval, std::vector<std::vector<double>> & eigvec) {
    int dim, len, ret;
    double * data_buf, * eig_buf, * work_buf;
    char task = 'V', type = 'L';

    intern::get_dim(data, dim);

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
}

}

namespace lapack::intern {

bool valid_dim (int dim) {
    return (dim > 0);
}

bool valid_dim (int rows, int cols) {
    return (rows > 0 && cols > 0);
}

void get_dim (const std::vector<int> & vec, int & dim) {
    dim = vec.size();
    if (!valid_dim(dim)) {
        std::raise(SIGFPE);
    }
}

void get_dim (const std::vector<double> & vec, int & dim) {
    dim = vec.size();
    if (!valid_dim(dim)) {
        std::raise(SIGFPE);
    }
}

void get_dim (const std::vector<std::vector<int>> & mat, int & rows, int & cols) {
    rows = 0; cols = 0;

    if (!mat.empty()) {
        rows = mat.size();
        cols = mat.front().size();
    }

    for (auto elem : mat) {
        if (int(elem.size()) != cols) {
            std::raise(SIGFPE);
        }
    }

    if (!valid_dim(rows, cols)) {
        std::raise(SIGFPE);
    }
}

void get_dim (const std::vector<std::vector<double>> & mat, int & rows, int & cols) {
    rows = 0; cols = 0;

    if (!mat.empty()) {
        rows = mat.size();
        cols = mat.front().size();
    }

    for (auto elem : mat) {
        if (int(elem.size()) != cols) {
            std::raise(SIGFPE);
        }
    }

    if (!valid_dim(rows, cols)) {
        std::raise(SIGFPE);
    }
}

void get_dim (const std::vector<std::vector<int>> & mat, int & dim) {
    int rows, cols;

    get_dim(mat, rows, cols);
    dim = rows;

    if (rows != cols) {
        std::raise(SIGFPE);
    }
}

void get_dim (const std::vector<std::vector<double>> & mat, int & dim) {
    int rows, cols;

    get_dim(mat, rows, cols);
    dim = rows;

    if (rows != cols) {
        std::raise(SIGFPE);
    }
}

}

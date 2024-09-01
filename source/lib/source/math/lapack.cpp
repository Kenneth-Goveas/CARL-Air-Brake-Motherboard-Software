#include <csignal>

#include <vector>

#include <math/lapack.hpp>

extern "C" {

void dgeqrf_ (
    const int * rows, const int * cols, double * data_buf, const int * lead,
    double * coef_buf, double * work_buf, const int * len, int * ret
);
void dorgqr_ (
    const int * rows, const int * cols, const int * count, double * data_buf,
    const int * lead, const double * coef_buf, double * work_buf,
    const int * len, int * ret
);
void dgerqf_ (
    const int * rows, const int * cols, double * data_buf, const int * lead,
    double * coef_buf, double * work_buf, const int * len, int * ret
);
void dorgrq_ (
    const int * rows, const int * cols, const int * count, double * data_buf,
    const int * lead, const double * coef_buf, double * work_buf,
    const int * len, int * ret
);
void dgeqlf_ (
    const int * rows, const int * cols, double * data_buf, const int * lead,
    double * coef_buf, double * work_buf, const int * len, int * ret
);
void dorgql_ (
    const int * rows, const int * cols, const int * count, double * data_buf,
    const int * lead, const double * coef_buf, double * work_buf,
    const int * len, int * ret
);
void dgelqf_ (
    const int * rows, const int * cols, double * data_buf, const int * lead,
    double * coef_buf, double * work_buf, const int * len, int * ret
);
void dorglq_ (
    const int * rows, const int * cols, const int * count, double * data_buf,
    const int * lead, const double * coef_buf, double * work_buf,
    const int * len, int * ret
);
void dgetrf_ (
    const int * rows, const int * cols, double * data_buf, const int * lead,
    int * pivot_buf, int * ret
);
void dgetri_ (
    const int * dim, double * data_buf, const int * lead, const int * pivot_buf,
    double * work_buf, const int * len, int * ret
);
void dsyev_ (
    const char * task, const char * type, const int * dim, double * data_buf,
    const int * lead, double * eig_buf, double * work_buf, const int * len,
    int * ret
);

}

namespace lapack :: intern {

void get_dim (std::vector<int> vec, int * dim);
void get_dim (std::vector<double> vec, int * dim);
void get_dim (std::vector<std::vector<int>> mat, int * dim);
void get_dim (std::vector<std::vector<double>> mat, int * dim);
void get_dim (std::vector<std::vector<int>> mat, int * rows, int * cols);
void get_dim (std::vector<std::vector<double>> mat, int * rows, int * cols);

}

namespace lapack {

void dgeqrf (
    std::vector<std::vector<double>> data,
    std::vector<std::vector<double>> * fact,
    std::vector<double> * coef
) {
    int ret;
    int rows, cols, comm, len;
    double * data_buf, * coef_buf, * work_buf;

    intern::get_dim(data, &rows, &cols);

    if (rows < cols) {
        comm = rows;
    } else {
        comm = cols;
    }

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

    len = (int)(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dgeqrf_(&rows, &cols, data_buf, &rows, coef_buf, work_buf, &len, &ret);

    (*fact).resize(rows);
    for (int i = 0; i < rows; i++) {
        (*fact)[i].resize(cols);
        for (int j = 0; j < cols; j++) {
            (*fact)[i][j] = data_buf[i + j * rows];
        }
    }

    (*coef).resize(comm);
    for (int i = 0; i < comm; i++) {
        (*coef)[i] = coef_buf[i];
    }

    delete [] data_buf;
    delete [] coef_buf;
    delete [] work_buf;
}

void dorgqr (
    std::vector<std::vector<double>> fact,
    std::vector<double> coef,
    std::vector<std::vector<double>> * orth,
    std::vector<std::vector<double>> * uppr
) {
    int ret;
    int rows, cols, comm, len;
    double * fact_buf, * coef_buf, * work_buf;

    intern::get_dim(fact, &rows, &cols);
    intern::get_dim(coef, &comm);

    if (rows < cols) {
        if (comm != rows) {
            std::raise(SIGFPE);
        }
    } else {
        if (comm != cols) {
            std::raise(SIGFPE);
        }
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

    dorgqr_(
        &rows, &comm, &comm, fact_buf, &rows, coef_buf, work_buf, &len, &ret
    );

    len = (int)(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dorgqr_(
        &rows, &comm, &comm, fact_buf, &rows, coef_buf, work_buf, &len, &ret
    );

    (*orth).resize(rows);
    for (int i = 0; i < rows; i++) {
        (*orth)[i].resize(comm);
        for (int j = 0; j < comm; j++) {
            (*orth)[i][j] = fact_buf[i + j * rows];
        }
    }

    (*uppr).resize(comm);
    for (int i = 0; i < comm; i++) {
        (*uppr)[i].resize(cols);
        for (int j = 0; j < cols; j++) {
            if (i > j) {
                (*uppr)[i][j] = 0;
            } else {
                (*uppr)[i][j] = fact[i][j];
            }
        }
    }

    delete [] fact_buf;
    delete [] coef_buf;
    delete [] work_buf;
}

void dgerqf (
    std::vector<std::vector<double>> data,
    std::vector<std::vector<double>> * fact,
    std::vector<double> * coef
) {
    int ret;
    int rows, cols, comm, len;
    double * data_buf, * coef_buf, * work_buf;

    intern::get_dim(data, &rows, &cols);

    if (rows < cols) {
        comm = rows;
    } else {
        comm = cols;
    }

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

    len = (int)(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dgerqf_(&rows, &cols, data_buf, &rows, coef_buf, work_buf, &len, &ret);

    (*fact).resize(rows);
    for (int i = 0; i < rows; i++) {
        (*fact)[i].resize(cols);
        for (int j = 0; j < cols; j++) {
            (*fact)[i][j] = data_buf[i + j * rows];
        }
    }

    (*coef).resize(comm);
    for (int i = 0; i < comm; i++) {
        (*coef)[i] = coef_buf[i];
    }

    delete [] data_buf;
    delete [] coef_buf;
    delete [] work_buf;
}

void dorgrq (
    std::vector<std::vector<double>> fact,
    std::vector<double> coef,
    std::vector<std::vector<double>> * uppr,
    std::vector<std::vector<double>> * orth
) {
    int ret;
    int rows, cols, comm, len;
    double * fact_buf, * coef_buf, * work_buf;

    intern::get_dim(fact, &rows, &cols);
    intern::get_dim(coef, &comm);

    if (rows < cols) {
        if (comm != rows) {
            std::raise(SIGFPE);
        }
    } else {
        if (comm != cols) {
            std::raise(SIGFPE);
        }
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

    dorgrq_(
        &comm, &cols, &comm, fact_buf, &comm, coef_buf, work_buf, &len, &ret
    );

    len = (int)(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dorgrq_(
        &comm, &cols, &comm, fact_buf, &comm, coef_buf, work_buf, &len, &ret
    );

    (*orth).resize(comm);
    for (int i = 0; i < comm; i++) {
        (*orth)[i].resize(cols);
        for (int j = 0; j < cols; j++) {
            (*orth)[i][j] = fact_buf[i + j * comm];
        }
    }

    (*uppr).resize(rows);
    for (int i = 0; i < rows; i++) {
        (*uppr)[i].resize(comm);
        for (int j = 0; j < comm; j++) {
            if (i - j > rows - comm) {
                (*uppr)[i][j] = 0;
            } else {
                (*uppr)[i][j] = fact[i][cols - comm + j];
            }
        }
    }

    delete [] fact_buf;
    delete [] coef_buf;
    delete [] work_buf;
}

void dgeqlf (
    std::vector<std::vector<double>> data,
    std::vector<std::vector<double>> * fact,
    std::vector<double> * coef
) {
    int ret;
    int rows, cols, comm, len;
    double * data_buf, * coef_buf, * work_buf;

    intern::get_dim(data, &rows, &cols);

    if (rows < cols) {
        comm = rows;
    } else {
        comm = cols;
    }

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

    len = (int)(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dgeqlf_(&rows, &cols, data_buf, &rows, coef_buf, work_buf, &len, &ret);

    (*fact).resize(rows);
    for (int i = 0; i < rows; i++) {
        (*fact)[i].resize(cols);
        for (int j = 0; j < cols; j++) {
            (*fact)[i][j] = data_buf[i + j * rows];
        }
    }

    (*coef).resize(comm);
    for (int i = 0; i < comm; i++) {
        (*coef)[i] = coef_buf[i];
    }

    delete [] data_buf;
    delete [] coef_buf;
    delete [] work_buf;
}

void dorgql (
    std::vector<std::vector<double>> fact,
    std::vector<double> coef,
    std::vector<std::vector<double>> * orth,
    std::vector<std::vector<double>> * lowr
) {
    int ret;
    int rows, cols, comm, len;
    double * fact_buf, * coef_buf, * work_buf;

    intern::get_dim(fact, &rows, &cols);
    intern::get_dim(coef, &comm);

    if (rows < cols) {
        if (comm != rows) {
            std::raise(SIGFPE);
        }
    } else {
        if (comm != cols) {
            std::raise(SIGFPE);
        }
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

    dorgql_(
        &rows, &comm, &comm, fact_buf, &rows, coef_buf, work_buf, &len, &ret
    );

    len = (int)(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dorgql_(
        &rows, &comm, &comm, fact_buf, &rows, coef_buf, work_buf, &len, &ret
    );

    (*orth).resize(rows);
    for (int i = 0; i < rows; i++) {
        (*orth)[i].resize(comm);
        for (int j = 0; j < comm; j++) {
            (*orth)[i][j] = fact_buf[i + j * rows];
        }
    }

    (*lowr).resize(comm);
    for (int i = 0; i < comm; i++) {
        (*lowr)[i].resize(cols);
        for (int j = 0; j < cols; j++) {
            if (j - i > cols - comm) {
                (*lowr)[i][j] = 0;
            } else {
                (*lowr)[i][j] = fact[rows - comm + i][j];
            }
        }
    }

    delete [] fact_buf;
    delete [] coef_buf;
    delete [] work_buf;
}

void dgelqf (
    std::vector<std::vector<double>> data,
    std::vector<std::vector<double>> * fact,
    std::vector<double> * coef
) {
    int ret;
    int rows, cols, comm, len;
    double * data_buf, * coef_buf, * work_buf;

    intern::get_dim(data, &rows, &cols);

    if (rows < cols) {
        comm = rows;
    } else {
        comm = cols;
    }

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

    len = (int)(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dgelqf_(&rows, &cols, data_buf, &rows, coef_buf, work_buf, &len, &ret);

    (*fact).resize(rows);
    for (int i = 0; i < rows; i++) {
        (*fact)[i].resize(cols);
        for (int j = 0; j < cols; j++) {
            (*fact)[i][j] = data_buf[i + j * rows];
        }
    }

    (*coef).resize(comm);
    for (int i = 0; i < comm; i++) {
        (*coef)[i] = coef_buf[i];
    }

    delete [] data_buf;
    delete [] coef_buf;
    delete [] work_buf;
}

void dorglq (
    std::vector<std::vector<double>> fact,
    std::vector<double> coef,
    std::vector<std::vector<double>> * lowr,
    std::vector<std::vector<double>> * orth
) {
    int ret;
    int rows, cols, comm, len;
    double * fact_buf, * coef_buf, * work_buf;

    intern::get_dim(fact, &rows, &cols);
    intern::get_dim(coef, &comm);

    if (rows < cols) {
        if (comm != rows) {
            std::raise(SIGFPE);
        }
    } else {
        if (comm != cols) {
            std::raise(SIGFPE);
        }
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

    dorglq_(
        &comm, &cols, &comm, fact_buf, &comm, coef_buf, work_buf, &len, &ret
    );

    len = (int)(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dorglq_(
        &comm, &cols, &comm, fact_buf, &comm, coef_buf, work_buf, &len, &ret
    );

    (*orth).resize(comm);
    for (int i = 0; i < comm; i++) {
        (*orth)[i].resize(cols);
        for (int j = 0; j < cols; j++) {
            (*orth)[i][j] = fact_buf[i + j * comm];
        }
    }

    (*lowr).resize(rows);
    for (int i = 0; i < rows; i++) {
        (*lowr)[i].resize(comm);
        for (int j = 0; j < comm; j++) {
            if (j > i) {
                (*lowr)[i][j] = 0;
            } else {
                (*lowr)[i][j] = fact[i][j];
            }
        }
    }

    delete [] fact_buf;
    delete [] coef_buf;
    delete [] work_buf;
}

void dgetrf (
    std::vector<std::vector<double>> data,
    std::vector<std::vector<double>> * perm,
    std::vector<std::vector<double>> * lowr,
    std::vector<std::vector<double>> * uppr
) {
    int ret;
    int rows, cols, comm;
    double * data_buf;
    int * pivot_buf;

    intern::get_dim(data, &rows, &cols);

    if (rows < cols) {
        comm = rows;
    } else {
        comm = cols;
    }

    data_buf = new double [rows * cols];
    pivot_buf = new int [comm];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            data_buf[i + j * rows] = data[i][j];
        }
    }

    dgetrf_(&rows, &cols, data_buf, &rows, pivot_buf, &ret);

    for (int i = 0; i < comm; i++) {
        pivot_buf[i] -= 1;
    }

    (*lowr).resize(rows);
    for (int i = 0; i < rows; i++) {
        (*lowr)[i].resize(comm);
        for (int j = 0; j < comm; j++) {
            if (i < j) {
                (*lowr)[i][j] = 0;
            } else if (i == j) {
                (*lowr)[i][j] = 1;
            } else {
                (*lowr)[i][j] = data_buf[i + j * rows];
            }
        }
    }

    (*uppr).resize(comm);
    for (int i = 0; i < comm; i++) {
        (*uppr)[i].resize(cols);
        for (int j = 0; j < cols; j++) {
            if (i > j) {
                (*uppr)[i][j] = 0;
            } else {
                (*uppr)[i][j] = data_buf[i + j * rows];
            }
        }
    }

    (*perm).resize(rows);
    for (int i = 0; i < rows; i++) {
        (*perm)[i].resize(rows);
        for (int j = 0; j < rows; j++) {
            if (i == j) {
                (*perm)[i][j] = 1;
            } else {
                (*perm)[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < comm; i++) {
        if (pivot_buf[i] != i) {
            for (int j = 0; j < rows; j++) {
                if ((*perm)[j][i] == 1) {
                    (*perm)[j][i] = 0;
                    (*perm)[j][pivot_buf[i]] = 1;
                } else if ((*perm)[j][pivot_buf[i]] == 1) {
                    (*perm)[j][i] = 1;
                    (*perm)[j][pivot_buf[i]] = 0;
                }
            }
        }
    }

    delete [] data_buf;
    delete [] pivot_buf;
}

void dgetrf (
    std::vector<std::vector<double>> data,
    std::vector<double> * diag, int * sign
) {
    int ret;
    int dim;
    double * data_buf;
    int * pivot_buf;

    intern::get_dim(data, &dim);

    data_buf = new double [dim * dim];
    pivot_buf = new int [dim];

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            data_buf[i + j * dim] = data[i][j];
        }
    }

    dgetrf_(&dim, &dim, data_buf, &dim, pivot_buf, &ret);

    *sign = 1;
    (*diag).resize(dim);
    for (int i = 0; i < dim; i++) {
        if (pivot_buf[i] != i + 1) {
            *sign = -(*sign);
        }
        (*diag)[i] = data_buf[i * (dim + 1)];
    }

    delete [] data_buf;
    delete [] pivot_buf;
}

void dgetrf (
    std::vector<std::vector<double>> data,
    std::vector<std::vector<double>> * fact,
    std::vector<int> * pivot
) {
    int ret;
    int dim;
    double * data_buf;
    int * pivot_buf;

    intern::get_dim(data, &dim);

    data_buf = new double [dim * dim];
    pivot_buf = new int [dim];

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            data_buf[i + j * dim] = data[i][j];
        }
    }

    dgetrf_(&dim, &dim, data_buf, &dim, pivot_buf, &ret);

    (*fact).resize(dim);
    for (int i = 0; i < dim; i++) {
        (*fact)[i].resize(dim);
        for (int j = 0; j < dim; j++) {
            (*fact)[i][j] = data_buf[i + j * dim];
        }
    }

    (*pivot).resize(dim);
    for (int i = 0; i < dim; i++) {
        (*pivot)[i] = pivot_buf[i];
    }

    delete [] data_buf;
    delete [] pivot_buf;
}

void dgetri (
    std::vector<std::vector<double>> fact,
    std::vector<int> pivot,
    std::vector<std::vector<double>> * inv
) {
    int ret;
    int dim, comm, len;
    double * fact_buf, * work_buf;
    int * pivot_buf;

    intern::get_dim(fact, &dim);
    intern::get_dim(pivot, &comm);

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

    len = (int)(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dgetri_(&dim, fact_buf, &dim, pivot_buf, work_buf, &len, &ret);
    if (ret != 0) {
        delete [] fact_buf;
        delete [] pivot_buf;
        delete [] work_buf;
        std::raise(SIGFPE);
    }

    (*inv).resize(dim);
    for (int i = 0; i < dim; i++) {
        (*inv)[i].resize(dim);
        for (int j = 0; j < dim; j++) {
            (*inv)[i][j] = fact_buf[i + j * dim];
        }
    }

    delete [] fact_buf;
    delete [] pivot_buf;
    delete [] work_buf;
}

void dsyev (
    std::vector<std::vector<double>> data,
    std::vector<double> * eigval
) {
    int ret;
    int dim, len;
    double * data_buf, * eig_buf, * work_buf;
    char task = 'N', type = 'L';

    intern::get_dim(data, &dim);

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

    len = (int)(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dsyev_(&task, &type, &dim, data_buf, &dim, eig_buf, work_buf, &len, &ret);
    if (ret != 0) {
        delete [] data_buf;
        delete [] eig_buf;
        delete [] work_buf;
        std::raise(SIGFPE);
    }

    (*eigval).resize(dim);
    for (int i = 0; i < dim; i++) {
        (*eigval)[i] = eig_buf[i];
    }

    delete [] data_buf;
    delete [] eig_buf;
    delete [] work_buf;
}

void dsyev (
    std::vector<std::vector<double>> data,
    std::vector<std::vector<double>> * eigval,
    std::vector<std::vector<double>> * eigvec
) {
    int ret;
    int dim, len;
    double * data_buf, * eig_buf, * work_buf;
    char task = 'V', type = 'L';

    intern::get_dim(data, &dim);

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

    len = (int)(*work_buf);

    delete work_buf;
    work_buf = new double [len];

    dsyev_(&task, &type, &dim, data_buf, &dim, eig_buf, work_buf, &len, &ret);
    if (ret != 0) {
        delete [] data_buf;
        delete [] eig_buf;
        delete [] work_buf;
        std::raise(SIGFPE);
    }

    (*eigval).resize(dim);
    for (int i = 0; i < dim; i++) {
        (*eigval)[i].resize(dim);
        for (int j = 0; j < dim; j++) {
            if (i == j) {
                (*eigval)[i][j] = eig_buf[i];
            } else {
                (*eigval)[i][j] = 0;
            }
        }
    }

    (*eigvec).resize(dim);
    for (int i = 0; i < dim; i++) {
        (*eigvec)[i].resize(dim);
        for (int j = 0; j < dim; j++) {
            (*eigvec)[i][j] = data_buf[i + j * dim];
        }
    }

    delete [] data_buf;
    delete [] eig_buf;
    delete [] work_buf;
}

}

namespace lapack :: intern {

void get_dim (std::vector<int> vec, int * dim) {
    *dim = vec.size();
    if ((*dim) == 0) {
        std::raise(SIGFPE);
    }
}

void get_dim (std::vector<double> vec, int * dim) {
    *dim = vec.size();
    if ((*dim) == 0) {
        std::raise(SIGFPE);
    }
}

void get_dim (std::vector<std::vector<int>> mat, int * dim) {
    *dim = mat.size();
    if ((*dim) == 0) {
        std::raise(SIGFPE);
    }

    for (int i = 0; i < (*dim); i++) {
        if (mat[i].size() != (*dim)) {
            std::raise(SIGFPE);
        }
    }
}

void get_dim (std::vector<std::vector<double>> mat, int * dim) {
    *dim = mat.size();
    if ((*dim) == 0) {
        std::raise(SIGFPE);
    }

    for (int i = 0; i < (*dim); i++) {
        if (mat[i].size() != (*dim)) {
            std::raise(SIGFPE);
        }
    }
}

void get_dim (std::vector<std::vector<int>> mat, int * rows, int * cols) {
    *rows = mat.size();
    if ((*rows) == 0) {
        std::raise(SIGFPE);
    }

    *cols = mat[0].size();
    if ((*cols) == 0) {
        std::raise(SIGFPE);
    }

    for (int i = 0; i < (*rows); i++) {
        if (mat[i].size() != (*cols)) {
            std::raise(SIGFPE);
        }
    }
}

void get_dim (std::vector<std::vector<double>> mat, int * rows, int * cols) {
    *rows = mat.size();
    if ((*rows) == 0) {
        std::raise(SIGFPE);
    }

    *cols = mat[0].size();
    if ((*cols) == 0) {
        std::raise(SIGFPE);
    }

    for (int i = 0; i < (*rows); i++) {
        if (mat[i].size() != (*cols)) {
            std::raise(SIGFPE);
        }
    }
}

}

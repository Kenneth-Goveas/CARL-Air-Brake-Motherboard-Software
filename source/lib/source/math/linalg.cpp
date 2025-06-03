#include <cmath>
#include <csignal>

#include <vector>
#include <tuple>
#include <initializer_list>

#include <math/lapack.hpp>
#include <math/linalg.hpp>

namespace linalg::intern {

bool valid_dim (int dim);
bool valid_dim (int rows, int cols);

void get_dim (std::initializer_list<int> list, int & dim);
void get_dim (std::initializer_list<double> list, int & dim);

void get_dim (std::initializer_list<std::vector<int>> list, int & rows, int & cols);
void get_dim (std::initializer_list<std::vector<double>> list, int & rows, int & cols);

}

namespace linalg {

ivector::ivector (void) {
    this->dim = 0;
    this->data = std::vector<int>(this->dim, 0);
}

ivector::ivector (int dim) {
    if (!intern::valid_dim(dim)) {
        std::raise(SIGFPE);
    }
    this->dim = dim;
    this->data = std::vector<int>(this->dim, 0);
}

ivector::ivector (std::initializer_list<int> list) {
    intern::get_dim(list, this->dim);
    this->data = std::vector<int>(list);
}

ivector::ivector (const ivector & vec) {
    this->dim = vec.dim;
    this->data = vec.data;
}

imatrix::imatrix (void) {
    this->rows = 0;
    this->cols = 0;
    this->data = std::vector<std::vector<int>>(this->rows, std::vector<int>(this->cols, 0));
}

imatrix::imatrix (int rows, int cols) {
    if (!intern::valid_dim(rows, cols)) {
        std::raise(SIGFPE);
    }
    this->rows = rows;
    this->cols = cols;
    this->data = std::vector<std::vector<int>>(this->rows, std::vector<int>(this->cols, 0));
}

imatrix::imatrix (std::initializer_list<std::vector<int>> list) {
    intern::get_dim(list, this->rows, this->cols);
    this->data = std::vector<std::vector<int>>(list);
}

imatrix::imatrix (const imatrix & mat) {
    this->rows = mat.rows;
    this->cols = mat.cols;
    this->data = mat.data;
}

fvector::fvector (void) {
    this->dim = 0;
    this->data = std::vector<double>(this->dim, 0);
}

fvector::fvector (int dim) {
    if (!intern::valid_dim(dim)) {
        std::raise(SIGFPE);
    }
    this->dim = dim;
    this->data = std::vector<double>(this->dim, 0);
}

fvector::fvector (std::initializer_list<double> list) {
    intern::get_dim(list, this->dim);
    this->data = std::vector<double>(list);
}

fvector::fvector (const fvector & vec) {
    this->dim = vec.dim;
    this->data = vec.data;
}

fmatrix::fmatrix (void) {
    this->rows = 0;
    this->cols = 0;
    this->data = std::vector<std::vector<double>>(this->rows, std::vector<double>(this->cols, 0));
}

fmatrix::fmatrix (int rows, int cols) {
    if (!intern::valid_dim(rows, cols)) {
        std::raise(SIGFPE);
    }
    this->rows = rows;
    this->cols = cols;
    this->data = std::vector<std::vector<double>>(this->rows, std::vector<double>(this->cols, 0));
}

fmatrix::fmatrix (std::initializer_list<std::vector<double>> list) {
    intern::get_dim(list, this->rows, this->cols);
    this->data = std::vector<std::vector<double>>(list);
}

fmatrix::fmatrix (const fmatrix & mat) {
    this->rows = mat.rows;
    this->cols = mat.cols;
    this->data = mat.data;
}

int & ivector::operator [] (int idx) {
    return this->data[idx];
}

const int & ivector::operator [] (int idx) const {
    return this->data[idx];
}

std::vector<int> & imatrix::operator [] (int idx) {
    return this->data[idx];
}

const std::vector<int> & imatrix::operator [] (int idx) const {
    return this->data[idx];
}

double & fvector::operator [] (int idx) {
    return this->data[idx];
}

const double & fvector::operator [] (int idx) const {
    return this->data[idx];
}

std::vector<double> & fmatrix::operator [] (int idx) {
    return this->data[idx];
}

const std::vector<double> & fmatrix::operator [] (int idx) const {
    return this->data[idx];
}

ivector & ivector::operator = (const ivector & vec) {
    this->dim = vec.dim;
    this->data = vec.data;
    return *this;
}

ivector & ivector::operator += (const ivector & vec) {
    if (null(*this) || null(vec)) {
        std::raise(SIGFPE);
    } else if (this->dim != vec.dim) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] += vec.data[i];
    }
    return *this;
}

ivector & ivector::operator -= (const ivector & vec) {
    if (null(*this) || null(vec)) {
        std::raise(SIGFPE);
    } else if (this->dim != vec.dim) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] -= vec.data[i];
    }
    return *this;
}

ivector & ivector::operator *= (int scl) {
    if (null(*this)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] *= scl;
    }
    return *this;
}

ivector & ivector::operator /= (int scl) {
    if (null(*this) || scl == 0) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] /= scl;
    }
    return *this;
}

imatrix & imatrix::operator = (const imatrix & mat) {
    this->rows = mat.rows;
    this->cols = mat.cols;
    this->data = mat.data;
    return *this;
}

imatrix & imatrix::operator += (const imatrix & mat) {
    if (null(*this) || null(mat)) {
        std::raise(SIGFPE);
    } else if (this->rows != mat.rows || this->cols != mat.cols) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] += mat.data[i][j];
        }
    }
    return *this;
}

imatrix & imatrix::operator -= (const imatrix & mat) {
    if (null(*this) || null(mat)) {
        std::raise(SIGFPE);
    } else if (this->rows != mat.rows || this->cols != mat.cols) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] -= mat.data[i][j];
        }
    }
    return *this;
}

imatrix & imatrix::operator *= (int scl) {
    if (null(*this)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] *= scl;
        }
    }
    return *this;
}

imatrix & imatrix::operator /= (int scl) {
    if (null(*this) || scl == 0) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] /= scl;
        }
    }
    return *this;
}

fvector & fvector::operator = (const fvector & vec) {
    this->dim = vec.dim;
    this->data = vec.data;
    return *this;
}

fvector & fvector::operator += (const fvector & vec) {
    if (null(*this) || null(vec)) {
        std::raise(SIGFPE);
    } else if (this->dim != vec.dim) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] += vec.data[i];
    }
    return *this;
}

fvector & fvector::operator -= (const fvector & vec) {
    if (null(*this) || null(vec)) {
        std::raise(SIGFPE);
    } else if (this->dim != vec.dim) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] -= vec.data[i];
    }
    return *this;
}

fvector & fvector::operator *= (double scl) {
    if (null(*this)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] *= scl;
    }
    return *this;
}

fvector & fvector::operator /= (double scl) {
    if (null(*this) || scl == 0) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] /= scl;
    }
    return *this;
}

fmatrix & fmatrix::operator = (const fmatrix & mat) {
    this->rows = mat.rows;
    this->cols = mat.cols;
    this->data = mat.data;
    return *this;
}

fmatrix & fmatrix::operator += (const fmatrix & mat) {
    if (null(*this) || null(mat)) {
        std::raise(SIGFPE);
    } else if (this->rows != mat.rows || this->cols != mat.cols) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] += mat.data[i][j];
        }
    }
    return *this;
}

fmatrix & fmatrix::operator -= (const fmatrix & mat) {
    if (null(*this) || null(mat)) {
        std::raise(SIGFPE);
    } else if (this->rows != mat.rows || this->cols != mat.cols) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] -= mat.data[i][j];
        }
    }
    return *this;
}

fmatrix & fmatrix::operator *= (double scl) {
    if (null(*this)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] *= scl;
        }
    }
    return *this;
}

fmatrix & fmatrix::operator /= (double scl) {
    if (null(*this) || scl == 0) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] /= scl;
        }
    }
    return *this;
}

bool operator == (const ivector & vec1, const ivector & vec2) {
    if (null(vec1) && null(vec2)) {
        return true;
    } else if (vec1.dim != vec2.dim) {
        return false;
    } else {
        for (int i = 0; i < vec1.dim; i++) {
            if (vec1.data[i] != vec2.data[i]) {
                return false;
            }
        }
        return true;
    }
}

bool operator != (const ivector & vec1, const ivector & vec2) {
    if (null(vec1) && null(vec2)) {
        return false;
    } else if (vec1.dim != vec2.dim) {
        return true;
    } else {
        for (int i = 0; i < vec1.dim; i++) {
            if (vec1.data[i] != vec2.data[i]) {
                return true;
            }
        }
        return false;
    }
}

bool operator >= (const ivector & vec1, const ivector & vec2) {
    if (null(vec1) && null(vec2)) {
        return true;
    } else if (vec1.dim != vec2.dim) {
        return false;
    } else {
        for (int i = 0; i < vec1.dim; i++) {
            if (vec1.data[i] < vec2.data[i]) {
                return false;
            }
        }
        return true;
    }
}

bool operator <= (const ivector & vec1, const ivector & vec2) {
    if (null(vec1) && null(vec2)) {
        return true;
    } else if (vec1.dim != vec2.dim) {
        return false;
    } else {
        for (int i = 0; i < vec1.dim; i++) {
            if (vec1.data[i] > vec2.data[i]) {
                return false;
            }
        }
        return true;
    }
}

bool operator > (const ivector & vec1, const ivector & vec2) {
    if (null(vec1) && null(vec2)) {
        return false;
    } else if (vec1.dim != vec2.dim) {
        return false;
    } else {
        for (int i = 0; i < vec1.dim; i++) {
            if (vec1.data[i] <= vec2.data[i]) {
                return false;
            }
        }
        return true;
    }
}

bool operator < (const ivector & vec1, const ivector & vec2) {
    if (null(vec1) && null(vec2)) {
        return false;
    } else if (vec1.dim != vec2.dim) {
        return false;
    } else {
        for (int i = 0; i < vec1.dim; i++) {
            if (vec1.data[i] >= vec2.data[i]) {
                return false;
            }
        }
        return true;
    }
}

bool operator == (const imatrix & mat1, const imatrix & mat2) {
    if (null(mat1) && null(mat2)) {
        return true;
    } else if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        return false;
    } else {
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (mat1.data[i][j] != mat2.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
}

bool operator != (const imatrix & mat1, const imatrix & mat2) {
    if (null(mat1) && null(mat2)) {
        return false;
    } else if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        return true;
    } else {
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (mat1.data[i][j] != mat2.data[i][j]) {
                    return true;
                }
            }
        }
        return false;
    }
}

bool operator >= (const imatrix & mat1, const imatrix & mat2) {
    if (null(mat1) && null(mat2)) {
        return true;
    } else if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        return false;
    } else {
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (mat1.data[i][j] < mat2.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
}

bool operator <= (const imatrix & mat1, const imatrix & mat2) {
    if (null(mat1) && null(mat2)) {
        return true;
    } else if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        return false;
    } else {
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (mat1.data[i][j] > mat2.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
}

bool operator > (const imatrix & mat1, const imatrix & mat2) {
    if (null(mat1) && null(mat2)) {
        return false;
    } else if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        return false;
    } else {
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (mat1.data[i][j] <= mat2.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
}

bool operator < (const imatrix & mat1, const imatrix & mat2) {
    if (null(mat1) && null(mat2)) {
        return false;
    } else if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        return false;
    } else {
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (mat1.data[i][j] >= mat2.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
}

bool operator == (const fvector & vec1, const fvector & vec2) {
    if (null(vec1) && null(vec2)) {
        return true;
    } else if (vec1.dim != vec2.dim) {
        return false;
    } else {
        for (int i = 0; i < vec1.dim; i++) {
            if (vec1.data[i] != vec2.data[i]) {
                return false;
            }
        }
        return true;
    }
}

bool operator != (const fvector & vec1, const fvector & vec2) {
    if (null(vec1) && null(vec2)) {
        return false;
    } else if (vec1.dim != vec2.dim) {
        return true;
    } else {
        for (int i = 0; i < vec1.dim; i++) {
            if (vec1.data[i] != vec2.data[i]) {
                return true;
            }
        }
        return false;
    }
}

bool operator >= (const fvector & vec1, const fvector & vec2) {
    if (null(vec1) && null(vec2)) {
        return true;
    } else if (vec1.dim != vec2.dim) {
        return false;
    } else {
        for (int i = 0; i < vec1.dim; i++) {
            if (vec1.data[i] < vec2.data[i]) {
                return false;
            }
        }
        return true;
    }
}

bool operator <= (const fvector & vec1, const fvector & vec2) {
    if (null(vec1) && null(vec2)) {
        return true;
    } else if (vec1.dim != vec2.dim) {
        return false;
    } else {
        for (int i = 0; i < vec1.dim; i++) {
            if (vec1.data[i] > vec2.data[i]) {
                return false;
            }
        }
        return true;
    }
}

bool operator > (const fvector & vec1, const fvector & vec2) {
    if (null(vec1) && null(vec2)) {
        return false;
    } else if (vec1.dim != vec2.dim) {
        return false;
    } else {
        for (int i = 0; i < vec1.dim; i++) {
            if (vec1.data[i] <= vec2.data[i]) {
                return false;
            }
        }
        return true;
    }
}

bool operator < (const fvector & vec1, const fvector & vec2) {
    if (null(vec1) && null(vec2)) {
        return false;
    } else if (vec1.dim != vec2.dim) {
        return false;
    } else {
        for (int i = 0; i < vec1.dim; i++) {
            if (vec1.data[i] >= vec2.data[i]) {
                return false;
            }
        }
        return true;
    }
}

bool operator == (const fmatrix & mat1, const fmatrix & mat2) {
    if (null(mat1) && null(mat2)) {
        return true;
    } else if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        return false;
    } else {
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (mat1.data[i][j] != mat2.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
}

bool operator != (const fmatrix & mat1, const fmatrix & mat2) {
    if (null(mat1) && null(mat2)) {
        return false;
    } else if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        return true;
    } else {
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (mat1.data[i][j] != mat2.data[i][j]) {
                    return true;
                }
            }
        }
        return false;
    }
}

bool operator >= (const fmatrix & mat1, const fmatrix & mat2) {
    if (null(mat1) && null(mat2)) {
        return true;
    } else if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        return false;
    } else {
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (mat1.data[i][j] < mat2.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
}

bool operator <= (const fmatrix & mat1, const fmatrix & mat2) {
    if (null(mat1) && null(mat2)) {
        return true;
    } else if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        return false;
    } else {
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (mat1.data[i][j] > mat2.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
}

bool operator > (const fmatrix & mat1, const fmatrix & mat2) {
    if (null(mat1) && null(mat2)) {
        return false;
    } else if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        return false;
    } else {
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (mat1.data[i][j] <= mat2.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
}

bool operator < (const fmatrix & mat1, const fmatrix & mat2) {
    if (null(mat1) && null(mat2)) {
        return false;
    } else if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        return false;
    } else {
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (mat1.data[i][j] >= mat2.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
}

ivector operator + (const ivector & vec) {
    ivector res;
    if (null(vec)) {
        std::raise(SIGFPE);
    }
    resize(res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = +(vec.data[i]);
    }
    return res;
}

ivector operator - (const ivector & vec) {
    ivector res;
    if (null(vec)) {
        std::raise(SIGFPE);
    }
    resize(res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = -(vec.data[i]);
    }
    return res;
}

imatrix operator + (const imatrix & mat) {
    imatrix res;
    if (null(mat)) {
        std::raise(SIGFPE);
    }
    resize(res, mat.rows, mat.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = +(mat.data[i][j]);
        }
    }
    return res;
}

imatrix operator - (const imatrix & mat) {
    imatrix res;
    if (null(mat)) {
        std::raise(SIGFPE);
    }
    resize(res, mat.rows, mat.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = -(mat.data[i][j]);
        }
    }
    return res;
}

fvector operator + (const fvector & vec) {
    fvector res;
    if (null(vec)) {
        std::raise(SIGFPE);
    }
    resize(res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = +(vec.data[i]);
    }
    return res;
}

fvector operator - (const fvector & vec) {
    fvector res;
    if (null(vec)) {
        std::raise(SIGFPE);
    }
    resize(res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = -(vec.data[i]);
    }
    return res;
}

fmatrix operator + (const fmatrix & mat) {
    fmatrix res;
    if (null(mat)) {
        std::raise(SIGFPE);
    }
    resize(res, mat.rows, mat.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = +(mat.data[i][j]);
        }
    }
    return res;
}

fmatrix operator - (const fmatrix & mat) {
    fmatrix res;
    if (null(mat)) {
        std::raise(SIGFPE);
    }
    resize(res, mat.rows, mat.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = -(mat.data[i][j]);
        }
    }
    return res;
}

ivector operator + (const ivector & vec1, const ivector & vec2) {
    ivector res;
    if (null(vec1) || null(vec2)) {
        std::raise(SIGFPE);
    } else if (vec1.dim != vec2.dim) {
        std::raise(SIGFPE);
    }
    resize(res, vec1.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec1.data[i] + vec2.data[i];
    }
    return res;
}

ivector operator - (const ivector & vec1, const ivector & vec2) {
    ivector res;
    if (null(vec1) || null(vec2)) {
        std::raise(SIGFPE);
    } else if (vec1.dim != vec2.dim) {
        std::raise(SIGFPE);
    }
    resize(res, vec1.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec1.data[i] - vec2.data[i];
    }
    return res;
}

ivector operator * (const ivector & vec, int scl) {
    ivector res;
    if (null(vec)) {
        std::raise(SIGFPE);
    }
    resize(res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec.data[i] * scl;
    }
    return res;
}

ivector operator * (int scl, const ivector & vec) {
    ivector res;
    if (null(vec)) {
        std::raise(SIGFPE);
    }
    resize(res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec.data[i] * scl;
    }
    return res;
}

ivector operator / (const ivector & vec, int scl) {
    ivector res;
    if (null(vec) || scl == 0) {
        std::raise(SIGFPE);
    }
    resize(res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec.data[i] / scl;
    }
    return res;
}

imatrix operator + (const imatrix & mat1, const imatrix & mat2) {
    imatrix res;
    if (null(mat1) || null(mat2)) {
        std::raise(SIGFPE);
    } else if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        std::raise(SIGFPE);
    }
    resize(res, mat1.rows, mat1.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat1.data[i][j] + mat2.data[i][j];
        }
    }
    return res;
}

imatrix operator - (const imatrix & mat1, const imatrix & mat2) {
    imatrix res;
    if (null(mat1) || null(mat2)) {
        std::raise(SIGFPE);
    } else if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        std::raise(SIGFPE);
    }
    resize(res, mat1.rows, mat1.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat1.data[i][j] - mat2.data[i][j];
        }
    }
    return res;
}

imatrix operator * (const imatrix & mat, int scl) {
    imatrix res;
    if (null(mat)) {
        std::raise(SIGFPE);
    }
    resize(res, mat.rows, mat.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat.data[i][j] * scl;
        }
    }
    return res;
}

imatrix operator * (int scl, const imatrix & mat) {
    imatrix res;
    if (null(mat)) {
        std::raise(SIGFPE);
    }
    resize(res, mat.rows, mat.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat.data[i][j] * scl;
        }
    }
    return res;
}

imatrix operator / (const imatrix & mat, int scl) {
    imatrix res;
    if (null(mat) || scl == 0) {
        std::raise(SIGFPE);
    }
    resize(res, mat.rows, mat.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat.data[i][j] / scl;
        }
    }
    return res;
}

fvector operator + (const fvector & vec1, const fvector & vec2) {
    fvector res;
    if (null(vec1) || null(vec2)) {
        std::raise(SIGFPE);
    } else if (vec1.dim != vec2.dim) {
        std::raise(SIGFPE);
    }
    resize(res, vec1.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec1.data[i] + vec2.data[i];
    }
    return res;
}

fvector operator - (const fvector & vec1, const fvector & vec2) {
    fvector res;
    if (null(vec1) || null(vec2)) {
        std::raise(SIGFPE);
    } else if (vec1.dim != vec2.dim) {
        std::raise(SIGFPE);
    }
    resize(res, vec1.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec1.data[i] - vec2.data[i];
    }
    return res;
}

fvector operator * (const fvector & vec, double scl) {
    fvector res;
    if (null(vec)) {
        std::raise(SIGFPE);
    }
    resize(res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec.data[i] * scl;
    }
    return res;
}

fvector operator * (double scl, const fvector & vec) {
    fvector res;
    if (null(vec)) {
        std::raise(SIGFPE);
    }
    resize(res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec.data[i] * scl;
    }
    return res;
}

fvector operator / (const fvector & vec, double scl) {
    fvector res;
    if (null(vec) || scl == 0) {
        std::raise(SIGFPE);
    }
    resize(res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec.data[i] / scl;
    }
    return res;
}

fmatrix operator + (const fmatrix & mat1, const fmatrix & mat2) {
    fmatrix res;
    if (null(mat1) || null(mat2)) {
        std::raise(SIGFPE);
    } else if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        std::raise(SIGFPE);
    }
    resize(res, mat1.rows, mat1.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat1.data[i][j] + mat2.data[i][j];
        }
    }
    return res;
}

fmatrix operator - (const fmatrix & mat1, const fmatrix & mat2) {
    fmatrix res;
    if (null(mat1) || null(mat2)) {
        std::raise(SIGFPE);
    } else if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        std::raise(SIGFPE);
    }
    resize(res, mat1.rows, mat1.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat1.data[i][j] - mat2.data[i][j];
        }
    }
    return res;
}

fmatrix operator * (const fmatrix & mat, double scl) {
    fmatrix res;
    if (null(mat)) {
        std::raise(SIGFPE);
    }
    resize(res, mat.rows, mat.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat.data[i][j] * scl;
        }
    }
    return res;
}

fmatrix operator * (double scl, const fmatrix & mat) {
    fmatrix res;
    if (null(mat)) {
        std::raise(SIGFPE);
    }
    resize(res, mat.rows, mat.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat.data[i][j] * scl;
        }
    }
    return res;
}

fmatrix operator / (const fmatrix & mat, double scl) {
    fmatrix res;
    if (null(mat) || scl == 0) {
        std::raise(SIGFPE);
    }
    resize(res, mat.rows, mat.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat.data[i][j] / scl;
        }
    }
    return res;
}

int operator * (const ivector & vec1, const ivector & vec2) {
    int res = 0;
    if (null(vec1) || null(vec2)) {
        std::raise(SIGFPE);
    } else if (vec1.dim != vec2.dim) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < vec1.dim; i++) {
        res += vec1.data[i] * vec2.data[i];
    }
    return res;
}

ivector operator * (const ivector & vec, const imatrix & mat) {
    ivector res;
    if (null(vec) || null(mat)) {
        std::raise(SIGFPE);
    } else if (vec.dim != mat.rows) {
        std::raise(SIGFPE);
    }
    resize(res, mat.cols);
    for (int i = 0; i < res.dim; i++) {
        for (int j = 0; j < vec.dim; j++) {
            res.data[i] += vec.data[j] * mat.data[j][i];
        }
    }
    return res;
}

ivector operator * (const imatrix & mat, const ivector & vec) {
    ivector res;
    if (null(mat) || null(vec)) {
        std::raise(SIGFPE);
    } else if (mat.cols != vec.dim) {
        std::raise(SIGFPE);
    }
    resize(res, mat.rows);
    for (int i = 0; i < res.dim; i++) {
        for (int j = 0; j < vec.dim; j++) {
            res.data[i] += mat.data[i][j] * vec.data[j];
        }
    }
    return res;
}

imatrix operator * (const imatrix & mat1, const imatrix & mat2) {
    imatrix res;
    if (null(mat1) || null(mat2)) {
        std::raise(SIGFPE);
    } else if (mat1.cols != mat2.rows) {
        std::raise(SIGFPE);
    }
    resize(res, mat1.rows, mat2.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            for (int k = 0; k < mat1.cols; k++) {
                res.data[i][j] += mat1.data[i][k] * mat2.data[k][j];
            }
        }
    }
    return res;
}

imatrix operator ^ (const ivector & vec1, const ivector & vec2) {
    imatrix res;
    if (null(vec1) || null(vec2)) {
        std::raise(SIGFPE);
    }
    resize(res, vec1.dim, vec2.dim);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = vec1.data[i] * vec2.data[j];
        }
    }
    return res;
}

double operator * (const fvector & vec1, const fvector & vec2) {
    double res = 0;
    if (null(vec1) || null(vec2)) {
        std::raise(SIGFPE);
    } else if (vec1.dim != vec2.dim) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < vec1.dim; i++) {
        res += vec1.data[i] * vec2.data[i];
    }
    return res;
}

fvector operator * (const fvector & vec, const fmatrix & mat) {
    fvector res;
    if (null(vec) || null(mat)) {
        std::raise(SIGFPE);
    } else if (vec.dim != mat.rows) {
        std::raise(SIGFPE);
    }
    resize(res, mat.cols);
    for (int i = 0; i < res.dim; i++) {
        for (int j = 0; j < vec.dim; j++) {
            res.data[i] += vec.data[j] * mat.data[j][i];
        }
    }
    return res;
}

fvector operator * (const fmatrix & mat, const fvector & vec) {
    fvector res;
    if (null(mat) || null(vec)) {
        std::raise(SIGFPE);
    } else if (mat.cols != vec.dim) {
        std::raise(SIGFPE);
    }
    resize(res, mat.rows);
    for (int i = 0; i < res.dim; i++) {
        for (int j = 0; j < vec.dim; j++) {
            res.data[i] += mat.data[i][j] * vec.data[j];
        }
    }
    return res;
}

fmatrix operator * (const fmatrix & mat1, const fmatrix & mat2) {
    fmatrix res;
    if (null(mat1) || null(mat2)) {
        std::raise(SIGFPE);
    } else if (mat1.cols != mat2.rows) {
        std::raise(SIGFPE);
    }
    resize(res, mat1.rows, mat2.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            for (int k = 0; k < mat1.cols; k++) {
                res.data[i][j] += mat1.data[i][k] * mat2.data[k][j];
            }
        }
    }
    return res;
}

fmatrix operator ^ (const fvector & vec1, const fvector & vec2) {
    fmatrix res;
    if (null(vec1) || null(vec2)) {
        std::raise(SIGFPE);
    }
    resize(res, vec1.dim, vec2.dim);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = vec1.data[i] * vec2.data[j];
        }
    }
    return res;
}

int dim (const ivector & vec) {
    return vec.dim;
}

int rows (const imatrix & mat) {
    return mat.rows;
}

int cols (const imatrix & mat) {
    return mat.cols;
}

int dim (const fvector & vec) {
    return vec.dim;
}

int rows (const fmatrix & mat) {
    return mat.rows;
}

int cols (const fmatrix & mat) {
    return mat.cols;
}

void resize (ivector & vec, int dim) {
    if (!intern::valid_dim(dim)) {
        std::raise(SIGFPE);
    }
    vec.dim = dim;
    vec.data = std::vector<int>(vec.dim, 0);
}

void resize (imatrix & mat, int rows, int cols) {
    if (!intern::valid_dim(rows, cols)) {
        std::raise(SIGFPE);
    }
    mat.rows = rows;
    mat.cols = cols;
    mat.data = std::vector<std::vector<int>>(mat.rows, std::vector<int>(mat.cols, 0));
}

void resize (fvector & vec, int dim) {
    if (!intern::valid_dim(dim)) {
        std::raise(SIGFPE);
    }
    vec.dim = dim;
    vec.data = std::vector<double>(vec.dim, 0);
}

void resize (fmatrix & mat, int rows, int cols) {
    if (!intern::valid_dim(rows, cols)) {
        std::raise(SIGFPE);
    }
    mat.rows = rows;
    mat.cols = cols;
    mat.data = std::vector<std::vector<double>>(mat.rows, std::vector<double>(mat.cols, 0));
}

bool null (const ivector & vec) {
    return vec.data.empty();
}

bool null (const imatrix & mat) {
    return mat.data.empty();
}

bool null (const fvector & vec) {
    return vec.data.empty();
}

bool null (const fmatrix & mat) {
    return mat.data.empty();
}

bool sym (const fmatrix & mat) {
    if (null(mat)) {
        std::raise(SIGFPE);
    }

    if (mat.rows != mat.cols) {
        return false;
    }

    for (int i = 0; i < mat.rows - 1; i++) {
        for (int j = i + 1; j < mat.cols; j++) {
            if (mat.data[i][j] != mat.data[j][i]) {
                return false;
            }
        }
    }

    return true;
}

bool sym_pdf (const fmatrix & mat) {
    std::vector<double> eigval;

    if (null(mat)) {
        std::raise(SIGFPE);
    }

    if (!sym(mat)) {
        return false;
    }

    lapack::dsyev(mat.data, eigval);
    for (auto elem : eigval) {
        if (elem <= 0) {
            return false;
        }
    }

    return true;
}

bool sym_ndf (const fmatrix & mat) {
    std::vector<double> eigval;

    if (null(mat)) {
        std::raise(SIGFPE);
    }

    if (!sym(mat)) {
        return false;
    }

    lapack::dsyev(mat.data, eigval);
    for (auto elem : eigval) {
        if (elem >= 0) {
            return false;
        }
    }

    return true;
}

bool sym_psdf (const fmatrix & mat) {
    std::vector<double> eigval;

    if (null(mat)) {
        std::raise(SIGFPE);
    }

    if (!sym(mat)) {
        return false;
    }

    lapack::dsyev(mat.data, eigval);
    for (auto elem : eigval) {
        if (elem < 0) {
            return false;
        }
    }

    return true;
}

bool sym_nsdf (const fmatrix & mat) {
    std::vector<double> eigval;

    if (null(mat)) {
        std::raise(SIGFPE);
    }

    if (!sym(mat)) {
        return false;
    }

    lapack::dsyev(mat.data, eigval);
    for (auto elem : eigval) {
        if (elem > 0) {
            return false;
        }
    }

    return true;
}

double norm (const fvector & vec) {
    double res = 0;
    if (null(vec)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < vec.dim; i++) {
        res += vec.data[i] * vec.data[i];
    }
    res = std::sqrt(res);
    return res;
}

double trac (const fmatrix & mat) {
    double res = 0;
    if (null(mat)) {
        std::raise(SIGFPE);
    } else if (mat.rows != mat.cols) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < mat.rows; i++) {
        res += mat.data[i][i];
    }
    return res;
}

double det (const fmatrix & mat) {
    double res = 1;

    std::vector<std::vector<double>> fact;
    std::vector<int> pivot;

    if (null(mat)) {
        std::raise(SIGFPE);
    } else if (mat.rows != mat.cols) {
        std::raise(SIGFPE);
    }

    lapack::dgetrf(mat.data, fact, pivot);
    for (int i = 0; i < mat.rows; i++) {
        res *= fact[i][i];
        if (pivot[i] != i + 1) {
            res = -res;
        }
    }

    return res;
}

fmatrix tran (const fmatrix & mat) {
    fmatrix res;
    if (null(mat)) {
        std::raise(SIGFPE);
    }
    resize(res, mat.cols, mat.rows);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat.data[j][i];
        }
    }
    return res;
}

fmatrix inv (const fmatrix & mat) {
    fmatrix res;

    std::vector<std::vector<double>> fact;
    std::vector<int> pivot;

    if (null(mat)) {
        std::raise(SIGFPE);
    } else if (mat.rows != mat.cols) {
        std::raise(SIGFPE);
    }

    lapack::dgetrf(mat.data, fact, pivot);
    for (int i = 0; i < mat.rows; i++) {
        if (fact[i][i] == 0) {
            std::raise(SIGFPE);
        }
    }

    resize(res, mat.rows, mat.cols);
    lapack::dgetri(fact, pivot, res.data);

    return res;
}

std::tuple<fvector, fmatrix> eig (const fmatrix & mat) {
    fvector eigval;
    fmatrix eigvec;

    if (null(mat)) {
        std::raise(SIGFPE);
    } else if (!sym(mat)) {
        std::raise(SIGFPE);
    }

    resize(eigval, mat.rows);
    resize(eigvec, mat.rows, mat.cols);

    lapack::dsyev(mat.data, eigval.data, eigvec.data);

    return {eigval, eigvec};
}

}

namespace linalg::intern {

bool valid_dim (int dim) {
    return (dim >= 0);
}

bool valid_dim (int rows, int cols) {
    return ((rows > 0 && cols > 0) || (rows == 0 && cols == 0));
}

void get_dim (std::initializer_list<int> list, int & dim) {
    dim = list.size();
    if (!valid_dim(dim)) {
        std::raise(SIGFPE);
    }
}

void get_dim (std::initializer_list<double> list, int & dim) {
    dim = list.size();
    if (!valid_dim(dim)) {
        std::raise(SIGFPE);
    }
}

void get_dim (std::initializer_list<std::vector<int>> list, int & rows, int & cols) {
    rows = 0; cols = 0;

    if (!std::empty(list)) {
        rows = list.size();
        cols = list.begin()->size();
    }

    for (auto elem : list) {
        if (int(elem.size()) != cols) {
            std::raise(SIGFPE);
        }
    }

    if (!valid_dim(rows, cols)) {
        std::raise(SIGFPE);
    }
}

void get_dim (std::initializer_list<std::vector<double>> list, int & rows, int & cols) {
    rows = 0; cols = 0;

    if (!std::empty(list)) {
        rows = list.size();
        cols = list.begin()->size();
    }

    for (auto elem : list) {
        if (int(elem.size()) != cols) {
            std::raise(SIGFPE);
        }
    }

    if (!valid_dim(rows, cols)) {
        std::raise(SIGFPE);
    }
}

}

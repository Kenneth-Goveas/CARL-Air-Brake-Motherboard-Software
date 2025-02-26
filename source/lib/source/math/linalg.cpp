#include <cmath>
#include <csignal>

#include <utility>
#include <algorithm>
#include <vector>
#include <tuple>
#include <initializer_list>

#include <math/lapack.hpp>
#include <math/linalg.hpp>

namespace linalg :: intern {

bool valid_dim (int dim);
bool valid_dim (int rows, int cols);

int get_dim (std::initializer_list<int> list);
int get_dim (std::initializer_list<double> list);
std::tuple<int, int> get_dim (std::initializer_list<std::vector<int>> list);
std::tuple<int, int> get_dim (std::initializer_list<std::vector<double>> list);

}

namespace linalg {

ivector :: ivector (void) {
    resize(*this, 0);
}

ivector :: ivector (int dim) {
    if (!intern::valid_dim(dim)) {
        std::raise(SIGFPE);
    }
    resize(*this, dim);
}

ivector :: ivector (const ivector & vec) {
    resize(*this, vec.dim);
    for (int i = 0; i < this->dim; i++) {
        this->data[i] = vec.data[i];
    }
}

ivector :: ivector (std::initializer_list<int> list) {
    int dim = intern::get_dim(list);
    resize(*this, dim);
    for (int i = 0; i < this->dim; i++) {
        this->data[i] = list.begin()[i];
    }
}

imatrix :: imatrix (void) {
    resize(*this, 0, 0);
}

imatrix :: imatrix (int rows, int cols) {
    if (!intern::valid_dim(rows, cols)) {
        std::raise(SIGFPE);
    }
    resize(*this, rows, cols);
}

imatrix :: imatrix (const imatrix & mat) {
    resize(*this, mat.rows, mat.cols);
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] = mat.data[i][j];
        }
    }
}

imatrix :: imatrix (std::initializer_list<std::vector<int>> list) {
    int rows, cols;
    std::tie(rows, cols) = intern::get_dim(list);
    resize(*this, rows, cols);
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] = list.begin()[i][j];
        }
    }
}

fvector :: fvector (void) {
    resize(*this, 0);
}

fvector :: fvector (int dim) {
    if (!intern::valid_dim(dim)) {
        std::raise(SIGFPE);
    }
    resize(*this, dim);
}

fvector :: fvector (const fvector & vec) {
    resize(*this, vec.dim);
    for (int i = 0; i < this->dim; i++) {
        this->data[i] = vec.data[i];
    }
    if (!is_null(*this) && !is_finite(*this)) {
        std::raise(SIGFPE);
    }
}

fvector :: fvector (std::initializer_list<double> list) {
    int dim = intern::get_dim(list);
    resize(*this, dim);
    for (int i = 0; i < this->dim; i++) {
        this->data[i] = list.begin()[i];
    }
    if (!is_null(*this) && !is_finite(*this)) {
        std::raise(SIGFPE);
    }
}

fmatrix :: fmatrix (void) {
    resize(*this, 0, 0);
}

fmatrix :: fmatrix (int rows, int cols) {
    if (!intern::valid_dim(rows, cols)) {
        std::raise(SIGFPE);
    }
    resize(*this, rows, cols);
}

fmatrix :: fmatrix (const fmatrix & mat) {
    resize(*this, mat.rows, mat.cols);
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] = mat.data[i][j];
        }
    }
    if (!is_null(*this) && !is_finite(*this)) {
        std::raise(SIGFPE);
    }
}

fmatrix :: fmatrix (std::initializer_list<std::vector<double>> list) {
    int rows, cols;
    std::tie(rows, cols) = intern::get_dim(list);
    resize(*this, rows, cols);
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] = list.begin()[i][j];
        }
    }
    if (!is_null(*this) && !is_finite(*this)) {
        std::raise(SIGFPE);
    }
}

int & ivector :: operator [] (int idx) {
    return this->data[idx];
}

const int & ivector :: operator [] (int idx) const {
    return this->data[idx];
}

std::vector<int> & imatrix :: operator [] (int idx) {
    return this->data[idx];
}

const std::vector<int> & imatrix :: operator [] (int idx) const {
    return this->data[idx];
}

double & fvector :: operator [] (int idx) {
    return this->data[idx];
}

const double & fvector :: operator [] (int idx) const {
    return this->data[idx];
}

std::vector<double> & fmatrix :: operator [] (int idx) {
    return this->data[idx];
}

const std::vector<double> & fmatrix :: operator [] (int idx) const {
    return this->data[idx];
}

ivector & ivector :: operator = (const ivector & vec) {
    if (this != &vec) {
        resize(*this, vec.dim);
        for (int i = 0; i < this->dim; i++) {
            this->data[i] = vec.data[i];
        }
    }
    return *this;
}

ivector & ivector :: operator += (const ivector & vec) {
    if (is_null(*this) || is_null(vec)) {
        std::raise(SIGFPE);
    } else if (this->dim != vec.dim) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] += vec.data[i];
    }
    return *this;
}

ivector & ivector :: operator -= (const ivector & vec) {
    if (is_null(*this) || is_null(vec)) {
        std::raise(SIGFPE);
    } else if (this->dim != vec.dim) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] -= vec.data[i];
    }
    return *this;
}

ivector & ivector :: operator *= (int scl) {
    if (is_null(*this)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] *= scl;
    }
    return *this;
}

ivector & ivector :: operator /= (int scl) {
    if (is_null(*this) || scl == 0) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] /= scl;
    }
    return *this;
}

imatrix & imatrix :: operator = (const imatrix & mat) {
    if (this != &mat) {
        resize(*this, mat.rows, mat.cols);
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                this->data[i][j] = mat.data[i][j];
            }
        }
    }
    return *this;
}

imatrix & imatrix :: operator += (const imatrix & mat) {
    if (is_null(*this) || is_null(mat)) {
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

imatrix & imatrix :: operator -= (const imatrix & mat) {
    if (is_null(*this) || is_null(mat)) {
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

imatrix & imatrix :: operator *= (int scl) {
    if (is_null(*this)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] *= scl;
        }
    }
    return *this;
}

imatrix & imatrix :: operator /= (int scl) {
    if (is_null(*this) || scl == 0) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] /= scl;
        }
    }
    return *this;
}

fvector & fvector :: operator = (const fvector & vec) {
    if (!is_null(vec) && !is_finite(vec)) {
        std::raise(SIGFPE);
    }
    if (this != &vec) {
        resize(*this, vec.dim);
        for (int i = 0; i < this->dim; i++) {
            this->data[i] = vec.data[i];
        }
    }
    return *this;
}

fvector & fvector :: operator += (const fvector & vec) {
    if (!is_finite(*this) || !is_finite(vec)) {
        std::raise(SIGFPE);
    } else if (this->dim != vec.dim) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] += vec.data[i];
    }
    return *this;
}

fvector & fvector :: operator -= (const fvector & vec) {
    if (!is_finite(*this) || !is_finite(vec)) {
        std::raise(SIGFPE);
    } else if (this->dim != vec.dim) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] -= vec.data[i];
    }
    return *this;
}

fvector & fvector :: operator *= (double scl) {
    if (!is_finite(*this) || !is_finite(scl)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] *= scl;
    }
    return *this;
}

fvector & fvector :: operator /= (double scl) {
    if (!is_finite(*this) || !is_finite(scl) || scl == 0) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] /= scl;
    }
    return *this;
}

fmatrix & fmatrix :: operator = (const fmatrix & mat) {
    if (!is_null(mat) && !is_finite(mat)) {
        std::raise(SIGFPE);
    }
    if (this != &mat) {
        resize(*this, mat.rows, mat.cols);
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                this->data[i][j] = mat.data[i][j];
            }
        }
    }
    return *this;
}

fmatrix & fmatrix :: operator += (const fmatrix & mat) {
    if (!is_finite(*this) || !is_finite(mat)) {
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

fmatrix & fmatrix :: operator -= (const fmatrix & mat) {
    if (!is_finite(*this) || !is_finite(mat)) {
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

fmatrix & fmatrix :: operator *= (double scl) {
    if (!is_finite(*this) || !is_finite(scl)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] *= scl;
        }
    }
    return *this;
}

fmatrix & fmatrix :: operator /= (double scl) {
    if (!is_finite(*this) || !is_finite(scl) || scl == 0) {
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
    if (is_null(vec1) && is_null(vec2)) {
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
    if (is_null(vec1) && is_null(vec2)) {
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
    if (is_null(vec1) && is_null(vec2)) {
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
    if (is_null(vec1) && is_null(vec2)) {
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
    if (is_null(vec1) && is_null(vec2)) {
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
    if (is_null(vec1) && is_null(vec2)) {
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
    if (is_null(mat1) && is_null(mat2)) {
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
    if (is_null(mat1) && is_null(mat2)) {
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
    if (is_null(mat1) && is_null(mat2)) {
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
    if (is_null(mat1) && is_null(mat2)) {
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
    if (is_null(mat1) && is_null(mat2)) {
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
    if (is_null(mat1) && is_null(mat2)) {
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
    if ((!is_null(vec1) && !is_finite(vec1)) || (!is_null(vec2) && !is_finite(vec2))) {
        std::raise(SIGFPE);
    }
    if (is_null(vec1) && is_null(vec2)) {
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
    if ((!is_null(vec1) && !is_finite(vec1)) || (!is_null(vec2) && !is_finite(vec2))) {
        std::raise(SIGFPE);
    }
    if (is_null(vec1) && is_null(vec2)) {
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
    if ((!is_null(vec1) && !is_finite(vec1)) || (!is_null(vec2) && !is_finite(vec2))) {
        std::raise(SIGFPE);
    }
    if (is_null(vec1) && is_null(vec2)) {
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
    if ((!is_null(vec1) && !is_finite(vec1)) || (!is_null(vec2) && !is_finite(vec2))) {
        std::raise(SIGFPE);
    }
    if (is_null(vec1) && is_null(vec2)) {
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
    if ((!is_null(vec1) && !is_finite(vec1)) || (!is_null(vec2) && !is_finite(vec2))) {
        std::raise(SIGFPE);
    }
    if (is_null(vec1) && is_null(vec2)) {
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
    if ((!is_null(vec1) && !is_finite(vec1)) || (!is_null(vec2) && !is_finite(vec2))) {
        std::raise(SIGFPE);
    }
    if (is_null(vec1) && is_null(vec2)) {
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
    if ((!is_null(mat1) && !is_finite(mat1)) || (!is_null(mat2) && !is_finite(mat2))) {
        std::raise(SIGFPE);
    }
    if (is_null(mat1) && is_null(mat2)) {
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
    if ((!is_null(mat1) && !is_finite(mat1)) || (!is_null(mat2) && !is_finite(mat2))) {
        std::raise(SIGFPE);
    }
    if (is_null(mat1) && is_null(mat2)) {
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
    if ((!is_null(mat1) && !is_finite(mat1)) || (!is_null(mat2) && !is_finite(mat2))) {
        std::raise(SIGFPE);
    }
    if (is_null(mat1) && is_null(mat2)) {
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
    if ((!is_null(mat1) && !is_finite(mat1)) || (!is_null(mat2) && !is_finite(mat2))) {
        std::raise(SIGFPE);
    }
    if (is_null(mat1) && is_null(mat2)) {
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
    if ((!is_null(mat1) && !is_finite(mat1)) || (!is_null(mat2) && !is_finite(mat2))) {
        std::raise(SIGFPE);
    }
    if (is_null(mat1) && is_null(mat2)) {
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
    if ((!is_null(mat1) && !is_finite(mat1)) || (!is_null(mat2) && !is_finite(mat2))) {
        std::raise(SIGFPE);
    }
    if (is_null(mat1) && is_null(mat2)) {
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
    if (is_null(vec)) {
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
    if (is_null(vec)) {
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
    if (is_null(mat)) {
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
    if (is_null(mat)) {
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
    if (!is_finite(vec)) {
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
    if (!is_finite(vec)) {
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
    if (!is_finite(mat)) {
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
    if (!is_finite(mat)) {
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
    if (is_null(vec1) || is_null(vec2)) {
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
    if (is_null(vec1) || is_null(vec2)) {
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
    if (is_null(vec)) {
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
    if (is_null(vec)) {
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
    if (is_null(vec) || scl == 0) {
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
    if (is_null(mat1) || is_null(mat2)) {
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
    if (is_null(mat1) || is_null(mat2)) {
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
    if (is_null(mat)) {
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
    if (is_null(mat)) {
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
    if (is_null(mat) || scl == 0) {
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
    if (!is_finite(vec1) || !is_finite(vec2)) {
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
    if (!is_finite(vec1) || !is_finite(vec2)) {
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
    if (!is_finite(vec) || !is_finite(scl)) {
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
    if (!is_finite(vec) || !is_finite(scl)) {
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
    if (!is_finite(vec) || !is_finite(scl) || scl == 0) {
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
    if (!is_finite(mat1) || !is_finite(mat2)) {
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
    if (!is_finite(mat1) || !is_finite(mat2)) {
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
    if (!is_finite(mat) || !is_finite(scl)) {
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
    if (!is_finite(mat) || !is_finite(scl)) {
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
    if (!is_finite(mat) || !is_finite(scl) || scl == 0) {
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
    if (is_null(vec1) || is_null(vec2)) {
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
    if (is_null(vec) || is_null(mat)) {
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
    if (is_null(mat) || is_null(vec)) {
        std::raise(SIGFPE);
    } else if (vec.dim != mat.cols) {
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
    if (is_null(mat1) || is_null(mat2)) {
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
    if (is_null(vec1) || is_null(vec2)) {
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
    if (!is_finite(vec1) || !is_finite(vec2)) {
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
    if (!is_finite(vec) || !is_finite(mat)) {
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
    if (!is_finite(mat) || !is_finite(vec)) {
        std::raise(SIGFPE);
    } else if (vec.dim != mat.cols) {
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
    if (!is_finite(mat1) || !is_finite(mat2)) {
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
    if (!is_finite(vec1) || !is_finite(vec2)) {
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
    if (!is_null(vec) && !is_finite(vec)) {
        std::raise(SIGFPE);
    }
    return vec.dim;
}

int rows (const fmatrix & mat) {
    if (!is_null(mat) && !is_finite(mat)) {
        std::raise(SIGFPE);
    }
    return mat.rows;
}

int cols (const fmatrix & mat) {
    if (!is_null(mat) && !is_finite(mat)) {
        std::raise(SIGFPE);
    }
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

bool is_null (const ivector & vec) {
    return vec.data.empty();
}

bool is_null (const imatrix & mat) {
    return mat.data.empty();
}

bool is_null (const fvector & vec) {
    return vec.data.empty();
}

bool is_null (const fmatrix & mat) {
    return mat.data.empty();
}

bool is_finite (double scl) {
    int fpc = std::fpclassify(scl);
    if (fpc == FP_NAN || fpc == FP_INFINITE) {
        return false;
    } else {
        return true;
    }
}

bool is_finite (const fvector & vec) {
    int fpc;
    if (vec.data.empty()) {
        return false;
    }
    for (int i = 0; i < vec.dim; i++) {
        fpc = std::fpclassify(vec.data[i]);
        if (fpc == FP_NAN || fpc == FP_INFINITE) {
            return false;
        }
    }
    return true;
}

bool is_finite (const fmatrix & mat) {
    int fpc;
    if (mat.data.empty()) {
        return false;
    }
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            fpc = std::fpclassify(mat.data[i][j]);
            if (fpc == FP_NAN || fpc == FP_INFINITE) {
                return false;
            }
        }
    }
    return true;
}

double norm (const fvector & vec) {
    double res = 0;
    if (!is_finite(vec)) {
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
    if (!is_finite(mat)) {
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

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    } else if (mat.rows != mat.cols) {
        std::raise(SIGFPE);
    }

    std::tie(fact, pivot) = lapack::dgetrf(mat.data);
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
    if (!is_finite(mat)) {
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

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    } else if (mat.rows != mat.cols) {
        std::raise(SIGFPE);
    } else if (det(mat) == 0) {
        std::raise(SIGFPE);
    }

    resize(res, mat.rows, mat.cols);

    std::tie(fact, pivot) = lapack::dgetrf(mat.data);
    res.data = lapack::dgetri(fact, pivot);

    return res;
}

bool is_sym (const fmatrix & mat) {
    if (!is_finite(mat)) {
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

bool is_sym_pdf (const fmatrix & mat) {
    std::vector<double> eig;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }

    if (!is_sym(mat)) {
        return false;
    }

    std::tie(eig, std::ignore) = lapack::dsyev(mat.data);
    for (int i = 0; i < mat.rows; i++) {
        if (eig[i] <= 0) {
            return false;
        }
    }

    return true;
}

bool is_sym_ndf (const fmatrix & mat) {
    std::vector<double> eig;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }

    if (!is_sym(mat)) {
        return false;
    }

    std::tie(eig, std::ignore) = lapack::dsyev(mat.data);
    for (int i = 0; i < mat.rows; i++) {
        if (eig[i] >= 0) {
            return false;
        }
    }

    return true;
}

bool is_sym_psdf (const fmatrix & mat) {
    std::vector<double> eig;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }

    if (!is_sym(mat)) {
        return false;
    }

    std::tie(eig, std::ignore) = lapack::dsyev(mat.data);
    for (int i = 0; i < mat.rows; i++) {
        if (eig[i] < 0) {
            return false;
        }
    }

    return true;
}

bool is_sym_nsdf (const fmatrix & mat) {
    std::vector<double> eig;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }

    if (!is_sym(mat)) {
        return false;
    }

    std::tie(eig, std::ignore) = lapack::dsyev(mat.data);
    for (int i = 0; i < mat.rows; i++) {
        if (eig[i] > 0) {
            return false;
        }
    }

    return true;
}

std::tuple<fmatrix, fmatrix, fmatrix> plu (const fmatrix & mat) {
    int rows = mat.rows, cols = mat.cols, comm = std::min(mat.rows, mat.cols);
    fmatrix matp, matl, matu;
    std::vector<std::vector<double>> fact;
    std::vector<int> pivot;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }

    resize(matp, rows, rows);
    resize(matl, rows, comm);
    resize(matu, comm, cols);

    std::tie(fact, pivot) = lapack::dgetrf(mat.data);
    for (int i = 0; i < comm; i++) {
        pivot[i] -= 1;
    }

    for (int i = 0; i < rows; i++) {
        matp.data[i][i] = 1;
    }
    for (int i = 0; i < comm; i++) {
        if (pivot[i] != i) {
            for (int j = 0; j < rows; j++) {
                if (matp.data[j][i] == 1) {
                    matp.data[j][i] = 0;
                    matp.data[j][pivot[i]] = 1;
                } else if (matp.data[j][pivot[i]] == 1) {
                    matp.data[j][i] = 1;
                    matp.data[j][pivot[i]] = 0;
                }
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < comm; j++) {
            if (i < j) {
                matl.data[i][j] = 0;
            } else if (i == j) {
                matl.data[i][j] = 1;
            } else {
                matl.data[i][j] = fact[i][j];
            }
        }
    }

    for (int i = 0; i < comm; i++) {
        for (int j = 0; j < cols; j++) {
            if (i > j) {
                matu.data[i][j] = 0;
            } else {
                matu.data[i][j] = fact[i][j];
            }
        }
    }

    return {matp, matl, matu};
}

std::tuple<fmatrix, fmatrix> qr (const fmatrix & mat) {
    int rows = mat.rows, cols = mat.cols, comm = std::min(mat.rows, mat.cols);
    fmatrix matq, matr;
    std::vector<std::vector<double>> fact;
    std::vector<double> coef;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }

    resize(matq, rows, comm);
    resize(matr, comm, cols);

    std::tie(fact, coef) = lapack::dgeqrf(mat.data);
    std::tie(matq.data, matr.data) = lapack::dorgqr(fact, coef);

    return {matq, matr};
}

std::tuple<fmatrix, fmatrix> rq (const fmatrix & mat) {
    int rows = mat.rows, cols = mat.cols, comm = std::min(mat.rows, mat.cols);
    fmatrix matr, matq;
    std::vector<std::vector<double>> fact;
    std::vector<double> coef;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }

    resize(matr, rows, comm);
    resize(matq, comm, cols);

    std::tie(fact, coef) = lapack::dgerqf(mat.data);
    std::tie(matr.data, matq.data) = lapack::dorgrq(fact, coef);

    return {matr, matq};
}

std::tuple<fmatrix, fmatrix> ql (const fmatrix & mat) {
    int rows = mat.rows, cols = mat.cols, comm = std::min(mat.rows, mat.cols);
    fmatrix matq, matl;
    std::vector<std::vector<double>> fact;
    std::vector<double> coef;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }

    resize(matq, rows, comm);
    resize(matl, comm, cols);

    std::tie(fact, coef) = lapack::dgeqlf(mat.data);
    std::tie(matq.data, matl.data) = lapack::dorgql(fact, coef);

    return {matq, matl};
}

std::tuple<fmatrix, fmatrix> lq (const fmatrix & mat) {
    int rows = mat.rows, cols = mat.cols, comm = std::min(mat.rows, mat.cols);
    fmatrix matl, matq;
    std::vector<std::vector<double>> fact;
    std::vector<double> coef;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }

    resize(matl, rows, comm);
    resize(matq, comm, cols);

    std::tie(fact, coef) = lapack::dgelqf(mat.data);
    std::tie(matl.data, matq.data) = lapack::dorglq(fact, coef);

    return {matl, matq};
}

std::tuple<fmatrix, fmatrix> qdq (const fmatrix & mat) {
    fmatrix matq, matd;
    std::vector<double> eig;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    } else if (!is_sym(mat)) {
        std::raise(SIGFPE);
    }

    resize(matq, mat.rows, mat.cols);
    resize(matd, mat.rows, mat.cols);

    std::tie(eig, matq.data) = lapack::dsyev(mat.data);
    for (int i = 0; i < mat.rows; i++) {
        matd.data[i][i] = eig[i];
    }

    return {matq, matd};
}

}

namespace linalg :: intern {

bool valid_dim (int dim) {
    return (dim >= 0);
}

bool valid_dim (int rows, int cols) {
    return ((rows == 0 && cols == 0) || (rows > 0 && cols > 0));
}

int get_dim (std::initializer_list<int> list) {
    int dim = list.size();

    if (!valid_dim(dim)) {
        std::raise(SIGFPE);
    }

    return dim;
}

int get_dim (std::initializer_list<double> list) {
    int dim = list.size();

    if (!valid_dim(dim)) {
        std::raise(SIGFPE);
    }

    return dim;
}

std::tuple<int, int> get_dim (std::initializer_list<std::vector<int>> list) {
    int rows = 0, cols = 0;

    if (!std::empty(list)) {
        rows = list.size();
        cols = (*list.begin()).size();
    }

    for (auto elem : list) {
        if (elem.size() != cols) {
            std::raise(SIGFPE);
        }
    }

    if (!valid_dim(rows, cols)) {
        std::raise(SIGFPE);
    }

    return {rows, cols};
}

std::tuple<int, int> get_dim (std::initializer_list<std::vector<double>> list) {
    int rows = 0, cols = 0;

    if (!std::empty(list)) {
        rows = list.size();
        cols = (*list.begin()).size();
    }

    for (auto elem : list) {
        if (elem.size() != cols) {
            std::raise(SIGFPE);
        }
    }

    if (!valid_dim(rows, cols)) {
        std::raise(SIGFPE);
    }

    return {rows, cols};
}

}

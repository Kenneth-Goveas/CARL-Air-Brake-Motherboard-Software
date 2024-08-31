#include <cmath>
#include <csignal>

#include <vector>

#include <math/lapack.hpp>
#include <math/linalg.hpp>

namespace linalg :: intern {

int tol_ord = 0;
double tol_abs = 0;
double tol_rel = 0;

}

namespace linalg {

ivector :: ivector (void) {
    resize(this, 0);
}

ivector :: ivector (int dim) {
    if (!(dim >= 0)) {
        std::raise(SIGFPE);
    }
    resize(this, dim);
}

ivector :: ivector (const ivector & vec) {
    if (!(is_null(vec) || is_finite(vec))) {
        std::raise(SIGFPE);
    }
    resize(this, vec.dim);
    for (int i = 0; i < this->dim; i++) {
        this->data[i] = vec.data[i];
    }
}

ivector :: ~ivector (void) {
    resize(this, 0);
}

imatrix :: imatrix (void) {
    resize(this, 0, 0);
}

imatrix :: imatrix (int rows, int cols) {
    if (!((rows > 0 && cols > 0) || (rows == 0 && cols == 0))) {
        std::raise(SIGFPE);
    }
    resize(this, rows, cols);
}

imatrix :: imatrix (const imatrix & mat) {
    if (!(is_null(mat) || is_finite(mat))) {
        std::raise(SIGFPE);
    }
    resize(this, mat.rows, mat.cols);
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] = mat.data[i][j];
        }
    }
}

imatrix :: ~imatrix (void) {
    resize(this, 0, 0);
}

fvector :: fvector (void) {
    resize(this, 0);
}

fvector :: fvector (int dim) {
    if (!(dim >= 0)) {
        std::raise(SIGFPE);
    }
    resize(this, dim);
}

fvector :: fvector (const fvector & vec) {
    if (!(is_null(vec) || is_finite(vec))) {
        std::raise(SIGFPE);
    }
    resize(this, vec.dim);
    for (int i = 0; i < this->dim; i++) {
        this->data[i] = vec.data[i];
    }
    filter(this);
}

fvector :: ~fvector (void) {
    resize(this, 0);
}

fmatrix :: fmatrix (void) {
    resize(this, 0, 0);
}

fmatrix :: fmatrix (int rows, int cols) {
    if (!((rows > 0 && cols > 0) || (rows == 0 && cols == 0))) {
        std::raise(SIGFPE);
    }
    resize(this, rows, cols);
}

fmatrix :: fmatrix (const fmatrix & mat) {
    if (!(is_null(mat) || is_finite(mat))) {
        std::raise(SIGFPE);
    }
    resize(this, mat.rows, mat.cols);
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] = mat.data[i][j];
        }
    }
    filter(this);
}

fmatrix :: ~fmatrix (void) {
    resize(this, 0, 0);
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
    if (!(is_null(vec) || is_finite(vec))) {
        std::raise(SIGFPE);
    }
    if (this != &vec) {
        resize(this, vec.dim);
        for (int i = 0; i < this->dim; i++) {
            this->data[i] = vec.data[i];
        }
    }
    return *this;
}

ivector & ivector :: operator += (const ivector & vec) {
    if (!(is_finite(*this) && is_finite(vec))) {
        std::raise(SIGFPE);
    } else if (!(this->dim == vec.dim)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] += vec.data[i];
    }
    return *this;
}

ivector & ivector :: operator -= (const ivector & vec) {
    if (!(is_finite(*this) && is_finite(vec))) {
        std::raise(SIGFPE);
    } else if (!(this->dim == vec.dim)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] -= vec.data[i];
    }
    return *this;
}

ivector & ivector :: operator *= (int scl) {
    if (!(is_finite(*this) && is_finite(scl))) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] *= scl;
    }
    return *this;
}

ivector & ivector :: operator /= (int scl) {
    if (!(is_finite(*this) && is_finite(scl) && scl != 0)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] /= scl;
    }
    return *this;
}

imatrix & imatrix :: operator = (const imatrix & mat) {
    if (!(is_null(mat) || is_finite(mat))) {
        std::raise(SIGFPE);
    }
    if (this != &mat) {
        resize(this, mat.rows, mat.cols);
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                this->data[i][j] = mat.data[i][j];
            }
        }
    }
    return *this;
}

imatrix & imatrix :: operator += (const imatrix & mat) {
    if (!(is_finite(*this) && is_finite(mat))) {
        std::raise(SIGFPE);
    } else if (!(this->rows == mat.rows && this->cols == mat.cols)) {
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
    if (!(is_finite(*this) && is_finite(mat))) {
        std::raise(SIGFPE);
    } else if (!(this->rows == mat.rows && this->cols == mat.cols)) {
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
    if (!(is_finite(*this) && is_finite(scl))) {
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
    if (!(is_finite(*this) && is_finite(scl) && scl != 0)) {
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
    if (!(is_null(vec) || is_finite(vec))) {
        std::raise(SIGFPE);
    }
    if (this != &vec) {
        resize(this, vec.dim);
        for (int i = 0; i < this->dim; i++) {
            this->data[i] = vec.data[i];
        }
    }
    filter(this);
    return *this;
}

fvector & fvector :: operator += (const fvector & vec) {
    if (!(is_finite(*this) && is_finite(vec))) {
        std::raise(SIGFPE);
    } else if (!(this->dim == vec.dim)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] += vec.data[i];
    }
    filter(this);
    return *this;
}

fvector & fvector :: operator -= (const fvector & vec) {
    if (!(is_finite(*this) && is_finite(vec))) {
        std::raise(SIGFPE);
    } else if (!(this->dim == vec.dim)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] -= vec.data[i];
    }
    filter(this);
    return *this;
}

fvector & fvector :: operator *= (double scl) {
    if (!(is_finite(*this) && is_finite(scl))) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] *= scl;
    }
    filter(this);
    return *this;
}

fvector & fvector :: operator /= (double scl) {
    if (!(is_finite(*this) && is_finite(scl) && scl != 0)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->dim; i++) {
        this->data[i] /= scl;
    }
    filter(this);
    return *this;
}

fmatrix & fmatrix :: operator = (const fmatrix & mat) {
    if (!(is_null(mat) || is_finite(mat))) {
        std::raise(SIGFPE);
    }
    if (this != &mat) {
        resize(this, mat.rows, mat.cols);
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->cols; j++) {
                this->data[i][j] = mat.data[i][j];
            }
        }
    }
    filter(this);
    return *this;
}

fmatrix & fmatrix :: operator += (const fmatrix & mat) {
    if (!(is_finite(*this) && is_finite(mat))) {
        std::raise(SIGFPE);
    } else if (!(this->rows == mat.rows && this->cols == mat.cols)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] += mat.data[i][j];
        }
    }
    filter(this);
    return *this;
}

fmatrix & fmatrix :: operator -= (const fmatrix & mat) {
    if (!(is_finite(*this) && is_finite(mat))) {
        std::raise(SIGFPE);
    } else if (!(this->rows == mat.rows && this->cols == mat.cols)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] -= mat.data[i][j];
        }
    }
    filter(this);
    return *this;
}

fmatrix & fmatrix :: operator *= (double scl) {
    if (!(is_finite(*this) && is_finite(scl))) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] *= scl;
        }
    }
    filter(this);
    return *this;
}

fmatrix & fmatrix :: operator /= (double scl) {
    if (!(is_finite(*this) && is_finite(scl) && scl != 0)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->data[i][j] /= scl;
        }
    }
    filter(this);
    return *this;
}

bool operator == (const ivector & vec1, const ivector & vec2) {
    if (
           !(is_null(vec1) || is_finite(vec1))
        || !(is_null(vec2) || is_finite(vec2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(vec1) && is_null(vec2)) {
        return true;
    } else if (is_finite(vec1) && is_finite(vec2)) {
        if (vec1.dim != vec2.dim) {
            return false;
        }
        for (int i = 0; i < vec1.dim; i++) {
            if (vec1.data[i] != vec2.data[i]) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator != (const ivector & vec1, const ivector & vec2) {
    if (
           !(is_null(vec1) || is_finite(vec1))
        || !(is_null(vec2) || is_finite(vec2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(vec1) && is_null(vec2)) {
        return false;
    } else if (is_finite(vec1) && is_finite(vec2)) {
        if (vec1.dim != vec2.dim) {
            return true;
        }
        for (int i = 0; i < vec1.dim; i++) {
            if (vec1.data[i] != vec2.data[i]) {
                return true;
            }
        }
        return false;
    } else {
        return true;
    }
}

bool operator >= (const ivector & vec1, const ivector & vec2) {
    if (
           !(is_null(vec1) || is_finite(vec1))
        || !(is_null(vec2) || is_finite(vec2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(vec1) && is_null(vec2)) {
        return true;
    } else if (is_finite(vec1) && is_finite(vec2)) {
        if (vec1.dim != vec2.dim) {
            return false;
        }
        for (int i = 0; i < vec1.dim; i++) {
            if (vec1.data[i] < vec2.data[i]) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator <= (const ivector & vec1, const ivector & vec2) {
    if (
           !(is_null(vec1) || is_finite(vec1))
        || !(is_null(vec2) || is_finite(vec2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(vec1) && is_null(vec2)) {
        return true;
    } else if (is_finite(vec1) && is_finite(vec2)) {
        if (vec1.dim != vec2.dim) {
            return false;
        }
        for (int i = 0; i < vec1.dim; i++) {
            if (vec1.data[i] > vec2.data[i]) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator > (const ivector & vec1, const ivector & vec2) {
    if (
           !(is_null(vec1) || is_finite(vec1))
        || !(is_null(vec2) || is_finite(vec2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(vec1) && is_null(vec2)) {
        return false;
    } else if (is_finite(vec1) && is_finite(vec2)) {
        if (vec1.dim != vec2.dim) {
            return false;
        }
        for (int i = 0; i < vec1.dim; i++) {
            if (vec1.data[i] <= vec2.data[i]) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator < (const ivector & vec1, const ivector & vec2) {
    if (
           !(is_null(vec1) || is_finite(vec1))
        || !(is_null(vec2) || is_finite(vec2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(vec1) && is_null(vec2)) {
        return false;
    } else if (is_finite(vec1) && is_finite(vec2)) {
        if (vec1.dim != vec2.dim) {
            return false;
        }
        for (int i = 0; i < vec1.dim; i++) {
            if (vec1.data[i] >= vec2.data[i]) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator == (const imatrix & mat1, const imatrix & mat2) {
    if (
           !(is_null(mat1) || is_finite(mat1))
        || !(is_null(mat2) || is_finite(mat2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(mat1) && is_null(mat2)) {
        return true;
    } else if (is_finite(mat1) && is_finite(mat2)) {
        if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
            return false;
        }
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (mat1.data[i][j] != mat2.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator != (const imatrix & mat1, const imatrix & mat2) {
    if (
           !(is_null(mat1) || is_finite(mat1))
        || !(is_null(mat2) || is_finite(mat2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(mat1) && is_null(mat2)) {
        return false;
    } else if (is_finite(mat1) && is_finite(mat2)) {
        if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
            return true;
        }
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (mat1.data[i][j] != mat2.data[i][j]) {
                    return true;
                }
            }
        }
        return false;
    } else {
        return true;
    }
}

bool operator >= (const imatrix & mat1, const imatrix & mat2) {
    if (
           !(is_null(mat1) || is_finite(mat1))
        || !(is_null(mat2) || is_finite(mat2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(mat1) && is_null(mat2)) {
        return true;
    } else if (is_finite(mat1) && is_finite(mat2)) {
        if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
            return false;
        }
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (mat1.data[i][j] < mat2.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator <= (const imatrix & mat1, const imatrix & mat2) {
    if (
           !(is_null(mat1) || is_finite(mat1))
        || !(is_null(mat2) || is_finite(mat2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(mat1) && is_null(mat2)) {
        return true;
    } else if (is_finite(mat1) && is_finite(mat2)) {
        if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
            return false;
        }
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (mat1.data[i][j] > mat2.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator > (const imatrix & mat1, const imatrix & mat2) {
    if (
           !(is_null(mat1) || is_finite(mat1))
        || !(is_null(mat2) || is_finite(mat2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(mat1) && is_null(mat2)) {
        return false;
    } else if (is_finite(mat1) && is_finite(mat2)) {
        if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
            return false;
        }
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (mat1.data[i][j] <= mat2.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator < (const imatrix & mat1, const imatrix & mat2) {
    if (
           !(is_null(mat1) || is_finite(mat1))
        || !(is_null(mat2) || is_finite(mat2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(mat1) && is_null(mat2)) {
        return false;
    } else if (is_finite(mat1) && is_finite(mat2)) {
        if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
            return false;
        }
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (mat1.data[i][j] >= mat2.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator == (const fvector & vec1, const fvector & vec2) {
    if (
           !(is_null(vec1) || is_finite(vec1))
        || !(is_null(vec2) || is_finite(vec2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(vec1) && is_null(vec2)) {
        return true;
    } else if (is_finite(vec1) && is_finite(vec2)) {
        if (vec1.dim != vec2.dim) {
            return false;
        }
        for (int i = 0; i < vec1.dim; i++) {
            if (is_neq(vec1.data[i], vec2.data[i])) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator != (const fvector & vec1, const fvector & vec2) {
    if (
           !(is_null(vec1) || is_finite(vec1))
        || !(is_null(vec2) || is_finite(vec2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(vec1) && is_null(vec2)) {
        return false;
    } else if (is_finite(vec1) && is_finite(vec2)) {
        if (vec1.dim != vec2.dim) {
            return true;
        }
        for (int i = 0; i < vec1.dim; i++) {
            if (is_neq(vec1.data[i], vec2.data[i])) {
                return true;
            }
        }
        return false;
    } else {
        return true;
    }
}

bool operator >= (const fvector & vec1, const fvector & vec2) {
    if (
           !(is_null(vec1) || is_finite(vec1))
        || !(is_null(vec2) || is_finite(vec2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(vec1) && is_null(vec2)) {
        return true;
    } else if (is_finite(vec1) && is_finite(vec2)) {
        if (vec1.dim != vec2.dim) {
            return false;
        }
        for (int i = 0; i < vec1.dim; i++) {
            if (is_lneq(vec1.data[i], vec2.data[i])) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator <= (const fvector & vec1, const fvector & vec2) {
    if (
           !(is_null(vec1) || is_finite(vec1))
        || !(is_null(vec2) || is_finite(vec2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(vec1) && is_null(vec2)) {
        return true;
    } else if (is_finite(vec1) && is_finite(vec2)) {
        if (vec1.dim != vec2.dim) {
            return false;
        }
        for (int i = 0; i < vec1.dim; i++) {
            if (is_gneq(vec1.data[i], vec2.data[i])) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator > (const fvector & vec1, const fvector & vec2) {
    if (
           !(is_null(vec1) || is_finite(vec1))
        || !(is_null(vec2) || is_finite(vec2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(vec1) && is_null(vec2)) {
        return false;
    } else if (is_finite(vec1) && is_finite(vec2)) {
        if (vec1.dim != vec2.dim) {
            return false;
        }
        for (int i = 0; i < vec1.dim; i++) {
            if (is_leq(vec1.data[i], vec2.data[i])) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator < (const fvector & vec1, const fvector & vec2) {
    if (
           !(is_null(vec1) || is_finite(vec1))
        || !(is_null(vec2) || is_finite(vec2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(vec1) && is_null(vec2)) {
        return false;
    } else if (is_finite(vec1) && is_finite(vec2)) {
        if (vec1.dim != vec2.dim) {
            return false;
        }
        for (int i = 0; i < vec1.dim; i++) {
            if (is_geq(vec1.data[i], vec2.data[i])) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator == (const fmatrix & mat1, const fmatrix & mat2) {
    if (
           !(is_null(mat1) || is_finite(mat1))
        || !(is_null(mat2) || is_finite(mat2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(mat1) && is_null(mat2)) {
        return true;
    } else if (is_finite(mat1) && is_finite(mat2)) {
        if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
            return false;
        }
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (is_neq(mat1.data[i][j], mat2.data[i][j])) {
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator != (const fmatrix & mat1, const fmatrix & mat2) {
    if (
           !(is_null(mat1) || is_finite(mat1))
        || !(is_null(mat2) || is_finite(mat2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(mat1) && is_null(mat2)) {
        return false;
    } else if (is_finite(mat1) && is_finite(mat2)) {
        if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
            return true;
        }
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (is_neq(mat1.data[i][j], mat2.data[i][j])) {
                    return true;
                }
            }
        }
        return false;
    } else {
        return true;
    }
}

bool operator >= (const fmatrix & mat1, const fmatrix & mat2) {
    if (
           !(is_null(mat1) || is_finite(mat1))
        || !(is_null(mat2) || is_finite(mat2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(mat1) && is_null(mat2)) {
        return true;
    } else if (is_finite(mat1) && is_finite(mat2)) {
        if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
            return false;
        }
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (is_lneq(mat1.data[i][j], mat2.data[i][j])) {
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator <= (const fmatrix & mat1, const fmatrix & mat2) {
    if (
           !(is_null(mat1) || is_finite(mat1))
        || !(is_null(mat2) || is_finite(mat2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(mat1) && is_null(mat2)) {
        return true;
    } else if (is_finite(mat1) && is_finite(mat2)) {
        if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
            return false;
        }
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (is_gneq(mat1.data[i][j], mat2.data[i][j])) {
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator > (const fmatrix & mat1, const fmatrix & mat2) {
    if (
           !(is_null(mat1) || is_finite(mat1))
        || !(is_null(mat2) || is_finite(mat2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(mat1) && is_null(mat2)) {
        return false;
    } else if (is_finite(mat1) && is_finite(mat2)) {
        if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
            return false;
        }
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (is_leq(mat1.data[i][j], mat2.data[i][j])) {
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator < (const fmatrix & mat1, const fmatrix & mat2) {
    if (
           !(is_null(mat1) || is_finite(mat1))
        || !(is_null(mat2) || is_finite(mat2))
    ) {
        std::raise(SIGFPE);
    }
    if (is_null(mat1) && is_null(mat2)) {
        return false;
    } else if (is_finite(mat1) && is_finite(mat2)) {
        if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
            return false;
        }
        for (int i = 0; i < mat1.rows; i++) {
            for (int j = 0; j < mat1.cols; j++) {
                if (is_geq(mat1.data[i][j], mat2.data[i][j])) {
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

ivector operator + (const ivector & vec) {
    ivector res;
    if (!is_finite(vec)) {
        std::raise(SIGFPE);
    }
    resize(&res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = +(vec.data[i]);
    }
    return res;
}

ivector operator - (const ivector & vec) {
    ivector res;
    if (!is_finite(vec)) {
        std::raise(SIGFPE);
    }
    resize(&res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = -(vec.data[i]);
    }
    return res;
}

imatrix operator + (const imatrix & mat) {
    imatrix res;
    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }
    resize(&res, mat.rows, mat.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = +(mat.data[i][j]);
        }
    }
    return res;
}

imatrix operator - (const imatrix & mat) {
    imatrix res;
    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }
    resize(&res, mat.rows, mat.cols);
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
    resize(&res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = +(vec.data[i]);
    }
    filter(&res);
    return res;
}

fvector operator - (const fvector & vec) {
    fvector res;
    if (!is_finite(vec)) {
        std::raise(SIGFPE);
    }
    resize(&res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = -(vec.data[i]);
    }
    filter(&res);
    return res;
}

fmatrix operator + (const fmatrix & mat) {
    fmatrix res;
    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }
    resize(&res, mat.rows, mat.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = +(mat.data[i][j]);
        }
    }
    filter(&res);
    return res;
}

fmatrix operator - (const fmatrix & mat) {
    fmatrix res;
    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }
    resize(&res, mat.rows, mat.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = -(mat.data[i][j]);
        }
    }
    filter(&res);
    return res;
}

ivector operator + (const ivector & vec1, const ivector & vec2) {
    ivector res;
    if (!(is_finite(vec1) && is_finite(vec2))) {
        std::raise(SIGFPE);
    } else if (!(vec1.dim == vec2.dim)) {
        std::raise(SIGFPE);
    }
    resize(&res, vec1.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec1.data[i] + vec2.data[i];
    }
    return res;
}

ivector operator - (const ivector & vec1, const ivector & vec2) {
    ivector res;
    if (!(is_finite(vec1) && is_finite(vec2))) {
        std::raise(SIGFPE);
    } else if (!(vec1.dim == vec2.dim)) {
        std::raise(SIGFPE);
    }
    resize(&res, vec1.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec1.data[i] - vec2.data[i];
    }
    return res;
}

ivector operator * (const ivector & vec, int scl) {
    ivector res;
    if (!(is_finite(vec) && is_finite(scl))) {
        std::raise(SIGFPE);
    }
    resize(&res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec.data[i] * scl;
    }
    return res;
}

ivector operator * (int scl, const ivector & vec) {
    ivector res;
    if (!(is_finite(vec) && is_finite(scl))) {
        std::raise(SIGFPE);
    }
    resize(&res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec.data[i] * scl;
    }
    return res;
}

ivector operator / (const ivector & vec, int scl) {
    ivector res;
    if (!(is_finite(vec) && is_finite(scl) && scl != 0)) {
        std::raise(SIGFPE);
    }
    resize(&res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec.data[i] / scl;
    }
    return res;
}

imatrix operator + (const imatrix & mat1, const imatrix & mat2) {
    imatrix res;
    if (!(is_finite(mat1) && is_finite(mat2))) {
        std::raise(SIGFPE);
    } else if (!(mat1.rows == mat2.rows && mat1.cols == mat2.cols)) {
        std::raise(SIGFPE);
    }
    resize(&res, mat1.rows, mat1.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat1.data[i][j] + mat2.data[i][j];
        }
    }
    return res;
}

imatrix operator - (const imatrix & mat1, const imatrix & mat2) {
    imatrix res;
    if (!(is_finite(mat1) && is_finite(mat2))) {
        std::raise(SIGFPE);
    } else if (!(mat1.rows == mat2.rows && mat1.cols == mat2.cols)) {
        std::raise(SIGFPE);
    }
    resize(&res, mat1.rows, mat1.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat1.data[i][j] - mat2.data[i][j];
        }
    }
    return res;
}

imatrix operator * (const imatrix & mat, int scl) {
    imatrix res;
    if (!(is_finite(mat) && is_finite(scl))) {
        std::raise(SIGFPE);
    }
    resize(&res, mat.rows, mat.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat.data[i][j] * scl;
        }
    }
    return res;
}

imatrix operator * (int scl, const imatrix & mat) {
    imatrix res;
    if (!(is_finite(mat) && is_finite(scl))) {
        std::raise(SIGFPE);
    }
    resize(&res, mat.rows, mat.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat.data[i][j] * scl;
        }
    }
    return res;
}

imatrix operator / (const imatrix & mat, int scl) {
    imatrix res;
    if (!(is_finite(mat) && is_finite(scl) && scl != 0)) {
        std::raise(SIGFPE);
    }
    resize(&res, mat.rows, mat.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat.data[i][j] / scl;
        }
    }
    return res;
}

fvector operator + (const fvector & vec1, const fvector & vec2) {
    fvector res;
    if (!(is_finite(vec1) && is_finite(vec2))) {
        std::raise(SIGFPE);
    } else if (!(vec1.dim == vec2.dim)) {
        std::raise(SIGFPE);
    }
    resize(&res, vec1.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec1.data[i] + vec2.data[i];
    }
    filter(&res);
    return res;
}

fvector operator - (const fvector & vec1, const fvector & vec2) {
    fvector res;
    if (!(is_finite(vec1) && is_finite(vec2))) {
        std::raise(SIGFPE);
    } else if (!(vec1.dim == vec2.dim)) {
        std::raise(SIGFPE);
    }
    resize(&res, vec1.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec1.data[i] - vec2.data[i];
    }
    filter(&res);
    return res;
}

fvector operator * (const fvector & vec, double scl) {
    fvector res;
    if (!(is_finite(vec) && is_finite(scl))) {
        std::raise(SIGFPE);
    }
    resize(&res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec.data[i] * scl;
    }
    filter(&res);
    return res;
}

fvector operator * (double scl, const fvector & vec) {
    fvector res;
    if (!(is_finite(vec) && is_finite(scl))) {
        std::raise(SIGFPE);
    }
    resize(&res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec.data[i] * scl;
    }
    filter(&res);
    return res;
}

fvector operator / (const fvector & vec, double scl) {
    fvector res;
    if (!(is_finite(vec) && is_finite(scl) && scl != 0)) {
        std::raise(SIGFPE);
    }
    resize(&res, vec.dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = vec.data[i] / scl;
    }
    filter(&res);
    return res;
}

fmatrix operator + (const fmatrix & mat1, const fmatrix & mat2) {
    fmatrix res;
    if (!(is_finite(mat1) && is_finite(mat2))) {
        std::raise(SIGFPE);
    } else if (!(mat1.rows == mat2.rows && mat1.cols == mat2.cols)) {
        std::raise(SIGFPE);
    }
    resize(&res, mat1.rows, mat1.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat1.data[i][j] + mat2.data[i][j];
        }
    }
    filter(&res);
    return res;
}

fmatrix operator - (const fmatrix & mat1, const fmatrix & mat2) {
    fmatrix res;
    if (!(is_finite(mat1) && is_finite(mat2))) {
        std::raise(SIGFPE);
    } else if (!(mat1.rows == mat2.rows && mat1.cols == mat2.cols)) {
        std::raise(SIGFPE);
    }
    resize(&res, mat1.rows, mat1.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat1.data[i][j] - mat2.data[i][j];
        }
    }
    filter(&res);
    return res;
}

fmatrix operator * (const fmatrix & mat, double scl) {
    fmatrix res;
    if (!(is_finite(mat) && is_finite(scl))) {
        std::raise(SIGFPE);
    }
    resize(&res, mat.rows, mat.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat.data[i][j] * scl;
        }
    }
    filter(&res);
    return res;
}

fmatrix operator * (double scl, const fmatrix & mat) {
    fmatrix res;
    if (!(is_finite(mat) && is_finite(scl))) {
        std::raise(SIGFPE);
    }
    resize(&res, mat.rows, mat.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat.data[i][j] * scl;
        }
    }
    filter(&res);
    return res;
}

fmatrix operator / (const fmatrix & mat, double scl) {
    fmatrix res;
    if (!(is_finite(mat) && is_finite(scl) && scl != 0)) {
        std::raise(SIGFPE);
    }
    resize(&res, mat.rows, mat.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat.data[i][j] / scl;
        }
    }
    filter(&res);
    return res;
}

int operator * (const ivector & vec1, const ivector & vec2) {
    int res = 0;
    if (!(is_finite(vec1) && is_finite(vec2))) {
        std::raise(SIGFPE);
    } else if (!(vec1.dim == vec2.dim)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < vec1.dim; i++) {
        res += vec1.data[i] * vec2.data[i];
    }
    return res;
}

ivector operator * (const ivector & vec, const imatrix & mat) {
    ivector res;
    if (!(is_finite(vec) && is_finite(mat))) {
        std::raise(SIGFPE);
    } else if (!(vec.dim == mat.rows)) {
        std::raise(SIGFPE);
    }
    resize(&res, mat.cols);
    for (int i = 0; i < res.dim; i++) {
        for (int j = 0; j < vec.dim; j++) {
            res.data[i] += vec.data[j] * mat.data[j][i];
        }
    }
    return res;
}

ivector operator * (const imatrix & mat, const ivector & vec) {
    ivector res;
    if (!(is_finite(mat) && is_finite(vec))) {
        std::raise(SIGFPE);
    } else if (!(vec.dim == mat.cols)) {
        std::raise(SIGFPE);
    }
    resize(&res, mat.rows);
    for (int i = 0; i < res.dim; i++) {
        for (int j = 0; j < vec.dim; j++) {
            res.data[i] += mat.data[i][j] * vec.data[j];
        }
    }
    return res;
}

imatrix operator * (const imatrix & mat1, const imatrix & mat2) {
    imatrix res;
    if (!(is_finite(mat1) && is_finite(mat2))) {
        std::raise(SIGFPE);
    } else if (!(mat1.cols == mat2.rows)) {
        std::raise(SIGFPE);
    }
    resize(&res, mat1.rows, mat2.cols);
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
    if (!(is_finite(vec1) && is_finite(vec2))) {
        std::raise(SIGFPE);
    }
    resize(&res, vec1.dim, vec2.dim);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = vec1.data[i] * vec2.data[j];
        }
    }
    return res;
}

double operator * (const fvector & vec1, const fvector & vec2) {
    double res = 0;
    if (!(is_finite(vec1) && is_finite(vec2))) {
        std::raise(SIGFPE);
    } else if (!(vec1.dim == vec2.dim)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < vec1.dim; i++) {
        res += vec1.data[i] * vec2.data[i];
    }
    filter(&res);
    return res;
}

fvector operator * (const fvector & vec, const fmatrix & mat) {
    fvector res;
    if (!(is_finite(vec) && is_finite(mat))) {
        std::raise(SIGFPE);
    } else if (!(vec.dim == mat.rows)) {
        std::raise(SIGFPE);
    }
    resize(&res, mat.cols);
    for (int i = 0; i < res.dim; i++) {
        for (int j = 0; j < vec.dim; j++) {
            res.data[i] += vec.data[j] * mat.data[j][i];
        }
    }
    filter(&res);
    return res;
}

fvector operator * (const fmatrix & mat, const fvector & vec) {
    fvector res;
    if (!(is_finite(mat) && is_finite(vec))) {
        std::raise(SIGFPE);
    } else if (!(vec.dim == mat.cols)) {
        std::raise(SIGFPE);
    }
    resize(&res, mat.rows);
    for (int i = 0; i < res.dim; i++) {
        for (int j = 0; j < vec.dim; j++) {
            res.data[i] += mat.data[i][j] * vec.data[j];
        }
    }
    filter(&res);
    return res;
}

fmatrix operator * (const fmatrix & mat1, const fmatrix & mat2) {
    fmatrix res;
    if (!(is_finite(mat1) && is_finite(mat2))) {
        std::raise(SIGFPE);
    } else if (!(mat1.cols == mat2.rows)) {
        std::raise(SIGFPE);
    }
    resize(&res, mat1.rows, mat2.cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            for (int k = 0; k < mat1.cols; k++) {
                res.data[i][j] += mat1.data[i][k] * mat2.data[k][j];
            }
        }
    }
    filter(&res);
    return res;
}

fmatrix operator ^ (const fvector & vec1, const fvector & vec2) {
    fmatrix res;
    if (!(is_finite(vec1) && is_finite(vec2))) {
        std::raise(SIGFPE);
    }
    resize(&res, vec1.dim, vec2.dim);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = vec1.data[i] * vec2.data[j];
        }
    }
    filter(&res);
    return res;
}

void init (int tol_ord, double tol_abs, double tol_rel) {
    if (tol_ord < 0 || tol_abs < 0 || tol_rel < 0) {
        std::raise(SIGFPE);
    }
    intern::tol_ord = tol_ord;
    intern::tol_abs = tol_abs;
    intern::tol_rel = tol_rel;
}

int dim (const ivector & vec) {
    if (!(is_null(vec) || is_finite(vec))) {
        std::raise(SIGFPE);
    }
    return vec.dim;
}

int rows (const imatrix & mat) {
    if (!(is_null(mat) || is_finite(mat))) {
        std::raise(SIGFPE);
    }
    return mat.rows;
}

int cols (const imatrix & mat) {
    if (!(is_null(mat) || is_finite(mat))) {
        std::raise(SIGFPE);
    }
    return mat.cols;
}

int dim (const fvector & vec) {
    if (!(is_null(vec) || is_finite(vec))) {
        std::raise(SIGFPE);
    }
    return vec.dim;
}

int rows (const fmatrix & mat) {
    if (!(is_null(mat) || is_finite(mat))) {
        std::raise(SIGFPE);
    }
    return mat.rows;
}

int cols (const fmatrix & mat) {
    if (!(is_null(mat) || is_finite(mat))) {
        std::raise(SIGFPE);
    }
    return mat.cols;
}

void resize (ivector * vec, int dim) {
    if (!(dim >= 0)) {
        std::raise(SIGFPE);
    }
    vec->dim = dim;
    vec->data.resize(vec->dim);
    for (int i = 0; i < vec->dim; i++) {
        vec->data[i] = 0;
    }
}

void resize (imatrix * mat, int rows, int cols) {
    if (!((rows > 0 && cols > 0) || (rows == 0 && cols == 0))) {
        std::raise(SIGFPE);
    }
    mat->rows = rows;
    mat->cols = cols;
    mat->data.resize(mat->rows);
    for (int i = 0; i < mat->rows; i++) {
        mat->data[i].resize(mat->cols);
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = 0;
        }
    }
}

void resize (fvector * vec, int dim) {
    if (!(dim >= 0)) {
        std::raise(SIGFPE);
    }
    vec->dim = dim;
    vec->data.resize(vec->dim);
    for (int i = 0; i < vec->dim; i++) {
        vec->data[i] = 0;
    }
}

void resize (fmatrix * mat, int rows, int cols) {
    if (!((rows > 0 && cols > 0) || (rows == 0 && cols == 0))) {
        std::raise(SIGFPE);
    }
    mat->rows = rows;
    mat->cols = cols;
    mat->data.resize(mat->rows);
    for (int i = 0; i < mat->rows; i++) {
        mat->data[i].resize(mat->cols);
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = 0;
        }
    }
}

bool is_null (int scl) {
    return false;
}

bool is_null (const ivector & vec) {
    return vec.data.empty();
}

bool is_null (const imatrix & mat) {
    return mat.data.empty();
}

bool is_null (double scl) {
    return false;
}

bool is_null (const fvector & vec) {
    return vec.data.empty();
}

bool is_null (const fmatrix & mat) {
    return mat.data.empty();
}

bool is_finite (int scl) {
    return true;
}

bool is_finite (const ivector & vec) {
    return !vec.data.empty();
}

bool is_finite (const imatrix & mat) {
    return !mat.data.empty();
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

bool is_eq (double scl1, double scl2) {
    double norm, diff, marg;

    if (!(is_finite(scl1) && is_finite(scl2))) {
        std::raise(SIGFPE);
    }

    norm = std::fabs(scl1) + std::fabs(scl2);
    diff = std::fabs(scl1 - scl2);

    if (intern::tol_abs >= intern::tol_rel * norm) {
        marg = intern::tol_abs;
    } else {
        marg = intern::tol_rel * norm;
    }

    if (diff <= marg) {
        return true;
    } else {
        return false;
    }
}

bool is_neq (double scl1, double scl2) {
    double norm, diff, marg;

    if (!(is_finite(scl1) && is_finite(scl2))) {
        std::raise(SIGFPE);
    }

    norm = std::fabs(scl1) + std::fabs(scl2);
    diff = std::fabs(scl1 - scl2);

    if (intern::tol_abs >= intern::tol_rel * norm) {
        marg = intern::tol_abs;
    } else {
        marg = intern::tol_rel * norm;
    }

    if (diff <= marg) {
        return false;
    } else {
        return true;
    }
}

bool is_geq (double scl1, double scl2) {
    if (!(is_finite(scl1) && is_finite(scl2))) {
        std::raise(SIGFPE);
    }
    return (scl1 > scl2 || is_eq(scl1, scl2));
}

bool is_leq (double scl1, double scl2) {
    if (!(is_finite(scl1) && is_finite(scl2))) {
        std::raise(SIGFPE);
    }
    return (scl1 < scl2 || is_eq(scl1, scl2));
}

bool is_gneq (double scl1, double scl2) {
    if (!(is_finite(scl1) && is_finite(scl2))) {
        std::raise(SIGFPE);
    }
    return (scl1 > scl2 && is_neq(scl1, scl2));
}

bool is_lneq (double scl1, double scl2) {
    if (!(is_finite(scl1) && is_finite(scl2))) {
        std::raise(SIGFPE);
    }
    return (scl1 < scl2 && is_neq(scl1, scl2));
}

void filter (double * scl) {
    if (!is_finite(*scl)) {
        std::raise(SIGFPE);
    }
    if (is_eq(*scl, 0)) {
        *scl = 0;
    }
}

void filter (fvector * vec) {
    std::vector<int> ord(vec->dim);
    int ord_min = 0, ord_max = 0;
    bool ord_found = false;

    if (!(is_null(*vec) || is_finite(*vec))) {
        std::raise(SIGFPE);
    }

    if (is_finite(*vec)) {
        for (int i = 0; i < vec->dim; i++) {
            if (is_eq(vec->data[i], 0)) {
                vec->data[i] = 0;
            } else {
                ord[i] = (int)std::floor(std::log10(std::fabs(vec->data[i])));
                if (!ord_found) {
                    ord_found = true;
                    ord_min = ord[i];
                    ord_max = ord[i];
                } else {
                    if (ord_min > ord[i]) {
                        ord_min = ord[i];
                    }
                    if (ord_max < ord[i]) {
                        ord_max = ord[i];
                    }
                }
            }
        }
        if (intern::tol_ord > 0 && ord_max - ord_min >= intern::tol_ord) {
            ord_min = ord_max - intern::tol_ord + 1;
            for (int i = 0; i < vec->dim; i++) {
                if (vec->data[i] != 0 && ord[i] < ord_min) {
                    vec->data[i] = 0;
                }
            }
        }
    }
}

void filter (fmatrix * mat) {
    std::vector<std::vector<int>> ord(mat->rows, std::vector<int>(mat->cols));
    int ord_min = 0, ord_max = 0;
    bool ord_found = false;

    if (!(is_null(*mat) || is_finite(*mat))) {
        std::raise(SIGFPE);
    }

    if (is_finite(*mat)) {
        for (int i = 0; i < mat->rows; i++) {
            for (int j = 0; j < mat->cols; j++) {
                if (is_eq(mat->data[i][j], 0)) {
                    mat->data[i][j] = 0;
                } else {
                    ord[i][j] = (int)std::floor(std::log10(std::fabs(
                        mat->data[i][j]
                    )));
                    if (!ord_found) {
                        ord_found = true;
                        ord_min = ord[i][j];
                        ord_max = ord[i][j];
                    } else {
                        if (ord_min > ord[i][j]) {
                            ord_min = ord[i][j];
                        }
                        if (ord_max < ord[i][j]) {
                            ord_max = ord[i][j];
                        }
                    }
                }
            }
        }
        if (intern::tol_ord > 0 && ord_max - ord_min >= intern::tol_ord) {
            ord_min = ord_max - intern::tol_ord + 1;
            for (int i = 0; i < mat->rows; i++) {
                for (int j = 0; j < mat->cols; j++) {
                    if (mat->data[i][j] != 0 && ord[i][j] < ord_min) {
                        mat->data[i][j] = 0;
                    }
                }
            }
        }
    }
}

fvector zeros (int dim) {
    fvector res;
    if (!(dim > 0)) {
        std::raise(SIGFPE);
    }
    resize(&res, dim);
    return res;
}

fmatrix zeros (int rows, int cols) {
    fmatrix res;
    if (!(rows > 0 && cols > 0)) {
        std::raise(SIGFPE);
    }
    resize(&res, rows, cols);
    return res;
}

fvector ones (int dim) {
    fvector res;
    if (!(dim > 0)) {
        std::raise(SIGFPE);
    }
    resize(&res, dim);
    for (int i = 0; i < res.dim; i++) {
        res.data[i] = 1;
    }
    filter(&res);
    return res;
}

fmatrix ones (int rows, int cols) {
    fmatrix res;
    if (!(rows > 0 && cols > 0)) {
        std::raise(SIGFPE);
    }
    resize(&res, rows, cols);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = 1;
        }
    }
    filter(&res);
    return res;
}

fvector ident (int dim, int idx) {
    fvector res;
    if (!(dim > 0 && idx >= 0 && idx < dim)) {
        std::raise(SIGFPE);
    }
    resize(&res, dim);
    res.data[idx] = 1;
    filter(&res);
    return res;
}

fmatrix ident (int dim) {
    fmatrix res;
    if (!(dim > 0)) {
        std::raise(SIGFPE);
    }
    resize(&res, dim, dim);
    for (int i = 0; i < res.rows; i++) {
        res.data[i][i] = 1;
    }
    filter(&res);
    return res;
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
    filter(&res);
    return res;
}

double trac (const fmatrix & mat) {
    double res = 0;
    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    } else if (!(mat.rows == mat.cols)) {
        std::raise(SIGFPE);
    }
    for (int i = 0; i < mat.rows; i++) {
        res += mat.data[i][i];
    }
    filter(&res);
    return res;
}

double det (const fmatrix & mat) {
    double res = 0;
    fvector diag;
    int sign;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    } else if (!(mat.rows == mat.cols)) {
        std::raise(SIGFPE);
    }

    resize(&diag, mat.rows);
    lapack::dgetrf(mat.data, &(diag.data), &sign);
    filter(&diag);

    res = sign;
    for (int i = 0; i < diag.dim; i++) {
        res *= diag.data[i];
    }
    filter(&res);

    return res;
}

fmatrix tran (const fmatrix & mat) {
    fmatrix res;
    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }
    resize(&res, mat.cols, mat.rows);
    for (int i = 0; i < res.rows; i++) {
        for (int j = 0; j < res.cols; j++) {
            res.data[i][j] = mat.data[j][i];
        }
    }
    filter(&res);
    return res;
}

fmatrix inv (const fmatrix & mat) {
    fmatrix res;
    std::vector<std::vector<double>> fact;
    std::vector<int> pivot;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    } else if (!(mat.rows == mat.cols)) {
        std::raise(SIGFPE);
    } else if (!(is_neq(det(mat), 0))) {
        std::raise(SIGFPE);
    }

    resize(&res, mat.rows, mat.cols);
    lapack::dgetrf(mat.data, &fact, &pivot);
    lapack::dgetri(fact, pivot, &(res.data));
    filter(&res);

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
            if (is_neq(mat.data[i][j], mat.data[j][i])) {
                return false;
            }
        }
    }

    return true;
}

bool is_sym_pdf (const fmatrix & mat) {
    fmatrix sym;
    fvector eig;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }

    if (!is_sym(mat)) {
        return false;
    }

    resize(&sym, mat.rows, mat.cols);
    resize(&eig, mat.rows);

    for (int i = 0; i < sym.rows; i++) {
        for (int j = 0; j < sym.cols; j++) {
            sym.data[i][j] = (mat.data[i][j] + mat.data[j][i]) / 2;
        }
    }

    lapack::dsyev(sym.data, &(eig.data));
    filter(&eig);

    for (int i = 0; i < eig.dim; i++) {
        if (is_leq(eig.data[i], 0)) {
            return false;
        }
    }

    return true;
}

bool is_sym_ndf (const fmatrix & mat) {
    fmatrix sym;
    fvector eig;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }

    if (!is_sym(mat)) {
        return false;
    }

    resize(&sym, mat.rows, mat.cols);
    resize(&eig, mat.rows);

    for (int i = 0; i < sym.rows; i++) {
        for (int j = 0; j < sym.cols; j++) {
            sym.data[i][j] = (mat.data[i][j] + mat.data[j][i]) / 2;
        }
    }

    lapack::dsyev(sym.data, &(eig.data));
    filter(&eig);

    for (int i = 0; i < eig.dim; i++) {
        if (is_geq(eig.data[i], 0)) {
            return false;
        }
    }

    return true;
}

bool is_sym_psdf (const fmatrix & mat) {
    fmatrix sym;
    fvector eig;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }

    if (!is_sym(mat)) {
        return false;
    }

    resize(&sym, mat.rows, mat.cols);
    resize(&eig, mat.rows);

    for (int i = 0; i < sym.rows; i++) {
        for (int j = 0; j < sym.cols; j++) {
            sym.data[i][j] = (mat.data[i][j] + mat.data[j][i]) / 2;
        }
    }

    lapack::dsyev(sym.data, &(eig.data));
    filter(&eig);

    for (int i = 0; i < eig.dim; i++) {
        if (is_lneq(eig.data[i], 0)) {
            return false;
        }
    }

    return true;
}

bool is_sym_nsdf (const fmatrix & mat) {
    fmatrix sym;
    fvector eig;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }

    if (!is_sym(mat)) {
        return false;
    }

    resize(&sym, mat.rows, mat.cols);
    resize(&eig, mat.rows);

    for (int i = 0; i < sym.rows; i++) {
        for (int j = 0; j < sym.cols; j++) {
            sym.data[i][j] = (mat.data[i][j] + mat.data[j][i]) / 2;
        }
    }

    lapack::dsyev(sym.data, &(eig.data));
    filter(&eig);

    for (int i = 0; i < eig.dim; i++) {
        if (is_gneq(eig.data[i], 0)) {
            return false;
        }
    }

    return true;
}

void plu (const fmatrix & mat, fmatrix * matp, fmatrix * matl, fmatrix * matu) {
    int rows = mat.rows, cols = mat.cols, comm;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }

    if (rows < cols) {
        comm = rows;
    } else {
        comm = cols;
    }

    resize(matp, rows, rows);
    resize(matl, rows, comm);
    resize(matu, comm, cols);

    lapack::dgetrf(mat.data, &(matp->data), &(matl->data), &(matu->data));

    filter(matp);
    filter(matl);
    filter(matu);
}

void qr (const fmatrix & mat, fmatrix * matq, fmatrix * matr) {
    int rows = mat.rows, cols = mat.cols, comm;
    std::vector<std::vector<double>> fact;
    std::vector<double> coef;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }

    if (rows < cols) {
        comm = rows;
    } else {
        comm = cols;
    }

    resize(matq, rows, comm);
    resize(matr, comm, cols);

    lapack::dgeqrf(mat.data, &fact, &coef);
    lapack::dorgqr(fact, coef, &(matq->data), &(matr->data));

    filter(matq);
    filter(matr);
}

void rq (const fmatrix & mat, fmatrix * matr, fmatrix * matq) {
    int rows = mat.rows, cols = mat.cols, comm;
    std::vector<std::vector<double>> fact;
    std::vector<double> coef;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }

    if (rows < cols) {
        comm = rows;
    } else {
        comm = cols;
    }

    resize(matr, rows, comm);
    resize(matq, comm, cols);

    lapack::dgerqf(mat.data, &fact, &coef);
    lapack::dorgrq(fact, coef, &(matr->data), &(matq->data));

    filter(matr);
    filter(matq);
}

void ql (const fmatrix & mat, fmatrix * matq, fmatrix * matl) {
    int rows = mat.rows, cols = mat.cols, comm;
    std::vector<std::vector<double>> fact;
    std::vector<double> coef;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }

    if (rows < cols) {
        comm = rows;
    } else {
        comm = cols;
    }

    resize(matq, rows, comm);
    resize(matl, comm, cols);

    lapack::dgeqlf(mat.data, &fact, &coef);
    lapack::dorgql(fact, coef, &(matq->data), &(matl->data));

    filter(matq);
    filter(matl);
}

void lq (const fmatrix & mat, fmatrix * matl, fmatrix * matq) {
    int rows = mat.rows, cols = mat.cols, comm;
    std::vector<std::vector<double>> fact;
    std::vector<double> coef;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    }

    if (rows < cols) {
        comm = rows;
    } else {
        comm = cols;
    }

    resize(matl, rows, comm);
    resize(matq, comm, cols);

    lapack::dgelqf(mat.data, &fact, &coef);
    lapack::dorglq(fact, coef, &(matl->data), &(matq->data));

    filter(matl);
    filter(matq);
}

void qdq (const fmatrix & mat, fmatrix * matq, fmatrix * matd) {
    fmatrix sym;

    if (!is_finite(mat)) {
        std::raise(SIGFPE);
    } else if (!is_sym(mat)) {
        std::raise(SIGFPE);
    }

    resize(&sym, mat.rows, mat.cols);
    for (int i = 0; i < sym.rows; i++) {
        for (int j = 0; j < sym.cols; j++) {
            sym.data[i][j] = (mat.data[i][j] + mat.data[j][i]) / 2;
        }
    }

    resize(matq, sym.rows, sym.cols);
    resize(matd, sym.rows, sym.cols);

    lapack::dsyev(sym.data, &(matd->data), &(matq->data));

    filter(matq);
    filter(matd);
}

}

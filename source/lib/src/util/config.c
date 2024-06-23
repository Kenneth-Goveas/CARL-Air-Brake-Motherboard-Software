#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <regex.h>
#include <sys/types.h>

#include <util/log.h>
#include <util/config.h>

LOG_DECLARE_MODULE("config");

static char ** _all_name = NULL;
static int _all_count = 0;

static char ** _bool_name = NULL;
static bool * _bool_val = NULL;
static int _bool_count = 0;

static char ** _text_name = NULL;
static char ** _text_val = NULL;
static int _text_count = 0;

static char ** _int_scal_name = NULL;
static int * _int_scal_val = NULL;
static int _int_scal_count = 0;

static char ** _flt_scal_name = NULL;
static double * _flt_scal_val = NULL;
static int _flt_scal_count = 0;

static char ** _int_vect_name = NULL;
static int ** _int_vect_elem = NULL;
static int * _int_vect_len = NULL;
static int _int_vect_count = 0;

static char ** _flt_vect_name = NULL;
static double ** _flt_vect_elem = NULL;
static int * _flt_vect_len = NULL;
static int _flt_vect_count = 0;

static char ** _int_matr_name = NULL;
static int *** _int_matr_elem = NULL;
static int * _int_matr_rows = NULL;
static int * _int_matr_cols = NULL;
static int _int_matr_count = 0;

static char ** _flt_matr_name = NULL;
static double *** _flt_matr_elem = NULL;
static int * _flt_matr_rows = NULL;
static int * _flt_matr_cols = NULL;
static int _flt_matr_count = 0;

static void _str_free (char ** res) {
    free(*res);
    *res = NULL;
}

static void _str_assign (char ** res, const char * arg) {
    if (arg != NULL) {
        *res = (char *)realloc(*res, strlen(arg) + 1);
        strcpy(*res, arg);
    } else {
        free(*res);
        *res = NULL;
    }
}

static void _str_append (char ** res, char arg) {
    if (arg != '\0') {
        if (*res != NULL) {
            *res = (char *)realloc(*res, strlen(*res) + 2);
            (*res)[strlen(*res) + 1] = '\0';
            (*res)[strlen(*res)] = arg;
        } else {
            *res = (char *)malloc(2);
            (*res)[0] = arg;
            (*res)[1] = '\0';
        }
    }
}

static void _str_join (char ** res, const char * arg) {
    if (arg != NULL) {
        if (*res != NULL) {
            *res = (char *)realloc(*res, strlen(*res) + strlen(arg) + 1);
            strcat(*res, arg);
        } else {
            *res = (char *)malloc(strlen(arg) + 1);
            strcpy(*res, arg);
        }
    }
}

static void _str_slice (char ** res, const char * arg, size_t beg, size_t end) {
    if (arg != NULL) {
        if (end > strlen(arg)) {
            end = strlen(arg);
        }
        if (beg > end) {
            beg = end;
        }
        *res = (char *)realloc(*res, end - beg + 1);
        strncpy(*res, arg + beg, end - beg);
        (*res)[end - beg] = '\0';
    } else {
        free(*res);
        *res = NULL;
    }
}

static bool _arr_is_vect (const char * str) {
    int ret;
    char * pat = NULL;
    regex_t reg;

    _str_assign(
        &pat,
        "^"
        "\\["
            "("
                "[[:space:]]*"
                "[^][[:space:],]+"
                "[[:space:]]*"
            ",)*"
            "[[:space:]]*"
            "[^][[:space:],]+"
            "[[:space:]]*"
        "\\]"
        "$"
    );

    (void)regcomp(&reg, pat, REG_EXTENDED);

    ret = regexec(&reg, str, 0, NULL, 0);
    if (ret == REG_NOMATCH) {
        _str_free(&pat);
        regfree(&reg);
        return false;
    } else {
        _str_free(&pat);
        regfree(&reg);
        return true;
    }
}

static bool _arr_is_vect_with_len (const char * str, int len) {
    int ret;
    char * pat = NULL;
    regex_t reg;

    _str_join(&pat, "^");
    _str_join(&pat, "\\[");
    for (int i = 0; i < len; i++) {
        _str_join(&pat, "[[:space:]]*");
        _str_join(&pat, "[^][[:space:],]+");
        _str_join(&pat, "[[:space:]]*");
        if (i < len - 1) {
            _str_join(&pat, ",");
        }
    }
    _str_join(&pat, "\\]");
    _str_join(&pat, "$");

    (void)regcomp(&reg, pat, REG_EXTENDED);

    ret = regexec(&reg, str, 0, NULL, 0);
    if (ret == REG_NOMATCH) {
        _str_free(&pat);
        regfree(&reg);
        return false;
    } else {
        _str_free(&pat);
        regfree(&reg);
        return true;
    }
}

static int _arr_get_vect_dim (const char * str, int * len) {
    if (!_arr_is_vect(str)) {
        return -1;
    }

    *len = 1;
    while (!_arr_is_vect_with_len(str, *len)) {
        (*len)++;
    }

    return 0;
}

static int _arr_parse_vect_int (const char * str, int len, int * elem) {
    int ret, count;
    char * pat = NULL;
    regmatch_t * match = NULL;
    regex_t reg;
    char * slice = NULL;

    _str_join(&pat, "^");
    _str_join(&pat, "\\[");
    for (int i = 0; i < len; i++) {
        _str_join(&pat, "[[:space:]]*");
        _str_join(&pat, "([^][[:space:],]+)");
        _str_join(&pat, "[[:space:]]*");
        if (i < len - 1) {
            _str_join(&pat, ",");
        }
    }
    _str_join(&pat, "\\]");
    _str_join(&pat, "$");

    match = (regmatch_t *)malloc((len + 1) * sizeof(regmatch_t));
    (void)regcomp(&reg, pat, REG_EXTENDED);
    (void)regexec(&reg, str, len + 1, match, 0);

    for (int i = 0; i < len; i++) {
        _str_slice(&slice, str, match[i + 1].rm_so, match[i + 1].rm_eo);
        ret = sscanf(slice, "%d%n", elem + i, &count);
        if (ret != 1) {
            _str_free(&slice);
            _str_free(&pat);
            free(match);
            regfree(&reg);
            return -1;
        } else if (count != strlen(slice)) {
            _str_free(&slice);
            _str_free(&pat);
            free(match);
            regfree(&reg);
            return -1;
        }
    }

    _str_free(&slice);
    _str_free(&pat);
    free(match);
    regfree(&reg);

    return 0;
}

static int _arr_parse_vect_flt (const char * str, int len, double * elem) {
    int ret, count;
    char * pat = NULL;
    regmatch_t * match = NULL;
    regex_t reg;
    char * slice = NULL;

    _str_join(&pat, "^");
    _str_join(&pat, "\\[");
    for (int i = 0; i < len; i++) {
        _str_join(&pat, "[[:space:]]*");
        _str_join(&pat, "([^][[:space:],]+)");
        _str_join(&pat, "[[:space:]]*");
        if (i < len - 1) {
            _str_join(&pat, ",");
        }
    }
    _str_join(&pat, "\\]");
    _str_join(&pat, "$");

    match = (regmatch_t *)malloc((len + 1) * sizeof(regmatch_t));
    (void)regcomp(&reg, pat, REG_EXTENDED);
    (void)regexec(&reg, str, len + 1, match, 0);

    for (int i = 0; i < len; i++) {
        _str_slice(&slice, str, match[i + 1].rm_so, match[i + 1].rm_eo);
        ret = sscanf(slice, "%lf%n", elem + i, &count);
        if (ret != 1) {
            _str_free(&slice);
            _str_free(&pat);
            free(match);
            regfree(&reg);
            return -1;
        } else if (count != strlen(slice)) {
            _str_free(&slice);
            _str_free(&pat);
            free(match);
            regfree(&reg);
            return -1;
        }
    }

    _str_free(&slice);
    _str_free(&pat);
    free(match);
    regfree(&reg);

    return 0;
}

static bool _arr_is_matr (const char * str) {
    int ret;
    char * pat = NULL;
    regex_t reg;

    _str_assign(
        &pat,
        "^"
        "\\["
            "("
                "[[:space:]]*"
                "\\["
                    "("
                        "[[:space:]]*"
                        "[^][[:space:],]+"
                        "[[:space:]]*"
                    ",)*"
                    "[[:space:]]*"
                    "[^][[:space:],]+"
                    "[[:space:]]*"
                "\\]"
                "[[:space:]]*"
            ",)*"
            "[[:space:]]*"
            "\\["
                "("
                    "[[:space:]]*"
                    "[^][[:space:],]+"
                    "[[:space:]]*"
                ",)*"
                "[[:space:]]*"
                "[^][[:space:],]+"
                "[[:space:]]*"
            "\\]"
            "[[:space:]]*"
        "\\]"
        "$"
    );

    (void)regcomp(&reg, pat, REG_EXTENDED);

    ret = regexec(&reg, str, 0, NULL, 0);
    if (ret == REG_NOMATCH) {
        _str_free(&pat);
        regfree(&reg);
        return false;
    } else {
        _str_free(&pat);
        regfree(&reg);
        return true;
    }
}

static bool _arr_is_matr_with_rows (const char * str, int rows) {
    int ret;
    char * pat = NULL;
    regex_t reg;

    _str_join(&pat, "^");
    _str_join(&pat, "\\[");
    for (int i = 0; i < rows; i++) {
        _str_join(&pat, "[[:space:]]*");
        _str_join(
            &pat,
            "\\["
                "("
                    "[[:space:]]*"
                    "[^][[:space:],]+"
                    "[[:space:]]*"
                ",)*"
                "[[:space:]]*"
                "[^][[:space:],]+"
                "[[:space:]]*"
            "\\]"
        );
        _str_join(&pat, "[[:space:]]*");
        if (i < rows - 1) {
            _str_join(&pat, ",");
        }
    }
    _str_join(&pat, "\\]");
    _str_join(&pat, "$");

    (void)regcomp(&reg, pat, REG_EXTENDED);

    ret = regexec(&reg, str, 0, NULL, 0);
    if (ret == REG_NOMATCH) {
        _str_free(&pat);
        regfree(&reg);
        return false;
    } else {
        _str_free(&pat);
        regfree(&reg);
        return true;
    }
}

static bool _arr_is_matr_with_rows_and_cols (
    const char * str, int rows, int cols
) {
    int ret;
    char * pat = NULL;
    regex_t reg;

    _str_join(&pat, "^");
    _str_join(&pat, "\\[");
    for (int i = 0; i < rows; i++) {
        _str_join(&pat, "[[:space:]]*");
        _str_join(&pat, "\\[");
        for (int j = 0; j < cols; j++) {
            _str_join(&pat, "[[:space:]]*");
            _str_join(&pat, "[^][[:space:],]+");
            _str_join(&pat, "[[:space:]]*");
            if (j < cols - 1) {
                _str_join(&pat, ",");
            }
        }
        _str_join(&pat, "\\]");
        _str_join(&pat, "[[:space:]]*");
        if (i < rows - 1) {
            _str_join(&pat, ",");
        }
    }
    _str_join(&pat, "\\]");
    _str_join(&pat, "$");

    (void)regcomp(&reg, pat, REG_EXTENDED);

    ret = regexec(&reg, str, 0, NULL, 0);
    if (ret == REG_NOMATCH) {
        _str_free(&pat);
        regfree(&reg);
        return false;
    } else {
        _str_free(&pat);
        regfree(&reg);
        return true;
    }
}

static int _arr_get_matr_dim (const char * str, int * rows, int * cols) {
    int ret;
    char * pat = NULL;
    regmatch_t * match = NULL;
    regex_t reg;
    char * slice = NULL;

    if (!_arr_is_matr(str)) {
        return -1;
    }

    *rows = 1;
    while (!_arr_is_matr_with_rows(str, *rows)) {
        (*rows)++;
    }

    _str_join(&pat, "^");
    _str_join(&pat, "\\[");
    for (int i = 0; i < *rows; i++) {
        _str_join(&pat, "[[:space:]]*");
        _str_join(&pat, "(\\[[^][]+\\])");
        _str_join(&pat, "[[:space:]]*");
        if (i < *rows - 1) {
            _str_join(&pat, ",");
        }
    }
    _str_join(&pat, "\\]");
    _str_join(&pat, "$");

    match = (regmatch_t *)malloc((*rows + 1) * sizeof(regmatch_t));
    (void)regcomp(&reg, pat, REG_EXTENDED);
    (void)regexec(&reg, str, *rows + 1, match, 0);

    _str_slice(&slice, str, match[1].rm_so, match[1].rm_eo);
    (void)_arr_get_vect_dim(slice, cols);

    _str_free(&slice);
    _str_free(&pat);
    free(match);
    regfree(&reg);

    if (!_arr_is_matr_with_rows_and_cols(str, *rows, *cols)) {
        return -1;
    }

    return 0;
}

static int _arr_parse_matr_int (
    const char * str, int rows, int cols, int ** elem
) {
    int ret, count;
    char * pat = NULL;
    regmatch_t * match = NULL;
    regex_t reg;
    char * slice = NULL;

    _str_join(&pat, "^");
    _str_join(&pat, "\\[");
    for (int i = 0; i < rows; i++) {
        _str_join(&pat, "[[:space:]]*");
        _str_join(&pat, "\\[");
        for (int j = 0; j < cols; j++) {
            _str_join(&pat, "[[:space:]]*");
            _str_join(&pat, "([^][[:space:],]+)");
            _str_join(&pat, "[[:space:]]*");
            if (j < cols - 1) {
                _str_join(&pat, ",");
            }
        }
        _str_join(&pat, "\\]");
        _str_join(&pat, "[[:space:]]*");
        if (i < rows - 1) {
            _str_join(&pat, ",");
        }
    }
    _str_join(&pat, "\\]");
    _str_join(&pat, "$");

    match = (regmatch_t *)malloc((rows * cols + 1) * sizeof(regmatch_t));
    (void)regcomp(&reg, pat, REG_EXTENDED);
    (void)regexec(&reg, str, rows * cols + 1, match, 0);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            _str_slice(
                &slice, str,
                match[i * cols + j + 1].rm_so, match[i * cols + j + 1].rm_eo
            );
            ret = sscanf(slice, "%d%n", elem[i] + j, &count);
            if (ret != 1) {
                _str_free(&slice);
                _str_free(&pat);
                free(match);
                regfree(&reg);
                return -1;
            } else if (count != strlen(slice)) {
                _str_free(&slice);
                _str_free(&pat);
                free(match);
                regfree(&reg);
                return -1;
            }
        }
    }

    _str_free(&slice);
    _str_free(&pat);
    free(match);
    regfree(&reg);

    return 0;
}

static int _arr_parse_matr_flt (
    const char * str, int rows, int cols, double ** elem
) {
    int ret, count;
    char * pat = NULL;
    regmatch_t * match = NULL;
    regex_t reg;
    char * slice = NULL;

    _str_join(&pat, "^");
    _str_join(&pat, "\\[");
    for (int i = 0; i < rows; i++) {
        _str_join(&pat, "[[:space:]]*");
        _str_join(&pat, "\\[");
        for (int j = 0; j < cols; j++) {
            _str_join(&pat, "[[:space:]]*");
            _str_join(&pat, "([^][[:space:],]+)");
            _str_join(&pat, "[[:space:]]*");
            if (j < cols - 1) {
                _str_join(&pat, ",");
            }
        }
        _str_join(&pat, "\\]");
        _str_join(&pat, "[[:space:]]*");
        if (i < rows - 1) {
            _str_join(&pat, ",");
        }
    }
    _str_join(&pat, "\\]");
    _str_join(&pat, "$");

    match = (regmatch_t *)malloc((rows * cols + 1) * sizeof(regmatch_t));
    (void)regcomp(&reg, pat, REG_EXTENDED);
    (void)regexec(&reg, str, rows * cols + 1, match, 0);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            _str_slice(
                &slice, str,
                match[i * cols + j + 1].rm_so, match[i * cols + j + 1].rm_eo
            );
            ret = sscanf(slice, "%lf%n", elem[i] + j, &count);
            if (ret != 1) {
                _str_free(&slice);
                _str_free(&pat);
                free(match);
                regfree(&reg);
                return -1;
            } else if (count != strlen(slice)) {
                _str_free(&slice);
                _str_free(&pat);
                free(match);
                regfree(&reg);
                return -1;
            }
        }
    }

    _str_free(&slice);
    _str_free(&pat);
    free(match);
    regfree(&reg);

    return 0;
}

static int _conv_str_to_bool (bool * val, const char * str) {
    if (strcmp(str, "true") == 0) {
        *val = true;
        return 0;
    } else if (strcmp(str, "false") == 0) {
        *val = false;
        return 0;
    } else {
        return -1;
    }
}

static void _conv_bool_to_str (char ** str, bool val) {
    if (val) {
        _str_assign(str, "True");
    } else {
        _str_assign(str, "False");
    }
}

static int _conv_str_to_text (char ** val, const char * str) {
    bool escap = false;

    _str_free(val);
    if (str[0] != '"' || str[strlen(str) - 1] != '"') {
        return -1;
    }

    for (size_t i = 1; i < strlen(str) - 1; i++) {
        if (escap) {
            escap = false;
            if (str[i] == '"' || str[i] == '\\') {
                _str_append(val, str[i]);
            } else {
                _str_free(val);
                return -1;
            }
        } else {
            if (str[i] == '\\') {
                if (i < strlen(str) - 2) {
                    escap = true;
                } else {
                    _str_free(val);
                    return -1;
                }
            } else if (str[i] == '"') {
                _str_free(val);
                return -1;
            } else {
                _str_append(val, str[i]);
            }
        }
    }

    return 0;
}

static void _conv_text_to_str (char ** str, const char * val) {
    _str_free(str);
    _str_join(str, "“");
    _str_join(str, val);
    _str_join(str, "”");
}

static int _conv_str_to_int_scal (int * val, const char * str) {
    int ret, count;

    ret = sscanf(str, "%d%n", val, &count);
    if (ret != 1) {
        return -1;
    } else if (count != strlen(str)) {
        return -1;
    }

    return 0;
}

static void _conv_int_scal_to_str (char ** str, int val) {
    _str_free(str);
    asprintf(str, "%d", val);
}

static int _conv_str_to_flt_scal (double * val, const char * str) {
    int ret, count;

    ret = sscanf(str, "%lf%n", val, &count);
    if (ret != 1) {
        return -1;
    } else if (count != strlen(str)) {
        return -1;
    }

    return 0;
}

static void _conv_flt_scal_to_str (char ** str, double val) {
    _str_free(str);
    asprintf(str, "%.3e", val);
}

static int _conv_str_to_int_vect (int ** elem, int * len, const char * str) {
    int ret;

    ret = _arr_get_vect_dim(str, len);
    if (ret < 0) {
        return -1;
    }

    *elem = (int *)malloc((*len) * sizeof(int));
    ret = _arr_parse_vect_int(str, *len, *elem);
    if (ret < 0) {
        free(*elem);
        return -1;
    }

    return 0;
}

static void _conv_int_vect_to_str (char ** str, const int * elem, int len) {
    char * elem_str = NULL;

    _str_free(str);
    _str_join(str, "[");
    for (int i = 0; i < len; i++) {
        asprintf(&elem_str, "%d", elem[i]);
        _str_join(str, elem_str);
        free(elem_str);
        if (i < len - 1) {
            _str_join(str, ", ");
        }
    }
    _str_join(str, "]");
}

static int _conv_str_to_flt_vect (double ** elem, int * len, const char * str) {
    int ret;

    ret = _arr_get_vect_dim(str, len);
    if (ret < 0) {
        return -1;
    }

    *elem = (double *)malloc((*len) * sizeof(double));
    ret = _arr_parse_vect_flt(str, *len, *elem);
    if (ret < 0) {
        free(*elem);
        return -1;
    }

    return 0;
}

static void _conv_flt_vect_to_str (char ** str, const double * elem, int len) {
    char * elem_str = NULL;

    _str_free(str);
    _str_join(str, "[");
    for (int i = 0; i < len; i++) {
        asprintf(&elem_str, "%.3e", elem[i]);
        _str_join(str, elem_str);
        free(elem_str);
        if (i < len - 1) {
            _str_join(str, ", ");
        }
    }
    _str_join(str, "]");
}

static int _conv_str_to_int_matr (
    int *** elem, int * rows, int * cols, const char * str
) {
    int ret;

    ret = _arr_get_matr_dim(str, rows, cols);
    if (ret < 0) {
        return -1;
    }

    *elem = (int **)malloc((*rows) * sizeof(int *));
    for (int i = 0; i < *rows; i++) {
        (*elem)[i] = (int *)malloc((*cols) * sizeof(int));
    }

    ret = _arr_parse_matr_int(str, *rows, *cols, *elem);
    if (ret < 0) {
        for (int i = 0; i < *rows; i++) {
            free((*elem)[i]);
        }
        free(*elem);
        return -1;
    }

    return 0;
}

static void _conv_int_matr_to_str (
    char ** str, int ** elem, int rows, int cols
) {
    char * elem_str = NULL;

    _str_free(str);
    _str_join(str, "[");
    for (int i = 0; i < rows; i++) {
        _str_join(str, "[");
        for (int j = 0; j < cols; j++) {
            asprintf(&elem_str, "%d", elem[i][j]);
            _str_join(str, elem_str);
            free(elem_str);
            if (j < cols - 1) {
                _str_join(str, ", ");
            }
        }
        _str_join(str, "]");
        if (i < rows - 1) {
            _str_join(str, ", ");
        }
    }
    _str_join(str, "]");
}

static int _conv_str_to_flt_matr (
    double *** elem, int * rows, int * cols, const char * str
) {
    int ret;

    ret = _arr_get_matr_dim(str, rows, cols);
    if (ret < 0) {
        return -1;
    }

    *elem = (double **)malloc((*rows) * sizeof(double *));
    for (int i = 0; i < *rows; i++) {
        (*elem)[i] = (double *)malloc((*cols) * sizeof(double));
    }

    ret = _arr_parse_matr_flt(str, *rows, *cols, *elem);
    if (ret < 0) {
        for (int i = 0; i < *rows; i++) {
            free((*elem)[i]);
        }
        free(*elem);
        return -1;
    }

    return 0;
}

static void _conv_flt_matr_to_str (
    char ** str, double ** elem, int rows, int cols
) {
    char * elem_str = NULL;

    _str_free(str);
    _str_join(str, "[");
    for (int i = 0; i < rows; i++) {
        _str_join(str, "[");
        for (int j = 0; j < cols; j++) {
            asprintf(&elem_str, "%.3e", elem[i][j]);
            _str_join(str, elem_str);
            free(elem_str);
            if (j < cols - 1) {
                _str_join(str, ", ");
            }
        }
        _str_join(str, "]");
        if (i < rows - 1) {
            _str_join(str, ", ");
        }
    }
    _str_join(str, "]");
}

static void _name_add (const char * name) {
    _all_count++;

    _all_name = (char **)realloc(_all_name, _all_count * sizeof(char *));

    _all_name[_all_count - 1] = NULL;
    _str_assign(_all_name + _all_count - 1, name);
}

static void _name_free (void) {
    for (int i = 0; i < _all_count; i++) {
        free(_all_name[i]);
    }
    free(_all_name);

    _all_name = NULL;
    _all_count = 0;
}

static void _bool_add (const char * name, bool val) {
    _bool_count++;

    _bool_name = (char **)realloc(_bool_name, _bool_count * sizeof(char *));
    _bool_val = (bool *)realloc(_bool_val, _bool_count * sizeof(bool));

    _bool_name[_bool_count - 1] = NULL;
    _str_assign(_bool_name + _bool_count - 1, name);

    _bool_val[_bool_count - 1] = val;
}

static void _bool_free (void) {
    for (int i = 0; i < _bool_count; i++) {
        free(_bool_name[i]);
    }
    free(_bool_name);
    free(_bool_val);

    _bool_name = NULL;
    _bool_val = NULL;
    _bool_count = 0;
}

static void _text_add (const char * name, const char * val) {
    _text_count++;

    _text_name = (char **)realloc(_text_name, _text_count * sizeof(char *));
    _text_val = (char **)realloc(_text_val, _text_count * sizeof(char *));

    _text_name[_text_count - 1] = NULL;
    _str_assign(_text_name + _text_count - 1, name);

    _text_val[_text_count - 1] = NULL;
    _str_assign(_text_val + _text_count - 1, val);
}

static void _text_free (void) {
    for (int i = 0; i < _text_count; i++) {
        free(_text_name[i]);
        free(_text_val[i]);
    }
    free(_text_name);
    free(_text_val);

    _text_name = NULL;
    _text_val = NULL;
    _text_count = 0;
}

static void _int_scal_add (const char * name, int val) {
    _int_scal_count++;

    _int_scal_name = (char **)realloc(
        _int_scal_name, _int_scal_count * sizeof(char *)
    );
    _int_scal_val = (int *)realloc(
        _int_scal_val, _int_scal_count * sizeof(int)
    );

    _int_scal_name[_int_scal_count - 1] = NULL;
    _str_assign(_int_scal_name + _int_scal_count - 1, name);

    _int_scal_val[_int_scal_count - 1] = val;
}

static void _int_scal_free (void) {
    for (int i = 0; i < _int_scal_count; i++) {
        free(_int_scal_name[i]);
    }
    free(_int_scal_name);
    free(_int_scal_val);

    _int_scal_name = NULL;
    _int_scal_val = NULL;
    _int_scal_count = 0;
}

static void _flt_scal_add (const char * name, double val) {
    _flt_scal_count++;

    _flt_scal_name = (char **)realloc(
        _flt_scal_name, _flt_scal_count * sizeof(char *)
    );
    _flt_scal_val = (double *)realloc(
        _flt_scal_val, _flt_scal_count * sizeof(double)
    );

    _flt_scal_name[_flt_scal_count - 1] = NULL;
    _str_assign(_flt_scal_name + _flt_scal_count - 1, name);

    _flt_scal_val[_flt_scal_count - 1] = val;
}

static void _flt_scal_free (void) {
    for (int i = 0; i < _flt_scal_count; i++) {
        free(_flt_scal_name[i]);
    }
    free(_flt_scal_name);
    free(_flt_scal_val);

    _flt_scal_name = NULL;
    _flt_scal_val = NULL;
    _flt_scal_count = 0;
}

static void _int_vect_add (const char * name, const int * elem, int len) {
    _int_vect_count++;

    _int_vect_name = (char **)realloc(
        _int_vect_name, _int_vect_count * sizeof(char *)
    );
    _int_vect_elem = (int **)realloc(
        _int_vect_elem, _int_vect_count * sizeof(int *)
    );
    _int_vect_len = (int *)realloc(
        _int_vect_len, _int_vect_count * sizeof(int)
    );

    _int_vect_name[_int_vect_count - 1] = NULL;
    _str_assign(_int_vect_name + _int_vect_count - 1, name);

    _int_vect_elem[_int_vect_count - 1] = (int *)malloc(len * sizeof(int));
    for (int i = 0; i < len; i++) {
        _int_vect_elem[_int_vect_count - 1][i] = elem[i];
    }

    _int_vect_len[_int_vect_count - 1] = len;
}

static void _int_vect_free (void) {
    for (int i = 0; i < _int_vect_count; i++) {
        free(_int_vect_name[i]);
        free(_int_vect_elem[i]);
    }
    free(_int_vect_name);
    free(_int_vect_elem);
    free(_int_vect_len);

    _int_vect_name = NULL;
    _int_vect_elem = NULL;
    _int_vect_len = NULL;
    _int_vect_count = 0;
}

static void _flt_vect_add (const char * name, const double * elem, int len) {
    _flt_vect_count++;

    _flt_vect_name = (char **)realloc(
        _flt_vect_name, _flt_vect_count * sizeof(char *)
    );
    _flt_vect_elem = (double **)realloc(
        _flt_vect_elem, _flt_vect_count * sizeof(double *)
    );
    _flt_vect_len = (int *)realloc(
        _flt_vect_len, _flt_vect_count * sizeof(int)
    );

    _flt_vect_name[_flt_vect_count - 1] = NULL;
    _str_assign(_flt_vect_name + _flt_vect_count - 1, name);

    _flt_vect_elem[_flt_vect_count - 1] = (double *)malloc(
        len * sizeof(double)
    );
    for (int i = 0; i < len; i++) {
        _flt_vect_elem[_flt_vect_count - 1][i] = elem[i];
    }

    _flt_vect_len[_flt_vect_count - 1] = len;
}

static void _flt_vect_free (void) {
    for (int i = 0; i < _flt_vect_count; i++) {
        free(_flt_vect_name[i]);
        free(_flt_vect_elem[i]);
    }
    free(_flt_vect_name);
    free(_flt_vect_elem);
    free(_flt_vect_len);

    _flt_vect_name = NULL;
    _flt_vect_elem = NULL;
    _flt_vect_len = NULL;
    _flt_vect_count = 0;
}

static void _int_matr_add (const char * name, int ** elem, int rows, int cols) {
    _int_matr_count++;

    _int_matr_name = (char **)realloc(
        _int_matr_name, _int_matr_count * sizeof(char *)
    );
    _int_matr_elem = (int ***)realloc(
        _int_matr_elem, _int_matr_count * sizeof(int **)
    );
    _int_matr_rows = (int *)realloc(
        _int_matr_rows, _int_matr_count * sizeof(int)
    );
    _int_matr_cols = (int *)realloc(
        _int_matr_cols, _int_matr_count * sizeof(int)
    );

    _int_matr_name[_int_matr_count - 1] = NULL;
    _str_assign(_int_matr_name + _int_matr_count - 1, name);

    _int_matr_elem[_int_matr_count - 1] = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        _int_matr_elem[_int_matr_count - 1][i] = (int *)malloc(
            cols * sizeof(int)
        );
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            _int_matr_elem[_int_matr_count - 1][i][j] = elem[i][j];
        }
    }

    _int_matr_rows[_int_matr_count - 1] = rows;
    _int_matr_cols[_int_matr_count - 1] = cols;
}

static void _int_matr_free (void) {
    for (int i = 0; i < _int_matr_count; i++) {
        free(_int_matr_name[i]);
        for (int j = 0; j < _int_matr_rows[i]; j++) {
            free(_int_matr_elem[i][j]);
        }
        free(_int_matr_elem[i]);
    }
    free(_int_matr_name);
    free(_int_matr_elem);
    free(_int_matr_rows);
    free(_int_matr_cols);

    _int_matr_name = NULL;
    _int_matr_elem = NULL;
    _int_matr_rows = NULL;
    _int_matr_cols = NULL;
    _int_matr_count = 0;
}

static void _flt_matr_add (
    const char * name, double ** elem, int rows, int cols
) {
    _flt_matr_count++;

    _flt_matr_name = (char **)realloc(
        _flt_matr_name, _flt_matr_count * sizeof(char *)
    );
    _flt_matr_elem = (double ***)realloc(
        _flt_matr_elem, _flt_matr_count * sizeof(double **)
    );
    _flt_matr_rows = (int *)realloc(
        _flt_matr_rows, _flt_matr_count * sizeof(int)
    );
    _flt_matr_cols = (int *)realloc(
        _flt_matr_cols, _flt_matr_count * sizeof(int)
    );

    _flt_matr_name[_flt_matr_count - 1] = NULL;
    _str_assign(_flt_matr_name + _flt_matr_count - 1, name);

    _flt_matr_elem[_flt_matr_count - 1] = (double **)malloc(
        rows * sizeof(double *)
    );
    for (int i = 0; i < rows; i++) {
        _flt_matr_elem[_flt_matr_count - 1][i] = (double *)malloc(
            cols * sizeof(double)
        );
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            _flt_matr_elem[_flt_matr_count - 1][i][j] = elem[i][j];
        }
    }

    _flt_matr_rows[_flt_matr_count - 1] = rows;
    _flt_matr_cols[_flt_matr_count - 1] = cols;
}

static void _flt_matr_free (void) {
    for (int i = 0; i < _flt_matr_count; i++) {
        free(_flt_matr_name[i]);
        for (int j = 0; j < _flt_matr_rows[i]; j++) {
            free(_flt_matr_elem[i][j]);
        }
        free(_flt_matr_elem[i]);
    }
    free(_flt_matr_name);
    free(_flt_matr_elem);
    free(_flt_matr_rows);
    free(_flt_matr_cols);

    _flt_matr_name = NULL;
    _flt_matr_elem = NULL;
    _flt_matr_rows = NULL;
    _flt_matr_cols = NULL;
    _flt_matr_count = 0;
}

static void _parse_comment (char * line) {
    bool quot = false, escap = false;

    for (size_t i = 0; i < strlen(line); i++) {
        if (quot && escap) {
            escap = false;
        } else if (quot && !escap) {
            if (line[i] == '"') {
                quot = false;
            } else if (line[i] == '\\') {
                escap = true;
            }
        } else {
            if (line[i] == '"') {
                quot = true;
            } else if (line[i] == '#') {
                line[i] = '\0';
                break;
            }
        }
    }
}

static bool _parse_blank (const char * line) {
    for (size_t i = 0; i < strlen(line); i++) {
        if (line[i] != ' ' && line[i] != '\t') {
            return false;
        }
    }
    return true;
}

static int _parse_split (const char * line, char ** name, char ** val) {
    int ret;
    char * pat = NULL;
    regmatch_t * match = NULL;
    regex_t reg;

    _str_assign(
        &pat,
        "^"
        "[[:space:]]*([[:alpha:]][[:alnum:]_]*)[[:space:]]*"
        "="
        "[[:space:]]*([^[:space:]]|[^[:space:]].*[^[:space:]])[[:space:]]*"
        "$"
    );

    match = (regmatch_t *)malloc(3 * sizeof(regmatch_t));
    (void)regcomp(&reg, pat, REG_EXTENDED);

    ret = regexec(&reg, line, 3, match, 0);
    if (ret != 0) {
        _str_free(&pat);
        free(match);
        regfree(&reg);
        return -1;
    }

    _str_slice(name, line, match[1].rm_so, match[1].rm_eo);
    _str_slice(val, line, match[2].rm_so, match[2].rm_eo);

    _str_free(&pat);
    free(match);
    regfree(&reg);

    return 0;
}

static int _parse_name (const char * name) {
    for (int i = 0; i < _all_count; i++) {
        if (strcmp(name, _all_name[i]) == 0) {
            return -1;
        }
    }
    return 0;
}

static int _parse_bool (const char * name, char ** val) {
    int ret;
    bool val_bool;

    ret = _conv_str_to_bool(&val_bool, *val);
    if (ret < 0) {
        return -1;
    }

    _conv_bool_to_str(val, val_bool);

    _name_add(name);
    _bool_add(name, val_bool);

    return 0;
}

static int _parse_text (const char * name, char ** val) {
    int ret;
    char * val_text = NULL;

    ret = _conv_str_to_text(&val_text, *val);
    if (ret < 0) {
        return -1;
    }

    _conv_text_to_str(val, val_text);

    _name_add(name);
    _text_add(name, val_text);

    _str_free(&val_text);

    return 0;
}

static int _parse_int_scal (const char * name, char ** val) {
    int ret;
    int val_int_scal;

    ret = _conv_str_to_int_scal(&val_int_scal, *val);
    if (ret < 0) {
        return -1;
    }

    _conv_int_scal_to_str(val, val_int_scal);

    _name_add(name);
    _int_scal_add(name, val_int_scal);

    return 0;
}

static int _parse_flt_scal (const char * name, char ** val) {
    int ret;
    double val_flt_scal;

    ret = _conv_str_to_flt_scal(&val_flt_scal, *val);
    if (ret < 0) {
        return -1;
    }

    _conv_flt_scal_to_str(val, val_flt_scal);

    _name_add(name);
    _flt_scal_add(name, val_flt_scal);

    return 0;
}

static int _parse_int_vect (const char * name, char ** val) {
    int ret;
    int * elem_int_vect;
    int len_int_vect;

    ret = _conv_str_to_int_vect(&elem_int_vect, &len_int_vect, *val);
    if (ret < 0) {
        return -1;
    }

    _conv_int_vect_to_str(val, elem_int_vect, len_int_vect);

    _name_add(name);
    _int_vect_add(name, elem_int_vect, len_int_vect);

    free(elem_int_vect);

    return 0;
}

static int _parse_flt_vect (const char * name, char ** val) {
    int ret;
    double * elem_flt_vect;
    int len_flt_vect;

    ret = _conv_str_to_flt_vect(&elem_flt_vect, &len_flt_vect, *val);
    if (ret < 0) {
        return -1;
    }

    _conv_flt_vect_to_str(val, elem_flt_vect, len_flt_vect);

    _name_add(name);
    _flt_vect_add(name, elem_flt_vect, len_flt_vect);

    free(elem_flt_vect);

    return 0;
}

static int _parse_int_matr (const char * name, char ** val) {
    int ret;
    int ** elem_int_matr;
    int rows_int_matr, cols_int_matr;

    ret = _conv_str_to_int_matr(
        &elem_int_matr, &rows_int_matr, &cols_int_matr, *val
    );

    if (ret < 0) {
        return -1;
    }

    _conv_int_matr_to_str(val, elem_int_matr, rows_int_matr, cols_int_matr);

    _name_add(name);
    _int_matr_add(name, elem_int_matr, rows_int_matr, cols_int_matr);

    for (int i = 0; i < rows_int_matr; i++) {
        free(elem_int_matr[i]);
    }
    free(elem_int_matr);

    return 0;
}

static int _parse_flt_matr (const char * name, char ** val) {
    int ret;
    double ** elem_flt_matr;
    int rows_flt_matr, cols_flt_matr;

    ret = _conv_str_to_flt_matr(
        &elem_flt_matr, &rows_flt_matr, &cols_flt_matr, *val
    );

    if (ret < 0) {
        return -1;
    }

    _conv_flt_matr_to_str(val, elem_flt_matr, rows_flt_matr, cols_flt_matr);

    _name_add(name);
    _flt_matr_add(name, elem_flt_matr, rows_flt_matr, cols_flt_matr);

    for (int i = 0; i < rows_flt_matr; i++) {
        free(elem_flt_matr[i]);
    }
    free(elem_flt_matr);

    return 0;
}

static int _parse (char * line) {
    int ret;
    char * name = NULL, * val = NULL;

    if (line[strlen(line) - 1] == '\n') {
        line[strlen(line) - 1] = '\0';
    }

    LOG_INF("Parsing line: “%s”", line);

    _parse_comment(line);

    if (_parse_blank(line)) {
        LOG_INF("Parsed line as blank");
        return 0;
    }

    ret = _parse_split(line, &name, &val);
    if (ret < 0) {
        LOG_ERR("Failed to parse line (Syntax error)");
        return -1;
    }

    ret = _parse_name(name);
    if (ret < 0) {
        LOG_ERR("Failed to parse line (Repeated name)");
        _str_free(&name);
        _str_free(&val);
        return -1;
    }

    ret = _parse_bool(name, &val);
    if (ret == 0) {
        LOG_INF(
            "Parsed line as boolean parameter: "
            "Name: “%s”, Value: %s",
            name, val
        );
        _str_free(&name);
        _str_free(&val);
        return 0;
    }

    ret = _parse_text(name, &val);
    if (ret == 0) {
        LOG_INF(
            "Parsed line as text parameter: "
            "Name: “%s”, Value: %s",
            name, val
        );
        _str_free(&name);
        _str_free(&val);
        return 0;
    }

    ret = _parse_int_scal(name, &val);
    if (ret == 0) {
        LOG_INF(
            "Parsed line as integer scalar parameter: "
            "Name: “%s”, Value: %s",
            name, val
        );
        _str_free(&name);
        _str_free(&val);
        return 0;
    }

    ret = _parse_flt_scal(name, &val);
    if (ret == 0) {
        LOG_INF(
            "Parsed line as floating point scalar parameter: "
            "Name: “%s”, Value: %s",
            name, val
        );
        _str_free(&name);
        _str_free(&val);
        return 0;
    }

    ret = _parse_int_vect(name, &val);
    if (ret == 0) {
        LOG_INF(
            "Parsed line as integer vector parameter: "
            "Name: “%s”, Value: %s",
            name, val
        );
        _str_free(&name);
        _str_free(&val);
        return 0;
    }

    ret = _parse_flt_vect(name, &val);
    if (ret == 0) {
        LOG_INF(
            "Parsed line as floating point vector parameter: "
            "Name: “%s”, Value: %s",
            name, val
        );
        _str_free(&name);
        _str_free(&val);
        return 0;
    }

    ret = _parse_int_matr(name, &val);
    if (ret == 0) {
        LOG_INF(
            "Parsed line as integer matrix parameter: "
            "Name: “%s”, Value: %s",
            name, val
        );
        _str_free(&name);
        _str_free(&val);
        return 0;
    }

    ret = _parse_flt_matr(name, &val);
    if (ret == 0) {
        LOG_INF(
            "Parsed line as floating point matrix parameter: "
            "Name: “%s”, Value: %s",
            name, val
        );
        _str_free(&name);
        _str_free(&val);
        return 0;
    }

    LOG_ERR("Failed to parse line (Syntax error)");

    _str_free(&name);
    _str_free(&val);

    return -1;
}

int config_init (const char * path) {
    int ret_int;
    ssize_t ret_ssiz;
    FILE * file = NULL;
    char * line = NULL;
    size_t len = 0;

    LOG_INF("Opening config file: %s", path);

    file = fopen(path, "r");
    if (file == NULL) {
        LOG_ERR("Failed to open config file (%s)", strerror(errno));
        return -1;
    }

    LOG_INF("Parsing config file");

    errno = 0;
    while (true) {
        ret_ssiz = getline(&line, &len, file);

        if (ret_ssiz < 0 && errno == 0) {
            free(line);
            break;
        }

        if (ret_ssiz < 0 && errno != 0) {
            LOG_ERR("Failed to read config file (%s)", strerror(errno));

            LOG_WRN("Closing config file");
            ret_int = fclose(file);
            if (ret_int < 0) {
                LOG_ERR("Failed to close config file");
            }

            LOG_WRN("Clearing config data");

            _name_free();
            _bool_free();
            _text_free();
            _int_scal_free();
            _flt_scal_free();
            _int_vect_free();
            _flt_vect_free();
            _int_matr_free();
            _flt_matr_free();

            free(line);

            return -1;
        }

        ret_int = _parse(line);
        if (ret_int < 0) {
            LOG_ERR("Failed to parse config file");

            LOG_WRN("Closing config file");
            ret_int = fclose(file);
            if (ret_int < 0) {
                LOG_ERR("Failed to close config file");
            }

            LOG_WRN("Clearing config data");

            _name_free();
            _bool_free();
            _text_free();
            _int_scal_free();
            _flt_scal_free();
            _int_vect_free();
            _flt_vect_free();
            _int_matr_free();
            _flt_matr_free();

            free(line);

            return -1;
        }
    }

    LOG_INF("Closing config file");

    ret_int = fclose(file);
    if (ret_int == EOF) {
        LOG_ERR("Failed to close config file (%s)", strerror(errno));

        LOG_WRN("Clearing config data");

        _name_free();
        _bool_free();
        _text_free();
        _int_scal_free();
        _flt_scal_free();
        _int_vect_free();
        _flt_vect_free();
        _int_matr_free();
        _flt_matr_free();

        return -1;
    }

    return 0;
}

void config_deinit (void) {
    LOG_INF("Clearing config data");

    _name_free();
    _bool_free();
    _text_free();
    _int_scal_free();
    _flt_scal_free();
    _int_vect_free();
    _flt_vect_free();
    _int_matr_free();
    _flt_matr_free();
}

int config_get_bool (const char * name, bool * val) {
    char * val_str = NULL;

    LOG_INF("Getting boolean parameter: Name: “%s”", name);

    for (int i = 0; i < _bool_count; i++) {
        if (strcmp(_bool_name[i], name) == 0) {
            _conv_bool_to_str(&val_str, _bool_val[i]);
            LOG_INF("Got boolean parameter: Value: %s", val_str);
            _str_free(&val_str);

            *val = _bool_val[i];

            return 0;
        }
    }

    LOG_ERR("Failed to get boolean parameter (Name not found)");

    return -1;
}

int config_get_text (const char * name, char ** val) {
    char * val_str = NULL;

    LOG_INF("Getting text parameter: Name: “%s”", name);

    *val = NULL;
    for (int i = 0; i < _text_count; i++) {
        if (strcmp(_text_name[i], name) == 0) {
            _conv_text_to_str(&val_str, _text_val[i]);
            LOG_INF("Got text parameter: Value: %s", val_str);
            _str_free(&val_str);

            _str_assign(val, _text_val[i]);

            return 0;
        }
    }

    LOG_ERR("Failed to get text parameter (Name not found)");

    return -1;
}

int config_get_int_scal (const char * name, int * val) {
    char * val_str = NULL;

    LOG_INF("Getting integer scalar parameter: Name: “%s”", name);

    for (int i = 0; i < _int_scal_count; i++) {
        if (strcmp(_int_scal_name[i], name) == 0) {
            _conv_int_scal_to_str(&val_str, _int_scal_val[i]);
            LOG_INF("Got integer scalar parameter: Value: %s", val_str);
            _str_free(&val_str);

            *val = _int_scal_val[i];

            return 0;
        }
    }

    LOG_ERR("Failed to get integer scalar parameter (Name not found)");

    return -1;
}

int config_get_flt_scal (const char * name, double * val) {
    char * val_str = NULL;

    LOG_INF("Getting floating point scalar parameter: Name: “%s”", name);

    for (int i = 0; i < _flt_scal_count; i++) {
        if (strcmp(_flt_scal_name[i], name) == 0) {
            _conv_flt_scal_to_str(&val_str, _flt_scal_val[i]);
            LOG_INF("Got floating point scalar parameter: Value: %s", val_str);
            _str_free(&val_str);

            *val = _flt_scal_val[i];

            return 0;
        }
    }

    LOG_ERR("Failed to get floating point scalar parameter (Name not found)");

    return -1;
}

int config_get_int_vect (const char * name, int ** elem, int * len) {
    char * val_str = NULL;

    LOG_INF("Getting integer vector parameter: Name: “%s”", name);

    for (int i = 0; i < _int_vect_count; i++) {
        if (strcmp(_int_vect_name[i], name) == 0) {
            _conv_int_vect_to_str(
                &val_str, _int_vect_elem[i], _int_vect_len[i]
            );
            LOG_INF("Got integer vector parameter: Value: %s", val_str);
            _str_free(&val_str);

            *elem = (int *)malloc(_int_vect_len[i] * sizeof(int));
            for (int j = 0; j < _int_vect_len[i]; j++) {
                (*elem)[j] = _int_vect_elem[i][j];
            }
            *len = _int_vect_len[i];

            return 0;
        }
    }

    LOG_ERR("Failed to get integer vector parameter (Name not found)");

    return -1;
}

int config_get_flt_vect (const char * name, double ** elem, int * len) {
    char * val_str = NULL;

    LOG_INF("Getting floating point vector parameter: Name: “%s”", name);

    for (int i = 0; i < _flt_vect_count; i++) {
        if (strcmp(_flt_vect_name[i], name) == 0) {
            _conv_flt_vect_to_str(
                &val_str, _flt_vect_elem[i], _flt_vect_len[i]
            );
            LOG_INF("Got floating point vector parameter: Value: %s", val_str);
            _str_free(&val_str);

            *elem = (double *)malloc(_flt_vect_len[i] * sizeof(double));
            for (int j = 0; j < _flt_vect_len[i]; j++) {
                (*elem)[j] = _flt_vect_elem[i][j];
            }
            *len = _flt_vect_len[i];

            return 0;
        }
    }

    LOG_ERR("Failed to get floating point vector parameter (Name not found)");

    return -1;
}

int config_get_int_matr (
    const char * name, int *** elem, int * rows, int * cols
) {
    char * val_str = NULL;

    LOG_INF("Getting integer matrix parameter: Name: “%s”", name);

    for (int i = 0; i < _int_matr_count; i++) {
        if (strcmp(_int_matr_name[i], name) == 0) {
            _conv_int_matr_to_str(
                &val_str,
                _int_matr_elem[i], _int_matr_rows[i], _int_matr_cols[i]
            );
            LOG_INF("Got integer matrix parameter: Value: %s", val_str);
            _str_free(&val_str);

            *elem = (int **)malloc(_int_matr_rows[i] * sizeof(int *));
            for (int j = 0; j < _int_matr_rows[i]; j++) {
                (*elem)[j] = (int *)malloc(
                    _int_matr_cols[i] * sizeof(int)
                );
            }
            for (int j = 0; j < _int_matr_rows[i]; j++) {
                for (int k = 0; k < _int_matr_cols[i]; k++) {
                    (*elem)[j][k] = _int_matr_elem[i][j][k];
                }
            }
            *rows = _int_matr_rows[i];
            *cols = _int_matr_cols[i];

            return 0;
        }
    }

    LOG_ERR("Failed to get integer matrix parameter (Name not found)");

    return -1;
}

int config_get_flt_matr (
    const char * name, double *** elem, int * rows, int * cols
) {
    char * val_str = NULL;

    LOG_INF("Getting floating point matrix parameter: Name: “%s”", name);

    for (int i = 0; i < _flt_matr_count; i++) {
        if (strcmp(_flt_matr_name[i], name) == 0) {
            _conv_flt_matr_to_str(
                &val_str,
                _flt_matr_elem[i], _flt_matr_rows[i], _flt_matr_cols[i]
            );
            LOG_INF("Got floating point matrix parameter: Value: %s", val_str);
            _str_free(&val_str);

            *elem = (double **)malloc(_flt_matr_rows[i] * sizeof(double *));
            for (int j = 0; j < _flt_matr_rows[i]; j++) {
                (*elem)[j] = (double *)malloc(
                    _flt_matr_cols[i] * sizeof(double)
                );
            }
            for (int j = 0; j < _flt_matr_rows[i]; j++) {
                for (int k = 0; k < _flt_matr_cols[i]; k++) {
                    (*elem)[j][k] = _flt_matr_elem[i][j][k];
                }
            }
            *rows = _flt_matr_rows[i];
            *cols = _flt_matr_cols[i];

            return 0;
        }
    }

    LOG_ERR("Failed to get floating point matrix parameter (Name not found)");

    return -1;
}

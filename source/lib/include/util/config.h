#ifndef __UTIL_CONFIG_H__
#define __UTIL_CONFIG_H__

#include <stdbool.h>

int config_init (const char * path);
void config_deinit (void);
int config_get_bool (const char * name, bool * val);
int config_get_text (const char * name, char ** val);
int config_get_int_scal (const char * name, int * val);
int config_get_flt_scal (const char * name, double * val);
int config_get_int_vect (const char * name, int ** elem, int * len);
int config_get_flt_vect (const char * name, double ** elem, int * len);
int config_get_int_matr (
    const char * name, int *** elem, int * rows, int * cols
);
int config_get_flt_matr (
    const char * name, double *** elem, int * rows, int * cols
);

#endif

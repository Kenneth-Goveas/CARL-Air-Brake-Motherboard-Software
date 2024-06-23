#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <inttypes.h>
#include <time.h>
#include <sys/file.h>

#include <util/log.h>

extern const char * _log_path;
extern const char * _log_unit;

static FILE * _file;
static int _desc;

static void _log_msg (
    const char * lev, const char * mod, const char * fmt, va_list args
) {
    int ret;
    char * fmt_full;
    struct timespec tspec;
    uint8_t day, hour, min, sec;
    uint16_t msec, usec, nsec;

    ret = flock(_desc, LOCK_EX);
    if (ret < 0) {
        return;
    }

    ret = clock_gettime(CLOCK_BOOTTIME, &tspec);
    if (ret < 0) {
        asprintf(
            &fmt_full, "[??:??:??.???,???,???] <%s> %s/%s: %s\n",
            lev, _log_unit, mod, fmt
        );
    } else {
        day = tspec.tv_sec / 86400;
        hour = tspec.tv_sec / 3600 - 24 * day;
        min = tspec.tv_sec / 60 - 1440 * day - 60 * hour;
        sec = tspec.tv_sec - 86400 * day - 3600 * hour - 60 * min;

        msec = tspec.tv_nsec / 1000000;
        usec = tspec.tv_nsec / 1000 - 1000 * msec;
        nsec = tspec.tv_nsec - 1000000 * msec - 1000 * usec;

        asprintf(
            &fmt_full,
            "["
            "%02" PRIu8
            ":"
            "%02" PRIu8
            ":"
            "%02" PRIu8
            "."
            "%03" PRIu16
            ","
            "%03" PRIu16
            ","
            "%03" PRIu16
            "]"
            " <%s> %s/%s: %s\n",
            hour, min, sec, msec, usec, nsec, lev, _log_unit, mod, fmt
        );
    }

    vfprintf(_file, fmt_full, args);

    free(fmt_full);

    flock(_desc, LOCK_UN);
}

void _log_init (void) {
    _file = fopen(_log_path, "a");
    _desc = fileno(_file);
    setbuf(_file, NULL);
}

void _log_deinit (void) {
    fclose(_file);
}

void _log_err (const char * mod, const char * fmt, ...) {
    va_list args;

    va_start(args, fmt);
    _log_msg("err", mod, fmt, args);
    va_end(args);
}

void _log_wrn (const char * mod, const char * fmt, ...) {
    va_list args;

    va_start(args, fmt);
    _log_msg("wrn", mod, fmt, args);
    va_end(args);
}

void _log_inf (const char * mod, const char * fmt, ...) {
    va_list args;

    va_start(args, fmt);
    _log_msg("inf", mod, fmt, args);
    va_end(args);
}

#include <string.h>
#include <errno.h>
#include <time.h>

#include <util/log.h>

#include <dev/clock.h>

LOG_DECLARE_MODULE("clock");

static double _conv_tspec_to_time (struct timespec tspec) {
    double time;

    time = (double)tspec.tv_sec + 1e-9 * (double)tspec.tv_nsec;

    return time;
}

static struct timespec _conv_time_to_tspec (double time) {
    struct timespec tspec;

    tspec.tv_sec = (time_t)time;
    tspec.tv_nsec = (long)(1e9 * (time - (time_t)time));

    return tspec;
}

int clock_get_time (double * time) {
    int ret;
    struct timespec tspec;

    LOG_INF("Getting current time");

    ret = clock_gettime(CLOCK_BOOTTIME, &tspec);
    if (ret < 0) {
        LOG_ERR("Failed to get current time (%s)", strerror(errno));
        return -1;
    }

    *time = _conv_tspec_to_time(tspec);

    LOG_INF("Got current time: %.3es", *time);

    return 0;
}

int clock_sleep (double dur) {
    int ret;
    double rem;
    struct timespec tspec_dur, tspec_rem;

    tspec_dur = _conv_time_to_tspec(dur);

    LOG_INF("Entering sleep cycle: Dur: %.3es", dur);

    ret = nanosleep(&tspec_dur, &tspec_rem);
    if (ret < 0) {
        if (errno == EINTR) {
            rem = _conv_tspec_to_time(tspec_rem);
            LOG_WRN("Sleep cycle interrupted: Rem: %.3es", rem);
            return 0;
        } else {
            LOG_ERR("Failed to complete sleep cycle (%s)", strerror(errno));
            return -1;
        }
    }

    return 0;
}

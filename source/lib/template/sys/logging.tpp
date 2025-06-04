#include <string>

#include <sys/file.h>

namespace logging {

template <typename ... types>
void instance::inf (const types & ... args) {
    this->msg("inf", args ...);
}

template <typename ... types>
void instance::wrn (const types & ... args) {
    this->msg("wrn", args ...);
}

template <typename ... types>
void instance::err (const types & ... args) {
    this->msg("err", args ...);
}

template <typename type, typename ... types>
void instance::body (const type & arg, const types & ... args) {
    instance::strm << arg;
    this->body(args ...);
}

template <typename ... types>
void instance::msg (const std::string & lev, const types & ... args) {
    int ret;

    if (!instance::init) {
        return;
    }

    ret = flock(instance::desc, LOCK_EX);
    if (ret < 0) {
        return;
    }

    this->head(lev);
    this->body(args ...);

    flock(instance::desc, LOCK_UN);
}

}

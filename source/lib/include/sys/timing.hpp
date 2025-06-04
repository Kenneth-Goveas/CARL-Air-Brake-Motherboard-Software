#ifndef __SYS_TIMING_HPP__
#define __SYS_TIMING_HPP__

#include <sys/logging.hpp>

namespace timing {

class tracker {
    protected:
        static bool exist;
        static bool instantiate (void);
};

class instance : private tracker {
    public:
        instance (void);

        double get_time (void);

        void sleep (double dur);

        enum class except_ctor {fail};
        enum class except_get_time {fail};
        enum class except_sleep {fail};

    private:
        logging::instance logging;
        bool init;
};

}

#endif

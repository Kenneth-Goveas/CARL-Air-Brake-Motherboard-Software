#ifndef __SYS_INTERRUPT_HPP__
#define __SYS_INTERRUPT_HPP__

#include <sys/logging.hpp>

namespace interrupt {

class tracker {
    protected:
        static bool exist;
        static bool instantiate (void);
};

class instance : private tracker {
    public:
        instance (void);

        bool caught (void);

        enum class except_ctor {fail};
        enum class except_caught {fail};

    private:
        static bool flag;

        logging::instance logging;
        bool init;

        static void handler (int signum);
};

}

#endif

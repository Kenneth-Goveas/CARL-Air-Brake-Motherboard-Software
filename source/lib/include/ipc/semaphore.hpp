#ifndef __IPC_SEMAPHORE_HPP__
#define __IPC_SEMAPHORE_HPP__

#include <string>

#include <semaphore.h>

#include <sys/logging.hpp>

namespace semaphore {

enum class mode {none, create, remove, both};

class tracker {
    protected:
        static int count;
        static int instantiate (void);
};

class instance : private tracker {
    public:
        instance (const std::string & name, mode mode = mode::none);
        ~instance (void);

        void acquire (void);
        void release (void);

        enum class except_ctor {fail};
        enum class except_acquire {fail};
        enum class except_release {fail};

    private:
        logging::instance logging;
        bool init;
        int id;

        sem_t * sem;
        std::string name;
        bool locked;
        bool create, remove;
};

}

#endif

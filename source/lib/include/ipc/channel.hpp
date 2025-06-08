#ifndef __IPC_CHANNEL_HPP__
#define __IPC_CHANNEL_HPP__

#include <math/linalg.hpp>
#include <sys/logging.hpp>
#include <ipc/semaphore.hpp>
#include <ipc/sharedmem.hpp>

namespace channel {

enum class mode {none, create, remove, both};

class tracker {
    protected:
        static bool exist;
        static bool instantiate (void);
};

class instance : private tracker {
    public:
        instance (mode mode = mode::none);

        void put_ctrl (double ctrl);
        void put_stat (const linalg::fvector & stat);
        void put_estm (const linalg::fvector & estm);
        void put_sens (const linalg::fvector & sens);

        double get_ctrl (void);
        linalg::fvector get_stat (void);
        linalg::fvector get_estm (void);
        linalg::fvector get_sens (void);

        void clear (void);

        void acquire (void);
        void release (void);

        enum class except_ctor {fail};
        enum class except_put_ctrl {fail};
        enum class except_put_stat {fail};
        enum class except_put_estm {fail};
        enum class except_put_sens {fail};
        enum class except_get_ctrl {fail, blank};
        enum class except_get_stat {fail, blank};
        enum class except_get_estm {fail, blank};
        enum class except_get_sens {fail, blank};
        enum class except_clear {fail};
        enum class except_acquire {fail};
        enum class except_release {fail};

    private:
        logging::instance logging;
        bool init;

        bool locked;
        semaphore::instance acces;
        sharedmem::instance<bool> ctrl_drdy, stat_drdy, estm_drdy, sens_drdy;
        sharedmem::instance<double> ctrl_data;
        sharedmem::instance<linalg::fvector> stat_data, estm_data, sens_data;
};

}

#endif

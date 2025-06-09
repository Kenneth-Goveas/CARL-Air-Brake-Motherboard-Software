#ifndef __DEV_SERVO_HPP__
#define __DEV_SERVO_HPP__

#include <sys/logging.hpp>
#include <dev/i2c.hpp>

#define SERVO_ADDR  0x10

namespace servo {

class tracker {
    protected:
        static bool exist;
        static bool instantiate (void);
};

class instance : private tracker {
    public:
        instance (i2c::instance & i2c);

        void put_pos (double pos);

        enum class except_ctor {fail};
        enum class except_put_pos {fail};

    private:
        logging::instance logging;
        bool init;

        i2c::instance * i2c;
};

}

#endif

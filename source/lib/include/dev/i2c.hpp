#ifndef __DEV_I2C_HPP__
#define __DEV_I2C_HPP__

#include <cstdint>

#include <string>
#include <vector>

#include <sys/logging.hpp>

namespace i2c {

class tracker {
    protected:
        static bool exist;
        static bool instantiate (void);
};

class instance : private tracker {
    public:
        instance (const std::string & path);
        ~instance (void);

        void send (std::uint8_t addr, const std::vector<std::uint8_t> & data);
        void recv (std::uint8_t addr, std::vector<std::uint8_t> & data);
        void exec (void);

        enum class except_ctor {fail};
        enum class except_send {fail};
        enum class except_recv {fail};
        enum class except_exec {fail};

    private:
        logging::instance logging;
        bool init;

        int desc;
        std::vector<std::uint8_t> addr;
        std::vector<bool> send_flag;
        std::vector<bool> recv_flag;
        std::vector<std::vector<std::uint8_t>> send_data;
        std::vector<std::vector<std::uint8_t> *> recv_data;
};

}

#endif

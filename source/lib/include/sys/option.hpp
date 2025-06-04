#ifndef __SYS_OPTION_HPP__
#define __SYS_OPTION_HPP__

#include <string>
#include <vector>

#include <sys/logging.hpp>

namespace option {

class tracker {
    protected:
        static bool exist;
        static bool instantiate (void);
};

class instance : private tracker {
    public:
        instance (int argc, char ** argv, const std::vector<std::string> & names);

        bool get (const std::string & name);

        enum class except_ctor {fail};
        enum class except_get {fail};

    private:
        logging::instance logging;
        bool init;

        std::vector<std::string> name;
        std::vector<bool> val;
};

}

#endif

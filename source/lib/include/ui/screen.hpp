#ifndef __UI_SCREEN_HPP__
#define __UI_SCREEN_HPP__

#include <sys/logging.hpp>
#include <ui/terminal.hpp>

namespace screen {

class tracker {
    protected:
        static bool exist;
        static bool instantiate (void);
};

class instance : private tracker {
    public:
        instance (terminal::instance & terminal);
        ~instance (void);

        void erase (void);

        enum class except_ctor {fail};
        enum class except_erase {fail};

    private:
        logging::instance logging;
        bool init;

        terminal::instance * terminal;
};

}

#endif

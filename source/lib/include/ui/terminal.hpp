#ifndef __UI_TERMINAL_HPP__
#define __UI_TERMINAL_HPP__

#include <sys/logging.hpp>

namespace terminal {

enum class color;

class tracker {
    protected:
        static bool exist;
        static bool instantiate (void);
};

class instance : private tracker {
    public:
        instance (void);

        void erase (void);
        void color_default (void);
        void color_fore (color color);
        void color_back (color color);
        void cursor_hide (void);
        void cursor_show (void);
        void cursor_move (int row, int col);

        enum class except_ctor {fail};
        enum class except_erase {fail};
        enum class except_color_default {fail};
        enum class except_color_fore {fail};
        enum class except_color_back {fail};
        enum class except_cursor_hide {fail};
        enum class except_cursor_show {fail};
        enum class except_cursor_move {fail};

    private:
        logging::instance logging;
        bool init;
};

enum class color {
    black        = 0,
    white        = 15,
    darkgray     = 8,
    darkred      = 1,
    darkgreen    = 2,
    darkyellow   = 3,
    darkblue     = 4,
    darkmagenta  = 5,
    darkcyan     = 6,
    lightgray    = 7,
    lightred     = 9,
    lightgreen   = 10,
    lightyellow  = 11,
    lightblue    = 12,
    lightmagenta = 13,
    lightcyan    = 14
};

logging::stream & operator << (logging::stream & strm, color color);

}

#endif

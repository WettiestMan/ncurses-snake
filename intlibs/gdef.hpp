#ifndef GDEF_HPP_TTT
#define GDEF_HPP_TTT

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif

#include <memory>
#include <ncursesw/curses.h>

using win_uptr = std::unique_ptr<WINDOW, decltype(&delwin)>;

namespace gbconst{
    constexpr uint8_t gamefield_width = 80;
    constexpr uint8_t gamefield_height = 24;

    constexpr size_t points_len = 15;
}

struct global_elems{
    bool keep_running;
    bool game_over;

    wchar_t points_tmpl[gbconst::points_len];
    int points;

    cchar_t vchar;
    cchar_t hchar;
};

#endif // GDEF_HPP_TTT
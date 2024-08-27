#ifndef GDEF_HPP_NSNK
#define GDEF_HPP_NSNK

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif

#include <memory>
#include <cstdint>
#include <ncursesw/curses.h>
#include "gobjs.hpp" // not a fan if this, but I did it to use
//assets::snake::direction as the type for global_elems::dir_map

using win_uptr = std::unique_ptr<WINDOW, decltype(&delwin)>;

namespace gbconst{
    constexpr uint8_t gamefield_width = 80;
    constexpr uint8_t gamefield_height = 24;

    constexpr size_t points_len = 15;

    constexpr uint8_t snake_default_x = 39;
    constexpr uint8_t snake_default_y = 11;
}

struct global_elems{
    bool keep_running;
    bool game_over;

    wchar_t points_tmpl[gbconst::points_len];
    int points;

    cchar_t vchar;
    cchar_t hchar;
    cchar_t space;

    assets::snake::direction dir_map[gbconst::gamefield_height][gbconst::gamefield_width];
};

#endif // GDEF_HPP_TTT
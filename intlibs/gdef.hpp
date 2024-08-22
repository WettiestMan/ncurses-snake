#ifndef GDEF_HPP_NSNK
#define GDEF_HPP_NSNK

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif

#include <memory>
#include <cstdint>
#include <ncursesw/curses.h>
#include "gobjs.hpp"

using win_uptr = std::unique_ptr<WINDOW, decltype(&delwin)>;

// forward declaration for the snake struct, why?
// cuz that would allow me to use assets::snake::direction as type for dir_map
// that way, I stop myself from writing static_cast<uint8_t>() like a monkey
// everytime I want to add something to it.

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
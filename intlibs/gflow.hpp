#ifndef GFLOW_HPP_NSNK
#define GFLOW_HPP_NSNK

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif

#include <ncursesw/curses.h>
#include "gdef.hpp"
#include "gobjs.hpp"

namespace flowctrl {
    void reset_game(WINDOW* gamefield, global_elems& gbvars, assets::snake& player);

    void generate_first_food(WINDOW* gamefield, global_elems& gbvars);

    void check_snake_new_pos(WINDOW* gamefield, global_elems& gbvars, assets::snake& player);

    void update_snake_head(WINDOW* gamefield, global_elems& gbvars, assets::snake& player);

    inline void set_game_over(global_elems& gbvars) noexcept{
        gbvars.game_over = true;
    }

    void show_game_over(global_elems& gbvars);

    // the functions in this namespace are only used for gflow implementation and are not used outside of it
    namespace internal{
        void generate_food(WINDOW* gamefield, global_elems& gbvars);

        void update_snake_tail(WINDOW* gamefield, global_elems& gbvars, assets::snake& player);

        void update_current_points(global_elems& gbvars);
    }
}

#endif // GFLOW_HPP_NSNK
#ifndef GHANDLE_HPP_NSNK
#define GHANDLE_HPP_NSNK

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif

#include <ncursesw/curses.h>

#include "gdef.hpp"
#include "gobjs.hpp"

namespace hdl{
    bool handle_exit_dlg_keyboard(const win_uptr& exitwin, global_elems& gbvars);

    void handle_gamefield_keyboard(WINDOW* gamewin, global_elems& gbvars, assets::snake& player);
}

#endif // GHANDLE_HPP_TTT
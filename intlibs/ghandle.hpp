#ifndef GHANDLE_HPP_TTT
#define GHANDLE_HPP_TTT

#include <ncursesw/curses.h>

#include "gdef.hpp"

bool handle_exit_dlg_keyboard(const win_uptr& exitwin, global_elems& gbvars);

bool handle_gamefield_keyboard(const win_uptr& gamewin, global_elems& gbvars);

#endif // GHANDLE_HPP_TTT
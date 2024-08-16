#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif

#include <ncursesw/curses.h>
#include "gdef.hpp"
#include "ghandle.hpp"

constexpr int no_x_pos = 30;
constexpr int yes_x_pos = 14;
constexpr int opts_y_pos = 3;

namespace hdl{
    bool handle_exit_dlg_keyboard(const win_uptr& exitwin, global_elems& gbvars){
        
        auto exitptr = exitwin.get();
        wint_t input;
        int keytype = get_wch(&input);

        int x = getcurx(exitptr);

        bool ret = true;
        if(keytype == KEY_CODE_YES){
            switch (input)
            {
            case KEY_LEFT:
                x = (x == no_x_pos) ? yes_x_pos : no_x_pos;
                wmove(exitptr, opts_y_pos, x);
                break;
            case KEY_RIGHT:
                x = (x == yes_x_pos) ? no_x_pos : yes_x_pos;
                wmove(exitptr, opts_y_pos, x);
                break;
            }
        }
        else{
            switch (input)
            {
            case L'a':
            case L'A':
                x = (x == no_x_pos) ? yes_x_pos : no_x_pos;
                wmove(exitptr, opts_y_pos, x);
                break;
            case L'd':
            case L'D':
                x = (x == yes_x_pos) ? no_x_pos : yes_x_pos;
                wmove(exitptr, opts_y_pos, x);
                break;
            case L'\r':
            case L'\n':
                if(x == yes_x_pos){
                    gbvars.game_over = true;
                    gbvars.keep_running = false;
                }
                return false;
            }
        }

        return true;
    }
}
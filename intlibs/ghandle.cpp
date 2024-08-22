#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif

#include <ncursesw/curses.h>
#include "gdef.hpp"
#include "ghandle.hpp"
#include "gobjs.hpp"

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

    void handle_gamefield_keyboard(WINDOW* gamefield, global_elems& gbvars, assets::snake& player){
        wint_t input = 0;

        switch (get_wch(&input))
        {
        case OK:
            switch (input)
            {
            case L'w':
            case L'W':
                player.current_dir = assets::snake::direction::up;
                break;
            case L'a':
            case L'A':
                player.current_dir = assets::snake::direction::left;
                break;
            case L's':
            case L'S':
                player.current_dir = assets::snake::direction::down;
                break;
            case L'd':
            case L'D':
                player.current_dir = assets::snake::direction::right;
                break;
            
            default:
                break;
            }
            break;
        case KEY_CODE_YES:
            switch (input)
            {
            case KEY_UP:
                player.current_dir = assets::snake::direction::up;
                break;
            case KEY_LEFT:
                player.current_dir = assets::snake::direction::left;
                break;
            case KEY_DOWN:
                player.current_dir = assets::snake::direction::down;
                break;
            case KEY_RIGHT:
                player.current_dir = assets::snake::direction::right;
                break;

            default:
                break;
            }
            break;
        default:
            break;
        }

    }
}
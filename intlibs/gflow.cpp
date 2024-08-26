#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif

#include <ncursesw/curses.h>

#include <tuple>
#include <cstdio>

#include "gdef.hpp"
#include "gobjs.hpp"
#include "gflow.hpp"

typedef assets::snake::direction snake_direction;

namespace flowctrl
{
    namespace internal{
        void generate_food(WINDOW* gamefield, global_elems& gbvars){
            assets::food new_food;
            cchar_t char_struct;
            wchar_t curr_char[CCHARW_MAX];

            do{
                new_food = assets::food::make();

                mvwin_wch(gamefield, new_food.y, new_food.x, &char_struct);

                attr_t dummy1;
                short dummy2;

                getcchar(&char_struct, curr_char, &dummy1, &dummy2, nullptr);
            }while (*curr_char != L' ');

            //setcchar(&char_struct, assets::food::icon, WA_NORMAL, 0, nullptr);
            mvwadd_wch(gamefield, new_food.y, new_food.x, &assets::food::icon_st);
        }

        void update_snake_tail(WINDOW* gamefield, global_elems& gbvars, assets::snake& player){
            std::tie(player.tail_prev_x, player.tail_prev_y) = {player.tail_x, player.tail_y};

            switch (gbvars.dir_map[player.tail_y][player.tail_x])
            {
            case snake_direction::up:
                player.tail_y--;
                break;
            case snake_direction::left:
                player.tail_x--;
                break;
            case snake_direction::down:
                player.tail_y++;
                break;
            case snake_direction::right:
                player.tail_x++;
                break;
            }

            mvwadd_wch(gamefield, player.tail_prev_y, player.tail_prev_x, &gbvars.space);
            gbvars.dir_map[player.tail_prev_y][player.tail_prev_x] = snake_direction::no_dir;
        }

        void update_current_points(global_elems& gbvars){
            static wchar_t points_print_str[7] = {0};
            std::swprintf(points_print_str, L"%06d", gbvars.points);
            mvaddnwstr(0, 8, points_print_str, 6);

            wnoutrefresh(stdscr);
        }
    }

    void reset_game(WINDOW* gamefield, global_elems& gbvars, assets::snake& player){
        gbvars.points = 0;
        gbvars.game_over = false;
        
        player.current_dir = snake_direction::right;
        player.head_prev_x = player.head_x = player.tail_prev_x = player.tail_x = gbconst::snake_default_x;
        player.head_prev_y = player.head_y = player.tail_prev_y = player.tail_y = gbconst::snake_default_y;

        mvwaddnwstr(stdscr, 0, 0, gbvars.points_tmpl, gbconst::points_len);

        werase(gamefield);

        box_set(gamefield, &gbvars.vchar, &gbvars.hchar);
        mvwadd_wch(gamefield, gbconst::snake_default_y, gbconst::snake_default_x, &assets::snake::icon_st);
        gbvars.dir_map[gbconst::snake_default_y][gbconst::snake_default_x] = snake_direction::right;
    }

    void generate_first_food(WINDOW* gamefield, global_elems& gbvars){
        internal::generate_food(gamefield, gbvars);
    }

    void update_snake_head(WINDOW* gamefield, global_elems& gbvars, assets::snake& player){
        std::tie(player.head_prev_x, player.head_prev_y) = {player.head_x, player.head_y};
        
        switch (player.current_dir)
        {
        case snake_direction::up:
            player.head_y--;
            break;
        case snake_direction::left:
            player.head_x--;
            break;
        case snake_direction::down:
            player.head_y++;
            break;
        case snake_direction::right:
            player.head_x++;
            break;
        }

        gbvars.dir_map[player.head_y][player.head_x] = player.current_dir;
        gbvars.dir_map[player.head_prev_y][player.head_prev_x] = player.current_dir; // So it can handle direcion shifts
    }

    void check_snake_new_pos(WINDOW* gamefield, global_elems& gbvars, assets::snake& player){
        cchar_t head_st;
        mvwin_wch(gamefield, player.head_y, player.head_x, &head_st);

        wchar_t head_curr_char[CCHARW_MAX];
        attr_t dummy1;
        short dummy2;
        getcchar(&head_st, head_curr_char, &dummy1, &dummy2, nullptr);

        switch (*head_curr_char)
        {
        case *assets::food::icon:
            gbvars.points++;
            internal::update_current_points(gbvars);
            internal::generate_food(gamefield, gbvars);
            break;
        case *assets::snake::icon:
        case *assets::misc::vchar_icon:
        case *assets::misc::hchar_icon:
            set_game_over(gbvars);
            internal::update_snake_tail(gamefield, gbvars, player);
            break;
        case *assets::misc::space_icon:
            internal::update_snake_tail(gamefield, gbvars, player);
            break;
        }

        mvwadd_wch(gamefield, player.head_y, player.head_x, &assets::snake::icon_st);
        wnoutrefresh(gamefield);
    }

    void show_game_over(global_elems& gbvars) {
        win_uptr game_over_win(newwin(5, 46, 9, 16), &delwin);
        refresh();

        box_set(game_over_win.get(), &gbvars.vchar, &gbvars.hchar);

        mvwaddnwstr(game_over_win.get(), 1, 20, L"¡Auch!", 6);
        mvwaddnwstr(game_over_win.get(), 3, 2, L"'espacio': salir,  otra tecla: reintentar.", 42);

        wint_t input = 0;
        if((wget_wch(game_over_win.get(), &input) == OK) && (input == L' '))
            gbvars.keep_running = false;
    }
} // namespace flowctrl

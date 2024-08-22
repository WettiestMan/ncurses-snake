#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif

#include <ncursesw/curses.h>

#include <clocale>
#include <cwchar>
#include <cassert>
#include <cstdint>
#include <csignal>
#include <thread>
#include <chrono>

#include "intlibs/gdef.hpp"
#include "intlibs/gutils.hpp"
#include "intlibs/gobjs.hpp"
#include "intlibs/ghandle.hpp"
#include "intlibs/gflow.hpp"

/**
 * TODO: Son demasiadas variables globales en mi opinión ¿están de acuerdo?
 * 
 * EZ_SOLUTION: pon gbvars en int main(int,char**) y pásalo como referencia
 * a initialize_stuff(), absolutamente todas las funciones que dependen
 * de gbvars ahora lo toman como referencia. 
 * 
 * UPDATE: Esto no funciona por 2 motivos:
 * 
 * 1: no hay manera de pasar argumentos directamente a un manejador de señal
 * (como se llame)
 * 2: lambdas que capturan argumentos no pueden ser convertidos a punteros de
 * funciones, al parecer.
 */

// Ver intlibs/gdef.hpp para más detalles
struct global_elems gbvars = {
    true,                 // bool keep_running
    false,                // bool game_over
    L"puntos: 000000",    // wchar_t points_tmpl[gbconst::points_len]
    0,                    // int points
    {0},                  // cchar_t vchar
    {0},                  // cchar_t hchar
    {0},                  // cchar_t space
    {assets::snake::direction::no_dir}                   // uint8_t dir_map[gbconst::gamefield_height][gbconst::gamefield_width]
};

/*
┌────────────────────────────────────────────┐
│ ¿Está seguro de que desea salir del juego? │
│                                            │
│             si              no             │
└────────────────────────────────────────────┘
*/

WINDOW* initialize_stuff()
{
    std::setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, true);
    noecho();

    setcchar(&gbvars.vchar, assets::misc::vchar_icon, WA_NORMAL, 0, nullptr);
    setcchar(&gbvars.hchar, assets::misc::hchar_icon, WA_NORMAL, 0, nullptr);
    setcchar(&gbvars.space, assets::misc::space_icon, WA_NORMAL, 0, nullptr);

    setcchar(&assets::food::icon_st, assets::food::icon, WA_NORMAL, 0, nullptr);
    setcchar(&assets::snake::icon_st, assets::snake::icon, WA_NORMAL, 0, nullptr);

    std::signal(SIGINT, [](int arg) -> void
                        {
                            win_uptr exit_msgbox (newwin(5, 46, 9, 16), &delwin);
                            refresh();

                            box_set(exit_msgbox.get(), &gbvars.vchar, &gbvars.hchar);
                            mvwaddnwstr(exit_msgbox.get(), 1, 2, L"¿Está seguro de que desea salir del juego?", 42);
                            mvwaddnwstr(exit_msgbox.get(), 3, 2, L"            si              no            ", 42);
                            curs_set(2);
                            wmove(exit_msgbox.get(), 3, 30);
                            wrefresh(exit_msgbox.get());

                            bool option_not_chosen = true;
                            while(option_not_chosen)
                            {
                                option_not_chosen = hdl::handle_exit_dlg_keyboard(exit_msgbox, gbvars);
                                wrefresh(exit_msgbox.get());
                            }
                            curs_set(0);
                        });

    utils::initialize_rng();
    utils::set_rng_int_range(1, gbconst::gamefield_width - 2, 1, gbconst::gamefield_height - 2);

    WINDOW* gamefield = newwin(gbconst::gamefield_height, gbconst::gamefield_width, 1, 0);

    if(gamefield == nullptr)
        return nullptr;
    
    keypad(gamefield, true);
    nodelay(gamefield, true);
    return gamefield;
}

int main(int argc, char** argv){

    using namespace std::chrono_literals;

    assets::snake player;

    WINDOW* gamefield = initialize_stuff();
    if(!gamefield)
    {
        endwin();
        std::fwprintf(stderr, L"No se pudo cargar el juego, vuelva a intentarlo más tarde...\n"
                        L"Presione \"enter\" para cerrar el juego.");
        std::getwchar();
        return 1;
        
    }
    refresh();

    curs_set(0);

    box_set(gamefield, &gbvars.vchar, &gbvars.hchar);
    wrefresh(gamefield);

    while(gbvars.keep_running){
        //mvwaddnwstr(stdscr, 0, 0, gbvars.points_tmpl, gbconst::points_len);
        flowctrl::reset_game(gamefield, gbvars, player);
        flowctrl::generate_first_food(gamefield, gbvars);
        refresh();

        while(!gbvars.game_over){
            std::this_thread::sleep_for(16ms);
            hdl::handle_gamefield_keyboard(gamefield, gbvars, player);
            flowctrl::update_snake_head(gamefield, gbvars, player);
            //flowctrl::update_snake_tail(gamefield, gbvars, player); going to be called by check_snake_new_pos() if snake doesn't land on food
            flowctrl::check_snake_new_pos(gamefield, gbvars, player); // this function will also call flowctrl::generate_food() if snake lands on food
            wrefresh(gamefield);
        }
    }

    delwin(gamefield);
    endwin();
    return 0;
}
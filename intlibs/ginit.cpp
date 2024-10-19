#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif

#include <cwchar>
#include <csignal>
#include <chrono>
#include <thread>

#include <ncursesw/curses.h>

#include "ginit.hpp"
#include "gdef.hpp"
#include "gobjs.hpp"
#include "ghandle.hpp"
#include "gflow.hpp"
#include "gutils.hpp"

win_uptr initialize_stuff() {
    std::setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, true);
    noecho();

    setcchar(&assets::misc::vchar, assets::misc::vchar_icon, WA_NORMAL, 0, nullptr);
    setcchar(&assets::misc::hchar, assets::misc::hchar_icon, WA_NORMAL, 0, nullptr);
    setcchar(&assets::misc::space, assets::misc::space_icon, WA_NORMAL, 0, nullptr);

    setcchar(&assets::food::icon_st, assets::food::icon, WA_NORMAL, 0, nullptr);
    setcchar(&assets::snake::icon_st, assets::snake::icon, WA_NORMAL, 0, nullptr);

    /* Considering my current strategy, if I want this to work I should make global_elems a singleton
     * Using the struct pattern doesn't work for signal handlers becuz... you can't pass parameters
     * Of, course, I can also drop this idea and make it a normal action.
     * 
     * I'm gonna have to commit to a ton of refactoring...
     * 
     * Nah, I have a way to exit the game gracefully already for the moment
    std::signal(SIGINT, [](int signal) -> void
                        {
                            win_uptr exit_msgbox (newwin(5, 46, 9, 16), &delwin);
                            refresh();

                            box_set(exit_msgbox.get(), &assets::misc::vchar, &assets::misc::hchar);
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
                        });*/

    utils::initialize_rng();
    utils::set_rng_int_range(1, gbconst::gamefield_width - 2, 1, gbconst::gamefield_height - 2);

    WINDOW* gamefield = newwin(gbconst::gamefield_height, gbconst::gamefield_width, 1, 0);

    if(gamefield == nullptr)
        return {nullptr, &delwin};
    
    keypad(gamefield, true);
    nodelay(gamefield, true);
    return {gamefield, &delwin};
}

int run(int argc, char** argv) {
    using namespace std::chrono;
    using namespace std::chrono_literals;

    assets::snake player;
    global_elems gbvars = {
        true,                 // bool keep_running
        false,                // bool game_over
        L"puntos: 000000",    // wchar_t points_tmpl[gbconst::points_len]
        0,                    // int points
        {assets::snake::direction::no_dir}                   // uint8_t dir_map[gbconst::gamefield_height][gbconst::gamefield_width]
    };

    win_uptr gamefield = initialize_stuff();
    if(!gamefield)
    {
        printw("%ls", L"No se pudo cargar el juego, vuelva a intentarlo más tarde...\n"
                        L"Presione cualquier tecla para cerrar el juego.");
        
        wint_t p;
        get_wch(&p);
        return 1;
    }

    refresh();

    curs_set(0);

    box_set(gamefield.get(), &assets::misc::vchar, &assets::misc::hchar);
    wrefresh(gamefield.get());

    while(gbvars.keep_running){
        gamefield = flowctrl::reset_game(std::move(gamefield), gbvars, player);

        while(!gbvars.game_over){
            time_point tick_start = high_resolution_clock::now();
            // std::this_thread::sleep_for(56ms);     // TODO: Do research about how to implement δ-time here. Imagine
            // If somebody runs this game at four hundred thousand fps in a four hundred thousand dollars PC. 
            hdl::handle_gamefield_keyboard(gamefield.get(), gbvars, player);
            flowctrl::update_snake_head(gamefield.get(), gbvars, player);
            flowctrl::check_snake_new_pos(gamefield.get(), gbvars, player);
            doupdate();

            auto tick_time = duration_cast<milliseconds>(high_resolution_clock::now() - tick_start); // Is this δ-time?
            duration sleeping_time = 62.5ms - tick_time;
            if(sleeping_time.count() > 0) 
                std::this_thread::sleep_for(sleeping_time); // Is this formula right? 
        }
        flowctrl::show_game_over(gbvars);
    }

    return 0;
}
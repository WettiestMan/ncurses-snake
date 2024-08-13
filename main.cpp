#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif

#include <ncursesw/curses.h>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
#else
#include <unistd.h>
#endif // __WIN32__

#include <SDL2/SDL_keyboard.h>
#include <clocale>
#include <cwchar>
#include <memory>
#include <cassert>
#include <thread>
#include <chrono>
#include <cstdint>
#include <csignal>

#include "intlibs/gdef.hpp"
#include "intlibs/gutils.hpp"
#include "intlibs/gobjs.hpp"
#include "intlibs/ghandle.hpp"

/* Thought about resizing the window, it's taking me a ton of time to implement
 * I think it's just better to create a subwindow in the TTY
[[nodiscard]]
void preserve_actual_cmd(HANDLE& buf, CONSOLE_SCREEN_BUFFER_INFOEX& infobuf){
    HANDLE original_window = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFOEX original_window_info = {0};

    if(!SetConsoleScreenBufferInfoEx(original_window, &original_window_info)){
        buf = nullptr;
    }


}

[[nodiscard]]
std::tuple<int, HANDLE, CONSOLE_SCREEN_BUFFER_INFOEX> resizecmd(int x, int y){
    HANDLE original_window = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFOEX original_window_info = {0};

    if(!SetConsoleScreenBufferInfoEx(original_window, &original_window_info))
        return {1, nullptr, {0}};

    assert((x <= original_window_info.dwMaximumWindowSize.X &&
            y <= original_window_info.dwMaximumWindowSize.Y) &&
            (x > 0 && y > 0) && "Coordinates x or y (or both) are 0 or smaller or are too big");

    std::tuple<int, HANDLE, CONSOLE_SCREEN_BUFFER_INFOEX> ret = {0, nullptr, {0}};

    HANDLE new_window = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr,
                                        CONSOLE_TEXTMODE_BUFFER, nullptr);

    if(new_window != nullptr)
    {
        if(SetConsoleActiveScreenBuffer(new_window)){
            SMALL_RECT new_window_dims = {0, 0, x - 1, y - 1};
        }
        else
            std::get<0>(ret) = 3;
    }
    else
    {
        std::get<0>(ret) = 2;
    }

    return ret;
}*/

/**
 * TODO: Son demasiadas variables globales en mi opinión ¿están de acuerdo?
 */

// Ver intlibs/gdef.hpp para más detalles
struct global_elems gbvars = {
    true,                 // bool keep_running
    false,                // bool game_over
    L"puntos: 000000",    // wchar_t points_tmpl[gbconst::points_len]
    0,                    // int points
    {0},                  // cchar_t vchar
    {0}                   // cchar_t hchar
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

    setcchar(&gbvars.vchar, L"│", WA_NORMAL, 0, nullptr);
    setcchar(&gbvars.hchar, L"─", WA_NORMAL, 0, nullptr);

    std::signal(SIGINT, [](int arg) -> void
                        {
                            win_uptr exit_msgbox (newwin(5, 46, 9, 16), &delwin);

                            box_set(exit_msgbox.get(), &gbvars.vchar, &gbvars.hchar);
                            mvwaddnwstr(exit_msgbox.get(), 1, 2, L"¿Está seguro de que desea salir del juego?", 42);
                            mvwaddnwstr(exit_msgbox.get(), 3, 2, L"            si              no            ", 42);
                            curs_set(2);
                            wmove(exit_msgbox.get(), 3, 30);

                            bool option_not_chosen = true;
                            while(option_not_chosen)
                            {
                                option_not_chosen = handle_exit_dlg_keyboard(exit_msgbox, gbvars);
                            }
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
    { // Because this dummy decided to use smart pointers, he decided to create a scope so
      // the smart pointers' destructors get called before endwin()

      // He's gonna regret this decision eventually and decide to pass it everything to manual calls (or at least the gamefield WINDOW*)

        // so I do not have to deallocate it manually
        win_uptr gamefield(initialize_stuff(), &delwin);
        if(!gamefield)
        {
            endwin();
            std::fwprintf(stderr, L"No se pudo cargar el juego, vuelva a intentarlo más tarde...\n"
                            "Presione \"enter\" para cerrar el juego.");
            std::getwchar();
            return 1;
            
        }
        
        curs_set(0);

        box_set(gamefield.get(), &gbvars.vchar, &gbvars.hchar);

        while(gbvars.keep_running){
            mvwaddnwstr(stdscr, 0, 0, gbvars.points_tmpl, gbconst::points_len);

            while(!gbvars.game_over){
                std::this_thread::sleep_for(16ms);
            }
        }
    }
    endwin();
    return 0;
}
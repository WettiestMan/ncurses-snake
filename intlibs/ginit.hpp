#ifndef GINIT_HPP_NSNK
#define GINIT_HPP_NSNK

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif

#include <ncursesw/curses.h>
#include "gdef.hpp"

win_uptr initialize_stuff();

/**
 * This function is the one that runs the whole game
 * 
 * @param argc: The number of the program args (not used for anything meaningful)
 * @param argv: The actual args (not used for anything meaningful)
 * @return an int which is the exit value
 */
int run(int argc, char** argv);

#endif
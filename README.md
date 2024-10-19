# ncurses-snake

--

This a simple snake game made for the terminal using the ncurses library.  

This project takes heavy inspiration from this other [(This one)](https://github.com/Shellyda/snake-game-bootloader-project.git).
You could consider this as a rewrite of that repo: the idea and the L&F is the same, but the code was rewritten from scratch
(quite ironic, considering the other project is done in ASM and made to run on technically any IBM PC compatible without an OS).  

Anyways, you might wanna check out that repo, it's simple but if you're interested in the world of bootloaders, you might
find it cool.

As for this project, it's just a OS user application version of that game, but I'll see if I can add more stuff to this to
differentiate it a bit more from the inspiration.

---

# Running this code

---

You can use CMake to build this code like any other CMake project, just create a folder for the CMake output and from there
run ```cmake ..``` to generate a project to build (you can also specify your generator and other options if you like).

If you use the CMake Tools extension from VSCode, you can run any of the 4 configurations set inside the CMakePresets.json file,
they're just to generate a VS 17 2022 project (```vs-*``` config) and a project with MSYS makefiles (```mingw-*``` config),
the ```*``` can be ```dbg``` for debug builds (they write on __build-db__ folder) or ```rel``` for release builds (they
write on __build__ folder). You can modify the CMakePresets.json file to add new configurations.

**NOTE**: If you are using the VS configuration with the MSVC compiler (which I'm pretty sure most of the ones that choose this
config do) you should install an implementation of the ncurses library, if you have GCC or Clang installed from MSYS2, it is
likely that you have ncurses installed for your compiler.

---

# Controls

---

Use the arrow keys or WASD to move the snake.

~~Press Ctrl+C if you are done and want to leave (BUGGY FOR THE MOMENT).~~
I'm removing this way of leaving the game for the moment. If you wanna stop playing... Just die LOL, and press space when you
see the Game Over screen.

---

# Why using the wide char version of the ncurses library? Can you just be normal and use normal chars?

## Docs for the normal char version of ncurses are all around the web, finding info for the wchar apis is more difficult

###### (Do you like pain?)

---

I did this project using Windows lol. And thought that using the char apis might cause the text to break on other computers
that don't have the UTF-8 codepage activated (and I didn't want my code to only work for one random non universal codepage).

So yeah, I worked with wchars to make my text UTF-16 compatible on Windows (maybe the ncurses implementation for MinGW have
some kind of translation layer for text or does nothing and just passes the characters to the console or something, I don't
know).

ofc, this also means that if you compile for Linux or any other *nix OS you will get UTF-32 strings.

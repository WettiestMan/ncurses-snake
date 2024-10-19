#ifndef GOBJS_HPP_NSNK
#define GOBJS_HPP_NSNK

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif

#include <ncursesw/curses.h>
#include <cstdint>

namespace assets{
    struct food{
        static constexpr wchar_t icon[] = L"@";

        int x;
        int y;

        static inline cchar_t icon_st = {0};

        food() noexcept;
        food(int nx, int ny) noexcept;
        food(const food& f) noexcept;
        food(food&& f) noexcept;
        
        void reroll() noexcept;

        static food make() noexcept;

        food operator=(food&& f) noexcept;

        bool operator==(const food& f) const noexcept;
    };

    struct snake{
        static constexpr wchar_t icon[] = L"*";

        static inline cchar_t icon_st = {0};

        enum class direction : int8_t{
            no_dir = 0,
            left,
            right,
            up,
            down
        };

        int head_x;
        int head_y;
        int tail_x;
        int tail_y;

        int head_prev_x;
        int head_prev_y;
        int tail_prev_x;
        int tail_prev_y;

        direction current_dir;
    };

    namespace misc {
        inline constexpr wchar_t vchar_icon[] = L"│";
        inline constexpr wchar_t hchar_icon[] = L"─";
        inline constexpr wchar_t space_icon[] = L" ";

        inline cchar_t vchar = {0};
        inline cchar_t hchar = {0};
        inline cchar_t space = {0};
    }
}

#endif // GOBJS_HPP_TTT
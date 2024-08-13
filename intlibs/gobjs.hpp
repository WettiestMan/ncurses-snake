#ifndef GOBJS_HPP_TTT
#define GOBJS_HPP_TTT

#include <cstdint>
#include <utility>

namespace assets{
    struct food{
        int x;
        int y;

        food() noexcept;
        food(int nx, int ny) noexcept;
        food(const food& f) noexcept;
        food(food&& f) noexcept;
        
        void reroll() noexcept;

        static food make() noexcept;

        food operator=(food&& f) noexcept;
    };
}

#endif // GOBJS_HPP_TTT
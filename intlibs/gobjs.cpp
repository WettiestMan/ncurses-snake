#include "gobjs.hpp"
#include "gutils.hpp"
#include <tuple>

namespace assets{
    food::food() noexcept : x(utils::generate_random_x())
                            , y(utils::generate_random_y()) {}

    food::food(int nx, int ny) noexcept : x(nx), y(ny) {}

    food::food(food&& f) noexcept: x(f.x), y(f.y) {}

    food::food(const food& f) noexcept: x(f.x), y(f.y) {}

    void food::reroll() noexcept
    {
        this->x = utils::generate_random_x();
        this->y = utils::generate_random_y();
    }

    food food::make() noexcept
    {
        auto[x, y] = utils::generate_two_ints();
        return food(x, y);
    }

    food food::operator=(food&& f) noexcept
    {
        this->x = f.x;
        this->y = f.y;
        return *this;
    }
}
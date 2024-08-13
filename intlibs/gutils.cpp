#include <random>
#include <cassert>
#include "gutils.hpp"

std::mt19937 rng;
std::uniform_int_distribution<int> xdist, ydist;

namespace utils{

    void initialize_rng() noexcept
    {
        std::random_device sysrng{};
        std::seed_seq seeds{sysrng(), sysrng(), sysrng(), sysrng(), sysrng()};
        rng = std::mt19937{seeds};
    }

    void set_rng_int_range(int xmin, int xmax, int ymin, int ymax) noexcept
    {
        assert(((xmin < xmax) && (ymin < ymax)) && "Parametros inválidos: [x/y]min no puede ser mayor o igual a [x/y]max");

        xdist = std::uniform_int_distribution<int>(xmin, xmax);
        ydist = std::uniform_int_distribution<int>(ymin, ymax);
    }

    int generate_random_x() noexcept
    {
        return xdist(rng);
    }

    int generate_random_y() noexcept
    {
        return ydist(rng);
    }

    std::pair<int, int> generate_two_ints() noexcept
    {
        return {xdist(rng), ydist(rng)};
    }
}
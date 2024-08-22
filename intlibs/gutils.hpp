#ifndef GUTILS_HPP_NSNK
#define GUTILS_HPP_NSNK

#include <cstdint>
#include <utility>

namespace utils
{
    // these functions use std::mt19937 to generate numbers

    /**
     * Initializes the mersene twister generator using random
     * values fron the current system random number generator as its seed
     */
    void initialize_rng() noexcept;

    /**
     * Configures the distribution of the generated random numbers
     * 
     * @param xmin: the minimum value for the x distribution
     * @param xmax: the maximum value for the x distribution, inclusive
     * @param ymin: the minimum value for the y distribution
     * @param ymax: the maximum value for the y distribution, inclusive
     */
    void set_rng_int_range(int xmin, int xmax, int ymin, int ymax) noexcept;

    /**
     * Generates a random number from the mersene twister generator.
     * Uses the distribution for x numbers
     * 
     * @return a random integer
     */
    int generate_random_x() noexcept;

    /**
     * Generates a random number from the mersene twister generator.
     * Uses the distribution for y numbers
     * 
     * @return a random integer
     */
    int generate_random_y() noexcept;

    /**
     * Generates a std::pair of random integers from the mersene twister
     * generator, the first is a number from the x distribution and the second is
     * a number from the y distribution
     * 
     * @return a std::pair of random integers
     */
    std::pair<int, int> generate_two_ints() noexcept;
} // namespace utils
#endif // GUTILS_HPP_TTT
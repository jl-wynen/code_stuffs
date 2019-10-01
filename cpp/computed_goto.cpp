/**
 * Demonstrate computed goto, a vendor extension to allow constructing jump tables.
 * Tested with gcc 9.1.0, clang 8.0.1
 *
 * g++ --std=c++17 -Wall -Wextra -Wpedantic -Werror computed_goto.cpp
 */


#include <cassert>
#include <iostream>


void print(int const n)
{
    assert(n < 2);

// do not show warnings about non-standard features from here on
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

    // construct jump table
    constexpr void *labels[] = {
        &&label_1, &&label_2
    };

    // computed goto using label from table
    goto* labels[n];

label_1:
    std::cout << "0\n";
    return;

label_2:
    std::cout << "1\n";
    return;

// switch back to normal warnings
#pragma GCC diagnostic pop
}


int main(int argc, [[maybe_unused]] char *argv[])
{
    print(argc-1);

    return EXIT_SUCCESS;
}

#include "base.hpp"

#include <iostream>

// Implementation of Base type.
// Separated from header to require base program and addon to link against base library.

Base::Base(int x_) : x{x_}
{
    std::cout << "Base::Base(" << x_ << ")\n";
}


Base::~Base() noexcept = default;

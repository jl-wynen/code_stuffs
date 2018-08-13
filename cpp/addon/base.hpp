#pragma once

// base class for main and addon

struct Base
{
    int x;

    explicit Base(int x_) : x{x_} {}
    virtual ~Base() noexcept = default;

    virtual int foo(int y) = 0;
};

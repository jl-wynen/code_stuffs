#pragma once

// Base class for main and addon.

struct Base
{
    int x;

    explicit Base(int x_);
    virtual ~Base() noexcept;

    virtual int foo(int y) = 0;
};

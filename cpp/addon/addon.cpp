#include <stdexcept>
#include <iostream>

#include "base.hpp"


// The addon.
//
// Using a special method to allocate an instance of Derived to check that
// the user actually calls destroy() and not just delete.
//
// WARNING Only one instance of Derived may exist at any given time
//         (see special make, destroy functions; a hack).

struct Derived : Base
{
    // pointer to test if the dtor gets called
    int *z;

    explicit Derived(int x_, int z_) : Base{x_}, z{new int{z_}}
    {
        std::cout << "Derived::Derived(" << x_ << ", " << z_ << ")\n";
    }

    ~Derived() noexcept override
    {
        delete z;
    }

    int foo(int y) override
    {
        std::cout << "Derived::foo(" << y << ")\n";
        return x + y + *z;
    }
};

// a buffer to allocate a Derived in
static char buf[1024];

extern "C" {
    // make clang shut up (inline does not work here!)
    int foo(int x);
    int foo(int x) {
        // is this really safe?!
        // seems like the main code treats the function pointer as using C++ calling conventions.
        std::cout << "addon::foo(" << x << ")\n";
        throw std::exception();
        return x+7;
    }

    // construct a Derived
    Base *make(int x, int z)
    {
        std::cout << "addon::make(" << x << ", " << z << ")\n";
        // allocate the object in buf -> can't use a delete on the object, must use destroy()
        return new(buf) Derived{x, z};
    }

    // destroy a Derived
    void destroy(Base *obj) noexcept  // depends on dtor being noexcept!
    {
        std::cout << "addon::destroy()\n";
        // no delete because buf is static
        obj->~Base();
    }
}

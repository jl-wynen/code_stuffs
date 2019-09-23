#include <stdexcept>
#include "base.hpp"

// the addon
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

    explicit Derived(int x_, int z_) : Base{x_}, z{new int{z_}} { }

    ~Derived() noexcept override
    {
        delete z;
    }

    int foo(int y) override
    {
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
        throw std::exception();
        return x+7;
    }

    // construct a Derived
    Base *make(int x, int z)
    {
        // allocate the object in buf -> can't use a delete on the object, must use destroy()
        return new(buf) Derived{x, z};
    }

    // destroy a Derived
    void destroy(Base *obj) noexcept  // depends on dtor being noexcept!
    {
        // no delete because buf is static
        obj->~Base();
    }
}

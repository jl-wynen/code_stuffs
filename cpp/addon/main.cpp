// load and test the addon
//
// Uses a very simply solution with basic RAII wrappers and helper functions.
// Resource management is still quite difficult. The DL object must outlive all
// objects constructed from the library and functions can only be used while the object
// is alive.
// The former could be solved using shared_ptr for DL instead of unique_ptr and
// storing a ref in every instance created with loadObj (need something else
// than unique_ptr for the pbject too).
//
// NOTE valgrind erroneously detects 32 "still reachable" bytes in 1 block (2018-07-06)

#include <iostream>
#include <memory>
#include <string>

#include <dlfcn.h>

#include "base.hpp"

// wrap the dl in an RAII type
struct DLCloser
{
    void operator()(void *dl) noexcept
    {
        // I think this doesn't get called if dl==nullptr.
        // Check just in case.
        if (dl)
            dlclose(dl);
    }
};
using DL = std::unique_ptr<void, DLCloser>;

// open library
inline DL openDL(char const * const fname, int const flags=RTLD_LAZY)
{
    void *ptr = dlopen(fname, flags);
    if (ptr == nullptr)
        throw std::runtime_error(std::string("Unable to load so: ")+dlerror());

    // clear errors
    dlerror();

    return DL{ptr};
}

// load a symbol from a library
template <typename F>
F loadSym(DL const &dl, char const * const name)
{
    // not sure if we need to check dlsymError, found it on the web...
    // this cast is platform dependent, on POSIX, this should work (dlsym is POSIX...)
    F fn = reinterpret_cast<F>(dlsym(dl.get(), name));
    char const * const dlsymError = dlerror();
    if (fn == nullptr || dlsymError)
        throw std::runtime_error(std::string("Unable to load function: ")+dlsymError);
    return fn;
}

// smart pointer to base (holding stuff coming from addons)
// How can we handle objects created in the base program?
using BasePtr = std::unique_ptr<Base, void(*)(Base*)>;

// instanciate an object from a dl
// flexibility as to make and destroy functions?
template <typename... Args>
BasePtr loadObj(DL const &dl, Args && ... args)
{
    // ### this needs some exception safety bad!
    return BasePtr(
        // load make function and call it
        loadSym<Base*(*)(Args...)>(dl, "make")(std::forward<Args>(args)...),
        // load destroy function and store as deleter
        loadSym<void(*)(Base*)>(dl, "destroy"));
}


inline void testfunc()
{
    // need the full path here
    DL dl = openDL("./addon.so");

    auto fn = loadSym<int(*)(int)>(dl, "foo");

    try {
        std::cout << fn(3) << '\n';
    }
    catch (std::exception &) {
        // can get an exception from the addon
        std::cout << "Caught std::exception\n";
    }
}


inline void testclass()
{
    DL dl = openDL("./addon.so");

    auto obj = loadObj(dl, 3, 4);
    std::cout << obj->foo(6) << '\n';
}


int main()
{
    std::cout << "Testing function\n";
    testfunc();

    std::cout << "\nTesting class\n";
    testclass();

    return 0;
}

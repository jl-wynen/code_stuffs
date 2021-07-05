/*
 * Use a function template with runtime polymorphism.
 */

#include <iostream>
#include <memory>


template <class F>
int generic_function(F &&f) {
    return f.foo(3);
}


struct Foo1 {
    int foo(int x) {
        return 1 * x;
    }
};


struct Foo2 {
    int foo(int x) {
        return 2 * x;
    }
};


// Crappy implementation of Sean Parent's type erasure.
// Needs some work to use references correctly and avoid extra copies.
struct DynamicAdapter {
    struct ImplBase {
        virtual int foo(int) = 0;
    };

    template <class T>
    struct Impl : ImplBase {
        T obj;
        
        Impl(const T &o) : obj{o} {}

        int foo(int x) override {
            return obj.foo(x);
        }
    };
    
    std::unique_ptr<ImplBase> impl;
    
    template <class T>
    DynamicAdapter(const T &obj) : impl{std::make_unique<Impl<T>>(obj)} {}
    
    int foo(int x) {
        return impl->foo(x);
    }
};


int main() {
    // Two separate instantiations of generic_function.
    std::cout << "static Foo1:  " << generic_function(Foo1{}) << '\n';
    std::cout << "static Foo2:  " << generic_function(Foo2{}) << '\n';
    
    DynamicAdapter dyfoo1(Foo1{});
    DynamicAdapter dyfoo2(Foo2{});
    // Use the same instantiation of generic_function for both Foo1 and Foo2.
    std::cout << "dynamic Foo1: " << generic_function(dyfoo1) << '\n';
    std::cout << "dynamic Foo2: " << generic_function(dyfoo2) << '\n';
}

#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

struct Foo {
    Foo(int num) : num_(num) {}
    void print_add(int i) const {cout << num_+i << endl;}
    int num_;
};

void print_num(int i) {
    cout << i << endl;
}

int main() {
    // function<void(int)> f_display = print_num;
    // f_display(-9);

    // function<void()> f_display_42 = []() { print_num(42); };
    // f_display_42();

    // function<void()> f_display_31337 = bind(print_num, 31337);
    // f_display_31337();

    // function<void(const Foo&, int)> f_add_display = &Foo::print_add;
    // Foo foo(314159);
    // f_add_display(foo, 1);

    // auto f_display_mem = mem_fn(&Foo::print_add);
    // f_display_mem(foo, -1);


    // -------

    // use std::cref or std::ref to "pass functor by reference"
    // so only references are copied, not entire functors
    
    vector<int> v{1, 2, 3, 4, 5};
    int n = 2;
    
    auto f1 = [n](int i) {return i%n == 0;};
    cout << std::count_if(v.begin(), v.end(), cref(f1)) << endl;

    // turn lambda into proper functor
    std::function<bool(int)> f(std::cref(f1));
    cout << std::count_if(v.begin(), v.end(), cref(f)) << endl;

    // now it can be used by std::not1
    auto nf = std::not1(f);
    cout << std::count_if(v.begin(), v.end(), cref(nf)) << endl;
}

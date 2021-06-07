#include <iostream>

#include "dataset.hpp"
#include "variable.hpp"

int main() {
    Variable var1{'m', dtype<int>};
    std::cout << get_unit(var1) << '\n';
    Variable var{'m', dtype<Dataset>};
    std::cout << get_unit(var) << '\n';

    std::cout << foo(var1) << std::endl;
    std::cout << foo(var) << std::endl;
}

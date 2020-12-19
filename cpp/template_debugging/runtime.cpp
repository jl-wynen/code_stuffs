// gcc and clang only

// Jonathan O'Connor
// https://www.youtube.com/watch?v=83fMx7Grxys

#include <iostream>
#include <string>

template <typename T>
auto name_of_type(T* = nullptr)
{
    std::string const pretty_func{__PRETTY_FUNCTION__};
    auto const pos = pretty_func.find('=');
    if (pos == std::string::npos) {
        return std::string{"*failed to parse type* "} + pretty_func;
    }
    return pretty_func.substr(pos+2,
                              pretty_func.length() - pos - 3);
}

int main() 
{
    std::cout << name_of_type<std::string>() << '\n';
}

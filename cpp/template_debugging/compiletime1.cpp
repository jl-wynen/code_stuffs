// Ivan Čukić

#include <vector>

template <typename T>
class print_as_error;

void foo()
{
    print_as_error<std::vector<int>::value_type>();
}

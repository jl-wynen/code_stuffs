// Daniel Frey

#include <vector>

template <typename T>
[[deprecated]] constexpr bool print_type = true;

static_assert(print_type<std::vector<int>::value_type>);

#include <iostream>
#include <ranges>
#include <vector>

template <typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> const &v) {
  os << '[';
  for (size_t i = 0; i + 1 < size(v); ++i) {
    os << v[i] << ", ";
  }
  if (size(v) > 0) {
    os << v.back();
  }
  os << ']';
  return os;
}

template <template <class> class> struct collect;

template <> struct collect<std::vector> {
  template <std::ranges::input_range R> friend auto operator|(R &&r, collect) {
    std::vector<std::ranges::range_value_t<R>> v;
    if constexpr (requires { std::ranges::size(r); }) {
      v.reserve(std::ranges::size(r));
    }
    for (auto &&e : r) {
      v.push_back(std::forward<decltype(e)>(e));
    }
    return v;
  }
};

int main() {
  auto v = std::ranges::views::iota(1, 10) | collect<std::vector>();
  std::cout << v << '\n';
}

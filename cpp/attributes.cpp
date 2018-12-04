/*
 * Works with gcc and clang.
 */

[[gnu::always_inline]] inline int f1() {
    return 3;
}

// Does not modify external state.
[[gnu::pure]] int f2();

// Like pure but also does not read mutable external state (only constants).
// Not allowed to read from pointer parameters.
[[gnu::const]] int f3();

// Must use return value.
[[nodiscard]] int f4();

// Will never return (terminate program or throw exception).
[[noreturn]] void f5();

int main() { }

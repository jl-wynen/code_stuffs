#ifndef DEFERRED_DISPATCH_VARIABLE_HPP
#define DEFERRED_DISPATCH_VARIABLE_HPP

//#include "get_unit.hpp"
#include <cassert>
#include <unordered_map>

template<class T>
constexpr inline int dtype = -1;

template<>
constexpr inline int dtype<int> = 0;

struct Variable {
    char m_unit;
    int m_dtype;

    [[nodiscard]] auto &unit() const {
        return m_unit;
    }

    [[nodiscard]] auto &dtype() const {
        return m_dtype;
    }
};

template<class... Args>
auto get_dtype(const Variable &var, Args &&...) {
    return var.dtype();
}

struct Default;

template<class, class, bool>
struct Concept_;

template<class ConceptDef, bool Noexcept, class R, class... Args>
struct Concept_<ConceptDef, R(Args...), Noexcept> {
    static auto &table() {
        static std::unordered_map<int, R (*)(Args...) noexcept(Noexcept)> table_;
        return table_;
    }

    static const auto &default_impl(R (*f)(Args...)) {
        static R (*impl)(Args...);
        if (f != nullptr) {
            impl = f;
        }
        return impl;
    }

    template<typename Impl>
    static void add(int key, Impl &&impl) {
        table().template emplace(key, std::forward<Impl>(impl));
    }

    template<typename Impl>
    static void add_default(Impl &&impl) {
        default_impl(std::forward<Impl>(impl));
    }

    R operator()(Args... args) const {
        if (const auto dtype = get_dtype(std::forward<Args>(args)...);
            table().find(dtype) != table().end()) {
            return table().at(dtype)(std::forward<Args>(args)...);
        } else if (const auto &f = default_impl(nullptr); f != nullptr) {
            return f(std::forward<Args>(args)...);
        }
        assert(false);
    }
};


template<class, class>
struct Concept;

template<class ConceptDef, class R, class... Args>
struct Concept<ConceptDef, R(Args...)> : Concept_<ConceptDef, R(Args...), false> {
};

template<class ConceptDef, class R, class... Args>
struct Concept<ConceptDef, R(Args...) noexcept> : Concept_<ConceptDef, R(Args...), true>{
};

template<class T>
struct implement_for {
    template<class C, class F>
    implement_for(C &c, F &&f) {
        if constexpr (std::is_same_v<T, Default>) {
            c.add_default(std::forward<F>(f));
        } else {
            c.add(dtype<T>, std::forward<F>(f));
        }
    }
};

// inline linkage is bad
inline const auto get_unit = Concept<struct GetUnit, char(const Variable &) noexcept>{};
inline const auto gu = implement_for<int>(get_unit, [](const Variable &var) noexcept { return var.unit(); });

inline const auto foo = Concept<struct Foo, char(const Variable &)>{};
inline const auto fooo = implement_for<Default>(foo, [](const Variable &var) { return 'u'; });

#endif//DEFERRED_DISPATCH_VARIABLE_HPP

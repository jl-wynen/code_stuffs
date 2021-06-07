#ifndef DEFERRED_DISPATCH_DATASET_HPP
#define DEFERRED_DISPATCH_DATASET_HPP

#include "variable.hpp"

struct Dataset {
    Variable m_var;
};

template<>
constexpr inline int dtype<Dataset> = 10;


inline const auto gud = implement_for<Dataset>(get_unit,
                                           [](const Variable &var) noexcept { return 'x'; });

#endif//DEFERRED_DISPATCH_DATASET_HPP

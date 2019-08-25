#pragma once

#include <type_traits>

#include "flt/complex_types.h"
#include "flt/value_ref.h"
#include "flt/value.h"
#include "flt/compat_cast.h"

namespace flt
{

// TODO: Add std::math overloads? Maybe std::sqrt(), std::exp(), etc.?

// Helpers that are used to work out what the correct runtime type of the
// argument is.
template<class T> struct always_false : std::false_type {};

// Represents one of our simple types (floats or complex floats)
template <class T> struct is_simple   { static constexpr bool value = false; };
template <> struct is_simple<float>   { static constexpr bool value = true;  };
template <> struct is_simple<double>  { static constexpr bool value = true;  };
template <> struct is_simple<cfloat>  { static constexpr bool value = true;  };
template <> struct is_simple<cdouble> { static constexpr bool value = true;  };

template< class T >
inline constexpr bool is_simple_v = is_simple<T>::value;

// Represents which types may participate with the ops created in this file
template <class T> struct is_valid           { static constexpr bool value = false; };
template <> struct is_valid<float>           { static constexpr bool value = true;  };
template <> struct is_valid<double>          { static constexpr bool value = true;  };
template <> struct is_valid<cfloat>          { static constexpr bool value = true;  };
template <> struct is_valid<cdouble>         { static constexpr bool value = true;  };
template <> struct is_valid<value_ref>       { static constexpr bool value = true;  };
template <> struct is_valid<const_value_ref> { static constexpr bool value = true;  };
template <> struct is_valid<value>           { static constexpr bool value = true;  };

template< class T >
inline constexpr bool is_valid_v = is_valid<T>::value;

// -------------------------------------------------------------------------- //

template <class T>
constexpr int typeIndex(T&& val)
{
    using U = std::remove_cv_t<std::remove_reference_t<T>>;
    if constexpr (std::is_same_v<U, float>)
        return 0;
    else if constexpr (std::is_same_v<U, double>)
        return 1;
    else if constexpr (std::is_same_v<U, cfloat>)
        return 2;
    else if constexpr (std::is_same_v<U, cdouble>)
        return 3;
    else if constexpr (
        std::is_same_v<U, vector_ref>      ||
        // std::is_same_v<U, vector>          ||
        std::is_same_v<U, value_ref>       ||
        std::is_same_v<U, const_value_ref> ||
        std::is_same_v<U, value>
    )
        return val.typeIndex();
    else
        static_assert(always_false<T>::value, "typeIndex(): val is not a valid type!");
}

// These templates should handle all combinations of constants, value_refs, or
// values. We use the runtime type to work out which cast should be used.
// The smallest common type is preferred.
//
// NOTE: We use SFINAE to remove overload options for any type that is not on
// our predetermined list as well as to ensure we don't accidentally cover
// cases we shouldn't (e.g. cfloat + cfloat).
template <class LHS, class RHS>
inline constexpr
std::enable_if_t<
        is_valid_v<LHS> &&
        is_valid_v<RHS> &&
        (!is_simple_v<LHS> || !is_simple_v<RHS>),
    value >
operator+(LHS&& lhs, RHS&& rhs)
{
    int type = std::max(typeIndex(std::forward<LHS>(lhs)), typeIndex(std::forward<RHS>(rhs)));
    switch (type)
    {
        case 0: return value(compat_cast<float>  (std::forward<LHS>(lhs)) + compat_cast<float>  (std::forward<RHS>(rhs)));
        case 1: return value(compat_cast<double> (std::forward<LHS>(lhs)) + compat_cast<double> (std::forward<RHS>(rhs)));
        case 2: return value(compat_cast<cfloat> (std::forward<LHS>(lhs)) + compat_cast<cfloat> (std::forward<RHS>(rhs)));
        case 3: return value(compat_cast<cdouble>(std::forward<LHS>(lhs)) + compat_cast<cdouble>(std::forward<RHS>(rhs)));
        default: assert(false); return value(0.0);
    }
}

template <class LHS, class RHS>
inline constexpr
std::enable_if_t<
        is_valid_v<LHS> &&
        is_valid_v<RHS> &&
        (!is_simple_v<LHS> || !is_simple_v<RHS>),
    value >
operator-(LHS&& lhs, RHS&& rhs)
{
    int type = std::max(typeIndex(std::forward<LHS>(lhs)), typeIndex(std::forward<RHS>(rhs)));
    switch (type)
    {
        case 0: return value(compat_cast<float>  (std::forward<LHS>(lhs)) - compat_cast<float>  (std::forward<RHS>(rhs)));
        case 1: return value(compat_cast<double> (std::forward<LHS>(lhs)) - compat_cast<double> (std::forward<RHS>(rhs)));
        case 2: return value(compat_cast<cfloat> (std::forward<LHS>(lhs)) - compat_cast<cfloat> (std::forward<RHS>(rhs)));
        case 3: return value(compat_cast<cdouble>(std::forward<LHS>(lhs)) - compat_cast<cdouble>(std::forward<RHS>(rhs)));
        default: assert(false); return value(0.0);
    }
}

template <class LHS, class RHS>
inline constexpr
std::enable_if_t<
        is_valid_v<LHS> &&
        is_valid_v<RHS> &&
        (!is_simple_v<LHS> || !is_simple_v<RHS>),
    value >
operator*(LHS&& lhs, RHS&& rhs)
{
    int type = std::max(typeIndex(std::forward<LHS>(lhs)), typeIndex(std::forward<RHS>(rhs)));
    switch (type)
    {
        case 0: return value(compat_cast<float>  (std::forward<LHS>(lhs)) * compat_cast<float>  (std::forward<RHS>(rhs)));
        case 1: return value(compat_cast<double> (std::forward<LHS>(lhs)) * compat_cast<double> (std::forward<RHS>(rhs)));
        case 2: return value(compat_cast<cfloat> (std::forward<LHS>(lhs)) * compat_cast<cfloat> (std::forward<RHS>(rhs)));
        case 3: return value(compat_cast<cdouble>(std::forward<LHS>(lhs)) * compat_cast<cdouble>(std::forward<RHS>(rhs)));
        default: assert(false); return value(0.0);
    }
}

template <class LHS, class RHS>
inline constexpr
std::enable_if_t<
        is_valid_v<LHS> &&
        is_valid_v<RHS> &&
        (!is_simple_v<LHS> || !is_simple_v<RHS>),
    value >
operator/(LHS&& lhs, RHS&& rhs)
{
    int type = std::max(typeIndex(std::forward<LHS>(lhs)), typeIndex(std::forward<RHS>(rhs)));
    switch (type)
    {
        case 0: return value(compat_cast<float>  (std::forward<LHS>(lhs)) / compat_cast<float>  (std::forward<RHS>(rhs)));
        case 1: return value(compat_cast<double> (std::forward<LHS>(lhs)) / compat_cast<double> (std::forward<RHS>(rhs)));
        case 2: return value(compat_cast<cfloat> (std::forward<LHS>(lhs)) / compat_cast<cfloat> (std::forward<RHS>(rhs)));
        case 3: return value(compat_cast<cdouble>(std::forward<LHS>(lhs)) / compat_cast<cdouble>(std::forward<RHS>(rhs)));
        default: assert(false); return value(0.0);
    }
}

}

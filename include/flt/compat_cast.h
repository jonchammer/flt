#pragma once

#include "flt/complex_types.h"

namespace flt
{

// Allow for casting between complex numbers and real ones. The default template
// handles the sane cases, and other overloads (specified in other files) handle
// specific overloads.

template <class U>
constexpr U compat_cast(float x) { return U(x); }

template <class U>
constexpr U compat_cast(double x) { return U(x); }

template <class U>
constexpr U compat_cast(cfloat x) { return U(x); }

template<> constexpr float  compat_cast(cfloat x)  { return x.real(); }
template<> constexpr double compat_cast(cfloat x)  { return x.real(); }

template <class U>
constexpr U compat_cast(cdouble x) { return U(x); }

template<> constexpr float  compat_cast(cdouble x) { return x.real(); }
template<> constexpr double compat_cast(cdouble x) { return x.real(); }

}

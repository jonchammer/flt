#pragma once

#include <type_traits>
#include "flt/compat_cast.h"

namespace flt
{

// Represents the result of a [] operation on a flt::vector_ref or flt::vector.
// Can be used to read, write, and update any particular cell of the array.
class value_ref
{
public:
    constexpr value_ref(uint8_t* data, uint32_t index) :
        mData(data),
        mIndex(index)
    { }

    // We want value_ref objects to have reference semantics instead of pointer
    // semantics. Ideally, these constructors would function exactly as the
    // = operators defined below, but we can't have complete parity, since the
    // 'mData' field would not have a value. In our use case, these constructors
    // shouldn't be needed anyway, so it's easiest to just delete them.
    value_ref(const value_ref& other) = delete;
    value_ref(value_ref&& other)      = delete;

    // Casts the given cell to the desired type
    template <class T>
    constexpr T as() const
    {
        switch (mIndex)
        {
            case 0:  return compat_cast<T> (*(float*)   mData);
            case 1:  return compat_cast<T> (*(double*)  mData);
            case 2:  return compat_cast<T> (*(cfloat*)  mData);
            default: return compat_cast<T> (*(cdouble*) mData);
        }
    }

    // Class-specific assignment operators. Note that we can't use the default
    // implementation because we want to copy the 'value' from other into this
    // object. We don't want to copy the address.
    value_ref& operator=(const value_ref& other)
    {
        switch (mIndex)
        {
            case 0:  *(float*)   mData = other.as<float>();   break;
            case 1:  *(double*)  mData = other.as<double>();  break;
            case 2:  *(cfloat*)  mData = other.as<cfloat>();  break;
            default: *(cdouble*) mData = other.as<cdouble>(); break;
        }
        return *this;
    }

    value_ref& operator=(value_ref&& other)
    {
        switch (mIndex)
        {
            case 0:  *(float*)   mData = other.as<float>();   break;
            case 1:  *(double*)  mData = other.as<double>();  break;
            case 2:  *(cfloat*)  mData = other.as<cfloat>();  break;
            default: *(cdouble*) mData = other.as<cdouble>(); break;
        }
        return *this;
    }

    // Generic assignment operators. All values will be converted to the same
    // type as the parent, even if that conversion is lossy. This can be
    // especially eventful if 'val' has a complex type and the parent array has
    // a standard floating point type, as the imaginary component will be
    // effectively sliced out.
    template <class T>
    constexpr value_ref& operator=(T&& val)
    {
        switch (mIndex)
        {
            case 0:  *(float*)   mData = compat_cast<float>(val);   break;
            case 1:  *(double*)  mData = compat_cast<double>(val);  break;
            case 2:  *(cfloat*)  mData = compat_cast<cfloat>(val);  break;
            default: *(cdouble*) mData = compat_cast<cdouble>(val); break;
        }
        return *this;
    }

    template <class T>
    constexpr value_ref& operator+=(T&& val)
    {
        switch (mIndex)
        {
            case 0:  *(float*)   mData += compat_cast<float>(val);   break;
            case 1:  *(double*)  mData += compat_cast<double>(val);  break;
            case 2:  *(cfloat*)  mData += compat_cast<cfloat>(val);  break;
            default: *(cdouble*) mData += compat_cast<cdouble>(val); break;
        }
        return *this;
    }

    template <class T>
    constexpr value_ref& operator-=(T&& val)
    {
        switch (mIndex)
        {
            case 0:  *(float*)   mData -= compat_cast<float>(val);   break;
            case 1:  *(double*)  mData -= compat_cast<double>(val);  break;
            case 2:  *(cfloat*)  mData -= compat_cast<cfloat>(val);  break;
            default: *(cdouble*) mData -= compat_cast<cdouble>(val); break;
        }
        return *this;
    }

    template <class T>
    constexpr value_ref& operator*=(T&& val)
    {
        switch (mIndex)
        {
            case 0:  *(float*)   mData *= compat_cast<float>(val);   break;
            case 1:  *(double*)  mData *= compat_cast<double>(val);  break;
            case 2:  *(cfloat*)  mData *= compat_cast<cfloat>(val);  break;
            default: *(cdouble*) mData *= compat_cast<cdouble>(val); break;
        }
        return *this;
    }

    template <class T>
    constexpr value_ref& operator/=(T&& val)
    {
        switch (mIndex)
        {
            case 0:  *(float*)   mData /= compat_cast<float>(val);   break;
            case 1:  *(double*)  mData /= compat_cast<double>(val);  break;
            case 2:  *(cfloat*)  mData /= compat_cast<cfloat>(val);  break;
            default: *(cdouble*) mData /= compat_cast<cdouble>(val); break;
        }
        return *this;
    }

    // Returns the index of the currently active type
    constexpr uint32_t typeIndex() const
    {
        return mIndex;
    }

private:
    uint8_t* mData;
    uint32_t mIndex;
};

class const_value_ref
{
public:
    constexpr const_value_ref(uint8_t const* data, uint32_t index) :
        mData(data),
        mIndex(index)
    { }

    // Copy / move constructors are deleted
    const_value_ref(const const_value_ref& other) = delete;
    const_value_ref(const_value_ref&& other)      = delete;

    // Class-specific assignment operators are deleted
    const_value_ref& operator=(const const_value_ref& other) = delete;
    const_value_ref& operator=(const_value_ref&& other)      = delete;

    template <class T>
    constexpr T as() const
    {
        switch (mIndex)
        {
            case 0:  return compat_cast<T> (*(float const*)   mData);
            case 1:  return compat_cast<T> (*(double const*)  mData);
            case 2:  return compat_cast<T> (*(cfloat const*)  mData);
            default: return compat_cast<T> (*(cdouble const*) mData);
        }
    }

    // Returns the index of the currently active type
    constexpr uint32_t typeIndex() const
    {
        return mIndex;
    }

private:
    uint8_t const* mData;
    uint32_t mIndex;
};

// Specialization of compat_cast for value_ref objects
template <class U>
constexpr U compat_cast(const value_ref& x) { return x.as<U>(); }
template <class U>
constexpr U compat_cast(value_ref&& x) { return x.as<U>(); }

template <class U>
constexpr U compat_cast(const const_value_ref& x) { return x.as<U>(); }
template <class U>
constexpr U compat_cast(const_value_ref&& x) { return x.as<U>(); }

}

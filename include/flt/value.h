#pragma once

#include "flt/complex_types.h"
#include <cstring>

namespace flt
{

// This class represents the result of a temporary calculation or any other
// individual value that could be of any floating point type.
class value
{
public:

    // Creates a flt::value containing the given number
    value(float val) : mIndex(0)
    {
        // memset(mData, 0, sizeof(cdouble));
        memcpy(mData, &val, sizeof(float));
    }

    value(double val) : mIndex(1)
    {
        // memset(mData, 0, sizeof(cdouble));
        memcpy(mData, &val, sizeof(double));
    }

    value(cfloat val) : mIndex(2)
    {
        // memset(mData, 0, sizeof(cdouble));
        memcpy(mData, &val, sizeof(cfloat));
    }

    value(cdouble val) : mIndex(3)
    {
        // memset(mData, 0, sizeof(cdouble));
        memcpy(mData, &val, sizeof(cdouble));
    }

    // Returns the stored value casted to the desired type
    template <class T>
    constexpr T as() const
    {
        switch (mIndex)
        {
            case 0:  return compat_cast<T>(*(float*)   mData);
            case 1:  return compat_cast<T>(*(double*)  mData);
            case 2:  return compat_cast<T>(*(cfloat*)  mData);
            default: return compat_cast<T>(*(cdouble*) mData);
        }
    }

    // void print()
    // {
    //     for (int i = 0; i < sizeof(cdouble); ++i)
    //         std::cout << std::hex << (uint32_t) mData[i] << " ";
    //     std::cout << std::endl;
    // }

    // Returns the index of the currently active type
    constexpr uint32_t typeIndex() const
    {
        return mIndex;
    }

private:
    uint8_t mData[sizeof(cdouble)];
    uint32_t mIndex;
};

// Specialization of compat_cast for value objects
template <class U>
constexpr U compat_cast(const value& tmp) { return tmp.as<U>(); }
template <class U>
constexpr U compat_cast(value&& tmp) { return tmp.as<U>(); }

}

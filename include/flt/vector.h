#pragma once

#include <algorithm>
#include "flt/complex_types.h"
#include "flt/value_ref.h"

namespace flt
{

class vector
{
public:
    vector(size_t size, float val) :
        mData(new uint8_t[sizeof(float) * size]),
        mSize(size),
        mStride(sizeof(float)),
        mIndex(0)
    {
        std::fill((float*) mData, (float*) mData + size, val);
    }

    vector(size_t size, double val) :
        mData(new uint8_t[sizeof(double) * size]),
        mSize(size),
        mStride(sizeof(double)),
        mIndex(1)
    {
        std::fill((double*) mData, (double*) mData + size, val);
    }

    vector(size_t size, cfloat val) :
        mData(new uint8_t[sizeof(cfloat) * size]),
        mSize(size),
        mStride(sizeof(cfloat)),
        mIndex(2)
    {
        std::fill((cfloat*) mData, (cfloat*) mData + size, val);
    }

    vector(size_t size, cdouble val) :
        mData(new uint8_t[sizeof(cdouble) * size]),
        mSize(size),
        mStride(sizeof(cdouble)),
        mIndex(3)
    {
        std::fill((cdouble*) mData, (cdouble*) mData + size, val);
    }

    ~vector()
    {
        delete[] mData;
    }

    constexpr value_ref operator[](const size_t index)
    {
        return value_ref(mData + index * mStride, mIndex);
    }

    constexpr const_value_ref operator[](const size_t index) const
    {
        return const_value_ref(mData + index * mStride, mIndex);
    }

    constexpr size_t size() const
    {
        return mSize;
    }

    // Returns the index of the currently active type
    constexpr uint32_t typeIndex() const
    {
        return mIndex;
    }

private:
    uint8_t* mData;
    size_t mSize;
    uint32_t mStride;
    uint32_t mIndex;
};

}

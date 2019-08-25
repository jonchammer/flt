#pragma once

#include <vector>
#include "flt/value_ref.h"

namespace flt
{

class vector_ref
{
public:
    vector_ref(std::vector<float>& src)  :
        mData((uint8_t*) src.data()),
        mSize(src.size()),
        mStride(sizeof(float)),
        mIndex(0)
    {}

    vector_ref(std::vector<double>& src) :
        mData((uint8_t*) src.data()),
        mSize(src.size()),
        mStride(sizeof(double)),
        mIndex(1)
    {}

    vector_ref(std::vector<cfloat>& src)  :
        mData((uint8_t*) src.data()),
        mSize(src.size()),
        mStride(sizeof(cfloat)),
        mIndex(2)
    {}

    vector_ref(std::vector<cdouble>& src) :
        mData((uint8_t*) src.data()),
        mSize(src.size()),
        mStride(sizeof(cdouble)),
        mIndex(3)
    {}

    value_ref operator[](const size_t index)
    {
        return value_ref(mData + index * mStride, mIndex);
    }

    const_value_ref operator[](const size_t index) const
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

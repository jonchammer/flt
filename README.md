## FLT
Flt is a small header-only library designed to abstract vectors of floats,
doubles, complex floats, and complex doubles so they can be used with a common
type-erased interface.

Useful typedefs:
* `flt::cfloat`  - Maps to `std::complex<float>`
* `flt::cdouble` - Maps to `std::complex<double>`

Important types:
* `flt::vector` - Similar to `std::vector<float>`, `std::vector<double>`,
`std::vector<flt::cfloat>`, and `std::vector<flt::cdouble>`. This class manages
its own memory, but doesn't provide most of the std::vector interface.
* `flt::vector_ref` - Type erased wrapper for a floating point vector. Changes
made to the wrapper affect the underlying std::vector, as they share the same
data in memory. Think of this as a `std::vector<T>&`.

## Example Usage
```c++
#include <vector>
#include "flt/flt.h"

std::vector<double> v1(16, 3.0);
std::vector<float> v2(16, 2.0f);

flt::vector_ref v1Ref(v1);
flt::vector_ref v2Ref(v2);

v1Ref[0] = v1Ref[0] * 5.0f + v2Ref[0];
std::cout << v1Ref[0].as<double>() << std::endl;
```

## Building
The library is header only. Simply include the entire `/include` directory to
get started. It can also be 'built' as a CMake interface library for inclusion
with other projects.

The `/tests` directory contains several function and performance tests that
ensure correctness and speed.

## Why?
When used correctly, flt's primitives add no additional overhead to std::vectors,
but they can be used without templates, which is desirable in certain situations.
That being said, when the underlying type of the container is difficult or
impossible to determine at compile time, the generated assembly can be very
suboptimal, leading to poor performance. In particular, writing functions that
take `flt::vector` or `flt::vector_ref` objects (either by value or reference)
will likely produce poor results, though the actual assembly should be examined
if possible.

Ultimately, this project was a fun thought experiment and may not provide much
real world value, but the techniques used for implementation may be helpful for
some other use case. :-)

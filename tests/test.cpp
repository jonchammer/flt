// #include <vector>
// #include <complex>
// #include <iostream>
// #include <cassert>
// #include <chrono>
// #include <cmath>

#include <iostream>
#include "flt/flt.h"

// Returns the current system time (UNIX timestamp) in seconds with millisecond
// precision.
inline double currentTimeSeconds()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count() / 1000.0;
}

// Floating point equality comparison macro
#define ASSERT_EQUAL(v1, v2) assert(std::abs((v1) - (v2)) < 1E-8)

void testSimpleAssignment()
{
    // Create some test data
    flt::vector vec(5, 0.1);

    // Replace an element with a flt::value_ref&&
    vec[0] = vec[1];
    ASSERT_EQUAL(vec[0].as<double>(), 0.1);

    // Replace an element with a flt::value_ref&
    auto proxy = vec[1];
    vec[0]     = proxy;
    ASSERT_EQUAL(vec[0].as<double>(), 0.1);

    // Replace an element with a constant
    vec[0] = 5.0;
    ASSERT_EQUAL(vec[0].as<double>(), 5.0);

    // Replace an element with an flt::value
    vec[0] = vec[1] + vec[2];
    ASSERT_EQUAL(vec[0].as<double>(), 0.2);

    // Replace an element with a more complicated expression involving constants,
    // flt::value_refs, and flt::values.
    vec[0] = vec[1] + (vec[2] * 2.0);
    ASSERT_EQUAL(vec[0].as<double>(), 0.3);

    // All changes should be reflected in the original vector
    ASSERT_EQUAL(vec[0].as<double>(), 0.3);
    ASSERT_EQUAL(vec[1].as<double>(), 0.1);
    ASSERT_EQUAL(vec[2].as<double>(), 0.1);
    ASSERT_EQUAL(vec[3].as<double>(), 0.1);
    ASSERT_EQUAL(vec[4].as<double>(), 0.1);

    std::cout << "Simple Assignment - Pass" << std::endl;
}

void testSimpleAssignmentRef()
{
    using namespace flt;

    // Create some test data
    std::vector<double> orig {0.1, 0.2, 0.3, 0.4, 0.5};
    flt::vector_ref vec(orig);

    // Replace an element with a flt::value_ref&&
    vec[0] = vec[1];
    ASSERT_EQUAL(vec[0].as<double>(), 0.2);

    // // Replace an element with a flt::value_ref&
    auto proxy = vec[1];
    vec[0]     = proxy;
    ASSERT_EQUAL(vec[0].as<double>(), 0.2);

    // Replace an element with a constant
    vec[0] = 5.0;
    ASSERT_EQUAL(vec[0].as<double>(), 5.0);

    // Replace an element with an flt::value
    vec[0] = vec[1] + vec[2];
    ASSERT_EQUAL(vec[0].as<double>(), 0.5);

    // Replace an element with a more complicated expression involving constants,
    // flt::value_refs, and flt::values.
    vec[0] = vec[1] + (vec[2] * 2.0);
    ASSERT_EQUAL(vec[0].as<double>(), 0.8);

    // All changes should be reflected in the original vector
    ASSERT_EQUAL(orig[0], 0.8);
    ASSERT_EQUAL(orig[1], 0.2);
    ASSERT_EQUAL(orig[2], 0.3);
    ASSERT_EQUAL(orig[3], 0.4);
    ASSERT_EQUAL(orig[4], 0.5);

    std::cout << "Simple Assignment Ref - Pass" << std::endl;
}

void testCompoundAssignment()
{
    // Create some test data
    flt::vector vec(5, 0.1);

    // Addition
    double sum = vec[0].as<double>() + vec[1].as<double>();
    vec[0]    += vec[1];
    ASSERT_EQUAL(vec[0].as<double>(), sum);

    // Subtraction
    double diff = vec[0].as<double>() - vec[1].as<double>();
    vec[0]     -= vec[1];
    ASSERT_EQUAL(vec[0].as<double>(), diff);

    // Multiplication
    double product = vec[0].as<double>() * vec[1].as<double>();
    vec[0]        *= vec[1];
    ASSERT_EQUAL(vec[0].as<double>(), product);

    // Division
    double quotient = vec[0].as<double>() / vec[1].as<double>();
    vec[0]         /= vec[1];
    ASSERT_EQUAL(vec[0].as<double>(), quotient);

    std::cout << "Compound Assignment - Pass" << std::endl;
}

void testCompoundAssignmentRef()
{
    // Create some test data
    std::vector<double> orig {0.1, 0.2, 0.3, 0.4, 0.5};
    flt::vector_ref vec(orig);

    // Addition
    double sum = orig[0] + orig[1];
    vec[0]    += vec[1];
    ASSERT_EQUAL(vec[0].as<double>(), sum);

    // Subtraction
    double diff = orig[0] - orig[1];
    vec[0]     -= vec[1];
    ASSERT_EQUAL(vec[0].as<double>(), diff);

    // Multiplication
    double product = orig[0] * orig[1];
    vec[0]        *= vec[1];
    ASSERT_EQUAL(vec[0].as<double>(), product);

    // Division
    double quotient = orig[0] / orig[1];
    vec[0]         /= vec[1];
    ASSERT_EQUAL(vec[0].as<double>(), quotient);

    std::cout << "Compound Assignment Ref - Pass" << std::endl;
}

void testBinaryOps()
{
    std::vector<double> orig {0.1, 0.2, 0.3, 0.4, 0.5};
    flt::vector_ref vec(orig);

    // Addition
    auto sum = vec[0] + vec[1];
    static_assert(std::is_same_v<decltype(sum), flt::value>, "Sum is not a flt::value");
    ASSERT_EQUAL(sum.as<double>(), (orig[0] + orig[1]));

    // Subtraction
    auto diff = vec[0] - vec[1];
    static_assert(std::is_same_v<decltype(diff), flt::value>, "Diff is not a flt::value");
    ASSERT_EQUAL(diff.as<double>(), (orig[0] - orig[1]));

    // Multiplication
    auto product = vec[0] * vec[1];
    static_assert(std::is_same_v<decltype(product), flt::value>, "Product is not a flt::value");
    ASSERT_EQUAL(product.as<double>(), (orig[0] * orig[1]));

    // Division
    auto quotient = vec[0] / vec[1];
    static_assert(std::is_same_v<decltype(quotient), flt::value>, "Quotient is not a flt::value");
    ASSERT_EQUAL(quotient.as<double>(), (orig[0] / orig[1]));

    std::cout << "Binary Ops - Pass" << std::endl;
}

void testTypeConversions()
{
    using namespace flt;

    static_assert(  std::is_constructible_v< vector_ref, std::vector<float>& >,   "Construction error" );
    static_assert(  std::is_constructible_v< vector_ref, std::vector<double>& >,  "Construction error" );
    static_assert(  std::is_constructible_v< vector_ref, std::vector<cfloat>& >,  "Construction error" );
    static_assert(  std::is_constructible_v< vector_ref, std::vector<cdouble>& >, "Construction error" );
    static_assert( !std::is_constructible_v< vector_ref, std::vector<int>& >,     "Construction error" );

    std::vector<float>   floats {0.1f, 0.2f, 0.3f, 0.4f, 0.5f};
    std::vector<double>  doubles {0.1, 0.2, 0.3, 0.4, 0.5};
    std::vector<cfloat>  cfloats {{0.1f, 0.1f}, {0.2f, 0.2f}, {0.3f, 0.3f}, {0.4f, 0.4f}, {0.5f, 0.5f}};
    std::vector<cdouble> cdoubles {{0.1, 0.1}, {0.2, 0.2}, {0.3, 0.3}, {0.4, 0.4}, {0.5, 0.5}};

    vector_ref fref  = vector_ref(floats);
    vector_ref dref  = vector_ref(doubles);
    vector_ref cfref = vector_ref(cfloats);
    vector_ref cdref = vector_ref(cdoubles);

    // Assignment with each type
    fref[0] = 0.1f;
    ASSERT_EQUAL(fref[0].as<float>(), 0.1f);
    fref[0] = 0.1;
    ASSERT_EQUAL(fref[0].as<float>(), 0.1f);
    fref[0] = flt::cfloat(0.1f, 0.1f);
    ASSERT_EQUAL(fref[0].as<float>(), 0.1f);
    fref[0] = flt::cdouble(0.1, 0.1);
    ASSERT_EQUAL(fref[0].as<float>(), 0.1f);

    dref[0] = 0.1f;
    ASSERT_EQUAL(dref[0].as<double>(), 0.1);
    dref[0] = 0.1;
    ASSERT_EQUAL(dref[0].as<double>(), 0.1);
    dref[0] = flt::cfloat(0.1f, 0.1f);
    ASSERT_EQUAL(dref[0].as<double>(), 0.1);
    dref[0] = flt::cdouble(0.1, 0.1);
    ASSERT_EQUAL(dref[0].as<double>(), 0.1);

    cfref[0] = 0.1f;
    ASSERT_EQUAL(cfref[0].as<cfloat>(), cfloat(0.1f, 0.0f));
    cfref[0] = 0.1;
    ASSERT_EQUAL(cfref[0].as<cfloat>(), cfloat(0.1f, 0.0f));
    cfref[0] = cfloat(0.1f, 0.0f);
    ASSERT_EQUAL(cfref[0].as<cfloat>(), cfloat(0.1f, 0.0f));
    cfref[0] = cdouble(0.1, 0.0);
    ASSERT_EQUAL(cfref[0].as<cfloat>(), cfloat(0.1f, 0.0f));

    cdref[0] = 0.1f;
    ASSERT_EQUAL(cdref[0].as<cdouble>(), cdouble(0.1, 0.0));
    cdref[0] = 0.1;
    ASSERT_EQUAL(cdref[0].as<cdouble>(), cdouble(0.1, 0.0));
    cdref[0] = cfloat(0.1f, 0.0f);
    ASSERT_EQUAL(cdref[0].as<cdouble>(), cdouble(0.1, 0.0));
    cdref[0] = cdouble(0.1, 0.0);
    ASSERT_EQUAL(cdref[0].as<cdouble>(), cdouble(0.1, 0.0));

    std::cout << "Type Conversions - Pass" << std::endl;
}

void performanceTest()
{
    auto m = [](const auto& b, const auto& a, const auto& x, auto& y) constexpr
    {
        const int N = (int) b.size();
        const int M = (int) a.size();
        const int P = (int) x.size();

        for (int i = 0; i < P; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                if (i - j >= 0)
                    y[i] += b[j] * x[i - j];
            }

            for (int j = 0; j < M; ++j)
            {
                if (i - j - 1 >= 0)
                    y[i] -= a[j] * y[i - j - 1];
            }
        }
    };

    std::vector<double> b(20, 0.1);
    std::vector<double> a(19, 0.1);
    std::vector<double> x(1024);
    std::vector<double> y(1024);
    for (size_t i = 0; i < x.size(); ++i)
        x[i] = i;

    flt::vector_ref bRef(b);
    flt::vector_ref aRef(a);
    flt::vector_ref xRef(x);
    flt::vector_ref yRef(y);

    const int ITERATIONS = 1000;

    // Profile using std::vector
    double start = currentTimeSeconds();
    for (int i = 0; i < ITERATIONS; ++i)
    {
        m(b, a, x, y);
        std::fill(y.begin(), y.end(), 0.0);
    }
    double vecTime = (currentTimeSeconds() - start) / ITERATIONS;

    // Profile using flt::vector_ref
    start = currentTimeSeconds();
    for (int i = 0; i < ITERATIONS; ++i)
    {
        m(bRef, aRef, xRef, yRef);
        std::fill(y.begin(), y.end(), 0.0);
    }
    double fltTime = (currentTimeSeconds() - start) / ITERATIONS;

    std::cout << "Optimized (ms): " << 1000.0 * vecTime << std::endl;
    std::cout << "Flt (ms):       " << 1000.0 * fltTime << std::endl;
    std::cout << "% Difference:   " << 100.0 * (vecTime - fltTime) / vecTime  << std::endl;
    std::cout << "Multiplier:     " << fltTime / vecTime << std::endl;
}

int main()
{
    testSimpleAssignment();
    testSimpleAssignmentRef();
    testCompoundAssignment();
    testCompoundAssignmentRef();
    testBinaryOps();
    testTypeConversions();

    performanceTest();
    return 0;
}

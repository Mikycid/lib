#ifndef UNROLL_LOOPS_MULTI_COMPILER
#define UNROLL_LOOPS_MULTI_COMPILER
    /// Helper macros for stringification
    #define TO_STRING_HELPER(X)   #X
    #define TO_STRING(X)          TO_STRING_HELPER(X)

    // Define loop unrolling depending on the compiler
    #if defined(__ICC) || defined(__ICL)
    #define UNROLL_LOOP(n)      _Pragma(TO_STRING(unroll (n)))
    #elif defined(__clang__)
    #define UNROLL_LOOP(n)      _Pragma(TO_STRING(unroll (n)))
    #elif defined(__GNUC__) && !defined(__clang__)
    #define UNROLL_LOOP(n)      _Pragma(TO_STRING(GCC unroll (n)))
    #elif defined(_MSC_BUILD)
    #pragma message ("Microsoft Visual C++ (MSVC) detected: Loop unrolling not supported!")
    #define UNROLL_LOOP(n)
    #else
    #warning "Unknown compiler: Loop unrolling not supported!"
    #define UNROLL_LOOP(n)
    #endif

    // USAGE : UNROLL_LOOP(n) before a loop

#endif
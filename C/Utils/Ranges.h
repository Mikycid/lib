#ifndef DEFINE_C_RANGES
#define DEFINE_C_RANGES

#include <C/Utils/MinMax.h>
#include <C/Performance/UnrollLoops.h>
#include <C/ThreadPool.h>

typedef enum RangeFlags {
    INCLUDE_OUTER = 1 << 0,
    EXCLUDE_INNER = 1 << 1,
    LENGHT = 1 << 2
} RangeFlags;

static int *range(const int min, const int max, const int step, const RangeFlags flags)
{
    static size_t length = 0;
    const int realMin = min(min, max);
    const int realMax = max(min, max);
    int *result = (int *) malloc((realMax - realMin + 1) * sizeof(int));

    UNROLL_LOOP(16)
    for(int i = realMin; i < realMax; i += abs(step))
    {
        length += 1;
        *result = i;
        result++;
    }
    result -= length;
    return result;
    
}

static void fastRange(const int min, const int max, const int step, const RangeFlags flags, int *dest, size_t destSize)
{
    size_t length = 0;
    const int realMin = min(min, max);
    const int realMax = max(min, max);
    const int realStep = abs(step);

    UNROLL_LOOP(16)
    for(int i = realMin; i < realMax; i += realStep)
    {
        dest[length] = i;
        length += 1;
    }
    
}

static void MTRange(const int min, const int max, const int step, const RangeFlags flags, int *dest, size_t destSize, ThreadPool *pool)
{
    size_t length = 0;
    const int realMin = min(min, max);
    const int realMax = max(min, max);
    const int realStep = abs(step);

    UNROLL_LOOP(16)
    for(int i = realMin; i < realMax; i += realStep)
    {
        dest[length] = i;
        length += 1;
    }
    
}

#endif
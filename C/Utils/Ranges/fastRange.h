#ifndef DEF_C_FASTRANGE
#define DEF_C_FASTRANGE

#include <C/Utils/MinMax.h>
#include <C/Performance/UnrollLoops.h>
#include <stdint.h>

#ifndef DEF_RangeFlags_ENUM 
#define DEF_RangeFlags_ENUM 
    typedef enum RangeFlags {
        INCLUDE_OUTER = 1 << 0,
        EXCLUDE_FIRST = 1 << 1,
        LENGHT = 1 << 2
    } RangeFlags;
#endif

static void fastRange(uint32_t min, uint32_t max, const uint32_t step, const RangeFlags flags, uint32_t *dest)
{
    // To use fastRange, you have to provide an array of integers which can contain the range, don't forget to fix size + 1 if you use INCLUDE_OUTER
    if(flags & INCLUDE_LAST)
    {
        if(max < min){
            max--;
        } else {
            max++;
        }
    }
    if(flags & EXCLUDE_FIRST)
    {
        if(min < max)
        {
            min++;
        }
        else
        {
            min--;
        }
    }
    size_t length;
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
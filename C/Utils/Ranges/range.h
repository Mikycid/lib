#ifndef DEF_C_RANGE
#define DEF_C_RANGE


#include <C/Utils/MinMax.h>
#include <C/Performance/UnrollLoops.h>
#include <stdint.h>

#ifndef DEF_RangeFlags_ENUM  
#define DEF_RangeFlags_ENUM 
    typedef enum RangeFlags {
        INCLUDE_LAST = 1 << 0,
        EXCLUDE_FIRST = 1 << 1,
        LENGHT = 1 << 2
    } RangeFlags;
#endif

static uint32_t *range(uint32_t min, uint32_t max, const uint32_t step, const RangeFlags flags)
{
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
    size_t length = 0;
    const uint32_t realMin = min(min, max);
    const uint32_t realMax = max(min, max);
    const uint32_t diff = realMax - realMin;
    uint32_t *result = (uint32_t *) malloc((realMax - realMin + 1) * sizeof(uint32_t));

    UNROLL_LOOP(16)
    for(int i = realMin; i < realMax; i += abs(step))
    {
        result[length] = i;

        length++;
    }
    return result;
    
}

#endif
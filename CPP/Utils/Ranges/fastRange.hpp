#ifndef DEF_CPP_FASTRANGE
#define DEF_CPP_FASTRANGE

#include <cstdint>
#include <cstddef>
#include <C/Performance/UnrollLoops.h>
#include <CPP/Utils/TemplateAbs.hpp>

namespace Utils {
    #ifndef DEF_RangeFlags_ENUM  
    #define DEF_RangeFlags_ENUM 
        enum RangeFlags {
            INCLUDE_LAST = 1 << 0,
            EXCLUDE_FIRST = 1 << 1,
            NONE = 1 << 2
        };
    #endif

    template<typename T>
    constexpr void fastRange(T &&min, T &&max, const T &__restrict__ step, const RangeFlags flags, int *__restrict__ dest) noexcept
    {
        // Only use if you know the array length at compile time

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
        const T realMin = std::min(min, max);
        const T realMax = std::max(min, max);

        UNROLL_LOOP(16)
        for(T i = realMin; i < realMax; i += Utils::abs(step))
        {
            dest[length] = i;
            length++;
        }
        
    }
};


#endif
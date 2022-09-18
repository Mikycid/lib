#ifndef DEF_CPP_RANGE
#define DEF_CPP_RANGE

#include <array>
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


    template<typename T, size_t size>
    constexpr auto range(T &&__restrict__ min, T &&__restrict__ max, const T &__restrict__ step, const RangeFlags flags) noexcept -> std::array<T, size>
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
        const T realMin = std::min(min, max);
        const T realMax = std::max(min, max);
        std::array<T, size> result;

        UNROLL_LOOP(16)
        for(T i = realMin; i < realMax; i += Utils::abs(step))
        {
            result[length] = i;
            length++;
        }
        return result;
        
    }
};


#endif
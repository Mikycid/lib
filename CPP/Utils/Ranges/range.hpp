#ifndef DEF_CPP_RANGE
#define DEF_CPP_RANGE

#include <vector>
#include <cstdint>
#include <cstddef>
#include <cmath>
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
    constexpr auto range(T min, T max, const uint32_t &__restrict__ step = 1, const RangeFlags flags = NONE) noexcept -> std::vector<T>
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
        
        std::vector<T> result;
        result.resize(std::round(Utils::abs((realMax - realMin) / step)));


        UNROLL_LOOP(16)
        for(auto &val : result)
        {
            //T i = realMin; i < realMax; i += Utils::abs(step)
            val = length * step + realMin;
            length++;
        }
        return result;
        
    }
};


#endif
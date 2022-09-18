#ifndef DEF_TEMPLATE_ABS
#define DEF_TEMPLATE_ABS

namespace Utils
{
    template <typename T> 
    constexpr auto abs(const T &__restrict__ v) noexcept -> T  
    { 
        return v < 0 ? -v : v; 
    }
}

#endif
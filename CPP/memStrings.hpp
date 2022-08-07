#ifndef DEF_CUSTOM_STRINGS_CPP
#define DEF_CUSTOM_STRINGS_CPP

#include <cstring>
#include <cstdlib>
#include <cstdio>

/*
    This lib makes it easy to do string operation on char type.
    Everything allocated dynamically gets stored in an array, and can be freed whenever needed
    just by calling freeAll()
    Every function called using dynamic allocation is not freed, you have to call the freeAll() when you no longer need the memory.
    You can also use freeOne() if you want to free just one variable.
    Never free yourself without one of those funcs or it'll result in an unpredictable behaviour.

    List of dynamic allocation methods :
        - dynamicSplit
        - assign

    List of non-dynamic methods :
        - split
*/
namespace memStrings {
    #include <C/memStrings.h>  
};


#endif
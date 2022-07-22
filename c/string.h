#ifndef DEF_CUSTOM_STRINGS
#define DEF_CUSTOM_STRINGS

#include <string.h>

void split(char *__restrict__ src, size_t sizeDest, char dest[][sizeDest], size_t size, char* __restrict__ sep, size_t sepSize)
{
    // Split a string by sep, insert the result into a 2D array
    char current[size];
    memset(current, 0, size);
    size_t nFound = 0, nSplit = 0, nMinus = 0, inc = 1;
    for(size_t i = 0; i < strlen(src); i++)
    {
        current[i - nMinus] = src[i];
        if(current[i - nMinus] == sep[nFound])
        {
            nFound++;
            if(nFound == sepSize - 1)
            {
                memset(dest[nSplit], 0, sizeDest);
                memcpy(dest[nSplit], current, i - nMinus - nFound);
                memset(current, 0, size);
                nMinus = i + 1;
                nSplit++;
                inc = 0;
                nFound = 0;
            }
        } else {
            nFound = 0;
        }

    }

    memset(dest[nSplit], 0, sizeDest);
    memcpy(dest[nSplit], current, size);
}

#endif
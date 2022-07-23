#ifndef DEF_CUSTOM_STRINGS
#define DEF_CUSTOM_STRINGS

#include <string.h>

char **dynamicSplit(char *__restrict__ src, char* __restrict__ sep, size_t sepSize)
{
    // Split a string by sep, return a 2D array with the result
    size_t size = strlen(src);
    char current[size];
    int x = 5;
    char **result = (char**) malloc(x * sizeof(char*));

    memset(current, 0, size);
    size_t nFound = 0, nSplit = 0, nMinus = 0, maxStringSize = 0;
    size_t *sizeOfEach = (size_t *) malloc(x * sizeof(size_t));
    for(size_t i = 0; i < size; i++)
    {
        current[i - nMinus] = src[i];
        if(current[i - nMinus] == sep[nFound])
        {
            nFound++;
            if(nFound == sepSize - 1)
            {
                if(nSplit > x)
                {
                    x *= 2;
                    result = realloc(result, x * sizeof(char*));
                    sizeOfEach = realloc(sizeOfEach, x * sizeof(size_t));
                }

                size_t sizeOfCurrent = i - nMinus - nFound;
                sizeOfEach[nSplit] = sizeOfCurrent;
                if(sizeOfCurrent > maxStringSize)
                    maxStringSize = sizeOfCurrent;
                result[nSplit] = (char*) malloc(sizeOfCurrent * sizeof(char));
                memcpy(result[nSplit], current, sizeOfCurrent);
                memset(current, 0, sizeOfCurrent);
                nMinus = i + 1;
                nSplit++;
                nFound = 0;
            }
        } else {
            nFound = 0;
        }
        if(i == strlen(src) - 1)
        {
            if(nSplit > x)
            {
                x += 1;
                result = realloc(result, x);
            }
            result[nSplit] = (char*) malloc((i - nMinus) * sizeof(char));
            memcpy(result[nSplit], current, i - nMinus);
        }

    }
    
    char resultStack[nSplit + 1][maxStringSize];
    for(size_t i = 0; i < nSplit + 1; i++)
    {
        memcpy(resultStack[i], result[i], sizeOfEach[i]);
    }

    return result;
}

void split(char *__restrict__ src, size_t sizeDest, char dest[][sizeDest], size_t size, char* __restrict__ sep, size_t sepSize)
{
    // Split a string by sep, insert the result into a 2D array
    char current[size];

    memset(current, 0, size);
    size_t nFound = 0, nSplit = 0, nMinus = 0;

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
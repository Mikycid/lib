#ifndef DEF_CUSTOM_STRINGS_CPP
#define DEF_CUSTOM_STRINGS_CPP

#include <cstring>
#include <cstdlib>

char **dynamicSplit(char *__restrict__ src, char* __restrict__ sep, size_t sepSize)
{
    // Split a string by sep, return a 2D array with the result
    size_t size = strlen(src);
    char current[size];
    size_t x = 5;

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
                if(nSplit >= x)
                {
                    x *= 2;
                    result = (char **)realloc(result, x * sizeof(char*));
                    sizeOfEach = (size_t *)realloc(sizeOfEach, x * sizeof(size_t));
                }

                size_t sizeOfCurrent = i - nMinus - nFound + 1;
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
                result = (char **)realloc(result, x * sizeof(char*));
            }
            result[nSplit] = (char*) malloc((i - nMinus + 1) * sizeof(char));
            memcpy(result[nSplit], current, i - nMinus + 1);
        }

    }
    
    char resultStack[nSplit + 1][maxStringSize];
    for(size_t i = 0; i < nSplit + 1; i++)
    {
        memcpy(resultStack[i], result[i], sizeOfEach[i]);
    }

    return result;
}

template<size_t sizeDest>
void split(char *__restrict__ src, char dest[][sizeDest], size_t size, char* __restrict__ sep, size_t sepSize)
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
                memcpy(dest[nSplit], current, i - nMinus - nFound + 1);
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
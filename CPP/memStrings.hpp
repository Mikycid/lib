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
    static long getIndex(char *__restrict__ ptr, void **__restrict__ arr, size_t arrLength);
    static void reIndex(void **arr, size_t arrLength, size_t index);
    static int sum(int *arr, size_t arrLength);
    static void assign(char *__restrict__ src, char **__restrict__ dest);
    static void assign(const char *__restrict__ src, char **__restrict__ dest);

    static void store(char **str = nullptr, char **strArr = nullptr, size_t strArrLength = 0, bool free_p = false, char *oldAddress = nullptr, void *freeOne = nullptr)
    {
        static char ***storageArr;
        static char **storage;
        static size_t storageLengthX, storageLength;
        static size_t *yLength;

        if(storageLength && str && oldAddress)
        {
            long index = getIndex(oldAddress, (void**)storage, storageLength);
            if(index >= 0)
            {
                free(storage[index]);
                reIndex((void**)storage, storageLength, index);
                storageLength--;
            }
        }

        if(freeOne)
        {
            char strPtr[32];
            sprintf(strPtr, "%p", freeOne);
            long index = getIndex(strPtr, (void**)storage, storageLength);

            if(index >= 0)
            {
                free(storage[index]);
                reIndex((void**)storage, storageLength, index);
                storageLength--;
            }
            else 
            {
                index = getIndex(strPtr, (void**)storageArr, storageLengthX);
                if(index >= 0)
                {
                    for(size_t i = 0; i < yLength[index]; i++)
                    {
                        free(storageArr[index][i]);
                    }
                    free(storageArr[index]);
                    reIndex((void**)yLength, storageLengthX, index);
                    reIndex((void**)storageArr, storageLengthX, index);
                    storageLengthX--;
                }
            }
        }

        if(!free_p)
        {
            if(str && !storageLength)
            {
                storage = (char **) malloc(sizeof(char*));
                storageLength = 0;
            }
            if(str && storageLength)
            {
                storage = (char **) realloc(storage, sizeof(char*) * storageLength + 1);
            }
            if(str)
            {
                //storage[storageLength] = (char*) malloc(strlen(*str) * sizeof(char));
                //memset(storage[storageLength], 0, strlen(*str) * sizeof(char));
                storage[storageLength] = *str;
                storageLength++;
            }
                
            if(strArr && !storageLengthX)
            {
                storageArr = (char ***) malloc(sizeof(char**));
                yLength = (size_t *) malloc(sizeof(size_t));
                storageLengthX = 0;
            }

            if(strArr && storageLengthX)
            {
                storageArr = (char ***) realloc(storageArr, (storageLengthX + 1) * sizeof(char**));

                if(storageArr == nullptr)
                    printf("Failed to allocate");
                yLength = (size_t *) realloc(yLength, (storageLengthX + 1) * sizeof(size_t));
            }

            if(strArr)
            {
                
                //storageArr[storageLengthX] = (char**) malloc(strArrLength * sizeof(char*));
                storageArr[storageLengthX] = strArr;
                yLength[storageLengthX] = strArrLength;

                
                storageLengthX += 1;
            }

            

        }
        else
        {
            for(size_t i = 0; i < storageLength; i++)
            {
                if(storage[i])
                    free(storage[i]);
            }
            for(size_t i = 0; i < storageLengthX; i++)
            {
                if(storageArr[i])
                {
                    for(size_t j = 0; j <= yLength[i]; j++)
                    {
                        if(storageArr[i][j])
                            free(storageArr[i][j]);
                    }
                    free(storageArr[i]);
                }
                
                    
            }
            
            if(storageLength)
            {
                free(storage);
            }
            if(storageLengthX)
            {
                free(storageArr);

            }
            if(storageLengthX)
            {

                free(yLength);
            }
            storageArr = nullptr;
            storage = nullptr;
            storageLengthX = 0, storageLength = 0;
            yLength = nullptr;
        }
        
    }

    static void freeAll()
    {
        store(nullptr, nullptr, 0, true);
    }

    static void freeOne(void *ptr)
    {
        store(nullptr, nullptr, 0, false, nullptr, ptr);
    }

    static char **dynamicSplit(char *__restrict__ src, char* __restrict__ sep, size_t sepSize)
    {
        // Split a string by sep, return a 2D array with the result
        size_t size = strlen(src);
        char current[size];
        size_t x = 5;

        char **result = (char**) malloc(x * sizeof(char*));
        memset(current, 0, size);

        size_t nFound = 0, nSplit = 0, maxStringSize = 0;
        int nMinus = 0;
        int finalSize = x;

        for(int i = 0; i < size; i++)
        {
            current[i - nMinus] = src[i];
            if(current[i - nMinus] == sep[nFound])
            {
                nFound++;
                if(nFound == sepSize - 1)
                {
                    if(nSplit >= x)
                    {
                        finalSize = x;
                        x *= 2;
                        result = (char **)realloc(result, x * sizeof(char*));
                    }

                    int sizeOfCurrent = i - nMinus - nFound + 1;
                    if(sizeOfCurrent <= 0)
                    {
                        result[nSplit] = (char*) malloc(sizeof(char));
                        
                        memset(result[nSplit], 0, sizeof(char));
                        memcpy(result[nSplit], current, sizeOfCurrent);


                    }
                    else
                    {
                        if(sizeOfCurrent > maxStringSize)
                            maxStringSize = sizeOfCurrent;

                        result[nSplit] = (char*) malloc(sizeOfCurrent);
                        memset(result[nSplit], 0, sizeOfCurrent + 1);
                        memcpy(result[nSplit], current, sizeOfCurrent);
                        
                    }
                    memset(current, 0, sizeof(current));
                    nMinus = i + 1;
                    nSplit++;

                    nFound = 0;
                    finalSize--;
                    
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

                memset(result[nSplit], 0, i - nMinus + 2);
                memcpy(result[nSplit], current, i - nMinus + 1);

            }

        }
        store(nullptr, result, x - abs(finalSize));
        return result;
    }

    template<size_t sizeDest>
    static void split(char *__restrict__ src, char dest[][sizeDest], size_t size, char* __restrict__ sep, size_t sepSize)
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

    static void assign(char *__restrict__ src, char **__restrict__ dest)
    {
        char oldAddress[64];
        sprintf(oldAddress, "%p", *dest);
        char *tmp = (char *) malloc(strlen(src) * sizeof(char));

        memset(tmp, 0, strlen(src));
        strcat(tmp, src);
        *dest = tmp;

        store(dest, nullptr, 0, false, oldAddress);
    }

    static void assign(const char *__restrict__ src, char **__restrict__ dest)
    {
        char oldAddress[128];
        sprintf(oldAddress, "%p", *dest);

        char *tmp = (char *) malloc(strlen(src) * sizeof(char));
        memset(tmp, 0, strlen(src));
        //strcpy(tmp, src);
        strcat(tmp, src);
        *dest = tmp;

        store(dest, nullptr, 0, false, oldAddress);
    }

    static long getIndex(char *__restrict__ ptr, void **__restrict__ arr, size_t arrLength)
    {
        for(long i = 0; i < arrLength; i++)
        {
            char strPtr[64];
            sprintf(strPtr, "%p", arr[i]);
            if(strcmp(strPtr, ptr) == 0)
            {
                return i;
            }
        }
        return -1;
    }

    static void reIndex(void **arr, size_t arrLength, size_t index)
    {
        for(size_t i = 0; i < arrLength; i++)
        {
            arr[index] = arr[index + 1];
        }
    }

    static int sum(int *arr, size_t arrLength)
    {
        int result = 0;
        for(size_t i = 0; i <= arrLength; i++)
        {
            result += arr[i];
        }
        return result;
    }
};


#endif
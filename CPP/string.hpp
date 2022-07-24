#ifndef DEF_CUSTOM_STRINGS_CPP
#define DEF_CUSTOM_STRINGS_CPP

#include <cstring>
#include <cstdlib>

/*
    This lib makes it easy to do string operation on char type.
    Everything allocated dynamically gets stored in an array, and can be freed whenever needed
    just by calling freeAll()
    Every function called using dynamic allocation is not freed, you have to call the freeAll() when you no longer need the memory.

    List of dynamic allocation methods :
        - dynamicSplit
        - assign

    List of non-dynamic methods :
        - split
*/

long getIndex(char *__restrict__ ptr, void **__restrict__ arr, size_t arrLength);
void reIndex(void **arr, size_t arrLength, size_t index);

void store(char **str = nullptr, char **strArr = nullptr, size_t strArrLength = 0, bool free_p = false, char *oldAddress = nullptr, void *freeOne = nullptr)
{
    static char ***storageArr;
    static char **storage;
    static size_t storageLengthX, storageLength;
    static size_t yLength;

    if(storageLength && str && oldAddress)
    {
        long index = getIndex(oldAddress, (void**)storage, storageLength);
        if(index >= 0)
        {
            printf("on l'a eu ! : %s et la size : %lu\n", storage[index], storageLength);
            free(storage[index]);
            reIndex((void**)storage, storageLength, index);
            storageLength--;
            printf("nouvelle size : %lu \n", storageLength);
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
                for(size_t i = 0; i <= yLength; i++)
                {
                    free(storageArr[index][i]);
                }
                free(storageArr[index]);
                reIndex((void**)storageArr, storageLengthX, index);
                storageLengthX--;
            }
        }
    }

    if(!free_p)
    {
        if(str && !storageLength)
        {
            storage = (char **) malloc(sizeof(char**));
            storageLength = 0;
        }
        if(str && storageLength)
        {
            storage = (char **) realloc(storage, sizeof(char**) * storageLength + 1);
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
            storageLengthX = 0;
        }

        if(strArr && storageLengthX)
        {
            storageArr = (char ***) realloc(storage, (storageLengthX + 1) * sizeof(char**));
        }

        if(strArr)
        {
            

            //storageArr[storageLengthX] = (char**) malloc(strArrLength * sizeof(char*));
            storageArr[storageLengthX] = strArr;
            yLength += strArrLength;
            
            storageLengthX += 1;
        }

        

    }
    else
    {
        printf("on free storage, la size : %lu \n", storageLength);
        for(size_t i = 0; i < storageLength; i++)
        {
            printf("freeing : %s\n", storage[i]);
            if(storage[i])
                free(storage[i]);
            printf("c'est bon c'est free\n");
        }
        printf("on free storageArr, la size : %lu\n", storageLengthX);
        for(size_t i = 0; i < storageLengthX; i++)
        {
            if(storageArr[i])
            {
                printf("on test \n");
                for(size_t j = 0; j <= yLength; j++)
                {
                    printf("ben oui %s\n", storageArr[i][j]);
                    if(storageArr[i][j])
                        free(storageArr[i][j]);
                }
                free(storageArr[i]);
            }
            
                
        }

        free(storage);
        free(storageArr);

        storageArr = nullptr;
        storage = nullptr;
        storageLengthX = 0, storageLength = 0, yLength = 0;
    }
    
}

void freeAll()
{
    store(nullptr, nullptr, 0, true);
}

void freeOne(void *ptr)
{
    store(nullptr, nullptr, 0, false, nullptr, ptr);
}

char **dynamicSplit(char *__restrict__ src, char* __restrict__ sep, size_t sepSize)
{
    // Split a string by sep, return a 2D array with the result
    size_t size = strlen(src);
    char current[size];
    size_t x = 5;

    char **result = (char**) malloc(x * sizeof(char*));
    memset(current, 0, size);

    size_t nFound = 0, nSplit = 0, maxStringSize = 0;
    int nMinus = 0;
    int finalSize = 1;

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
                printf("current : %d \n", sizeOfCurrent);
                printf("and : %s \n", current);
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
    printf("la final size : %d\n", finalSize);
    store(nullptr, result, x - abs(finalSize));
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

void assign(char *__restrict__ src, char **__restrict__ dest)
{
    char oldAddress[64];
    sprintf(oldAddress, "%p", *dest);
    char *tmp = (char *) malloc(strlen(src) * sizeof(char));

    memset(tmp, 0, strlen(src));
    strcat(tmp, src);
    *dest = tmp;

    store(dest, nullptr, 0, false, oldAddress);
}

long getIndex(char *__restrict__ ptr, void **__restrict__ arr, size_t arrLength)
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

void reIndex(void **arr, size_t arrLength, size_t index)
{
    for(size_t i = 0; i < arrLength; i++)
    {
        arr[index] = arr[index + 1];
    }
}

#endif
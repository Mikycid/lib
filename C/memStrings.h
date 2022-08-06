#ifndef DEF_CUSTOM_STRINGS_C
#define DEF_CUSTOM_STRINGS_C

#include <string.h>
#include <stdint.h>

#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif


static long getIndex(char *__restrict__ ptr, void **__restrict__ arr, size_t arrLength);
static void reIndex(void **arr, size_t arrLength, size_t index);
static int sum(int *arr, size_t arrLength);
static void assign(char *__restrict__ src, char **__restrict__ dest);
static void assing_gc(char **__restrict__ src, char ***__restrict__ dst, size_t srcSize, size_t destSize);


static void store(char **str, char **strArr, size_t strArrLength, char free_p, char *oldAddress, void *freeOne, char ***maxiDest, size_t maxiDestInnerLength, size_t maxiDestInnerInnerMax)
{
    static char ****maxiStore;
    static char ***storageArr;
    static char **storage;
    static size_t storageLengthX, storageLength;
    static size_t *yLength;

    static size_t maxiStoreLength;
    static size_t x, *y, *z;

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

    if(storageLengthX && strArr)
    {

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
            char strPtr[32];
            sprintf(strPtr, "%p", strArr);
            long index = getIndex(strPtr, (void**)storageArr, storageLengthX);
            if(index >= 0)
            {
                return;
            }
            else
            {
              
                storageArr = (char ***) realloc(storageArr, (storageLengthX + 1) * sizeof(char**));

                if(storageArr == NULL)
                    printf("Failed to allocate");
                yLength = (size_t *) realloc(yLength, (storageLengthX + 1) * sizeof(size_t));
              
            }
        }

        if(strArr)
        {
            
            //storageArr[storageLengthX] = (char**) malloc(strArrLength * sizeof(char*));
            storageArr[storageLengthX] = strArr;
            yLength[storageLengthX] = strArrLength;

            printf("on stocke %s, la taille : %lu et pourtant %lu\n", strArr[0], yLength[storageLengthX], strArrLength);
            storageLengthX++;
            
        }

        if(maxiDest && !x)
        {
            maxiStore = (char ****) malloc(sizeof(char***));
            y = (size_t *) malloc(sizeof(size_t));
            z = (size_t *) malloc(sizeof(size_t ));
            x = 0;
        }

        if(maxiDest && x)
        {
            maxiStore = (char ****) realloc(maxiStore, sizeof(char***) * (x + 1));
            y = (size_t *) realloc(y, sizeof(size_t) * (x + 1));
            z = (size_t *) realloc(z, sizeof(size_t) * (x + 1));
            x++;
        }

        if(maxiDest)
        {
            maxiStore[x] = maxiDest;
            y[x] = maxiDestInnerLength;
            z[x] = maxiDestInnerInnerMax;
            x += 1;
        }

    }
    else
    {
        for(size_t i = 0; i < storageLength; i++)
        {
            printf("on free %s\n", storage[i]);
            if(storage[i])
                free(storage[i]);
        }
        for(size_t i = 0; i < storageLengthX; i++)
        {
            printf("dans la boucle ...\n");
            if(storageArr[i])
            {
                for(size_t j = 0; j <= yLength[i]; j++)
                {
                    printf("j : %lu, size : %lu\n", j, yLength[i]);
                    printf("on free %s ! \n", storageArr[i][j]);
                    if(storageArr[i][j])
                        free(storageArr[i][j]);
                }
                free(storageArr[i]);
            }
            
                
        }

        for(size_t i = 0; i < x; i++)
        {
            if(maxiStore[i])
            {
                for(size_t j = 0; j <= y[i]; j++)
                {
                    for(size_t k = 0; k <= z[i]; k++)
                    {
                        if(maxiStore[i][j][k])
                            free(maxiStore[i][j][k]);
                    }
                    free(maxiStore[i][j]);
                }
                free(maxiStore[i]);
            }
        }
        
        if(storageLength)
        {
            free(storage);
        }
        if(storageLengthX)
        {
            free(storageArr);
            free(yLength);

        }

        if(x)
        {
            free(maxiStore);
            free(y);
            free(z);
        }
        storageArr = NULL;
        storage = NULL;
        storageLengthX = 0, storageLength = 0;
        yLength = NULL;
    }
    
}

static void freeAll()
{
    store(NULL, NULL, 0, true, NULL, NULL, NULL, 0, 0);
}

static void freeOne(void *ptr)
{
    store(NULL, NULL, 0, false, NULL, ptr, NULL, 0, 0);
}

static char **dynamicSplit(char *__restrict__ src, char* __restrict__ sep, size_t sepSize, uint32_t max)
{
    // Split a string by sep, returns a 2D array with the result
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
        if(current[i - nMinus] == sep[nFound] && nSplit < max)
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
            break;
        }

    }
    store(NULL, result, x - abs(finalSize), 0, NULL, NULL, NULL, 0, 0);
    return result;
}


static void assign(char *__restrict__ src, char **__restrict__ dest)
{
    char oldAddress[64];
    sprintf(oldAddress, "%p", *dest);
    char *tmp = (char *) malloc(strlen(src) * sizeof(char));

    memset(tmp, 0, strlen(src));
    strcat(tmp, src);
    *dest = tmp;

    store(dest, NULL, 0, false, oldAddress, NULL, NULL, 0, 0);
}

static void assign_gc(char **__restrict__ src, char ***__restrict__ dest, size_t srcSize, size_t destSize)
{
    
    *dest = (char**) malloc(sizeof(char *) * srcSize);

    *dest = src;
    printf("la srcSize : %lu", srcSize);
    store(NULL, *dest, srcSize, 0, NULL, NULL, NULL, 0, 0);
}

static char *concat(char *__restrict__ str1, char *__restrict__ str2, size_t sizeofStr1, size_t sizeofStr2)
{
    size_t totalSize = sizeofStr1 + sizeofStr2;
    char *result = (char*) malloc(totalSize);

    printf("str 1 :%s\n", str1);
    printf("%lu\n", sizeofStr1);
    printf("str 2 : %s\n", str2);
    printf("%lu\n", sizeofStr2);
    size_t i, j;
    for(i = 0; i < sizeofStr1; i++)
    {
        result[i] = str1[i];
    }
    for(j = 0; j < sizeofStr2; j++)
    {
        result[i] = str2[j];
    }
    store(&result, NULL, 0, 0, NULL, NULL, NULL, 0, 0);
    return result;
}


static long getIndex(char *__restrict__ ptr, void **__restrict__ arr, size_t arrLength)
{
    printf("le pointeur : %p\n", ptr);
    for(long i = 0; i < arrLength; i++)
    {
        char strPtr[64];
        sprintf(strPtr, "%p", arr[i]);
        printf("a l'interieur : %s\n", strPtr);
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

#ifndef __cplusplus

static void split(char *__restrict__ src, size_t sizeDest, char dest[][sizeDest], size_t size, char* __restrict__ sep, size_t sepSize)
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

#else
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
#endif

#endif
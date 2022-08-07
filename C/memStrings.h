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
        - assign_c
        - assign_gc
        - concat

    List of non-dynamic methods :
        - split
*/

static long getIndex(char *__restrict__ ptr, void **__restrict__ arr, size_t arrLength);
static void reIndex(void **arr, size_t arrLength, size_t index);
static int sum(int *arr, size_t arrLength);
static void assign(char *__restrict__ src, char **__restrict__ dest);
static void assing_gc(char **__restrict__ src, char ***__restrict__ dst, size_t srcSize, size_t destSize);


static void store(char **str, char **strArr, size_t strArrLength, char free_p, char *oldAddress, void *freeOne, char ***maxiDest, size_t maxiDestInnerLength, size_t maxiDestInnerInnerMax)
{
    /*
        This is the function that stores every variable dynamically allocated, 
        it's meant to be used by the other functions of the library.
    */
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

    if(freeOne)
    {
        char strPtr[32];
        sprintf(strPtr, "%p", freeOne);
        long index = getIndex(strPtr, (void**)storage, storageLength);

        if(index >= 0)
        {
            printf("on free %s a l'adresse %p\n", storage[index], storage[index]);
            printf("taille du storage : %lu\n", storageLength);
            free(storage[index]);
            reIndex((void**)storage, storageLength, index);
            storageLength--;

            printf("taille du storage apres : %lu\n", storageLength);
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
            printf("on store %s a l'index %lu\n", *str, storageLength);
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
        printf("storage length : %lu\n", storageLength);
        for(size_t i = 0; i < storageLength; i++)
        {
            printf("on free %s a l'adresse %p\n",storage[i], storage[i]);
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
    // Free every variable stored in the store
    store(NULL, NULL, 0, true, NULL, NULL, NULL, 0, 0);
}

static void freeOne(void *ptr)
{
    // Free one variable, you need to still have the address of the pointer
    store(NULL, NULL, 0, false, NULL, ptr, NULL, 0, 0);
}

static char **dynamicSplit(char *__restrict__ src, char* __restrict__ sep, size_t sepSize, uint32_t max)
{
    // Split a string by sep, returns a 2D array with the result
    // Use max for the maximum number of iterance
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
    // This assigns a char pointer to a char destination
    // To work you need to pass the address of the destination
    // For example :
    /*
        char test[] = "test";
        char *dst;
        assign(test, &dst);
    */
    char oldAddress[64];
    sprintf(oldAddress, "%p", *dest);
    char *tmp = (char *) malloc(strlen(src) * sizeof(char));

    memset(tmp, 0, strlen(src));
    strcat(tmp, src);
    *dest = tmp;

    store(dest, NULL, 0, false, oldAddress, NULL, NULL, 0, 0);
}

static void assign_c(const char *__restrict__ src, char **__restrict__ dest)
{
    // This assigns a char array to a char destination, and stores the pointer.
    // To work you need to pass the address of the destination
    // For example :
    /*
        char test[4] = "test";
        char *dst;
        assign(test, &dst);
    */
    char oldAddress[128];
    sprintf(oldAddress, "%p", *dest);

    char *tmp = (char *) malloc(strlen(src) * sizeof(char));
    memset(tmp, 0, strlen(src));
    //strcpy(tmp, src);
    strcat(tmp, src);
    *dest = tmp;

    store(dest, NULL, 0, false, oldAddress, NULL, NULL, 0, 0);
}



static void assign_ca(char **__restrict__ src, char ***__restrict__ dest, size_t srcSize)
{
    // This function assigns a char array to a destination, and stores the pointer
    // To work you need to pass the address of the destination
    // For example :
    /*
        char **test;
        assign("test", test[0]);
        assign("test2", test[1]);
        char **dst;
        assign_ca(test, &dst);
    */
    *dest = (char**) malloc(sizeof(char *) * srcSize);

    *dest = src;

    store(NULL, *dest, srcSize, 0, NULL, NULL, NULL, 0, 0);
}

static char *concat(char *__restrict__ str1, char *__restrict__ str2, size_t sizeofStr1, size_t sizeofStr2)
{
    /* This function adds str2 to the end of str1 */
    size_t totalSize = sizeofStr1 + sizeofStr2;
    char *result = (char*) malloc(totalSize);
    memset(result, '\0', totalSize + 1);

    size_t i, j;
    for(i = 0; i < sizeofStr1; i++)
    {
        result[i] = str1[i];
    }
    for(j = 0; j < sizeofStr2; j++)
    {
        result[i+j] = str2[j];
    }
    //printf("le resultat : %s\n", result);
    store(&result, NULL, 0, 0, NULL, NULL, NULL, 0, 0);
    return result;
}


static long getIndex(char *__restrict__ ptr, void **__restrict__ arr, size_t arrLength)
{
    /* This function returns the index or the pointer ptr if it's contained in arr
        ptr should be a char ptr, use sprintf to put one in
    */
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
    // This function reIndex an array from index to arrLength
    for(size_t i = index; i < arrLength; i++)
    {
        arr[i] = arr[i + 1];
    }
}

static int sum(int *arr, size_t arrLength)
{
    // Returns the sum of an integer array
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
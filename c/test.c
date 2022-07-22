#include <stdio.h>
#include <stdlib.h>

#include "./string.h"

int main(int argc, char const *argv[])
{
    char dest[10][100];
    char str[30] = "abcd::pfe::mabc:::opke";

    split(str, 100, dest, sizeof(str) / sizeof(char), "::", sizeof("::") / sizeof(char));

    for(int i = 0; i < 4; i++)
    {
        printf("%s \n", dest[i]);
    }
    return 0;
}

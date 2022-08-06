#include <stdio.h>
#include "parser.h"


int main(int argc, char* argv[])
{
    Config *conf = readConf("test.conf");

    char *var;
    getConfVariable(&var, conf, "SCOPE2", "COP");
    printf("var : %s\n", var);

    freeAll();
    freeConf(conf);
    return 0;
}
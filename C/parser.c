#include <stdio.h>
#include "ConfigParser.h"


int main(int argc, char* argv[])
{
    Config *conf = readConf("test.conf");

    char *var;
    getConfVariable(&var, conf, "SCOPE2", "COP");
    printf("var : %s\n", var);

    writeConf(conf, "./hello.txt");

    freeAll();
    freeConf(conf);
    return 0;
}
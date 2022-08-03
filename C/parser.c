#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "memStrings.h"
#include "parser.h"

Config readConf(char *path)
{
    Config conf;
    FILE *fp = fopen(path, "rb");
    ssize_t read;
    if(fp == NULL)
        exit(1);

    char *line = (char*) malloc(100);
    int current;
    int counter = 0;
    int insideScope = 0;
    int nScopes = -1;
    int nVariables = 0;
    size_t len;
    while((current = fgetc(fp)) != EOF)
    {
        if(isspace(current))
        {
            continue;
        }
        else if(current == '[')
        {
            current = fgetc(fp);

            while(current != ']' && current != '\n')
            {
                if(current == EOF)
                    break;
                line[counter] = current;
                counter++;

                current = fgetc(fp);

            }
            if(strlen(line))
            {
                printf("on est bons\n");
                printf("%s\n", line);
                if(nScopes > -1)
                {
                    printf("on est dedans \n");
                    conf.scopes = (Scope *) realloc(conf.scopes, sizeof(Scope) * nScopes);
                }
                else
                    conf.scopes = (Scope *) malloc(sizeof(Scope));
                printf("avant d'assigner\n");
                assign(line, &conf.scopes[nScopes].name);
                printf("apres assigner\n");
                memset(line, 0, sizeof(line));
                printf("apres memset\n");
                counter = 0;

                insideScope = 1;
                nScopes++;
            }
        }
        else if(insideScope)
        {
            char saved = current;
            getline(&line, &len, fp);
            char *varLine = concat(&saved, line, sizeof(saved), len);
            char **variable = dynamicSplit(line, "=", 1, 1);
            if(strcmp(variable[0], line) != 0)
            {
                conf.scopes[nScopes].variables = variable;
            }
        }
        printf("%c", current);
    }
    fclose(fp);
    if(line)
        free(line);
    freeAll();

    return conf;
}
int main(int argc, char* argv[])
{
    Config conf = readConf("test.conf");
    printf("%s", getConfVariable(&conf.scopes[getScopeIndex(&conf, "MYSCOPE")], "abc"));
    return 0;
}
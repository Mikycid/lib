#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "memStrings.h"
#include "parser.h"

Config *readConf(char *path)
{
    Config *conf = malloc(sizeof(conf));
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
                    conf->scopes = (Scope *) realloc(conf->scopes, sizeof(Scope) * nScopes);
                }
                else
                {

                    conf->scopes = (Scope *) malloc(sizeof(Scope));
                    nScopes = 0;
                }
                printf("avant d'assigner\n");
                assign(line, &conf->scopes[nScopes].name);
                printf("apres assigner : %d : %s\n", nScopes, conf->scopes[nScopes].name);
                memset(line, '\0', strlen(line));
                counter = 0;

                insideScope = 1;
                nScopes++;
            }
        }
        else if(insideScope)
        {
            line[counter] = current;
            printf("on a eu : %s\n", line);
            while((current = fgetc(fp)) != EOF)
            {
                if(current == '\n')
                    break;
                counter++;
                line[counter] = current;
                printf("current : %c\n", current);
                
            }
            printf("line : %s\n", line);
            char **variable = dynamicSplit(line, "=", sizeof("="), 1);
            printf("split ok\n");
            memset(line, '\0', strlen(line));
            counter = 0;
            if(strcmp(variable[0], line) != 0)
            {
                assign_gc(variable, &conf->scopes[nScopes].variables, 2, conf->scopes[nScopes].nVariables);
                printf("l'adresse avant : %p\n", conf->scopes[nScopes].variables);
                printf("la variable : %s = %s\n", conf->scopes[nScopes].variables[0][0], conf->scopes[nScopes].variables[0][1]);
            }
        }
        printf("%c", current);
    }
    fclose(fp);

    if(line)
        free(line);

    printf("l'adresse encore avant : %p", conf);
    return conf;
}
int main(int argc, char* argv[])
{
    Config *conf = readConf("test.conf");
    printf("adresse avant : %p\n", conf);

    printf("%s", getConfVariable(&conf->scopes[getScopeIndex(conf, "MYSCOPE")], "abc"));

    freeAll();
    return 0;
}
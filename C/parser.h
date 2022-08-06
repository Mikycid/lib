#ifndef DEF_CONF_PARSER
#define DEF_CONF_PARSER

#include <stdlib.h>

#include <ctype.h>

#include "memStrings.h"

typedef struct Scope
{
    char *name;
    char ***variables;
    size_t nVariables;

} Scope;

typedef struct Config
{
    Scope **scopes;
    size_t nScopes;
} Config;

void addScopeVariable(Scope *scope, char **var)
{
    scope->variables[scope->nVariables] = var;
    scope->nVariables++;
}

void freeConf(Config *conf)
{
    for(size_t i = 0; i < conf->nScopes; i++)
    {
        free(conf->scopes[i]);
    }
}

void getConfVariable(char **__restrict__ dst, Config *conf, char *scopeName, char *varName)
{
    size_t index = 0;
    printf("l'adresse : %p\n", conf);

    for(size_t i = 0; i < conf->nScopes; i++)
    {
        printf("avant la scope..\n");
        printf("test de la scope : %s\n", conf->scopes[i]->name);
        if(strcmp(conf->scopes[i]->name, scopeName) == 0)
        {
            printf("trouvé : %lu\n", i);
            Scope *scope = conf->scopes[i];
            
            printf("les variables sont OK : %s et size : %lu\n", scope->name, scope->nVariables);
            for(size_t j = 0; j < scope->nVariables; j++)
            {
                printf("dans la scope la var %s c'est %s\n", scope->variables[j][0], scope->variables[j][1]);
                if(strcmp(scope->variables[j][0], varName) == 0)
                {
                    assign(scope->variables[j][1], dst);

                    printf("trouvé !!! dans dst on a : %s\n", *dst);
                    return;
                }
            }
            return;
        }
    }
    return;
}

Config *readConf(char *path)
{
    Config *conf = (Config *) malloc(sizeof(conf));
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
                if (nScopes >= 0)
                {
                    conf->scopes = (Scope **) realloc(conf->scopes, sizeof(Scope *) * (nScopes + 1));
                    
                    conf->nScopes++;
                }
                else
                {
                    conf->scopes = (Scope **) malloc(sizeof(Scope *));

                    conf->nScopes = 1;
                }

                nScopes++;
                printf("passé la scope %d\n", nScopes);
                conf->scopes[nScopes] = (Scope *) malloc(sizeof(Scope));
                
                assign(line, (char**)&conf->scopes[nScopes]->name);
                conf->scopes[nScopes]->nVariables = 0;
                printf("apres assigner : %d : %s\n", nScopes, conf->scopes[nScopes]->name);
                memset(line, '\0', strlen(line));
                counter = 0;

                insideScope = 1;
            }
        }
        else if(insideScope)
        {
            line[counter] = current;

            while((current = fgetc(fp)) != EOF)
            {
                if(current == '\n')
                    break;
                counter++;
                line[counter] = current;
                
            }
            char **variable = dynamicSplit(line, "=", sizeof("="), 1);
            memset(line, '\0', strlen(line));
            counter = 0;
            if(strcmp(variable[0], line) != 0)
            {
                if(conf->scopes[nScopes]->nVariables > 0)
                    conf->scopes[nScopes]->variables = (char ***) realloc(conf->scopes[nScopes]->variables, sizeof(char ***) * conf->scopes[nScopes]->nVariables);
                else
                    conf->scopes[nScopes]->variables = (char ***) malloc(sizeof(char ***));
                assign_gc(variable, &conf->scopes[nScopes]->variables[conf->scopes[nScopes]->nVariables], 2, conf->scopes[nScopes]->nVariables);

                conf->scopes[nScopes]->nVariables++;
            }
        }
    }
    fclose(fp);

    if(line)
        free(line);

    return conf;
}



#endif
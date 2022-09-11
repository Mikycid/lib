#ifndef DEF_CONF_PARSER
#define DEF_CONF_PARSER

#include <stdlib.h>

#include <ctype.h>

#include "memStrings.h"

#ifdef __cpluscplus
    using namespace memStrings;
#endif

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

inline void addScopeVariable(Scope *scope, char **var)
{
    scope->variables[scope->nVariables] = var;
    scope->nVariables++;
}

inline void freeConf(Config *conf)
{
    for(size_t i = 0; i < conf->nScopes; i++)
    {
        freeOne(conf->scopes[i]->name);
        freeOne(conf->scopes[i]->variables);
        free(conf->scopes[i]);
    }
}

inline void getConfVariable(char **__restrict__ dst, Config *conf, const char *__restrict__ scopeName, const char *__restrict__ varName)
{
    size_t index = 0;
    
    for(size_t i = 0; i < conf->nScopes; i++)
    {

        if(memcmp(conf->scopes[i]->name, scopeName, strlen(scopeName)) == 0)
        {
            Scope *scope = conf->scopes[i];
            for(size_t j = 0; j < scope->nVariables; j++)
            {
                if(memcmp(scope->variables[j][0], varName, strlen(varName)) == 0)
                {
                    assign(scope->variables[j][1], dst);

                    return;
                }
            }
            *dst = NULL;
            return;
        }
    }
    *dst = NULL;
}

inline void writeConf(Config *conf, char *__restrict__ path)
{
    FILE *fp = fopen(path, "w");

    for(size_t i = 0; i < conf->nScopes; i++)
    {
        Scope *scope = conf->scopes[i];
        char *name1 = concat("[", conf->scopes[i]->name, strlen("["), strlen(conf->scopes[i]->name));
        char *name2 = concat(name1, "]", strlen(name1), strlen("]"));
        char *nameFinal = concat(name2, "\n", strlen(name2), strlen("\n"));
        fputs(nameFinal, fp);
        for(size_t j = 0; j < scope->nVariables; j++)
        {
            char *str1 = concat(scope->variables[j][0], "=", strlen(scope->variables[j][0]), strlen("="));
            char *str2 = concat(str1, scope->variables[j][1], strlen(str1), strlen(scope->variables[j][1]));
            char *strFinal = concat(str2, "\n", strlen(str2), strlen("\n"));
            fputs(strFinal, fp);
            freeOne(str1);
            freeOne(str2);
            freeOne(strFinal);
        }
        freeOne(name1);
        freeOne(name2);
        freeOne(nameFinal);
    }
    fclose(fp);
}

inline Config *readConf(const char *path)
{
    Config *conf = (Config *) malloc(sizeof(conf));
    FILE *fp = fopen(path, "rb");
    ssize_t read;
    if(fp == NULL)
    {

        printf("Failed to load %s\n", path);
        exit(1);
    }

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

                conf->scopes[nScopes] = (Scope *) malloc(sizeof(Scope));
                
                assign(line, (char**)&conf->scopes[nScopes]->name);
                conf->scopes[nScopes]->nVariables = 0;
                
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
                assign_ca(variable, &conf->scopes[nScopes]->variables[conf->scopes[nScopes]->nVariables], 2);

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
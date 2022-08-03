#ifndef DEF_CONF_PARSER
#define DEF_CONF_PARSER

typedef struct Scope
{
    char *name;
    char **variables;


} Scope;

char *getConfVariable(Scope *scope, char *name)
{
    char **variables = scope->variables;
    while(1)
    {
        if(strcmp(*variables, name) == 0)
        {
            return *variables;
        }
        variables++;
        if(*variables == NULL)
        {
            return 0;
        }
    }
}


typedef struct Config
{
    Scope *scopes;
} Config;


size_t getScopeIndex(Config *conf, char *name)
{
    Scope *scope = conf->scopes;
    size_t index = 0;
    while(1)
    {
        if(strcmp(scope->name, name) == 0)
        {
            return index;
        }

        scope++;
        index++;
        if(scope == NULL)
            return 0;
    }
}

Config readConf(char *path);

#endif
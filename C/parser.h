#ifndef DEF_CONF_PARSER
#define DEF_CONF_PARSER

typedef struct Scope
{
    char *name;
    char ***variables;
    size_t nVariables;

} Scope;

void addScopeVariable(Scope *scope, char **var)
{
    scope->variables[scope->nVariables] = var;
    scope->nVariables++;
}

char *getConfVariable(Scope *scope, char *name)
{
    char ***variables = scope->variables;
    int i = 0;
    printf("variables ok ...%s\n", variables[0][0]);
    for(size_t i = 0; i < scope->nVariables; i++)
    {
        if(strcmp(*variables[i], name) == 0)
        {
            return *variables[i];
        }
        i++;
        if(variables[i] == NULL)
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
    printf("l'adresse : %p\n", conf);
    printf("les variables sont OK : %s et name : %s\n", scope[0]->name, name);

    while(1)
    {
        if(strcmp(scope->name, name) == 0)
        {
            printf("trouvé : %lu\n", index);
            return index;
        }

        scope++;
        index++;
        printf("index : %lu\n", index);
        if(scope == NULL)
            return 0;
    }
}

Config *readConf(char *path);

#endif
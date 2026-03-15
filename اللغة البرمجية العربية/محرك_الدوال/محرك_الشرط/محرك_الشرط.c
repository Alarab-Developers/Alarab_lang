#include <stdio.h>
#include <string.h>
#include "محرك_الشرط.h"

#define MAX_IF 100
#define MAX_BODY 2048

typedef struct {
    char name[64];
    char condition[256];
    char body[MAX_BODY];
} If;


static If ifs[MAX_IF];
static int if_count = 0;

void if_store(const char *name, const char *condition, const char *body)
{
    if (if_count >= MAX_IF)
        return;

    strcpy(ifs[if_count].name, name);
    strcpy(ifs[if_count].condition, condition);
    strcpy(ifs[if_count].body, body);

    if_count++;
}

const char *if_get(const char *name) {
    for (int i = 0; i < if_count; i++) {
        if (strcmp(ifs[i].name, name) == 0) {
            return ifs[i].body;
        }
    }
    return NULL;
}




const char *if_get_condition(const char *name)
{
    for (int i = 0; i < if_count; i++)
    {
        if (strcmp(ifs[i].name, name) == 0)
            return ifs[i].condition;
    }

    return NULL;
}

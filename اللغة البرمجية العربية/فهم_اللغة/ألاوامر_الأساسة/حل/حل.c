#include <string.h>
#include <stdio.h>
#include "حل.h"

int parse_solve_command(const char *clean, Command *cmd)
{
    if (strncmp(clean, "حل", 2) != 0)
        return 0;

    char *first_equal = strchr(clean, '=');

    if (!first_equal)
        return 0;

    char *eq_part = first_equal + 1;

    while (*eq_part == ' ')
        eq_part++;

    if (*eq_part == '\0')
        return 0;

    char *second_equal = strchr(eq_part, '=');

    cmd->type = CMD_SOLVE;

    if (second_equal)
    {
        *second_equal = '\0';

        snprintf(cmd->argument,
                 sizeof(cmd->argument),
                 "(%s)-(%s)",
                 eq_part,
                 second_equal + 1);
    }
    else
    {
        snprintf(cmd->argument,
                 sizeof(cmd->argument),
                 "%s",
                 eq_part);
    }

    return 1;
}

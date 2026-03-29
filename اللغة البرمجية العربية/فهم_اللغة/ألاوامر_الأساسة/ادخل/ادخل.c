#include <string.h>
#include "ادخل.h"

int parse_input_command(const char *clean, Command *cmd)
{
    if (strncmp(clean, "ادخل", 4) != 0)
        return 0;

    char *eq = strchr(clean, '=');

    if (!eq)
        return 0;

    eq++; // بعد =

    while (*eq == ' ')
        eq++;

    if (*eq == '\0')
        return 0;

    strncpy(cmd->argument, eq, sizeof(cmd->argument) - 1);
    cmd->argument[sizeof(cmd->argument) - 1] = '\0';

    cmd->type = CMD_INPUT;

    return 1;
}

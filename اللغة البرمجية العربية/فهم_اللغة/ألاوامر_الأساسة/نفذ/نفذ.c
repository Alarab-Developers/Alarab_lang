#include <string.h>
#include <stdio.h>
#include "نفذ.h"
#include "القاموس.h"

int parse_exec_command(const char *clean, Command *cmd)
{
    if (strncmp(clean, كلمة_نفذ, strlen(كلمة_نفذ)) != 0)
        return 0;

    const char *eq = strchr(clean, '=');

    if (!eq)
        return 0;

    eq++;

    while (*eq == ' ')
        eq++;

    strncpy(cmd->argument, eq, sizeof(cmd->argument) - 1);
    cmd->argument[sizeof(cmd->argument) - 1] = '\0';

    cmd->type = CMD_EXEC_SYSTEM;

    return 1; // تم التعرف على الأمر
}

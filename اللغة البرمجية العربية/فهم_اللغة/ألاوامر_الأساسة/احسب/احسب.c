#include <string.h>
#include <stdio.h>
#include "احسب.h"
#include "القاموس.h"

int parse_calc_command(const char *clean, Command *cmd)
{
    if (strncmp(clean, كلمة_احسب, strlen(كلمة_احسب)) != 0)
        return 0;

    const char *eq = strchr(clean, '=');

    if (!eq)
        return 0;

    eq++; // بعد =

    while (*eq == ' ')
        eq++;

    if (*eq == '\0')
        return 0;

    strncpy(cmd->argument, eq, sizeof(cmd->argument) - 1);
    cmd->argument[sizeof(cmd->argument) - 1] = '\0';

    cmd->type = CMD_EXEC;  // 👈 يروح للمحرك الرياضي

    return 1;
}

#include <string.h>
#include <stdio.h>
#include "تحميل.h"

int parse_import_command(const char *clean, Command *cmd)
{
    if (strncmp(clean, "تحميل(", 6) != 0)
        return 0;

    char filename[256];

    if (sscanf(clean, "تحميل(%255[^)])", filename) == 1)
    {
        cmd->type = CMD_IMPORT;

        strncpy(cmd->argument, filename, sizeof(cmd->argument) - 1);
        cmd->argument[sizeof(cmd->argument) - 1] = '\0';

        return 1;
    }

    return 0;
}

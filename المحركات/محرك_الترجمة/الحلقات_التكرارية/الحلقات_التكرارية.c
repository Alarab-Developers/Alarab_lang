#include <string.h>
#include <stdio.h>
#include "الحلقات_التكرارية.h"

int parse_loop_command(const char *clean, Command *cmd)
{
    /* ================================
       كرر
    ================================ */

    if (strncmp(clean, "كرر(", 5) == 0)
    {
        char num[64];

        if (sscanf(clean, "كرر(%63[^)])", num) == 1)
        {
            cmd->type = CMD_REPEAT;
            strcpy(cmd->argument, num);
            return 1;
        }
    }

    /* ================================
       كرر حتى
    ================================ */

    if (strncmp(clean, "كرر حتى(", 9) == 0)
    {
        char condition[256];

        if (sscanf(clean, "كرر حتى(%255[^)])", condition) == 1)
        {
            cmd->type = CMD_WHILE;
            strcpy(cmd->argument, condition);
            return 1;
        }
    }

    /* ================================
       كسر
    ================================ */

    if (strcmp(clean, "كسر") == 0)
    {
        cmd->type = CMD_BREAK;
        return 1;
    }

    return 0;
}

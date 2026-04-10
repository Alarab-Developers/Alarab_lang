#include <string.h>
#include <stdio.h>
#include "الشرط.h"
#include "القاموس.h"

int parse_condition_command(const char *clean, Command *cmd)
{
    /* ================================
       والا اذا
    ================================ */

    if (strncmp(clean, "والا اذا(", 8) == 0)
    {
        char condition[256];

        if (sscanf(clean, "والا اذا(%255[^)])", condition) == 1)
        {
            cmd->type = CMD_ELSE_IF;
            strcpy(cmd->argument, condition);
            return 1;
        }
    }

    /* ================================
       والا
    ================================ */

    if (strncmp(clean, كلمة_والا, strlen(كلمة_والا)) == 0)
    {
        cmd->type = CMD_ELSE;
        return 1;
    }

    /* ================================
       اذا
    ================================ */

    const char *if_prefix = كلمة_اذا "(";

    if (strncmp(clean, if_prefix, strlen(if_prefix)) == 0)
    {
        char condition[256];

        if (sscanf(clean, "اذا(%255[^)])", condition) == 1)
        {
            cmd->type = CMD_IF_DEF;
            cmd->name[0] = '\0';
            strcpy(cmd->argument, condition);
            return 1;
        }
    }

    return 0;
}

#include <string.h>
#include <stdio.h>
#include "الدالة.h"

int parse_function_def_command(const char *clean, Command *cmd)
{
    if (strncmp(clean, "دالة(", 5) != 0)
        return 0;

    char name[64];
    char extra[64];

    if (sscanf(clean, "دالة(%63[^)])%63s", name, extra) >= 1)
    {
        if (strchr(clean, ','))
        {
            printf("خطأ: لا يمكن وضع شرط داخل تعريف الدالة\n");
            return 0;
        }

        cmd->type = CMD_FUNCTION_DEF;
        strcpy(cmd->name, name);

        return 1;
    }

    return 0;
}

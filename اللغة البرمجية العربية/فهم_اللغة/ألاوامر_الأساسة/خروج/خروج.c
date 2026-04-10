#include <string.h>
#include "خروج.h"
#include "القاموس.h"

int parse_exit_command(const char *clean, Command *cmd)
{
    if (strcmp(clean, كلمة_خروج) != 0)
        return 0;

    cmd->type = CMD_EXIT;
    return 1;
}

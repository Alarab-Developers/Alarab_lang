#include <string.h>
#include "نظف.h"
#include "القاموس.h"

int parse_clear_command(const char *clean, Command *cmd)
{
    if (strcmp(clean, كلمة_نظف) != 0)
        return 0;

    cmd->type = CMD_CLEAR;
    return 1;
}

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "اطبع.h"
#include "القاموس.h"

int parse_print_command(const char *clean, Command *cmd)
{
    if (strncmp(clean, كلمة_اطبع, strlen(كلمة_اطبع)) != 0)
        return 0;

    char *eq = strchr(clean, '=');

    if (!eq)
        return 0;

    eq++;

    while (*eq == ' ')
        eq++;

    if (*eq != '(')
        return 0;

    char *open = eq;
    char *close = strchr(open, ')');

    if (!close)
        return 0;

    char text[256];

    size_t text_len = close - open - 1;

    if (text_len >= sizeof(text))
        text_len = sizeof(text) - 1;

    strncpy(text, open + 1, text_len);
    text[text_len] = '\0';

    cmd->type = CMD_PRINT;
    cmd->repeat = 1;

    char *star = strchr(close + 1, '*');

    if (star)
    {
        char *num = star + 1;

        while (*num == ' ')
            num++;

        if (*num != '\0')
            cmd->repeat = atoi(num);

        if (cmd->repeat <= 0)
            cmd->repeat = 1;
    }

    snprintf(cmd->argument,
             sizeof(cmd->argument),
             "%s",
             text);

    return 1; // تم التعرف على الأمر
}

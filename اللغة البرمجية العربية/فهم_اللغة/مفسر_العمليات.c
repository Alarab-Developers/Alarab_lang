#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "مفسر_العمليات.h"
#include "القاموس.h"
#include "محرك_الدالة.h"
#include "اطبع.h"
#include "نظف.h"
#include "ادخل.h"
#include "تحميل.h"
#include "خروج.h"
#include "نفذ.h"
#include "حل.h"
#include "احسب.h"
#include "الدالة.h"
#include "الشرط.h"
#include "الحلقات_التكرارية.h"
static void trim(char *str)
{
    if (!str) return;

    int start = 0;

    while (isspace((unsigned char)str[start]))
        start++;

    if (start > 0)
        memmove(str, str + start, strlen(str + start) + 1);

    int end = strlen(str) - 1;

    while (end >= 0 && isspace((unsigned char)str[end]))
    {
        str[end] = '\0';
        end--;
    }
}

Command parse_line(const char *line)
{
    Command cmd;

    cmd.type = CMD_UNKNOWN;
    cmd.name[0] = '\0';
    cmd.argument[0] = '\0';
    cmd.repeat = 1;

    if (!line)
        return cmd;

    char clean[512];

    strncpy(clean, line, sizeof(clean) - 1);
    clean[sizeof(clean) - 1] = '\0';

    clean[strcspn(clean, "\n")] = '\0';

    trim(clean);

    if (strlen(clean) == 0)
        return cmd;

    /* تجاهل الأقواس */
    if (strcmp(clean, "{") == 0 || strcmp(clean, "}") == 0)
        return cmd;
    /* ================================
   خروج (منفصل)
    ================================ */

    if (parse_exit_command(clean, &cmd))
    {
        return cmd;
    }

    /* ================================
   نظف (منفصل)
    ================================ */

    if (parse_clear_command(clean, &cmd))
    {
        return cmd;
    }
    
    /* ================================
       تحميل (منفصل)
    ================================ */

    if (parse_import_command(clean, &cmd))
    {
        return cmd;
    }

    /* ================================
   تعريف دالة (منفصل)
    ================================ */

    if (parse_function_def_command(clean, &cmd))
    {
        return cmd;
    }

    /* ================================
   الشرط (منفصل)
    ================================ */

    if (parse_condition_command(clean, &cmd))
    {
        return cmd;
    }

    /* ================================
       استدعاء دالة أو شرط
    ================================= */

    int len = strlen(clean);

    if (len > 2 && strcmp(clean + len - 2, "()") == 0)
    {
        int name_len = len - 2;

        if (name_len > 63)
            name_len = 63;

        snprintf(cmd.name, sizeof(cmd.name), "%.*s", name_len, clean);


        if (function_get(cmd.name))
        {
            cmd.type = CMD_FUNCTION_CALL;
            return cmd;
        }

        return cmd;
    }

    

    /* ================================
   حل (منفصل)
    ================================ */

    if (parse_solve_command(clean, &cmd))
    {
        return cmd;
    }

    /* ================================
       اطبع (منفصل)
    ================================= */

    if (parse_print_command(clean, &cmd))
    {
        return cmd;
    }

    /* ================================
   احسب (منفصل)
    ================================ */

    if (parse_calc_command(clean, &cmd))
    {
        return cmd;
    }

    /* ================================
   التكرار (منفصل)
    ================================ */

    if (parse_loop_command(clean, &cmd))
    {
        return cmd;
    }
    
    /* ================================
       نفذ (منفصل)
    ================================ */

    if (parse_exec_command(clean, &cmd))
    {
        return cmd;
    }


    /* ================================
   ادخل (منفصل)
    ================================ */

    if (parse_input_command(clean, &cmd))
    {
        return cmd;
    }
    
    /* ================================
       تعيين متغير
    ================================= */

    if (strchr(clean, '='))
    {
        cmd.type = CMD_ASSIGN;

        strncpy(cmd.argument, clean, sizeof(cmd.argument) - 1);
        cmd.argument[sizeof(cmd.argument) - 1] = '\0';

        return cmd;
    }

    return cmd;
}

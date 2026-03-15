#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "المنفذ.h"
#include "محرك_الدالة.h"
#include "محرك_الشرط.h"
#include "محرك_الاشارات.h"
#include "المحرك_الرياضي.h"
#include "مفسر_العمليات.h"

void execute_system_command(const char *cmd)
{
    if (!cmd || cmd[0] == '\0')
    {
        printf("خطأ: لا يوجد أمر للتنفيذ\n");
        return;
    }

    if (system(cmd) == -1)
        printf("فشل تنفيذ الأمر\n");
}

/* ===================================== */
/* تنفيذ كتلة أو جسم دالة                */
/* ===================================== */

static int execute_block(const char *body)
{
    char temp[4096];
    strncpy(temp, body, sizeof(temp)-1);
    temp[sizeof(temp)-1] = '\0';


    /* اصلاح }والا{ */

    char fixed[4096] = "";
    char *p = temp;

    while (*p)
    {
        if (strncmp(p, "}والا{", 7) == 0)
        {
            strcat(fixed, "}\nوالا{\n");
            p += 7;
        }
        else
        {
            size_t len = strlen(fixed);
            if (len < sizeof(fixed) - 1)
            {
                fixed[len] = *p;
                fixed[len+1] = '\0';
        }
            p++;
        }
    }

    strcpy(temp, fixed);

        char *lines[512];
        int total = 0;

    char *token = strtok(temp, "\n");

    while (token && total < 512)
    {
        lines[total++] = token;
        token = strtok(NULL, "\n");
    }

    for (int i = 0; i < total; i++)
    {
        Command cmd = parse_line(lines[i]);

        if (cmd.type == CMD_UNKNOWN)
            continue;

        /* ========================= */
        /* تعريف دالة داخل كتلة     */
        /* ========================= */

        if (cmd.type == CMD_FUNCTION_DEF)
        {
            char body2[4096] = "";
            int brace = 1;

            i++;

            while (i < total)
            {
                if (strchr(lines[i], '{')) brace++;
                if (strchr(lines[i], '}')) brace--;

                if (brace == 0)
                    break;

                strcat(body2, lines[i]);
                strcat(body2, "\n");

                i++;
            }

            function_store(cmd.name, body2);
            continue;
        }

        /* ========================= */
        /* تنفيذ اذا + والا         */
        /* ========================= */

        if (cmd.type == CMD_IF_DEF)
        {
            char if_body[4096] = "";
            char else_body[4096] = "";

            int brace = 1;
            i++;

            /* قراءة جسم if */

            while (i < total)
            {
                if (strchr(lines[i], '{')) brace++;
                if (strchr(lines[i], '}')) brace--;

                if (brace == 0)
                    break;

                strcat(if_body, lines[i]);
                strcat(if_body, "\n");
                i++;
            }

            /* ========================= */
            /* فحص وجود والا             */
            /* ========================= */

            int has_else = 0;

            if (i + 1 < total)
            {
                char *nextline = lines[i + 1];

                if (strstr(nextline, "والا") != NULL)
                {
                    has_else = 1;

                    /* تخطي سطر والا */
                    i += 2;

                    brace = 1;

                    while (i < total)
                    {
                        if (strchr(lines[i], '{')) brace++;
                        if (strchr(lines[i], '}')) brace--;

                        if (brace == 0)
                            break;

                        strcat(else_body, lines[i]);
                        strcat(else_body, "\n");
                        i++;
                    }
                }
            }

            /* ========================= */
            /* تقييم الشرط               */
            /* ========================= */

            double result = 0;

            if (math_eval(cmd.argument, &result) == 0 && result != 0)
            {
                execute_block(if_body);
            }
            else if (has_else)
            {
                execute_block(else_body);
            }

            continue;
        }

        /* ========================= */
        /* تنفيذ الأمر              */
        /* ========================= */

        if (execute_command(cmd))
            return 1;
    }

    return 0;
}

/* ===================================== */
/* التنفيذ الرئيسي للأوامر               */
/* ===================================== */

int execute_command(Command cmd)
{

    /* ========================= */
    /* استدعاء دالة             */
    /* ========================= */

    if (cmd.type == CMD_FUNCTION_CALL)
    {
        const char *body = function_get(cmd.name);

        if (!body)
        {
            printf("خطأ: الدالة '%s' غير موجودة\n", cmd.name);
            return 0;
        }

        return execute_block(body);
    }

    /* ========================= */
    /* استدعاء شرط              */
    /* ========================= */

    if (cmd.type == CMD_IF_CALL)
    {
        const char *body = if_get(cmd.name);
        const char *cond = if_get_condition(cmd.name);

        if (!body || !cond)
        {
            printf("خطأ: الشرط '%s' غير موجود\n", cmd.name);
            return 0;
        }

        double result = 0;

        if (math_eval(cond, &result) == 0 && result != 0)
            return execute_block(body);

        return 0;
    }

    /* ========================= */
    /* تنفيذ أمر نظام           */
    /* ========================= */

    if (cmd.type == CMD_EXEC_SYSTEM)
    {
        execute_system_command(cmd.argument);
        return 0;
    }

    /* ========================= */
    /* تنفيذ الأوامر العادية    */
    /* ========================= */

    if (runtime_execute(cmd))
        return 1;

    return 0;
}

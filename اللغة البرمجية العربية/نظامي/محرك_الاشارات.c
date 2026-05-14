#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "محرك_الاشارات.h"
#include "المنفذ.h"
#include "محرك_المتغيرات.h"
#include "المحرك_الرياضي.h"
#include "محرك_كشف_الأخطاء.h"

int وضع_التصحيح = 0;
extern int CURRENT_LINE;

// =====================================
// دالة لطباعة النصوص
// =====================================
void rt_print(const char *text)
{
    if (!text || text[0] == '\0')
        return;

    printf("%s\n", text);
}

// =====================================
// دالة لتنظيف الشاشة
// =====================================
void rt_clear(void)
{
    printf("\033[2J\033[H");
}

// =====================================
// التنفيذ الرئيسي
// =====================================
int runtime_execute(Command cmd)
{
    switch (cmd.type)
    {
        // ===============================
        // طباعة
        // ===============================
        case CMD_PRINT:
        {
            for (int i = 0; i < cmd.repeat; i++)
            {
                char buffer[256];
                strncpy(buffer, cmd.argument, sizeof(buffer) - 1);
                buffer[sizeof(buffer) - 1] = '\0';

                char *token = strtok(buffer, ",");

                while (token)
                {
                    while (isspace((unsigned char)*token))
                        token++;

                    int len = strlen(token);
                    while (len > 0 && isspace((unsigned char)token[len - 1]))
                    {
                        token[len - 1] = '\0';
                        len--;
                    }

                    if (len >= 2 && token[0] == '"' && token[len - 1] == '"')
                    {
                        token[len - 1] = '\0';
                        printf("%s ", token + 1);
                    }
                    else
                    {
                        double value;
                        int res = var_get(token, &value);

                        if (res == 1)
                        {
                            printf("%g ", value);
                        }
                        else if (res == -1)
                        {
                            printf("× المتغير %s ليس له قيمة ", token);
                        }
                        else
                        {
                            char msg[128];
                            snprintf(msg, sizeof(msg), "المتغير '%s' غير موجود", token);
                            error_add(ERR_UNDEFINED_VARIABLE, msg, CURRENT_LINE);
                        }
                    }

                    token = strtok(NULL, ",");
                }

                printf("\n");
            }

            break;
        }

        // ===============================
        // تنظيف الشاشة
        // ===============================
        case CMD_CLEAR:
            for (int i = 0; i < cmd.repeat; i++)
                rt_clear();
            break;

        // ===============================
        // تنفيذ تعبير رياضي
        // ===============================
        case CMD_EXEC:
        {
            double value;
            int ret = math_eval(cmd.argument, &value);

            if (ret == 0)
            {
                if (fabs(value - (long long)value) < 1e-9)
                    printf("الناتج = %lld\n", (long long)value);
                else
                    printf("الناتج = %.10f\n", value);
            }
            else if (ret == 2)
            {
                // BigInt
            }
            else
            {
                printf("❌ خطأ في التعبير الرياضي\n");
            }

            break;
        }

        // ===============================
        // إدخال
        // ===============================
        case CMD_INPUT:
        {
            double value;

            printf("ادخل قيمة %s: ", cmd.argument);

            if (scanf("%lf", &value) != 1)
            {
                printf("× إدخال غير صالح\n");

                int c;
                while ((c = getchar()) != '\n' && c != EOF);

                break;
            }

            getchar();

            var_set(cmd.argument, value);

            break;
        }

        // ===============================
        // 🔥 إسناد متغير (مصحح بالكامل)
        // ===============================
        case CMD_ASSIGN:
        {
            char name[32];
            double val;

            // ⚠️ لا تعدل cmd.argument مباشرة
            char temp[128];
            strncpy(temp, cmd.argument, sizeof(temp) - 1);
            temp[sizeof(temp) - 1] = '\0';

            char *eq = strchr(temp, '=');

            if (eq)
            {
                *eq = '\0';

                // 🔹 تنظيف الاسم
                char *start = temp;
                while (isspace((unsigned char)*start)) start++;

                char *end = start + strlen(start) - 1;
                while (end > start && isspace((unsigned char)*end))
                {
                    *end = '\0';
                    end--;
                }

                strcpy(name, start);

                // 🔹 تنظيف القيمة
                char *value_str = eq + 1;
                while (isspace((unsigned char)*value_str)) value_str++;

                if (strcmp(value_str, "؟") == 0)
                {
                    var_set(name, NAN);
                    break;
                }

                if (math_eval(value_str, &val) == 0)
                {
                    var_set(name, val);
                }
                else
                {
                    printf("❌ خطأ في التعبير بعد علامة =\n");
                }
            }

            break;
        }

        // ===============================
        // حل معادلة
        // ===============================
        case CMD_SOLVE:
        {
            double value;
            char solved_var[32];

            int r = solve_equation(cmd.argument, &value, solved_var);

            if (r == 0)
            {
                if (fabs(value - (long long)value) < 1e-9)
                    printf("✅ %s = %lld\n", solved_var, (long long)value);
                else
                    printf("✅ %s = %.10f\n", solved_var, value);

                var_set(solved_var, value);
            }
            else
            {
                printf("❌ فشل في حل المعادلة\n");
            }

            break;
        }

        // ===============================
        // تنفيذ أمر نظام
        // ===============================
        case CMD_EXEC_SYSTEM:
            execute_system_command(cmd.argument);
            break;

        // ===============================
        // خروج
        // ===============================
        case CMD_EXIT:
            return 1;

        case CMD_BREAK:
            return 2;

        // ===============================
        // غير معروف
        // ===============================
        default:
            printf("❌ أمر غير معروف\n");
            break;
    }

    return 0;
}

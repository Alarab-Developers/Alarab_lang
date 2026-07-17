#include <ctype.h>
#include <string.h>
#include "أدوات_الكشف.h"
int is_line_empty(const char *line) {
    while (*line) {
        if (!isspace(*line))
            return 0;
        line++;
    }
    return 1;
}
void remove_comments(char *line, int *inside_block)
{
    char result[512] = "";
    int i = 0, j = 0;

    while (line[i])
    {
        // =========================
        // داخل تعليق متعدد
        // =========================
        if (*inside_block)
        {
            if (line[i] == '*' && line[i+1] == '#')
            {
                *inside_block = 0;
                i += 2;
            }
            else
            {
                i++;
            }
            continue;
        }

        // =========================
        // بداية تعليق متعدد
        // =========================
        if (line[i] == '#' && line[i+1] == '*')
        {
            *inside_block = 1;
            i += 2;
            continue;
        }

        // =========================
        // تعليق سطر #
        // =========================
        if (line[i] == '#')
        {
            break; // تجاهل باقي السطر
        }

        // =========================
        // نسخ الحرف
        // =========================
        result[j++] = line[i++];
    }

    result[j] = '\0';
    strcpy(line, result);
}

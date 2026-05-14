#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "تحميل.h"

int parse_import_command(const char *clean, Command *cmd)
{
    if (!clean) return 0;

    // يجب أن يبدأ بـ "تحميل"
    if (strncmp(clean, "تحميل", 5) != 0)
        return 0;

    char filename[256] = {0};

    // =====================================
    // 🔥 دعم جميع الحالات:
    // تحميل(ملف)
    // تحميل = (ملف)
    // تحميل=(ملف)
    // =====================================

    const char *open = strchr(clean, '(');
    const char *close = strrchr(clean, ')');

    if (!open || !close || close < open)
        return 0;

    int len = close - open - 1;

    if (len <= 0 || len >= (int)sizeof(filename))
        return 0;

    strncpy(filename, open + 1, len);
    filename[len] = '\0';

    // =====================================
    // 🔥 تنظيف النص
    // =====================================

    // إزالة المسافات من البداية
    char *start = filename;
    while (isspace((unsigned char)*start)) start++;

    // إذا أصبح فارغ
    if (*start == '\0')
        return 0;

    // إزالة المسافات من النهاية
    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }

    // إزالة علامات الاقتباس " "
    if (start[0] == '"' && end > start && *end == '"') {
        start++;
        *end = '\0';
    }

    // تحقق أن الاسم ليس فارغ
    if (strlen(start) == 0)
        return 0;

    // =====================================
    // 🔥 تعبئة الأمر
    // =====================================

    cmd->type = CMD_IMPORT;

    strncpy(cmd->argument, start, sizeof(cmd->argument) - 1);
    cmd->argument[sizeof(cmd->argument) - 1] = '\0';

    return 1;
}

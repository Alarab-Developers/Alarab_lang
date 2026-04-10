#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "مفسر_العمليات.h"
#include "المنفذ.h"
#include "محرك_الدالة.h"
#include "محرك_كشف_الأخطاء.h"
#include "أدوات_الكشف.h"
int CURRENT_LINE = 0;
int inside_block_comment = 0;


int main(int argc, char *argv[]) {

    char line[512];

    // =====================================
    // تشغيل ملف إذا تم تمرير اسم ملف
    // =====================================
    if (argc > 1) {

        FILE *file = fopen(argv[1], "r");
        if (!file) {
            printf("خطأ: لا يمكن فتح الملف %s\n", argv[1]);
            return 1;
        }

        while (fgets(line, sizeof(line), file)) {

            CURRENT_LINE++;

        line[strcspn(line, "\n")] = '\0';
        remove_comments(line, &inside_block_comment);

// 2. تجاهل الفراغ
        if (is_line_empty(line)) {
            continue;
        }

// 1. تجاهل السطر الفارغ
        if (is_line_empty(line)) {
            continue;
        }

// 2. تجاهل التعليقات (حتى مع مسافات)
        char *p = line;
        while (*p == ' ') p++;

        if (*p == '#') {
            continue;
        }

// 3. الآن فقط نحلل
        Command cmd = parse_line(line);

// 4. تجاهل EMPTY (احتياط)
        if (cmd.type == CMD_EMPTY) {
            continue;
        }


            if (cmd.type == CMD_UNKNOWN) {
                char msg[512];

                int max_len = sizeof(msg) - 3;

                snprintf(msg, sizeof(msg), "{%.*s}", max_len, line);

                error_add(ERR_UNKNOWN_COMMAND, msg, CURRENT_LINE);
                continue;
            }
            // ===============================
            // تعريف دالة
            // ===============================
            if (cmd.type == CMD_FUNCTION_DEF) {

                char body[2048] = "";
                char inner[512];
                int brace_level = 1;

                while (fgets(inner, sizeof(inner), file)) {

                    inner[strcspn(inner, "\n")] = '\0';

                    if (strchr(inner, '{'))
                        brace_level++;

                    if (strchr(inner, '}'))
                        brace_level--;

                    if (brace_level == 0)
                        break;

                    strcat(body, inner);
                    strcat(body, "\n");
                }

                function_store(cmd.name, body);
                continue;
            }

            // ===============================
            // تنفيذ الأوامر
            // ===============================
            if (execute_command(cmd))
                break;
        }

        fclose(file);
        if (error_has_any()) {
            error_print_all();
            return 1;
        }
        return 0;
    }

    // =====================================
    // الوضع التفاعلي REPL
    // =====================================
    while (1) {

        printf("لغة> ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin))
            break;

        CURRENT_LINE++;

        line[strcspn(line, "\n")] = '\0';
        remove_comments(line, &inside_block_comment);

// 2. تجاهل الفراغ
        if (is_line_empty(line)) {
            continue;
        }

// 1. تجاهل السطر الفارغ
        if (is_line_empty(line)) {
            continue;
        }

// 2. تجاهل التعليقات (حتى مع مسافات)
        char *p = line;
        while (*p == ' ') p++;

        if (*p == '#') {
            continue;
        }

// 3. الآن فقط نحلل
        Command cmd = parse_line(line);

// 4. تجاهل EMPTY (احتياط)
        if (cmd.type == CMD_EMPTY) {
            continue;
        }

        if (cmd.type == CMD_UNKNOWN) {
                char msg[512];

                int max_len = sizeof(msg) - 3;

                snprintf(msg, sizeof(msg), "{%.*s}", max_len, line);

                error_add(ERR_UNKNOWN_COMMAND, msg, CURRENT_LINE);
            continue;
        }

        // ===============================
        // تعريف دالة
        // ===============================
        if (cmd.type == CMD_FUNCTION_DEF) {

            char body[2048] = "";
            char inner[512];
            int brace_level = 1;

            while (1) {

                if (!fgets(inner, sizeof(inner), stdin))
                    break;

                inner[strcspn(inner, "\n")] = '\0';

                if (strchr(inner, '{'))
                    brace_level++;

                if (strchr(inner, '}'))
                    brace_level--;

                if (brace_level == 0)
                    break;

                strcat(body, inner);
                strcat(body, "\n");
            }

            function_store(cmd.name, body);
            continue;
        }

        // ===============================
        // تنفيذ الأوامر
        // ===============================
        if (execute_command(cmd))
            break;
    }

    return 0;
}

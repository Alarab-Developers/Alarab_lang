#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "محرك_الترجمة.h"
#include "مدير_التنفيذ.h"
#include "محرك_الدالة.h"
#include "محرك_الفحص.h"
#include "أدوات_الكشف.h"

#include "مدير_البناء.h"

int CURRENT_LINE = 0;
int inside_block_comment = 0;

// =====================================
// معالجة سطر واحد (ملف أو REPL)
// =====================================
int معالجة_سطر(char *line, FILE *input) {

    line[strcspn(line, "\n")] = '\0';
    remove_comments(line, &inside_block_comment);

    if (is_line_empty(line)) return 0;

    char *p = line;
    while (*p == ' ') p++;
    if (*p == '#') return 0;

    Command cmd = parse_line(line);

    if (cmd.type == CMD_EMPTY) return 0;

    if (cmd.type == CMD_UNKNOWN) {
        char msg[512];
        int max_len = sizeof(msg) - 3;
        snprintf(msg, sizeof(msg), "{%.*s}", max_len, line);
        error_add(ERR_UNKNOWN_COMMAND, msg, CURRENT_LINE);
        return 0;
    }

    // ===============================
    // تعريف دالة
    // ===============================
    if (cmd.type == CMD_FUNCTION_DEF) {

        if (!check_function_syntax(line, CURRENT_LINE)) {

            char skip[512];

            while (fgets(skip, sizeof(skip), input)) {
                if (strchr(skip, '}'))
                    break;
            }

            return 0;
        }

        char body[2048] = "";
        char inner[512];
        int brace_level = 1;

        while (fgets(inner, sizeof(inner), input)) {

            inner[strcspn(inner, "\n")] = '\0';

            if (strchr(inner, '{')) brace_level++;
            if (strchr(inner, '}')) brace_level--;

            if (brace_level == 0)
                break;

            strcat(body, inner);
            strcat(body, "\n");
        }

        if (brace_level != 0) {
            error_add(ERR_SYNTAX, "الدالة غير مغلقة", CURRENT_LINE);
            return 0;
        }

        if (!error_check_program(body, CURRENT_LINE)) {
            return 0;
        }

        function_store(cmd.name, body);
        return 0;
    }

    // ===============================
    // تنفيذ
    // ===============================
    if (execute_command(cmd))
        return 1;

    return 0;
}


// =====================================
// main
// =====================================
int main(int argc, char *argv[]) {

    char line[512];
    const char *filename = NULL;

    // =====================================
    // أوامر CLI
    // =====================================

    // تشغيل
    if (argc >= 3 && strcmp(argv[1], "تشغيل") == 0) {
        filename = argv[2];
    }

    // بناء
    else if (argc >= 3 && strcmp(argv[1], "بناء") == 0) {

        const char *input_file = argv[2];

        FILE *in = fopen(input_file, "r");
        if (!in) {
            printf("خطأ: لا يمكن فتح الملف %s\n", input_file);
            return 1;
        }

        FILE *out = fopen("out.c", "w");
        if (!out) {
            printf("خطأ: لا يمكن إنشاء out.c\n");
            fclose(in);
            return 1;
        }

        fprintf(out, "#include <stdio.h>\nint main(){\n");

        char line_build[256];

        while (fgets(line_build, sizeof(line_build), in))
        {
            line_build[strcspn(line_build, "\n")] = '\0';
            ترجم_سطر(line_build, out);
        }

        fprintf(out, "return 0;\n}");

        fclose(in);
        fclose(out);

        printf("✔ تم إنشاء out.c\n");

        // تجميع
        int res = system("gcc out.c -o برنامج");

        if (res == 0)
            printf("✔ تم البناء: برنامج\n");
        else
            printf("❌ فشل البناء\n");

        return 0;
    }

    // تشغيل مباشر (./اللغة ملف.ax)
    else if (argc == 2) {
        filename = argv[1];
    }

    // =====================================
    // تشغيل ملف
    // =====================================
    if (filename) {

        FILE *file = fopen(filename, "r");
        if (!file) {
            printf("خطأ: لا يمكن فتح الملف %s\n", filename);
            return 1;
        }

        while (fgets(line, sizeof(line), file)) {
            CURRENT_LINE++;

            if (معالجة_سطر(line, file))
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
    // REPL
    // =====================================
    while (1) {

        printf("لغة> ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin))
            break;

        CURRENT_LINE++;

        if (معالجة_سطر(line, stdin))
            break;
    }

    return 0;
}

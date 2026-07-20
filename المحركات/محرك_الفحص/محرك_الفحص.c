#include "محرك_الفحص.h"
#include <stdio.h>
#include <string.h>
#include "أدوات_الكشف.h"

static Error errors[MAX_ERRORS];
static int error_count = 0;
const char* error_type_to_string(ErrorType type);

/* ===================================== */
/* إضافة خطأ */
/* ===================================== */
void error_add(ErrorType type, const char *message, int line)
{
    if (error_count >= MAX_ERRORS) return;
    if (!message || message[0] == '\0') return;

    errors[error_count].type = type;
    errors[error_count].line = line;

    strncpy(errors[error_count].message, message, 255);
    errors[error_count].message[255] = '\0';

    error_count++;
}

/* ===================================== */
/* طباعة الأخطاء */
/* ===================================== */
void error_print_all()
{
    for (int i = 0; i < error_count; i++)
    {
        printf("[%s] خطأ في السطر %d: %s\n",
            error_type_to_string(errors[i].type),
            errors[i].line,
            errors[i].message);
    }
}

/* ===================================== */
const char* error_type_to_string(ErrorType type)
{
    switch(type) {
        case ERR_SYNTAX: return "نحوي";
        case ERR_RUNTIME: return "تشغيلي";
        case ERR_UNDEFINED_VARIABLE: return "متغير";
        case ERR_UNKNOWN_COMMAND: return "أمر غير معروف";
        default: return "خطأ";
    }
}

/* ===================================== */
int error_has_any()
{
    return error_count > 0;
}

void error_clear()
{
    error_count = 0;
}

/* ===================================== */
/* 🔥 فحص الأقواس */
/* ===================================== */
int check_braces_balance(const char *code, int line)
{
    int brace = 0;

    for (int i = 0; code[i]; i++)
    {
        if (code[i] == '{') brace++;
        else if (code[i] == '}') brace--;

        if (brace < 0)
        {
            error_add(ERR_SYNTAX, "يوجد } بدون {", line);
            return 0;
        }
    }

    if (brace > 0)
    {
        error_add(ERR_SYNTAX, "يوجد { بدون }", line);
        return 0;
    }

    return 1;
}


int check_function_syntax(const char *line, int line_number)
{
    if (!line) return 1;

    // يبدأ بـ دالة(
    if (strncmp(line, "دالة(", 5) != 0)
        return 1; // ليس دالة → تجاهل

    const char *open = strchr(line, '(');
    const char *close = strchr(line, ')');

    if (!open || !close || close < open)
    {
        error_add(ERR_SYNTAX, "صيغة تعريف الدالة غير صحيحة", line_number);
        return 0;
    }

    // 🔥 التحقق من وجود {
    if (!strchr(line, '{'))
    {
        error_add(ERR_SYNTAX, "يجب وضع { بعد تعريف الدالة", line_number);
        return 0;
    }

    return 1;
}

/* ===================================== */
/* 🔥 فحص برنامج كامل */
/* ===================================== */
int error_check_program(const char *code, int line)
{
    if (!check_braces_balance(code, line))
        return 0;

    return 1;
}

#include "محرك_كشف_الأخطاء.h"
#include <stdio.h>
#include <string.h>
#include "أدوات_الكشف.h"

const char* error_type_to_string(ErrorType type);

static Error errors[MAX_ERRORS];
static int error_count = 0;

void error_add(ErrorType type, const char *message, int line)
{
    if (error_count >= MAX_ERRORS)
        return;

    if (!message || message[0] == '\0')
        return;

    errors[error_count].type = type;
    errors[error_count].line = line;

    strncpy(errors[error_count].message, message, 255);
    errors[error_count].message[255] = '\0';

    error_count++;
}

void error_print_all()
{
    for (int i = 0; i < error_count; i++)
    {
        printf("[%s] يوجد خطاء في السطر %d : هو %s\n",
                error_type_to_string(errors[i].type),
                errors[i].line,
                errors[i].message);
    }
}

const char* error_type_to_string(ErrorType type)
{
    switch(type) {
        case ERR_SYNTAX: return "موصلة";
        case ERR_RUNTIME: return "مشغل";
        case ERR_UNDEFINED_VARIABLE: return "متغير";
        case ERR_UNKNOWN_COMMAND: return "كتابة خاطئة";
        default: return "خطأ";
    }
}

int error_has_any()
{
    return error_count > 0;
}

void error_clear()
{
    error_count = 0;
}

#ifndef ERROR_ENGINE_H
#define ERROR_ENGINE_H

#define MAX_ERRORS 100

typedef enum {
    ERR_NONE,
    ERR_SYNTAX,
    ERR_UNKNOWN_COMMAND,
    ERR_UNDEFINED_VARIABLE,
    ERR_UNDEFINED_FUNCTION,
    ERR_RUNTIME,
    ERR_MEMORY
} ErrorType;

typedef struct {
    ErrorType type;
    char message[256];
    int line;
} Error;

// إضافة خطأ
void error_add(ErrorType type, const char *message, int line);

// طباعة كل الأخطاء
void error_print_all();

// هل يوجد أخطاء؟
int error_has_any();

// إعادة تعيين
void error_clear();

#endif

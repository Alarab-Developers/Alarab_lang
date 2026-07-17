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

void error_add(ErrorType type, const char *message, int line);
void error_print_all();
int error_has_any();
void error_clear();


int check_braces_balance(const char *code, int line);
int error_check_program(const char *code, int line);
int check_function_syntax(const char *line, int line_number);

#endif

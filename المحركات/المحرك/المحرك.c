#include "المحرك.h"
#include "محرك_الترجمة.h"
#include "التنفيذ_الفردي.h"

int engine_run_line(const char *line) {
    Command cmd = parse_line(line);
    return runtime_execute(cmd);   // ← يرجّع 1 عند خروج
}


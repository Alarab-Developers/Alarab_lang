#ifndef EXEC_CMD_H
#define EXEC_CMD_H

#include "مفسر_العمليات.h"

/* تحليل أمر نفذ */
int parse_exec_command(const char *clean, Command *cmd);

#endif

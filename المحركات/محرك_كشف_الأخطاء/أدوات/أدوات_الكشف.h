#ifndef TEXT_UTILS_H
#define TEXT_UTILS_H

int is_line_empty(const char *line);
void remove_comments(char *line, int *inside_block);

#endif

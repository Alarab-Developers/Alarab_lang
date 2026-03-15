#ifndef IF_ENGINE_H
#define IF_ENGINE_H

void if_store(const char *name, const char *condition, const char *body);

const char *if_get(const char *name);

const char *if_get_condition(const char *name);

#endif

#include <stdio.h>
#include <string.h>

void ترجم_سطر(const char* line, FILE* out)
{
    // احسب
    if (strncmp(line, "احسب", 4) == 0)
    {
        const char* eq = strchr(line, '=');
        if (!eq) return;

        eq++;

        while (*eq == ' ') eq++;

        fprintf(out, "printf(\"الناتج = %%f\\n\", %s);\n", eq);
    }

    // اطبع
    else if (strncmp(line, "اطبع", 4) == 0)
    {
        const char* eq = strchr(line, '=');
        if (!eq) return;

        eq++;

        while (*eq == ' ') eq++;

        fprintf(out, "printf(%s);\n", eq);
    }
}

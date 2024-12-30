#include <stdio.h>
#include "include/top.h"

int main(void)
{
    PROCTAB *proc = safe_create__proc();
    process_t *process = safe_create__process_struct(proc);

    char *data = read__proc(proc, process);
    if (data != NULL) {
        char *line = strtok(data, "\n");

        while (line != NULL) {
            puts(line);
            line = strtok(NULL, "\n");
        } free(data);
    }

    free(process);
    closeproc(proc);

    return EXIT_SUCCESS;
}

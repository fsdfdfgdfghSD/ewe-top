#include <stdio.h>
#include "main.h"

int main(void)
{
    PROCTAB *proc = openproc(PROC_FLAGS);
    if (proc == NULL)
        eputs("openproc");

    process_t *process = malloc(sizeof(process_t));
    if (process == NULL)
        eputs("malloc");

    /* Initialize process->p with zeros. */
    memset(&process->p, 0, sizeof(process->p));

    while ((process->p = readproc(proc, NULL)) != NULL) {
        process->cpu_usage = (process->p->utime + process->p->stime);

        char buf[1024];
        snprintf(buf, sizeof(buf), "PID: %d, Name: \"%s\", Status: '%c', CPU: %ld, Memory: %ld KB\n",
            process->p->tid,
            process->p->cmd,
            process->p->state,
            process->cpu_usage,
            process->p->vm_size
        );

        printf("%s", buf);
        //writetw((const char*)buf);
    }

    closeproc(proc);
    return EXIT_SUCCESS;
}

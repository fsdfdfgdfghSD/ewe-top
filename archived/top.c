#include <stdio.h>
#include "top.h"

char *uid_to_str(uid_t uid)
{
    struct passwd *pw = getpwuid(uid);
    if (pw == NULL)
        eputsfr("getpwuid", errno);

    return pw->pw_name;
}

process_t *safe_create__process_struct(PROCTAB *proc)
{
    process_t *process = (process_t *)malloc(sizeof(process_t));
    if (process == NULL) {
        closeproc(proc);
        eputs("malloc");
    }

    /* Initialize process->p with zeros */
    memset(&process->p, 0, sizeof(process->p));

    return process;
}

PROCTAB *safe_create__proc(void)
{
    PROCTAB *proc = openproc(PROC_FLAGS);
    if (proc == NULL)
        eputs("openproc");

    return proc;
}

void fetch_proc_data(process_t *process, char *buf, size_t bytes_write)
{
    /* TODO: Add Power-Usage, similar to windows */
    /* TODO: Rearrange the information in a better way (for unix), and get more information */
   
    /* TODO: Convert cpu to 0.1, 0.2, decimal basically */
    const char *fmt = "%-10s %-6d %-20s %-6c %-6d %-10ld";

    /* Porcess start time + Kernel time */
    process->cpu_usage = (process->p->utime + process->p->stime);
    process->uid_user = uid_to_str(process->p->euid);

    (void)snprintf(buf, bytes_write, fmt,
        process->uid_user,              /* UID (User) */
        process->p->tid,                /* PID */
        process->p->cmd,                /* CMD */
        process->p->state,              /* Status, e.g. 'S', 'R' */
        process->cpu_usage,             /* CPU */
        process->p->vm_size / 1024      /* MEM (MB) */
    );
}

char *read__proc(PROCTAB *proc, process_t *process)
{
    char *data = (char *)malloc(DATA_MAX_SIZE);
    if (data == NULL) {
        closeproc(proc);
        eputs("malloc");
    }

    size_t data_size = DATA_MAX_SIZE;
    size_t data_used = 0;

    while ((process->p = readproc(proc, NULL)) != NULL) {
        char *buf = (char *)malloc(ITERATION_CHUNK_SIZE);
        if (buf == NULL) {
            /* Skip the process */

            continue;
        }

        fetch_proc_data(process, buf, ITERATION_CHUNK_SIZE);
        size_t buf_used = strlen(buf) + 1;

        if (buf_used >= ITERATION_CHUNK_SIZE) {
            char *new_buf = (char *)realloc(buf, buf_used);
            if (new_buf == NULL) {
                free(buf);

                continue; /* Skip the process on error */
            }

            buf = new_buf;
        }

        if (data_used + buf_used + 1 > data_size) {
            data_size = data_used + buf_used + 1;

            char *new_data = (char *)realloc(data, data_size);
            if (new_data == NULL) {
                free(buf);
                free(data);
                closeproc(proc);

                eputs("realloc");
            }

            data = new_data;
        }

        memcpy(data + data_used, buf, buf_used - 1);
        data_used += buf_used - 1;
        data[data_used++] = '\n';

        free(buf);
    }

    data[data_used] = '\0';
    return data;
}

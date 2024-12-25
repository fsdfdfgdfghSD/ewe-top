#ifndef TOP_H_
#define TOP_H_

#include <stdint.h>
#include <pwd.h>

/* ----------------------- */

/*
 * No need to include stdlib.h, string.h,
 * `error.h' already has them included for us.
*/
#include "error.h"

#include <proc/readproc.h>
/* ----------------------- */

#define ITERATION_CHUNK_SIZE    256
#define DATA_MAX_SIZE           1024

#define PROC_FLAGS ( PROC_FILLMEM \
    | PROC_FILLSTAT |  PROC_FILLSTATUS )

typedef struct {
    uint64_t cpu_usage;
    const char *uid_user;
    struct proc_t *p;
} process_t;

/* <------> Functions <------> */
char *uid_to_str(uid_t uid);

process_t *safe_create__process_struct(PROCTAB *proc);
PROCTAB *safe_create__proc(void);
char *read__proc(PROCTAB *proc, process_t *process);

#endif /* TOP_H_ */

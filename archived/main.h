#ifndef MAIN_H_
#define MAIN_H_

#include <stdlib.h>
#include <stdint.h>

#include "include/error.h"

/* PROC RELATED */
#include <proc/readproc.h>

#define PROC_FLAGS ( PROC_FILLMEM \
    | PROC_FILLSTAT | PROC_FILLSTATUS )

typedef struct {
    int64_t cpu_usage;
    struct proc_t *p;
} process_t;

#endif /* MAIN_H_ */

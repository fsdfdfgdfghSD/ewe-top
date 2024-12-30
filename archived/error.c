#include <stdio.h>
#include "error.h"

void eputs(const char *e)
{
    /* Dont use `perror' because it appends errno at the end. */
    fprintf(stderr, "%s\n", e);

    exit(EXIT_CODE);
}

char *strerror_c(int errnum)
{
    /* Use errno as default */
    errnum = (errnum != 0) ? errnum : errno;

    char *buf = malloc(ERROR_MSG_MAX);
    if (buf == NULL)
        eputs("malloc");

    #ifdef _GNU_SOURCE
    /* GNU-Specific Version */

    char *msg = strerror_r(errno, buf, ERROR_MSG_MAX);
    if (msg != buf) {
        strncpy(buf, msg, ERROR_MSG_MAX - 1);
        buf[ERROR_MSG_MAX - 1] = '\0'
    }
    #else
    /* POSIX-Compliant Version */

    if (strerror_r(errno, buf, ERROR_MSG_MAX) != 0) {
        snprintf(
            buf,
            ERROR_MSG_MAX,
            "Unknown error code: %d",
            errno
        );

        /* We're done! */
    }

    #endif /* _GNU_SOURCE */
    return buf;
}

void eputsfr(const char *e, int errnum)
{
    char *error_message = strerror_c(errnum);
    char *final_message = malloc(ERROR_MSG_MAX);

    snprintf(
        final_message,
        ERROR_MSG_MAX,
        "%s | Error: \"%s\"",
        e, error_message
    );
    eputs((const char*)final_message);

    free(final_message); free(error_message);
}

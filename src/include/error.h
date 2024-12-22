#ifndef ERROR_H_
#define ERROR_H_

#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define EXIT_CODE -1
#define ERROR_MSG_MAX 256

/* Outputs the error to stderr and exits with `-1' */
void eputs(const char *e);

/* Compatible, safe, errno to human-readable version. */
char *strerror_c(int errnum);

/* Outputs the error using `eputs' and uses `strerror_c', also frees everything. */
void eputsfr(const char *e, int errnum);

/* Examples
 * `eputsfr("Hello, World!", errno);'
*/

#endif /* ERROR_H_ */

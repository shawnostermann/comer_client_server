#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/*
 * ====================================================================
 * Error - log an error entry
 * ====================================================================
 */
void
errexit(char *msg, ...)
{
    va_list ap;

    /* make the log entry */
    va_start(ap,msg);
    vfprintf(stderr,msg,ap);
    fprintf(stderr,"  (%d: %s)\n", errno, strerror(errno));
    va_end(ap);

    fflush(stderr);

    exit(1);
}

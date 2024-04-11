/* dcon.c - dcon */

#include <sys/ioctl.h>

#include <stdio.h>

#include "local.h"

/*------------------------------------------------------------------------
 * dcon - disconnect from remote
 *------------------------------------------------------------------------
 */
/*ARGSUSED*/
int
dcon(sfp, tfp, c)
FILE    *sfp, *tfp;
int     c;
{
        fprintf(tfp, "disconnecting.\n");
        ttyrestore(&oldtty);
        exit(0);
}

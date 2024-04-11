/* passiveTCP.c - passiveTCP */

#include "libnet.h"

/*------------------------------------------------------------------------
 * passiveTCP - create a passive socket for use in a TCP server
 *------------------------------------------------------------------------
 */
int
passiveTCP(char *service, int qlen)
                        /* service associated with the desired port     */
                        /* maximum server request queue length          */
{
        return passivesock(service, "tcp", qlen);
}

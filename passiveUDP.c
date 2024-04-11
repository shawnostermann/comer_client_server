/* passiveUDP.c - passiveUDP */

#include "libnet.h"

/*------------------------------------------------------------------------
 * passiveUDP - create a passive socket for use in a UDP server
 *------------------------------------------------------------------------
 */
int
passiveUDP(char *service)
                        /* service associated with the desired port     */
{
        return passivesock(service, "udp", 0);
}

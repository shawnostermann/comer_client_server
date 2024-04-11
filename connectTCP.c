/* connectTCP.c - connectTCP */

#include "libnet.h"

/*------------------------------------------------------------------------
 * connectTCP - connect to a specified TCP service on a specified host
 *------------------------------------------------------------------------
 */
int
connectTCP(char *host, char *service)
                        /* name of host to which connection is desired  */
                        /* service associated with the desired port     */
{
        return connectsock( host, service, "tcp");
}

/* TCPdaytimed.c - main */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/errno.h>
#include <time.h>
#include "libnet.h"

#define QLEN    5

void TCPdaytimed(int fd);

/*------------------------------------------------------------------------
 * main - Iterative TCP server for DAYTIME service
 *------------------------------------------------------------------------
 */
int
main(int argc, char **argv)
{
        struct  sockaddr_in fsin;       /* the from address of a client */
        char    *service = "daytime";   /* service name or port number  */
        int     msock, ssock;           /* master & slave sockets       */
        unsigned     alen;                   /* from-address length          */

        switch (argc) {
        case    1:
                break;
        case    2:
                service = argv[1];
                break;
        default:
                errexit("usage: TCPdaytimed [port]\n");
        }

        msock = passiveTCP(service, QLEN);

        while (1) {
                ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
                if (ssock < 0)
                        errexit("accept failed: %s\n",sys_errlist[errno]);
                (void) TCPdaytimed(ssock);
                (void) close(ssock);
        }
}

/*------------------------------------------------------------------------
 * TCPdaytimed - do TCP DAYTIME protocol
 *------------------------------------------------------------------------
 */
void
TCPdaytimed(int fd)
{
        char    *pts;                   /* pointer to time string       */
        time_t  now;                    /* current time                 */
        char    *ctime(const time_t *);

        (void) time(&now);
        pts = ctime(&now);
        (void) write(fd, pts, strlen(pts));
}

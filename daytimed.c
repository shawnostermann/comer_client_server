/* daytimed.c - main */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "libnet.h"

#define MAX(x, y)       ((x) > (y) ? (x) : (y))

#define QLEN              5

#define LINELEN         128

int daytime(char *buf);
/*------------------------------------------------------------------------
 * main - Iterative server for DAYTIME service
 *------------------------------------------------------------------------
 */
int
main(int argc, char **argv)
{
        char    *service = "daytime";   /* service name or port number  */
        char    buf[LINELEN+1];         /* buffer for one line of text  */
        struct sockaddr_in fsin;        /* the request from address     */
        unsigned     alen;                   /* from-address length          */
        int     tsock;                  /* TCP master socket            */
        int     usock;                  /* UDP socket                   */
        int     nfds;
        fd_set  rfds;                   /* readable file descriptors    */

        switch (argc) {
        case    1:
                break;
        case    2:
                service = argv[1];
                break;
        default:
                errexit("usage: daytimed [port]\n");
        }

        tsock = passiveTCP(service, QLEN);
        usock = passiveUDP(service);
        nfds = MAX(tsock, usock) + 1;   /* bit number of max fd */

        FD_ZERO(&rfds);

        while (1) {
                FD_SET(tsock, &rfds);
                FD_SET(usock, &rfds);

                if (select(nfds, &rfds, (fd_set *)0, (fd_set *)0,
                                (struct timeval *)0) < 0)
                        errexit("select error: %s\n", sys_errlist[errno]);
                if (FD_ISSET(tsock, &rfds)) {
                        int     ssock;          /* TCP slave socket     */

                        alen = sizeof(fsin);
                        ssock = accept(tsock, (struct sockaddr *)&fsin,
                                &alen);
                        if (ssock < 0)
                                errexit("accept failed: %s\n",
                                                sys_errlist[errno]);
                        daytime(buf);
                        (void) write(ssock, buf, strlen(buf));
                        (void) close(ssock);
                }
                if (FD_ISSET(usock, &rfds)) {
                        alen = sizeof(fsin);
                        if (recvfrom(usock, buf, sizeof(buf), 0,
                                (struct sockaddr *)&fsin, &alen) < 0)
                                errexit("recvfrom: %s\n",
                                        sys_errlist[errno]);
                        daytime(buf);
                        (void) sendto(usock, buf, strlen(buf), 0,
                                (struct sockaddr *)&fsin, sizeof(fsin));
                }
        }
        exit(0);

}

/*------------------------------------------------------------------------
 * daytime - fill the given buffer with the time of day
 *------------------------------------------------------------------------
 */
int
daytime(char *buf)
{
        char    *ctime(const time_t *);
        time_t  time(time_t *), now;

        (void) time(&now);
        sprintf(buf, "%s", ctime(&now));
        return(0);
}


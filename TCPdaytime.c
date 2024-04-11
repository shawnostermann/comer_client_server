/* TCPdaytime.c - TCPdaytime, main */

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

#define LINELEN         128

void TCPdaytime(char *host, char *service);

/*------------------------------------------------------------------------
 * main - TCP client for DAYTIME service
 *------------------------------------------------------------------------
 */
int
main(int argc, char **argv)
{
        char    *host = "localhost";    /* host to use if none supplied */
        char    *service = "daytime";   /* default service port         */

        switch (argc) {
        case 1:
                host = "localhost";
                break;
        case 3:
                service = argv[2];
                /* FALL THROUGH */
        case 2:
                host = argv[1];
                break;
        default:
                fprintf(stderr, "usage: TCPdaytime [host [port]]\n");
                exit(1);
        }
        TCPdaytime(host, service);
        exit(0);
}

/*------------------------------------------------------------------------
 * TCPdaytime - invoke Daytime on specified host and print results
 *------------------------------------------------------------------------
 */
void TCPdaytime(char *host, char *service)
{
        char    buf[LINELEN+1];         /* buffer for one line of text  */
        int     s, n;                   /* socket, read count           */

        s = connectTCP(host, service);

        while( (n = read(s, buf, LINELEN)) > 0) {
                buf[n] = '\0';          /* insure null-terminated       */
                (void) fputs( buf, stdout );
        }
}

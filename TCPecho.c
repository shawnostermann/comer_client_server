/* TCPecho.c - main, TCPecho */

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

void TCPecho(char *host, char *service);

#define LINELEN         128

/*------------------------------------------------------------------------
 * main - TCP client for ECHO service
 *------------------------------------------------------------------------
 */
int
main(int argc, char **argv)
{
        char    *host = "localhost";    /* host to use if none supplied */
        char    *service = "echo";      /* default service name         */

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
                fprintf(stderr, "usage: TCPecho [host [port]]\n");
                exit(1);
        }
        TCPecho(host, service);
        exit(0);
}

/*------------------------------------------------------------------------
 * TCPecho - send input to ECHO service on specified host and print reply
 *------------------------------------------------------------------------
 */
void
TCPecho(char *host, char *service)
{
        char    buf[LINELEN+1];         /* buffer for one line of text  */
        int     s, n;                   /* socket descriptor, read count*/
        int     outchars, inchars;      /* characters sent and received */

        s = connectTCP(host, service);

        while (fgets(buf, sizeof(buf), stdin)) {
                buf[LINELEN] = '\0';    /* insure line null-terminated  */
                outchars = strlen(buf);
                (void) write(s, buf, outchars);

                /* read it back */
                for (inchars = 0; inchars < outchars; inchars+=n ) {
                        n = read(s, &buf[inchars], outchars - inchars);
                        if (n < 0)
                                errexit("socket read failed: %s\n",
                                        sys_errlist[errno]);
                }
                fputs(buf, stdout);
        }
}

/* UDPechod.c - main */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdio.h>

#include "libnet.h"


#define BUFSIZE 65536


/*------------------------------------------------------------------------
 * main - Iterative UDP server for ECHO service
 *------------------------------------------------------------------------
 */
int
main(int argc, char **argv)
{
        struct sockaddr_in fsin;        /* the from address of a client */
        char    *service = "echo";      /* service name or port number  */
        char    buf[BUFSIZE];           /* "input" buffer */
        int     sock;                   /* server socket                */
        u_int   alen;                   /* from-address length          */
        int     buflen;

        switch (argc) {
        case    1:
                break;
        case    2:
                service = argv[1];
                break;
        default:
                errexit("usage: UDPechod [port]\n");
        }

        sock = passiveUDP(service);

        while (1) {
                alen = sizeof(fsin);
		/* wait for an incoming datagram */
                buflen = recvfrom(sock, buf, sizeof(buf), 0,
				  (struct sockaddr *)&fsin, &alen);
                if (buflen < 0)
		    errexit("recvfrom\n");

		/* just send it back */
                if (sendto(sock, buf, buflen, 0,
			   (struct sockaddr *)&fsin, sizeof(fsin)) < 0)
		    errexit("sendto\n");
        }
}

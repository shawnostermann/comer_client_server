/* TCPechod.c - main, TCPechod */

#include <sys/types.h>
#include <sys/signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include "libnet.h"

#define QLEN               5    /* maximum connection queue length      */
#define BUFSIZE         4096

void reaper(int sig_num);
int TCPechod(int fd);

/*------------------------------------------------------------------------
 * main - Concurrent TCP server for ECHO service
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
        char    *service = "echo";      /* service name or port number  */
        struct  sockaddr_in fsin;       /* the address of a client      */
        unsigned alen;                  /* length of client's address   */
        int     msock;                  /* master server socket         */
        int     ssock;                  /* slave server socket          */

        switch (argc) {
        case    1:
                break;
        case    2:
                service = argv[1];
                break;
        default:
                errexit("usage: TCPechod [port]\n");
        }

        msock = passiveTCP(service, QLEN);

        (void) signal(SIGCHLD, reaper);

        while (1) {
                alen = sizeof(fsin);
                ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
                if (ssock < 0) {
                        if (errno == EINTR)
                                continue;
                        errexit("accept: %s\n", strerror(errno));
                }
                switch (fork()) {
                case 0:         /* child */
                        (void) close(msock);
                        exit(TCPechod(ssock));
                default:        /* parent */
                        (void) close(ssock);
                        break;
                case -1:
                        errexit("fork: %s\n", strerror(errno));
                }
        }
}

/*------------------------------------------------------------------------
 * TCPechod - echo data until end of file
 *------------------------------------------------------------------------
 */
int
TCPechod(int fd)
{
        char    buf[BUFSIZ];
        u_int     cc;

        while ((cc = read(fd, buf, sizeof buf))) {
                if (cc < 0)
                        errexit("echo read: %s\n", strerror(errno));
                if (write(fd, buf, cc) < 0)
                        errexit("echo write: %s\n", strerror(errno));
        }
        return 0;
}

/*------------------------------------------------------------------------
 * reaper - clean up zombie children
 *------------------------------------------------------------------------
 */
reaper(int sig_num)
{
        int status;

        while (wait3(&status, WNOHANG, (struct rusage *)0) >= 0)
                /* empty */;
}

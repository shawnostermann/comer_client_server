/* TCPtecho.c - main, TCPtecho, reader, writer, mstime */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <time.h>
#include <sys/time.h>
#include "libnet.h"

void writer(int fd, fd_set *pfdset);
int reader(int fd, fd_set *pfdset);
void TCPtecho(fd_set  *pafds, int nfds, int ccount, int hcount);
long mstime(long *pms);

#define BUFSIZE         4096            /* write buffer size            */
#define CCOUNT          64*1024         /* default character count      */

#define USAGE   "usage: TCPtecho [ -c count ] host1 host2...\n"

#define NOFILE 100
char    *hname[NOFILE];                 /* fd to host name mapping      */
int     rc[NOFILE], wc[NOFILE];         /* read/write character counts  */
char    buf[BUFSIZE];                   /* read/write data buffer       */


/*------------------------------------------------------------------------
 * main - concurrent TCP client for ECHO service timing
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
        int     ccount = CCOUNT;
        int     i, hcount, maxfd, fd;
        int     one = 1;
        fd_set  afds;

        hcount = 0;
        maxfd = -1;
        for (i=1; i<argc; ++i) {
                if (strcmp(argv[i], "-c") == 0) {
                        if (++i < argc && (ccount = atoi(argv[i])))
                                continue;
                        errexit(USAGE);
                }
                /* else, a host */

                fd = connectTCP(argv[i], "echo");
                if (ioctl(fd, FIONBIO, (char *)&one))
                        errexit("can't mark socket nonblocking: %s\n",
                                strerror(errno));
                if (fd > maxfd)
                        maxfd = fd;
                hname[fd] = argv[i];
                ++hcount;
                FD_SET(fd, &afds);
        }
        TCPtecho(&afds, maxfd+1, ccount, hcount);
        exit(0);
}

/*------------------------------------------------------------------------
 * TCPtecho - time TCP ECHO requests to multiple servers
 *------------------------------------------------------------------------
 */
void
TCPtecho(fd_set  *pafds, int nfds, int ccount, int hcount)
{
        fd_set  rfds, wfds;             /* read/write fd sets           */
        fd_set  rcfds, wcfds;           /* read/write fd sets (copy)    */
        int     fd, i;

        for (i=0; i<BUFSIZE; ++i)       /* echo data    */
                buf[i] = 'D';
        bcopy((char *)pafds, (char *)&rcfds, sizeof(rcfds));
        bcopy((char *)pafds, (char *)&wcfds, sizeof(wcfds));
        for (fd=0; fd<nfds; ++fd)
                rc[fd] = wc[fd] = ccount;

        (void) mstime((long *)0);       /* set the epoch */

        while (hcount) {
                bcopy((char *)&rcfds, (char *)&rfds, sizeof(rfds));
                bcopy((char *)&wcfds, (char *)&wfds, sizeof(wfds));

                if (select(nfds, &rfds, &wfds, (fd_set *)0,
                                (struct timeval *)0) < 0)
                        errexit("select failed: %s\n",strerror(errno));
                for (fd=0; fd<nfds; ++fd) {
                        if (FD_ISSET(fd, &rfds))
                                if (reader(fd, &rcfds) == 0)
                                        hcount--;
                        if (FD_ISSET(fd, &wfds))
                                writer(fd, &wcfds);
                }
        }
}

/*------------------------------------------------------------------------
 * reader - handle ECHO reads
 *------------------------------------------------------------------------
 */
int
reader(int fd, fd_set  *pfdset)
{
        time_t  now;
        int     cc;

        cc = read(fd, buf, sizeof(buf));
        if (cc < 0)
                errexit("read: %s\n", strerror(errno));
        if (cc == 0)
                errexit("read: premature end of file\n");
        rc[fd] -= cc;
        if (rc[fd])
                return 1;
        (void) mstime(&now);
        printf("%s: %ld ms\n", hname[fd], now);
        (void) close(fd);
        FD_CLR(fd, pfdset);
        return 0;
}

#define MIN(a,b) a<b?(a):(b)
/*------------------------------------------------------------------------
 * writer - handle ECHO writes
 *------------------------------------------------------------------------
 */
void
writer(int fd, fd_set *pfdset)
{
        int     cc;

        cc = write(fd, buf, MIN(sizeof(buf), (unsigned)wc[fd]));
        if (cc == -1)
                errexit("read: %s\n", strerror(errno));
        wc[fd] -= cc;
        if (wc[fd] == 0) {
                (void) shutdown(fd, 1);
                FD_CLR(fd, pfdset);
        }
}

/*------------------------------------------------------------------------
 * mstime - report the number of milliseconds since Jan 1, 1970
 *------------------------------------------------------------------------
 */
long mstime(long *pms)
{
        static struct timeval   epoch;
        struct timeval          now;

        if (gettimeofday(&now, (struct timezone *)0))
                errexit("gettimeofday: %s\n", strerror(errno));
        if (!pms) {
                epoch = now;
                return 0;
        }
        *pms = (now.tv_sec - epoch.tv_sec) * 1000;
        *pms += (now.tv_usec - epoch.tv_usec + 500)/ 1000;
        return *pms;
}

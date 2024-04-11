/* local.h */

#include <sys/ioctl.h>

struct ttystate {
        struct sgttyb   ts_sgttyb;
        struct tchars   ts_tchars;
        struct ltchars  ts_ltchars;
};

extern FILE     *scrfp;
extern char     scrname[];
extern struct ttystate  oldtty;

extern char     t_flushc, t_intrc, t_quitc, sg_erase, sg_kill;

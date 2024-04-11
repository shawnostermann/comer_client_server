/* header file for libnet.a */

int connectsock(char *host, char *service, char *protocol);
int connectUDP(char *host, char *service);
int connectTCP(char *host, char *service);
void errexit(char *msg, ...);
int passiveTCP(char *service, int qlen);
int passiveUDP(char *service);
int passivesock(char *service, char *protocol, int qlen);

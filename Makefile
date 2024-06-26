#
#  Makefile for comer example routines
#

CC=gcc
# for full portability checking
CFLAGS= -Wall -Werror -O3 -pedantic -Wextra -Wno-unused-parameter 
# for normal use
CFLAGS= -Wall -Werror -O2

LDFLAGS=

LIBFILES =	connectsock.c connectUDP.c connectTCP.c errexit.c \
		passiveTCP.c passiveUDP.c passivesock.c status.c
LIBOBJS = ${LIBFILES:.c=.o}
LIBNET=libnet.a

TARGETS= UDPecho UDPechod UDPtime UDPtimed TCPdaytime TCPdaytimed TCPecho TCPechod TCPmechod TCPtecho daytimed superd
TARGETOBJS= ${TARGETS:.c=.o}

all: ${TARGETS}

${LIBNET}: ${LIBOBJS}
	${AR} r ${LIBNET} ${LIBOBJS}
	ranlib ${LIBNET} 

clean: 
	rm -f ${LIBOBJS} ${TARGETS} ${TARGETOBJS} ${LIBNET} *.o 

UDPecho: UDPecho.o  ${LIBNET}
UDPtime: UDPtime.o  ${LIBNET}
UDPtimed: UDPtimed.o  ${LIBNET}
UDPechod: UDPechod.o  ${LIBNET}
UDPechod.drop: UDPechod.drop.o  ${LIBNET}
TCPdaytime: TCPdaytime.o  ${LIBNET}
TCPdaytimed: TCPdaytimed.o  ${LIBNET}
TCPecho: TCPecho.o  ${LIBNET}
TCPechod: TCPechod.o  ${LIBNET}
TCPmechod: TCPmechod.o  ${LIBNET}
TCPtecho: TCPtecho.o  ${LIBNET}
daytimed: daytimed.o  ${LIBNET}
superd: superd.o sv_funcs.o ${LIBNET}

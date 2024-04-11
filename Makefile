#
#  Makefile for comer example routines
#

CC=gcc
CFLAGS= -Wall -Werror -O2
LDFLAGS=

LIBFILES =	connectsock.c connectUDP.c connectTCP.c errexit.c \
		passiveTCP.c passiveUDP.c passivesock.c
LIBOBJS = ${LIBFILES:.c=.o}
LIBNET=libnet.a

TARGETS= UDPecho UDPtime UDPtimed TCPdaytime TCPdaytimed TCPecho TCPechod TCPmechod TCPtecho daytimed
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

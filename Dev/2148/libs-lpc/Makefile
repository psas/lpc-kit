
NAME		= lpc2148
TYPE		= 2148

CC		= arm-elf-gcc
LD		= arm-elf-ld -v
AR		= arm-elf-ar rvs
AS		= arm-elf-as
CP		= arm-elf-objcopy
OD		= arm-elf-objdump

INCLUDE         = -I./src/include
CFLAGS		= $(INCLUDE) -c -Wall -fno-common -O0 -g -DDEBUG 
ASFLAGS		= -g -ahls -mapcs-32

CPFLAGS		= -O binary
HEXFLAGS	= -O ihex
ODFLAGS		= -x --syms

ARFLAGS         = -rcs

PREFIX		= ./src

LIBSRCS		= ${PREFIX}/hwsys.c          \
		  ${PREFIX}/printf.c         \
		  ${PREFIX}/conio.c          \
                  ${PREFIX}/pll.c            \
                  ${PREFIX}/helpers.c        \
                  ${PREFIX}/mam.c            \
                  ${PREFIX}/lpc-serial.c     \
                  ${PREFIX}/lpc-spi.c        \
                  ${PREFIX}/lpc-rtc.c        \
                  ${PREFIX}/lpc-timer.c      \
                  ${PREFIX}/lpc-watchdog.c   \
                  ${PREFIX}/interrupts.c     \
                  ${PREFIX}/olimex.c

LIBOBJS		= ${LIBSRCS:.c=.o}
LIBS            = ./lib/liblpc.a


.PHONY: all clean depend

.SUFFIXES : .c .o .s .a

.c.o :
	${CC} ${CFLAGS} -c $<

.s.o :
	${AS} ${ASFLAGS} -o $@ $< > ./lib/$*.lst

all: depend libdir ${LIBS}

${LIBS}: ${LIBOBJS}
	${AR} $@ ${LIBOBJS}

${LIBOBJS}: ${LIBSRCS} Makefile
	${MAKE} -C ${PREFIX} all

# dependency checking
depend: $(LIBSRCS)
	$(CC) $(CFLAGS) -MM $^ > .depend || rm -f .depend

libdir:
	if test -d lib; then echo lib exists; else mkdir lib; fi

clean:
	-rm -f *~ ${LIBS}; ${MAKE} -C ${PREFIX} clean

-include .depend

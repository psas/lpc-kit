

# Makefile for psas lpc library

NAME		= lpc2148
TYPE		= 2148
TARGET          = LPC214x

CC		= arm-elf-gcc
LD		= arm-elf-ld -v
AR		= arm-elf-ar rvs
AS		= arm-elf-as
CP		= arm-elf-objcopy
OD		= arm-elf-objdump


# CFLAGS		= -I./include -c -Wall -fno-common -O0 -g -DDEBUG -D$(TARGET) -mcpu=arm7tdmi 
CFLAGS		= -I./include -c -Wall -fno-common -O0 -g -DDEBUG -D$(TARGET) -mcpu=arm7tdmi-s 
ASFLAGS		= -g -ahls -mapcs-32

#ASRCS		= crt.s
SRCS		= hwsys.c               \
		  conio.c               \
		  printf.c              \
		  pll.c                 \
                  helpers.c             \
                  mam.c                 \
                  interrupts.c          \
                  olimex.c              \
                  lpc-serial.c          \
                  lpc-spi.c             \
                  lpc-timer.c           \
                  lpc-watchdog.c        \
                  lpc-rtc.c

OBJS		= ${ASRCS:.s=.o} ${SRCS:.c=.o} 

.PHONY: all depend clean

.SUFFIXES : .c .o .s 

.c.o :
	$(CC) $(CFLAGS) -c $<

.s.o :
	$(AS) $(ASFLAGS) -o $@ $< > $*.lst


all:  depend ${OBJS}
# dependency checking
depend: $(SRCS)
	$(CC) $(CFLAGS) -MM $^ > .depend || rm -f .depend


${OBJS}: ${SRCS} ${ASRCS}


clean:
	rm -f ${OBJS} *.lst *~ ./include/*~

-include .depend


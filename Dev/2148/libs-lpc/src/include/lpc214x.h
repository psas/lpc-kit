/*
	LPCUSB, an USB device driver for LPC microcontrollers	
	Copyright (C) 2006 Bertrik Sikken (bertrik@sikken.nl)

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.
	3. The name of the author may not be used to endorse or promote products
	   derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
	NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/**
	Hardware register definitions for the LPC214x USB controller

	These are private to the usbhw module
*/


#define MAMCR		*(volatile unsigned int *)0xE01FC000
#define MAMTIM		*(volatile unsigned int *)0xE01FC004

#define PLLCON		*(volatile unsigned int *)0xE01FC080
#define PLL0CON		*(volatile unsigned int *)0xE01FC080

#define PLLCFG		*(volatile unsigned int *)0xE01FC084
#define PLL0CFG		*(volatile unsigned int *)0xE01FC084

#define PLLSTAT		*(volatile unsigned int *)0xE01FC088
#define PLL0STAT	*(volatile unsigned int *)0xE01FC088

#define PLLFEED		*(volatile unsigned int *)0xE01FC08C
#define PLL0FEED	*(volatile unsigned int *)0xE01FC08C

#define VPBDIV		*(volatile unsigned int *)0xE01FC100

#define PINSEL0		*(volatile unsigned int *)0xE002C000

#define U0THR		*(volatile unsigned int *)0xE000C000
#define U0RBR		*(volatile unsigned int *)0xE000C000
#define U0DLL		*(volatile unsigned int *)0xE000C000
#define U0DLM		*(volatile unsigned int *)0xE000C004
#define U0FCR		*(volatile unsigned int *)0xE000C008
#define U0FDR		*(volatile unsigned int *)0xE000C028
#define U0LCR		*(volatile unsigned int *)0xE000C00C
#define U0LSR		*(volatile unsigned int *)0xE000C014

#define U1THR		*(volatile unsigned int *)0xE0010000
#define U1RBR		*(volatile unsigned int *)0xE0010000
#define U1DLL		*(volatile unsigned int *)0xE0010000
#define U1DLM		*(volatile unsigned int *)0xE0010004
#define U1FCR		*(volatile unsigned int *)0xE0010008
#define U1FDR		*(volatile unsigned int *)0xE0010028
#define U1LCR		*(volatile unsigned int *)0xE001000C
#define U1LSR		*(volatile unsigned int *)0xE0010014


/* Timer/Counter0 and Timer/Counter1 */
#define T0IR                    *(volatile unsigned int *)0xE0004000
#define T1IR                    *(volatile unsigned int *)0xE0008000

#define T0CTCR                  *(volatile unsigned int *)0xE0004070
#define T1CTCR                  *(volatile unsigned int *)0xE0008070

#define T0MCR                   *(volatile unsigned int *)0xE0004014
#define T1MCR                   *(volatile unsigned int *)0xE0008014

/* match registers */
#define T0MR0                   *(volatile unsigned int *)0xE0004018
#define T1MR0                   *(volatile unsigned int *)0xE0008018
#define T0MR1                   *(volatile unsigned int *)0xE000401C
#define T1MR1                   *(volatile unsigned int *)0xE000801C
#define T0MR2                   *(volatile unsigned int *)0xE0004020
#define T1MR2                   *(volatile unsigned int *)0xE0008020
#define T0MR3                   *(volatile unsigned int *)0xE0004024
#define T1MR3                   *(volatile unsigned int *)0xE0008024

#define T0TCR                   *(volatile unsigned int *)0xE0004004
#define T1TCR                   *(volatile unsigned int *)0xE0008004

/* SPI0 (Serial Peripheral Interface 0) */
#define S0SPCR			*(volatile unsigned int *)0xE0020000
#define S0SPSR			*(volatile unsigned int *)0xE0020004
#define S0SPDR			*(volatile unsigned int *)0xE0020008
#define S0SPCCR			*(volatile unsigned int *)0xE002000C
#define S0SPINT			*(volatile unsigned int *)0xE002001C

/* What are these?
#define S0SPTCR			*(volatile unsigned int *)0xE0020010
#define S0SPTSR			*(volatile unsigned int *)0xE0020014
#define S0SPTOR			*(volatile unsigned int *)0xE0020018
*/

/* SSP Controller */
#define SSPCR0			*(volatile unsigned int *)0xE0068000
#define SSPCR1			*(volatile unsigned int *)0xE0068004
#define SSPDR			*(volatile unsigned int *)0xE0068008
#define SSPSR			*(volatile unsigned int *)0xE006800C
#define SSPCPSR			*(volatile unsigned int *)0xE0068010
#define SSPIMSC			*(volatile unsigned int *)0xE0068014
#define SSPRIS			*(volatile unsigned int *)0xE0068018
#define SSPMIS			*(volatile unsigned int *)0xE006801C
#define SSPICR			*(volatile unsigned int *)0xE0068020

/* Watchdog */
#define WDMOD           	*(volatile unsigned int *)0xE0000000
#define WDTC            	*(volatile unsigned int *)0xE0000004
#define WDFEED          	*(volatile unsigned int *)0xE0000008
#define WDTV            	*(volatile unsigned int *)0xE000000C

/* Real Time Clock (RTC) */
#define ILR          	        *(volatile unsigned int *)0xE0024000
#define CTC                     *(volatile unsigned long *)0xE0024004
#define CCR                     *(volatile unsigned long *)0xE0024008
#define CIIR                    *(volatile unsigned long *)0xE002400C
#define AMR                     *(volatile unsigned long *)0xE0024010
#define CTIME0                  *(volatile unsigned long *)0xE0024014
#define CTIME1                  *(volatile unsigned long *)0xE0024018
#define CTIME2                  *(volatile unsigned long *)0xE002401C
#define SEC                     *(volatile unsigned long *)0xE0024020
#define MIN                     *(volatile unsigned long *)0xE0024024
#define HOUR                    *(volatile unsigned long *)0xE0024028
#define DOM                     *(volatile unsigned long *)0xE002402C
#define DOW                     *(volatile unsigned long *)0xE0024030
#define DOY                     *(volatile unsigned long *)0xE0024034
#define MONTH                   *(volatile unsigned long *)0xE0024038
#define YEAR                    *(volatile unsigned long *)0xE002403C
#define ALSEC                   *(volatile unsigned long *)0xE0024060
#define ALMIN                   *(volatile unsigned long *)0xE0024064
#define ALHOUR                  *(volatile unsigned long *)0xE0024068
#define ALDOM                   *(volatile unsigned long *)0xE002406C
#define ALDOW                   *(volatile unsigned long *)0xE0024070
#define ALDOY                   *(volatile unsigned long *)0xE0024074
#define ALMON                   *(volatile unsigned long *)0xE0024078
#define ALYEAR                  *(volatile unsigned long *)0xE002407C
#define PREINT                  *(volatile unsigned long *)0xE0024080
#define PREFRAC                 *(volatile unsigned long *)0xE0024084


/* External Interrupts   */
#define EXTINT                  *(volatile unsigned int *)0xE01FC140
#define INTWAKE                 *(volatile unsigned int *)0xE01FC144
#define EXTMODE                 *(volatile unsigned int *)0xE01FC148
#define EXTPOLAR                *(volatile unsigned int *)0xE01FC14C

/* Vectored Interrupt Controller */
#define VICIntSelect	        *(volatile unsigned int *)0xFFFFF00C
#define VICIntEnable	        *(volatile unsigned int *)0xFFFFF010

#define VICIntEnClear	        *(volatile unsigned int *)0xFFFFF014
#define VICIntEnClr	        *(volatile unsigned int *)0xFFFFF014

#define VICSoftInt              *(volatile unsigned int *)0xFFFFF018
#define VICSoftIntClr           *(volatile unsigned int *)0xFFFFF01C



#define VICVectAddr		*(volatile unsigned int *)0xFFFFF030
#define VICDefVectAddr		*(volatile unsigned int *)0xFFFFF034
#define VICDefVectAddr		*(volatile unsigned int *)0xFFFFF034

#define VICVectAddr0            *(volatile unsigned int *)0xFFFFF100
#define VICVectAddr1            *(volatile unsigned int *)0xFFFFF104
#define VICVectAddr2            *(volatile unsigned int *)0xFFFFF108
#define VICVectAddr3            *(volatile unsigned int *)0xFFFFF10C
#define VICVectAddr4            *(volatile unsigned int *)0xFFFFF110
#define VICVectAddr5            *(volatile unsigned int *)0xFFFFF114
#define VICVectAddr6            *(volatile unsigned int *)0xFFFFF118
#define VICVectAddr7            *(volatile unsigned int *)0xFFFFF11C
#define VICVectAddr8            *(volatile unsigned int *)0xFFFFF120
#define VICVectAddr9            *(volatile unsigned int *)0xFFFFF124
#define VICVectAddr10           *(volatile unsigned int *)0xFFFFF128
#define VICVectAddr11           *(volatile unsigned int *)0xFFFFF12C
#define VICVectAddr12           *(volatile unsigned int *)0xFFFFF130
#define VICVectAddr13           *(volatile unsigned int *)0xFFFFF134
#define VICVectAddr14           *(volatile unsigned int *)0xFFFFF138
#define VICVectAddr15           *(volatile unsigned int *)0xFFFFF13C

#define VICVectCntl0            *(volatile unsigned int *)0xFFFFF200
#define VICVectCntl1            *(volatile unsigned int *)0xFFFFF204
#define VICVectCntl2            *(volatile unsigned int *)0xFFFFF208
#define VICVectCntl3            *(volatile unsigned int *)0xFFFFF20C
#define VICVectCntl4            *(volatile unsigned int *)0xFFFFF210
#define VICVectCntl5            *(volatile unsigned int *)0xFFFFF214
#define VICVectCntl6            *(volatile unsigned int *)0xFFFFF218
#define VICVectCntl7            *(volatile unsigned int *)0xFFFFF21C
#define VICVectCntl8            *(volatile unsigned int *)0xFFFFF220
#define VICVectCntl9            *(volatile unsigned int *)0xFFFFF224
#define VICVectCntl10           *(volatile unsigned int *)0xFFFFF228
#define VICVectCntl11           *(volatile unsigned int *)0xFFFFF22C
#define VICVectCntl12           *(volatile unsigned int *)0xFFFFF230
#define VICVectCntl13           *(volatile unsigned int *)0xFFFFF234
#define VICVectCntl14           *(volatile unsigned int *)0xFFFFF238
#define VICVectCntl15           *(volatile unsigned int *)0xFFFFF23C



/* Common LPC2148 definitions, related to USB */
#define	PCONP			*(volatile unsigned int *)0xE01FC0C4
#define	PLL1CON			*(volatile unsigned int *)0xE01FC0A0
#define	PLL1CFG			*(volatile unsigned int *)0xE01FC0A4
#define	PLL1STAT		*(volatile unsigned int *)0xE01FC0A8
#define	PLL1FEED		*(volatile unsigned int *)0xE01FC0AC

#define PINSEL0			*(volatile unsigned int *)0xE002C000
#define PINSEL1			*(volatile unsigned int *)0xE002C004
#define PINSEL2			*(volatile unsigned int *)0xE002C014

#define IOPIN0			*(volatile unsigned int *)0xE0028000
#define IO0PIN			*(volatile unsigned int *)0xE0028000

#define IOSET0			*(volatile unsigned int *)0xE0028004
#define IO0SET			*(volatile unsigned int *)0xE0028004

#define IODIR0			*(volatile unsigned int *)0xE0028008
#define IO0DIR			*(volatile unsigned int *)0xE0028008

#define IOCLR0			*(volatile unsigned int *)0xE002800C
#define IO0CLR			*(volatile unsigned int *)0xE002800C

#define IOPIN1			*(volatile unsigned int *)0xE0028010
#define IO1PIN			*(volatile unsigned int *)0xE0028010

#define IOSET1			*(volatile unsigned int *)0xE0028014
#define IO1SET			*(volatile unsigned int *)0xE0028014

#define IODIR1			*(volatile unsigned int *)0xE0028018
#define IO1DIR			*(volatile unsigned int *)0xE0028018

#define IOCLR1			*(volatile unsigned int *)0xE002801C
#define IO1CLR			*(volatile unsigned int *)0xE002801C

/* PCONP bits */
#define PCTIM0		(1<<1)
#define PCTIM1		(1<<2)
#define PCUART0		(1<<3)
#define PCUART1		(1<<4)
#define PCPWM0		(1<<5)
#define PCI2C0		(1<<7)
#define PCSPI0		(1<<8)
#define PCRTC		(1<<9)
#define PCSPI1		(1<<10)
#define PCAD0		(1<<12)
#define PCI2C1		(1<<19)
#define PCAD1		(1<<20)
#define PCUSB		(1<<31)

/* USB register definitions */
#define USBIntSt		*(volatile unsigned int *)0xE01FC1C0

#define USBDevIntSt		*(volatile unsigned int *)0xE0090000
#define USBDevIntEn		*(volatile unsigned int *)0xE0090004
#define USBDevIntClr	*(volatile unsigned int *)0xE0090008
#define USBDevIntSet	*(volatile unsigned int *)0xE009000C
#define USBDevIntPri	*(volatile unsigned int *)0xE009002C

#define USBEpIntSt		*(volatile unsigned int *)0xE0090030
#define USBEpIntEn		*(volatile unsigned int *)0xE0090034
#define USBEpIntClr		*(volatile unsigned int *)0xE0090038
#define USBEpIntSet		*(volatile unsigned int *)0xE009003C
#define USBEpIntPri		*(volatile unsigned int *)0xE0090040

#define USBReEp			*(volatile unsigned int *)0xE0090044
#define USBEpInd		*(volatile unsigned int *)0xE0090048
#define USBMaxPSize		*(volatile unsigned int *)0xE009004C

#define USBRxData		*(volatile unsigned int *)0xE0090018
#define USBRxPLen		*(volatile unsigned int *)0xE0090020
#define USBTxData		*(volatile unsigned int *)0xE009001C
#define USBTxPLen		*(volatile unsigned int *)0xE0090024
#define USBCtrl			*(volatile unsigned int *)0xE0090028

#define USBCmdCode		*(volatile unsigned int *)0xE0090010
#define USBCmdData		*(volatile unsigned int *)0xE0090014

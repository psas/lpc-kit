
/*
 * rtc-demo.h
 */

#ifndef _RTC_DEMO_H
#define _RTC_DEMO_H


// which clock should RTC use?
#define SOURCE_PCLK 0
#define SOURCE_RTCX 1

#include "types.h"

/*
 * initialize
 * -----------------------------------------------
 * initialize the lpc2148 pll 
 * cpu interrupts
 * the uart0 serial port
 * the rtc VIC interrupt
 */
void initialize(void);

void RTC_Handler (void)   __attribute__ ((interrupt("IRQ")));

#endif

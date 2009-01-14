
/*
 * watchdog-demo.h
 */

#ifndef _WATCHDOG_DEMO_H
#define _WATCHDOG_DEMO_H

#include "types.h"
#include "lpc214x.h"



/*
 * initialize
 * -----------------------------------------------
 * initialize the lpc2148 pll 
 * cpu interrupts
 * the uart0 serial port
 * the rtc VIC interrupt
 */
void initialize(void);

void WATCHDOG_Handler (void)   __attribute__ ((interrupt("IRQ")));

#endif

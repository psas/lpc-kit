
/*
 * timer-demo.h
 */

#ifndef _TIMER_DEMO_H
#define _TIMER_DEMO_H


#include "types.h"
#include "lpc214x.h"

#define TIMER0_CHANNEL        4
/*
 * initialize
 * -----------------------------------------------
 * initialize the lpc2148 pll 
 * cpu interrupts
 * the uart0 serial port
 * the rtc VIC interrupt
 */
void initialize(void);


/*
 * disableTIMER0_INT
 * ------------------------------
 *
 */



void TIMER0_Handler (void)   __attribute__ ((interrupt("IRQ")));



#endif

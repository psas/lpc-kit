
/*
 * lpc-timer.h
 */

#ifndef _LPC_TIMER_H
#define _LPC_TIMER_H

#include "types.h"
#include "lpc214x.h"



#define ENABLE_TIMER0  T0TCR = (T0TCR | 0x1) & 0x3
#define ENABLE_TIMER1  T1TCR = (T1TCR | 0x1) & 0x3

#define DISABLE_TIMER0 T0TCR = (T0TCR & ~(0x1)) & 0x3
#define DISABLE_TIMER1 T1TCR = (T1TCR & ~(0x1)) & 0x3

/* Reference: User Manual LPC214x Rev. 02--25 July 2006 p249 */
#define RESET_TIMER0   T0TCR = (T0TCR | 0x2) & 0x3; T0TCR = (T0TCR & ~(0x2)) & 0x3;
#define RESET_TIMER1   T1TCR = (T1TCR | 0x2) & 0x3; T1TCR = (T1TCR & ~(0x2)) & 0x3;

// clear interrupt register
#define RESET_T0IR     T0IR  = T0IR
#define RESET_T1IR     T1IR  = T1IR

// timer control registers
#define SET_T0CTCR(a) (T0CTCR = a & 0xF)
#define SET_T1CTCR(a) (T1CTCR = a & 0xF)

// prescale registers
#define SET_PRESCALE0(a) (T0PR = a )
#define SET_PRESCALE1(a) (T1PR = a )

// reset match control
#define RESET_MATCH_CONTROL_0 (T0MCR = 0x0)
#define RESET_MATCH_CONTROL_1 (T1MCR = 0x0)


// match control register
#define SET_MATCH_CONTROL_0(a) (T0MCR = a & 0x0FFF)
#define SET_MATCH_CONTROL_1(a) (T1MCR = a & 0x0FFF)

// capture control register
#define SET_CAPTURE_CONTROL_0(a) (T0CCR = a & 0x0FFF)
#define SET_CAPTURE_CONTROL_1(a) (T1CCR = a & 0x0FFF)



#endif

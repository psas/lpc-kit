
/*
 * lpc-watchdog.h
 */

#ifndef _LPC_WATCHDOG_H
#define _LPC_WATCHDOG_H

#include "types.h"
#include "interrupts.h"
#include "lpc214x.h"


// WATCHDOG ENABLE and RESET can only be disabled (cleared) by a reset.
// Reference: User Manual LPC214x Rev. 02--25 July 2006 p292

#define WATCHDOG_ENABLE             (WDMOD = (WDMOD | 0x1)  & 0xF)
#define WATCHDOG_RESET_ENABLE       (WDMOD = (WDMOD | 0x2)  & 0xF)
#define WATCHDOG_CLEAR_TIMEOUT_FLAG (WDMOD = (WDMOD & 0x0))

#define WATCHDOG_CLEAR_INT  ( VICIntEnClr  = (1 << WATCHDOG_CHANNEL) )
#define WATCHDOG_ENABLE_INT ( VICIntEnable = (1 << WATCHDOG_CHANNEL) )


#define WATCHDOG_TIMER_CONST(a)  (WDTC = a)

/*
 * watchdog_feed
 * ---------------------------------
 * Feed the watchdog to run or reload
 */
void watchdog_feed(void);




#endif

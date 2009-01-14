
/*
 * lpc-watchdog.c
 */

#include "./include/lpc-watchdog.h"

#include "./include/types.h"
#include "./include/interrupts.h"


/*
 * watchdog_feed
 * ---------------------------------
 * Feed the watchdog to run or reload
 */
void watchdog_feed(void) {

// prevent interrupts during feed.
    disableIRQ();

    WDFEED = 0xAA;
    WDFEED = 0x55;

// re-enable interrupts.
    enableIRQ();

}

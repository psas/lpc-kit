

/*
 * olimex.h
 */


#ifndef _OLIMEX_H
#define _OLIMEX_H

#define LED1_INDEX             10
#define LED2_INDEX             11

// Macros to use for turning on/off LEDs...
// Need to know numbers of pins...
#define LED1_ON         IOCLR0 = (1 << LED1_INDEX)
#define LED2_ON         IOCLR0 = (1 << LED2_INDEX)


#define LED1_OFF        IOSET0 = (1 << LED1_INDEX)
#define LED2_OFF        IOSET0 = (1 << LED2_INDEX)


/* #define DEBUG_LED_ON(x)         IOCLR0 = (1 << x) */
/* #define DEBUG_LED_OFF(x)        IOSET0 = (1 << x) */


#include "lpc214x.h"

/*
 * enable_leds
 * -------------------------------
 * enable the leds (1 and 2)
 * These are connected to P0.11 & P0.10
 */
void enable_leds(void);

/*
 * disable_leds
 * ------------------------------
 * disable led 1 and 2
 */
void disable_leds(void);

/*
 * led1_invert
 * -------------------------
 * if it is on, turn it off
 * if it is off, turn it on
 */
void led1_invert(void);

/*
 * led2_invert
 * -------------------------
 * if it is on, turn it off
 * if it is off, turn it on
 */
void led2_invert(void);

/* 
 * led1_off
 * ---------------------------
 * turn off led1
 */
// arm-elf-gcc 4.2.3 fails with __attribute__ 
//  sorry, unimplemented: inlining failed in call to ‘led1_off’: function body not available

inline void led1_off(void); // __attribute__((always_inline));

/*
 * led1_on
 * ---------------------------
 *  turn on led1
 */
void led1_on(void);

/* 
 * led2_off
 * ---------------------------
 * turn off led2
 */
inline extern void led2_off(void) {
    IOSET0   |= (0x1 << 11);    // turn OFF led 2 (P0.11)
}

/*
 * led2_on
 * ---------------------------
 *  turn on led2
 */
void led2_on(void);


/*
 * flash_led1
 * -----------------------------
 * flash led1 n times
 */
void flash_led1(int n);


/*
 * flash_led2
 * -----------------------------
 * flash led2 n times
 */
void flash_led2(int n);


/*
 * beep_high
 * -----------------------------
 * Make a high beep.
 */
void beep_high();


/*
 * beep_low
 * -----------------------------
 * Make a high beep.
 */
void beep_low();


/*
 * beep_custom
 * ------------
 * use the buzzer to beep.
 */
void beep_custom(int halfperiod);



/*
 * waitCount
 * -----------------------
 * Spin in a loop for count ..
 */
void waitCount(int count);



#endif

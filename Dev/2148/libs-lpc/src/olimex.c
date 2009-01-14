/*
 * olimex.c
 * ------------------------------
 * Utility functions for the Olimex 2148 board.
 */

#include "./include/olimex.h"
#include "./include/lpc214x.h"

/*
 * enable_leds
 * -------------------------------
 * enable the leds (1 and 2)
 * These are connected to P0.11 & P0.10
 */
inline void enable_leds(void) {
  PINSEL0 &= 0xFF3FFFFF;      // GPIO Configuration for  P0.[10,11]
  IODIR0  |= 0x00000c00;      // P0.11 & P0.10 set to output
  LED1_OFF;
  LED2_OFF;
}

/*
 * disable_leds
 * ------------------------------
 * disable led 1 and 2
 */
void disable_leds(void) {
  LED1_OFF;
  LED2_OFF;
  IODIR0  &= 0xFFFFF3FF;     // P0.11 & P0.10 set to input

}


/*
 * led1_invert
 * -------------------------
 * if it is on, turn it off
 * if it is off, turn it on
 */
void led1_invert(void) {

    int led;
    led = IO0PIN;

    led = (0x1) & (led >> 10);
    if (led == 0x1) { 
	LED1_ON;
    }
    else { 
	LED1_OFF;
    }
}

/*
 * led2_invert
 * -------------------------
 * if it is on, turn it off
 * if it is off, turn it on
 */
void led2_invert(void) {

    int led;
    led = IO0PIN;

    led = (0x1) & (led >> 11);
    if (led == 0x1) { 
	LED2_ON;
    }
    else { 
	LED2_OFF;
    }
}

/* 
 * led1_off
 * ---------------------------
 * turn off led1
 */
inline void led1_off(void) {
    IOSET0   |= (0x1 << 10);    // turn OFF led 1 (P0.10)
}

/*
 * led1_on
 * ---------------------------
 *  turn on led1
 */
void led1_on(void) {
    IOCLR0   |= (0x1 << 10);    // turn ON led 1 (P0.10)
}

/* 
 * led2_off
 * ---------------------------
 * turn off led2
 */
void led2_off(void) {
    IOSET0   |= (0x1 << 11);    // turn OFF led 2 (P0.11)
}

/*
 * led2_on
 * ---------------------------
 *  turn on led2
 */
void led2_on(void) {
    IOCLR0   |= (0x1 << 11);    // turn ON led 1 (P0.11)
}

/*
 * flash_led1
 * -----------------------------
 * flash led1 n times
 */
void flash_led1(int n) {
    int i;

    for(i=0; i<n; ++i) {
	LED1_ON;
	waitCount(200000);
	LED1_OFF;
	waitCount(200000);
    }
}

/*
 * flash_led2
 * -----------------------------
 * flash led2 n times
 */
void flash_led2(int n) {
    int i;

    for(i=0; i<n; ++i) {
	LED2_ON;
	waitCount(200000);
	LED2_OFF;
	waitCount(200000);
    }
}

/*
 * beep_high
 * -----------------------------
 * Make a high beep.
 */
void beep_high() {

    beep_custom(800);

}

/*
 * beep_low
 * -----------------------------
 * Make a high beep.
 */
void beep_low() {

    beep_custom(1800);

}

/*
 * beep_custom
 * ------------
 * use the buzzer to beep.
 */
void beep_custom(int halfperiod) {

    int savedIODIR0 = IODIR0;   // read IODIR0 and save it in variable

    int j, k;
  
    IODIR0 |= 0x00003000;      // P0.13 & P0.12 set to output

    IOCLR0 =  0x00003000;      // buzzer:	clear both pins
    k=0;
    while ( k < 500 ) {
	k +=1;
	for (j = 0; j < halfperiod; j++ );  
	IOSET0 = 0x00001000;	             // buzzer
	for (j = 0; j < halfperiod ; j++ );
	IOCLR0 = 0x00001000;                 // buzzer
    } 
    IODIR0 = savedIODIR0;       // restore IODIR state
    waitCount(500000);
}


/*
 * waitCount
 * -----------------------
 * Spin in a loop for count ..
 */
void waitCount(int count) {
    int j;

    for(j=0; j<count; ++j);
}


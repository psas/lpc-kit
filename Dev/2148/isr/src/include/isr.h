
/*
 * isr.h
 */

#ifndef _ISR_H
#define _ISR_H


/*
 * initialize
 * -----------------------------------------------
 * initialize the lpc2148 pll 
 * cpu interrupts
 * the uart0 serial port
 * the rtc VIC interrupt
 */
void initialize(void);

void EINT0_Handler (void)   __attribute__ ((interrupt("IRQ")));

/* 'naked' means remove prologue and epilogue from function calls  */
// void EINT2_Handler (void)   __attribute__ ((interrupt("IRQ"), naked));
void EINT2_Handler (void)   __attribute__ ((interrupt("IRQ")));


#endif

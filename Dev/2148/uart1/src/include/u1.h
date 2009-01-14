

/*
 * u1.h
 */

#ifndef _U1_H
#define _U1_H

/*
 * initialize
 * -----------------------------------------------
 * initialize the lpc2148 pll and uart0 serial port
 */
void initialize(void);

void EINT0_Handler (void)   __attribute__ ((interrupt("IRQ")));
void EINT2_Handler (void)   __attribute__ ((interrupt("IRQ")));


#endif

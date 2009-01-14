

/*
 * poke.h
 */

#ifndef _POKE_H
#define _POKE_H

/*
 * initialize
 * -----------------------------------------------
 * initialize the lpc2148 pll and uart0 serial port
 */
void initialize(void);

/*
 * echo_line
 * ---------------------------------------
 * read and echo a line to the serial port
 */
void echo_line(void);


/* 'naked' means remove prologue and epilogue from function calls  */
/* We use our own entry/exit code for interrupts                   */
//void EINT0_Handler (void)   __attribute__ ((interrupt("IRQ"), naked));
//void EINT2_Handler (void)   __attribute__ ((interrupt("IRQ"), naked));
void EINT0_Handler (void)   __attribute__ ((interrupt("IRQ")));
void EINT2_Handler (void)   __attribute__ ((interrupt("IRQ")));


#endif

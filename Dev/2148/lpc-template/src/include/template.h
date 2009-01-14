

/*
 * template.h
 */

#ifndef _TEMPLATE_H
#define _TEMPLATE_H

/*
 * initialize
 * -----------------------------------------------
 * initialize the lpc2148 pll and uart0 serial port
 */
void initialize(void);

/* 'naked' means remove prologue and epilogue from function calls  */
/* We use our own entry/exit code for interrupts                   */
//void EINT0_Handler (void)   __attribute__ ((interrupt("IRQ"), naked));
//void EINT2_Handler (void)   __attribute__ ((interrupt("IRQ"), naked));
void EINT0_Handler (void)   __attribute__ ((interrupt("IRQ")));
void EINT2_Handler (void)   __attribute__ ((interrupt("IRQ")));


#endif


/*
 * interrupts.h
 */


#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H


#include "./lpc214x.h"
#include "./types.h"
#include "./lpc-timer.h"

#define RTC_CHANNEL           13

#define TIMER0_CHANNEL        4
#define TIMER1_CHANNEL        5

#define WATCHDOG_CHANNEL      0

/*
 *
 * MACRO Name: ISR_ENTRY()
 *
 * Description:
 *    This MACRO is used upon entry to an ISR.  The current version of
 *    the gcc compiler for ARM does not produce correct code for
 *    interrupt routines to operate properly with THUMB code.  The MACRO
 *    performs the following steps:
 *
 *    1 - Adjust address at which execution should resume after servicing
 *        ISR to compensate for IRQ entry
// *    2 - Save the non-banked registers r0-r12 and lr onto the IRQ stack.
 *    2 - Save the non-banked registers r0-r8 and lr onto the IRQ stack.
 *    3 - Get the status of the interrupted program is in SPSR.
 *    4 - Push it onto the IRQ stack as well.
 *
 */
#define ISR_ENTRY() asm volatile(" sub   lr, lr,#4\n"		\
                                 " stmfd sp!,{r0-r8,lr}\n"	\
                                 " mrs   r1, spsr\n"		\
                                 " stmfd sp!,{r1}")

/*
 *
 * MACRO Name: ISR_EXIT()
 *
 * Description:
 *    This MACRO is used to exit an ISR.  The current version of the gcc
 *    compiler for ARM does not produce correct code for interrupt
 *    routines to operate properly with THUMB code.  The MACRO performs
 *    the following steps:
 *
 *    1 - Recover SPSR value from stack       
 *    2 - and restore  its value                   
 *    3 - Pop the return address & the saved general registers from
 *        the IRQ stack & return
 *
 */
#define ISR_EXIT()  asm volatile(" ldmfd sp!,{r1}\n"		\
                                 " msr   spsr_c,r1\n"		\
                                 " ldmfd sp!,{r0-r8,pc}^")

/*
 * exitInterrupt
 * ------------------------------
 * Macro to consistently exit interrupt on LPC
 * user manual LPC214x p58
 * Writing VICVectAddr updates priority hardware.
 * This may turn into an actual function, if it
 * is decided that the gnu-arm compiler is not working
 * properly.
 */ 

#define EXIT_INTERRUPT  VICVectAddr=0x0

/*
 *
 * disableIRQ()
 * --------------------------------------------------------------
 *    This function sets the IRQ disable bit in the cpsr
 *
 * Returns:
 *    previous value of CPSR
 *
 */
U32 disableIRQ(void);

/*
 *
 * enableIRQ()
 * --------------------------------------------------------------
 *    This function clears the IRQ disable bit in the cpsr
 *
 * Returns:
 *    previous value of CPSR
 */
U32 enableIRQ(void);

/*
 *
 * restoreIRQ()
 * --------------------------------------------------------------
 *    This function restores the IRQ disable bit in the CPSR
 *    to the value contained within passed oldCPSR
 * Returns:
 *    previous value of CPSR
 */
U32 restoreIRQ(U32 oldCPSR);

/*
 *
 * disableFIQ()
 * --------------------------------------------------------------
 * Description:
 *    This function sets the FIQ disable bit in the CPSR
 * Returns:
 *    previous value of CPSR
 *
 */
U32 disableFIQ(void);

/*
 *
 * enableFIQ()
 * --------------------------------------------------------------
 *    This function clears the FIQ disable bit in the CPSR
 * Returns:
 *    previous value of CPSR
 */
U32 enableFIQ(void);

/*
 *
 * restoreIRQ()
 * --------------------------------------------------------------
 * Description:
 *    This function restores the FIQ disable bit in the status register
 *    to the value contained within passed oldCPSR
 * Returns:
 *    previous value of CPSR
 *
 */
U32 restoreFIQ(U32 oldCPSR);

/*
 * LPC214x manual
 * Rev. 02 — 25 July 2006
 *  p 59 Table 5-57
 */

/*
 * update_VIC_table
 * ------------------------------
 * isr_vec is the 'slot' in the VIC
 * int_handler is the U32 address of the ISR
 * isr_ctl is the VIC Control Register info
 */
void update_VIC_table(U16 isr_vec, U32 isr, U8 isr_ctl);

/*
 * enableWATCHDOG_INT
 * ------------------------------
 * 
 * isr_vec is the 'slot' in the Vect. interrupt to assign
 * the vectored address to. [0:15]. Any other value results
 * in this being a non-vectored IRQ
 *
 * int_handler is the address of the interrupt handler routine.
 */
void enableWATCHDOG_INT(U16 isr_vec, U32 int_handler);

/* GCC does not inline any functions when not optimizing unless                   */
/* you specify the `always_inline' attribute for the function, like this:         */

      /* Prototype.   */
/*      inline void foo (const char) __attribute__((always_inline));              */

/* The remainder of this section is specific to GNU C89 inlining.                 */

/* When an inline function is not static, then the compiler must assume that      */
/* there may be calls from other source files; since a global symbol can be       */
/* defined only once in any program, the function must not be defined in the      */
/* other source files, so the calls therein cannot be integrated. Therefore, a    */
/* non-static inline function is always compiled on its own in the usual fashion. */

/* If you specify both inline and extern in the function definition, then the     */
/* definition is used only for inlining. In no case is the function compiled on   */
/* its own, not even if you refer to its address explicitly. Such an address      */
/* becomes an external reference, as if you had only declared the function, and   */
/* had not defined it.                                                            */

/* This combination of inline and extern has almost the effect of a macro. The    */
/* way to use it is to put a function definition in a header file with these      */
/* keywords, and put another copy of the definition (lacking inline and extern)   */
/* in a library file. The definition in the header file will cause most calls to  */
/* the function to be inlined. If any uses of the function remain, they will      */
/* refer to the single copy in the library.                                       */


/*
 * disableWATCHDOG_INT
 * ------------------------------
 * 
 */
// a possible style of inline
inline void disableWATCHDOG_INT(void) __attribute__((always_inline));

/*
 * enableTIMER0_INT
 * ------------------------------
 * 
 * isr_vec is the 'slot' in the Vect. interrupt to assign
 * the vectored address to. [0:15]. Any other value results
 * in this being a non-vectored IRQ
 *
 * int_handler is the address of the interrupt handler routine.
 */
void enableTIMER0_INT(U16 isr_vec, U32 int_handler);

/*
 * disableTIMER0_INT
 * ------------------------------
 *
 */
// another style of inline, function declared in library also without inline extern.
inline extern void disableTIMER0_INT(void){
    VICIntEnClear = (0x1 << TIMER0_CHANNEL);
};

/*
 * enableTIMER1_INT
 * ------------------------------
 * 
 * isr_vec is the 'slot' in the Vect. interrupt to assign
 * the vectored address to. [0:15]. Any other value results
 * in this being a non-vectored IRQ
 *
 * int_handler is the address of the interrupt handler routine.
 */
void enableTIMER1_INT(U16 isr_vec, U32 int_handler);

/*
 * disableTIMER1_INT
 * ------------------------------
 * 
 */
inline void disableTIMER1_INT(void);




/*
 * enableRTC_INT
 * ------------------------------
 * 
 * isr_vec is the 'slot' in the Vect. interrupt to assign
 * the vectored address to. [0:15]. Any other value results
 * in this being a non-vectored IRQ
 *
 * int_handler is the address of the interrupt handler routine.
 */
void enableRTC_INT(U16 isr_vec, U32 int_handler);

/*
 * disableRTC_INT
 * ------------------------------
 * 
 */
inline void disableRTC_INT(void);

/*
 * enableEINT0
 * ------------------------------
 * Sets the PINSEL1 Register for P0.16 to EINT0 mode
 * Enables the INT in the INT Enable register.
 * 
 * isr_vec is the 'slot' in the Vect. interrupt to assign
 * the vectored address to. [0:15]. Any other value results
 * in this being a non-vectored IRQ
 * int_handler is the address of the interrupt handler routine.
 */
void enableEINT0(U16 isr_vec, U32 int_handler);

/*
 * disableEINT0
 * ------------------------------
 */
inline void disableEINT0(void);

/*
 * enableEINT2
 * ------------------------------
 * Sets the PINSEL0 Register for P0.15 to EINT2 mode
 * Enables the INT in the INT Enable register.
 * 
 * isr_vec is the 'slot' in the Vect. interrupt to assign
 * the vectored address to. [0:15]. Any other value results
 * in this being a non-vectored IRQ
 *
 * int_handler is the address of the interrupt handler routine.
 */
void enableEINT2(U16 isr_vec, U32 int_handler);

/*
 * disableEINT2
 * ------------------------------
 */
inline void disableEINT2(void);

/*
 * clearIntEnable
 * ----------------------------
 * write 1's to the VICIntEnClr register
 * Reference: User manual LPC214x p55
 */
void clearIntEnable();

/*
 * readIntEnable
 * ------------------------
 */
U32 readIntEnable(void);


/*
 * writeIntEnable
 * ------------------------
 */
void writeIntEnable(U32 status);

/*
 * readAndClearIntEnable
 * ----------------------------
 * Read and clear VICIntEnable register
 * Save and return the value prior to clear 
 * Reference: User manual LPC214x p55
 */
U32 readAndClearIntEnable();


/*
 * clearSoftInt
 * ----------------------------
 * Write all 1's to the VICSoftIntClear register
 * Reference: User manual LPC214x p53
 */
void clearSoftInt(void);

/*
 * readAndClearSoftInt
 * ----------------------------
 * Read and clear VICSoftInt register
 * Save and return the value prior to clear 
 * Reference: User manual LPC214x p52
 */
U32 readAndClearSoftInt();



#endif // _INTERRUPTS_H

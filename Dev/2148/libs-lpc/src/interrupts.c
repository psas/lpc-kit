
/*
 * Functions to help manage interrupts on LPC2148
 */

#include "./include/interrupts.h"

#include "./include/lpc214x.h"
#include "./include/types.h"
#include "./lpc-timer.h"
#include "./lpc-rtc.h"
/*
 * working with the CPU
 *    http://www.ethernut.de/en/documents/arm-inline-asm.html
 */

#define IRQ_MASK 0x00000080
#define FIQ_MASK 0x00000040
#define INT_MASK (IRQ_MASK | FIQ_MASK)

#define EXT0FLAG              0x1
#define EXT2FLAG              0x4

#define EINT0_MODE_LEVEL      0x0
#define EINT0_MODE_EDGE       0x1

#define EINT2_MODE_LEVEL      0x0
#define EINT2_MODE_EDGE       0x4

#define EINT0_POS_EDGE        0x1
#define EINT2_POS_EDGE        0x4

#define EINT0_LEVEL_HI        0x1
#define EINT2_LEVEL_HI        0x4

#define EINT0_CHANNEL         14
#define EINT2_CHANNEL         16

#define EINT2_PINSEL0_MASK    0x3FFFFFFF  
#define EINT0_PINSEL1_MASK    0xFFFFFFFC 

/* #define RTC_CHANNEL           13 */

/* #define TIMER0_CHANNEL        4 */
/* #define TIMER1_CHANNEL        5 */

/* #define WATCHDOG_CHANNEL      0 */


/*
 * How to mess with the arm cpsr from C
 */

/*
 * __get_cpsr
 * ---------------------------------------
 * Reads and returns the value of the cpsr
 */
static inline U32 __get_cpsr(void)
{
    U32 retval;
    asm volatile (" mrs  %0, cpsr" : "=r" (retval) : /* no inputs */  ); 
    return retval;
}


static inline void __set_cpsr(U32 val)
{
    asm volatile (" msr  cpsr, %0" : /* no outputs */ : "r" (val)  );	
}


/* The IRQ bit in the CPSR must be cleared for IRQ ints */
/* This function returns the value of cpsr prior to clear */
U32 disableIRQ(void)
{
    U32 _cpsr;

    _cpsr = __get_cpsr();
    __set_cpsr(_cpsr | IRQ_MASK);
    return _cpsr;
}


U32 restoreIRQ(unsigned oldCPSR)
{
    U32 _cpsr;

    _cpsr = __get_cpsr();
    __set_cpsr((_cpsr & ~IRQ_MASK) | (oldCPSR & IRQ_MASK));
    return _cpsr;
}

U32 enableIRQ(void)
{
    U32 _cpsr;

    _cpsr = __get_cpsr();
    __set_cpsr(_cpsr & ~IRQ_MASK);
    return _cpsr;
}


U32 disableFIQ(void)
{
    U32 _cpsr;

    _cpsr = __get_cpsr();
    __set_cpsr(_cpsr | FIQ_MASK);
    return _cpsr;
}

U32 enableFIQ(void)
{
    U32 _cpsr;

    _cpsr = __get_cpsr();
    __set_cpsr(_cpsr & ~FIQ_MASK);
    return _cpsr;
}


U32 restoreFIQ(U32 oldCPSR)
{
    U32 _cpsr;

    _cpsr = __get_cpsr();
    __set_cpsr((_cpsr & ~FIQ_MASK) | (oldCPSR & FIQ_MASK));
    return _cpsr;
}

/*
 * Interrupt enable:
 * LPC214x manual p 54
 */

/*
 * update_VIC_table
 * ------------------------------
 * isr_vec is the 'slot' in the VIC
 * int_handler is the U32 address of the ISR
 * isr_ctl is the VIC Control Register info
 */
void update_VIC_table(U16 isr_vec, U32 isr, U8 isr_ctl) {

    U8 ctl_mask = 0x3F;

    // clean up the isr_ctl.
    isr_ctl &= ctl_mask;
   
    // VIC register.
    switch (isr_vec) {
	case 0:
	    VICVectAddr0   = isr;
	    VICVectCntl0   = isr_ctl;
	case 1:
	    VICVectAddr1   = isr;
	    VICVectCntl1   = isr_ctl;
	case 2:
	    VICVectAddr2   = isr;
	    VICVectCntl2   = isr_ctl;
	case 3:
	    VICVectAddr3   = isr;
	    VICVectCntl3   = isr_ctl;
	case 4:
	    VICVectAddr4   = isr;
	    VICVectCntl4   = isr_ctl;
	case 5:
	    VICVectAddr5   = isr;
	    VICVectCntl5   = isr_ctl;
	case 6:
	    VICVectAddr6   = isr;
	    VICVectCntl6   = isr_ctl;
	case 7:
	    VICVectAddr7   = isr;
	    VICVectCntl7   = isr_ctl;

	case 8:
	    VICVectAddr8   = isr;
	    VICVectCntl8   = isr_ctl;
	case 9:
	    VICVectAddr9   = isr;
	    VICVectCntl9   = isr_ctl;
	case 10:
	    VICVectAddr10  = isr;
	    VICVectCntl10  = isr_ctl;
	case 11:
	    VICVectAddr11  = isr;
	    VICVectCntl11  = isr_ctl;
	case 12:
	    VICVectAddr12  = isr;
	    VICVectCntl12  = isr_ctl;
	case 13:
	    VICVectAddr13  = isr;
	    VICVectCntl13  = isr_ctl;
	case 14:
	    VICVectAddr14  = isr;
	    VICVectCntl14  = isr_ctl;
	case 15:
	    VICVectAddr15  = isr;
	    VICVectCntl15  = isr_ctl;
	default:	    
	    VICDefVectAddr = isr;       // non-vectored IRQ	    
    }
}

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
void enableWATCHDOG_INT(U16 isr_vec, U32 int_handler) {

    U8 watchdog_ctl = 0x00000020;

    disableWATCHDOG_INT();

    update_VIC_table(isr_vec, int_handler, watchdog_ctl);
  
    // wdt selected for IRQ   
    VICIntSelect &= ~(1 << WATCHDOG_CHANNEL);

    VICIntEnable =   (1 << WATCHDOG_CHANNEL);

}

/*
 * disableWATCHDOG_INT
 * ------------------------------
 * 
 */
inline void disableWATCHDOG_INT(void) {

    VICIntEnClr = (0x1 << TIMER0_CHANNEL);
}

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
void enableTIMER0_INT(U16 isr_vec, U32 int_handler) {

    U8 timer0_ctl = 0x24;

    disableTIMER0_INT();

    update_VIC_table(isr_vec, int_handler, timer0_ctl);

    RESET_T0IR;

    // timer0   
    VICIntSelect &= ~(1 << TIMER0_CHANNEL);

    VICIntEnable =   (1 << TIMER0_CHANNEL);

}

/*
 * disableTIMER0_INT
 * ------------------------------
 * 
 */
void disableTIMER0_INT(void) {

    VICIntEnClr = (0x1 << TIMER0_CHANNEL);
}

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
void enableTIMER1_INT(U16 isr_vec, U32 int_handler) {

    U8 timer1_ctl = 0x25;

    disableTIMER1_INT();

    update_VIC_table(isr_vec, int_handler, timer1_ctl);

    RESET_T1IR;

    // timer0   
    VICIntSelect &= ~(1 << TIMER1_CHANNEL);

    VICIntEnable =   (1 << TIMER1_CHANNEL);

}

/*
 * disableTIMER1_INT
 * ------------------------------
 * 
 */
inline void disableTIMER1_INT(void) {

    VICIntEnClr = (0x1 << TIMER1_CHANNEL);
}

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
void enableRTC_INT(U16 isr_vec, U32 int_handler) {

    U8 rtc_ctl = 0x2D;               // channel 13 for rtc

    disableRTC_INT();

    update_VIC_table(isr_vec, int_handler, rtc_ctl);

    // read and write the ILR to clear interrupt.
    RESET_ILR;

    // rtc   
    VICIntSelect &= ~(1 << RTC_CHANNEL);

    VICIntEnable =   (1 << RTC_CHANNEL);

}

/*
 * disableRTC_INT
 * ------------------------------
 * 
 */
inline void disableRTC_INT(void) {

    VICIntEnClr = (0x1 << RTC_CHANNEL);
}

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
void enableEINT0(U16 isr_vec, U32 int_handler) {

    U8 eint0_ctl = 0x2E;

    disableEINT0();                           // Disable EINT0

    // 01b is EINT0 mode, bits 1:0
    PINSEL1  = (PINSEL1 & EINT0_PINSEL1_MASK) | (0x1);

    // note: Only write 0's to high bits of EXT registers.

//    EXTMODE  = (EXTMODE & 0x000E) | EINT0_MODE_LEVEL;  
    EXTMODE  = (EXTMODE & 0x000E) | EINT0_MODE_EDGE;  

    EXTPOLAR = (EXTPOLAR & 0x000F) & ~(EINT0_LEVEL_HI); // level low
//    EXTPOLAR = (EXTPOLAR & 0x000F) | EINT0_LEVEL_HI;    // level hi
//    EXTPOLAR = (EXTPOLAR & 0x000F) | EINT0_POS_EDGE;    // pos edge
    EXTPOLAR = (EXTPOLAR & 0x000F) & ~(EINT0_POS_EDGE);   // neg edge

    // chaning EXT Registers may set an EXT flag.
    EXTINT   = EXT0FLAG;                          // Clear EINT0 Flag

    update_VIC_table(isr_vec, int_handler, eint0_ctl);

    // bit 1:0 of pinsel1 is P0.16  
    VICIntSelect &= ~(1 << EINT0_CHANNEL);

    VICIntEnable =   (1 << EINT0_CHANNEL);

}

/*
 * disableEINT0
 * ------------------------------
 * olimex b2
 */
inline void disableEINT0(void) {

    VICIntEnClr = (0x1 << EINT0_CHANNEL);
}

/*
 * enableEINT2
 * ------------------------------
 * olimex b1
 */
void enableEINT2(U16 isr_vec, U32 int_handler) {
    U32 eint2_ctl = 0x00000030;

    disableEINT2();                          // Disable EINT2

    // 10b is EINT mode, bits 31:30
    PINSEL0  = (PINSEL0 & EINT2_PINSEL0_MASK) | (0x2<<30);

    // note: Only write 0's to high bits of EXT registers.

//    EXTMODE  = (EXTMODE & 0x000B) | EINT2_MODE_LEVEL; 
    EXTMODE  = (EXTMODE & 0x000B) | EINT2_MODE_EDGE;  

//    EXTPOLAR = (EXTPOLAR & 0x000F) & ~(EINT2_LEVEL_HI); // level low
//    EXTPOLAR = (EXTPOLAR & 0x000F) | EINT2_LEVEL_HI;    // level hi
    EXTPOLAR = (EXTPOLAR & 0x000F) | EINT2_POS_EDGE;    // pos edge
//    EXTPOLAR = (EXTPOLAR & 0x000F) & ~(EINT2_POS_EDGE);   // neg edge

    EXTINT   = EXT2FLAG;                     // Clear the EINT2 interrupt flag

    update_VIC_table(isr_vec, int_handler, eint2_ctl);

    VICIntSelect &= ~(1 << EINT2_CHANNEL);   // IRQ

    VICIntEnable =   (1 << EINT2_CHANNEL);   // Enable the EINT2 interrupt
}

/*
 * disableEINT2
 * ------------------------------
 */
inline void disableEINT2(void) {
    VICIntEnClr = (0x1 << EINT2_CHANNEL);
}

/*
 * clearIntEnable
 * ----------------------------
 * write 1's to the VICIntEnClr register
 * Reference: User manual LPC214x p55
 */
inline void clearIntEnable(void) {
    VICIntEnClr=U32MAX;
}

/*
 * readIntEnable
 * ------------------------
 */
U32 readIntEnable(void) {
    return(VICIntEnable);
}

/*
 * writeIntEnable
 * ------------------------
 */
void writeIntEnable(U32 status) {
    VICIntEnable = status;
}

/*
 * readAndClearIntEnable
 * ----------------------------
 * Read and clear VICIntEnable register
 * Save and return the value prior to clear 
 * Reference: User manual LPC214x p55
 */
U32 readAndClearIntEnable(void) {
    U32 oldIntEnable;

    oldIntEnable = readIntEnable();
    clearIntEnable();
    return(oldIntEnable);
}

/*
 * clearSoftInt
 * ----------------------------
 * Write all 1's to the VICSoftIntClear register
 * Reference: User manual LPC214x p53
 */
void clearSoftInt(void) {
    VICSoftIntClr=U32MAX;
}

/*
 * readAndClearSoftInt
 * ----------------------------
 * Read and clear VICSoftInt register
 * Save and return the value prior to clear 
 * Reference: User manual LPC214x p52
 */
U32 readAndClearSoftInt() {
    U32 oldSoftInt;

    oldSoftInt = (U32) VICSoftInt;
    clearSoftInt();
    return(oldSoftInt);
}


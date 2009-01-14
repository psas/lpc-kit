
/*
 * timer-demo.c
 */


#include "./include/timer-demo.h"


#include "lpc214x.h"
#include "types.h"
#include "pll.h"
#include "olimex.h"
#include "interrupts.h"
#include "hwsys.h"
#include "conio.h"
#include "lpc-timer.h"

int main() {

    // Initialize the system
    initialize();

    LED1_ON;
    LED2_OFF;
    DBG("Hello: timer-demo. Watch the lights.\n");

    // run in timer mode.
    SET_T0CTCR(0x0);

    // match at this value and generate an interrupt.
    T0MR0=0xFFFFFF;

    // generate an interrupt, reset TC 
    SET_MATCH_CONTROL_0(0x3);

    RESET_TIMER0;
    
    ENABLE_TIMER0;

    while (1);

    return(0);

}



/*
 * initialize
 * -----------------------------------------------
 * initialize the lpc2148 pll 
 * cpu interrupts
 * the uart0 serial port
 * the rtc VIC interrupt
 */
void initialize(void)  {
      // PLL and MAM
      // hwSysInit(SIXTY_MHZ);
      // hwSysInit(FOURTYEIGHT_MHZ);
      // hwSysInit(THIRTYSIX_MHZ);
      hwSysInit(TWENTYFOUR_MHZ);
      // hwSysInit(TWELVE_MHZ);
    
    // enable interrupts in the CPSR
    enableIRQ();
    
    // enable the leds
    enable_leds();

    // 48Mhz PCLK, divisor is 0x1a for 115200
    console0Init(ONE_FIFTEEN_TWO_B);

    // enable the interrupt control register.
    enableTIMER0_INT(0x4, (unsigned) TIMER0_Handler );

}


/*
 * TIMER0_Handler
 * ------------------------
 * Code TIMER0 interrupt
 */
void TIMER0_Handler (void) {
  
    led1_invert();
    led2_invert();
 
    RESET_T0IR;     // Timer 0 Interrupt flag

    EXIT_INTERRUPT;
}


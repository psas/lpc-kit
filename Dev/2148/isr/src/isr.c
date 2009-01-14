
/*
 * isr.c
 * Experiment with setting new ENTRY/EXIT routines for ISR
 * Use B1 and B2 on Olimex lpc2148 board
 */


#include "./include/isr.h"

#include "types.h"
#include "lpc214x.h"
#include "pll.h"
#include "olimex.h"
#include "interrupts.h"
#include "hwsys.h"
#include "conio.h"

int main() {

    // Initialize the system
    initialize();

    // Test 0
    // start test, LED 1 lighted
    led2_off();
    led1_on();

    DBG("\nHello from: isr\n");

    while(1) { led1_on(); }

    return(0);
}


/*
 * initialize
 * -----------------------------------------------
 * initialize the lpc2148 pll and uart0 serial port
 */
void initialize(void)  {
    // PLL and MAM
    //  hwSysInit(SIXTY_MHZ);
    hwSysInit(FOURTYEIGHT_MHZ);
    //     hwSysInit(THIRTYSIX_MHZ);
    //     hwSysInit(TWENTYFOUR_MHZ);
    // hwSysInit(TWELVE_MHZ);

    // enable interrupts in the CPSR
    enableIRQ();

    // enable the leds
    enable_leds();

    console0Init(ONE_FIFTEEN_TWO_B);

    // enable the interrupt control register.
    enableEINT0(0, (unsigned) EINT0_Handler );  // use VIC slot 0
    enableEINT2(1, (unsigned) EINT2_Handler );  // use VIC slot 1
}

/*
 * EINT0_Handler
 * -------------------------
 * Code for a B2 press
 * Don't use ISR_ENTRY/ISR_EXIT
 */
void EINT0_Handler (void) {

    flash_led2(10);
    DBG("Button B2 pressed\n");

    EXTINT      = 0xF;
    EXIT_INTERRUPT;
}

/*
 * EINT2_Handler
 * ------------------------
 * Code for a B1 press
 * Do use ISR_ENTRY/ISR_EXIT
 */
void EINT2_Handler (void) {

    //    ISR_ENTRY();

    flash_led1(10);
    DBG("Button B1 pressed\n");

    EXTINT      = 0XF;

    EXIT_INTERRUPT;

    //   ISR_EXIT(); 

}



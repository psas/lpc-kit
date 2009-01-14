
/*
 * template.c
 * -------------------------------------------
 * Requires serial cable and terminal (e.g. minicom)
 */

#include "./include/template.h"

#include "lpc214x.h"
#include "types.h"
#include "helpers.h"
#include "pll.h"
#include "hwsys.h"
#include "conio.h"
#include "olimex.h"
#include "interrupts.h"

unsigned short int g_bpressed;

/*
 * main
 */
int main (void) {


    // used to count button presses.
    g_bpressed=0;

    // Initialize the system
    initialize();		

    // Test 0
    // start test, LED 1 lighted
    led2_off();
    led1_on();
    puts("Hello from LPC2148\n");
   // DBG("Debug statement from LPC2148\n");
    puts_u1("Hello from LPC2148 uart1\n");

    // wait for button press
    // while(!g_bpressed);

    // ...
    // Do stuff...
    // ...

    // End
    puts("...End...\n");
   // DBG("...End...\n");
    puts_u1("...End...\n");

    // end of test, LED 2 lighted
    led1_off();
    led2_on();

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

    // 115200
    console0Init(ONE_FIFTEEN_TWO_B);
    console1Init(ONE_FIFTEEN_TWO_B);
    // 57600
    // console0Init(FIFTY_SEVEN_SIX_B);

    // enable the interrupt control register.
    enableEINT0(0, (unsigned) EINT0_Handler );  // use VIC slot 0
    enableEINT2(1, (unsigned) EINT2_Handler );  // use VIC slot 1

}

/*
 * EINT0_Handler
 * -------------------------
 * Code for a B2 press
 */
void EINT0_Handler (void) {
//    ISR_ENTRY();

    flash_led2(10);
    puts("Button B2 pressed\n");
    g_bpressed++;

    EXTINT      = 0xF;
    VICVectAddr = 0x0;
//    ISR_EXIT();
}

/*
 * EINT2_Handler
 * ------------------------
 * Code for a B1 press
 */
void EINT2_Handler (void) {
 //   ISR_ENTRY();

    flash_led1(10);
    puts("Button B1 pressed\n");
    g_bpressed++;

    EXTINT      = 0XF;
    VICVectAddr = 0x0;
  //  ISR_EXIT();
}



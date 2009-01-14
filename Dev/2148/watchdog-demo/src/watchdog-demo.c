
/*
 * watchdog-demo.c
 */


#include "./include/watchdog-demo.h"

#include "lpc214x.h"
#include "types.h"
#include "helpers.h"
#include "pll.h"
#include "olimex.h"
#include "interrupts.h"
#include "hwsys.h"
#include "conio.h"
#include "lpc-watchdog.h"

int wd_int;

int main() {
    
    int i;
    U8 watchdog_timeout_flag;

    // Initialize the system
    initialize();

    wd_int=0;
    
    LED1_ON;
    LED2_OFF;

    DBG("\nHello from: watchdog-demo\n");

    watchdog_timeout_flag = ((WDMOD & 0x4) >> 2);

    if(watchdog_timeout_flag == 1) {
	DBG("Watchdog reset\n");
	WATCHDOG_CLEAR_TIMEOUT_FLAG;

	// debug info
	DBG("Reset WDTOF: ");
	DBG(itoa(WDMOD,16));
	DBG("\n");

    }
    else {
	DBG("Other reset\n");
    }

    // about 3 seconds at 48Mhz?
    // count = (sec * 48e6)/4
    WATCHDOG_TIMER_CONST(0x2255100);

    WATCHDOG_ENABLE;

    // This will reset the chip...no interrupt should be seen.
    // comment out next line to run in interrupt mode.
    WATCHDOG_RESET_ENABLE;

    watchdog_feed(); // begin.

    // feed the dog a few times...
    for(i=0; i<4; ++i) {  
	waitCount(2000000); 
	DBG("Feed the dog.\n");
	watchdog_feed(); // copies from timer const to timer.
    }

    while(1);

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
      hwSysInit(SIXTY_MHZ);
      // hwSysInit(FOURTYEIGHT_MHZ);
      // hwSysInit(THIRTYSIX_MHZ);
      // hwSysInit(TWENTYFOUR_MHZ);
      // hwSysInit(TWELVE_MHZ);
      //
    
    // enable interrupts in the CPSR
    enableIRQ();
    
    // enable the leds
    enable_leds();

    console0Init(ONE_FIFTEEN_TWO_B);

    // enable the interrupt control register.
    enableWATCHDOG_INT(0x0, (unsigned) WATCHDOG_Handler );

}

/*
 * WATCHDOG_Handler
 * ------------------------
 * Code WATCHDOG interrupt
 */
void WATCHDOG_Handler (void) {
  
    led1_invert();
    led2_invert();
    DBG("\nWATCHDOG_Handler: Watchdog Timeout...WOOF!\n");

    wd_int=1;

    WATCHDOG_CLEAR_TIMEOUT_FLAG; 

    // clear VIC flag
    // WDINT cannot be cleared without reset.
    // Reference: User Manual LPC214x Rev. 02--25 July 2006 p292
    WATCHDOG_CLEAR_INT;


    EXIT_INTERRUPT;
}




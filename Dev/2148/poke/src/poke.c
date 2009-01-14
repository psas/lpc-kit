
/*
 * poke.c
 * -------------------------------------------
 * Simple "poke stuff" exploratory program for  
 * Olimex LPC-P2148 board
 * Requires serial cable and terminal (e.g. minicom)
 */

#include "./include/poke.h"

#include "lpc214x.h"
#include "types.h"
#include "helpers.h"
#include "pll.h"
#include "hwsys.h"
#include "conio.h"
#include "olimex.h"
#include "interrupts.h"


unsigned short int g_bpressed=0;

/* Arm inline assy cookbook:                               */
/* http://www.ethernut.de/en/documents/arm-inline-asm.html */
/*
 * get_cpsr
 * ---------------------------------------
 * Reads and returns the value of the cpsr
 * (Example of inline assy for arm with arm-elf-gcc)
 * "You can add the volatile attribute to the asm  statement 
 * to instruct the compiler not to optimize your assembler code."-ibid 
 */
static inline U32 q_cpsr(void)
{
    U32 retval;
    asm volatile (" mrs  %0, cpsr" : "=r" (retval) : /* no inputs */  );
    return retval;
}


/*
 * get_r0
 * ---------------------------------------
 * Reads and returns the value of the 
 * (Example of inline assy for arm with arm-elf-gcc)
 * %0 is "=r"
 * %1 is "r"
 * 'volatile' keyword removed in this example
 */
static inline U32 q_r0(int value)
{
    U32 retval;
    asm (" mov  %0, %1" : "=r" (retval) : "r" (value) );
    return retval;
}


/*
 * main
 */
int main (void) {

    U32 cpsr_val=0x0;
    U32 r0_val  =0x0;

    // used to count button presses.
    g_bpressed=0;

    // Initialize the system
    initialize();		

    // Test 0
    // start test, LED 1 lighted
    led2_off();
    led1_on();
    puts("Hello from LPC2148\r");

    // Test 1:
    //  hit the buzzer....
    beep_high();

    // Test 2;
    // serial i/o
    puts("Type something to continue. LOCAL ECHO ON?\r");
    echo_line();

    // Test 3:
    // interrupts.
    puts("Press a button to continue\r");

    while(!g_bpressed);

    DBG("\n");

    // Test 4:
    // Read cpsr register
    cpsr_val = q_cpsr();
    DBG("CPSR Register is: ");
    DBG( itoa(cpsr_val, 16) );
    DBG("\n");

    // Test 5:
    // Read r0 register
    DBG("r0 Register is: 0x");
    DBG( itoa(r0_val, 16) );
    DBG("\n");
    DBG("Setting r0 Register to 0x42...\n");
    r0_val = q_r0(0x42);
    DBG("r0 Register is now: 0x%x\n",r0_val);

    if(r0_val != 0x42) { 
        DBG("Error: r0 is: %x\n",r0_val);
    }

    // End
    puts("...End...\n");

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

    console0Init(ONE_FIFTEEN_TWO_B);

    // enable the interrupt control register.
    enableEINT0(0, (unsigned) EINT0_Handler );  // use VIC slot 0
    enableEINT2(1, (unsigned) EINT2_Handler );  // use VIC slot 1

    DBG("End of init\n");

}

/*
 * echo_line
 * ---------------------------------------
 * read and echo a line to the serial port
 */
void echo_line(void) {

    char* reply = "\recho: ";
    char* echo;

    echo = serial_getline();

    puts(reply);
    puts(echo);
    putchar('\r');
}


/*
 * EINT0_Handler
 * -------------------------
 * Code for a B2 press
 */
void EINT0_Handler (void) {

//    ISR_ENTRY();

    flash_led2(10);
    puts("Button B2 pressed\r");
    g_bpressed++;

    EXTINT      = 0xF;
    VICVectAddr = 0x0;

 //   ISR_EXIT();
}

/*
 * EINT2_Handler
 * ------------------------
 * Code for a B1 press
 */
void EINT2_Handler (void) {
//    ISR_ENTRY();

    flash_led1(10);
    puts("Button B1 pressed\r");
    g_bpressed++;

    EXTINT      = 0XF;
    VICVectAddr = 0x0;
 //   ISR_EXIT();
}



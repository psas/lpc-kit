
/*
 * rtc-demo.c
 * -------------------------
 * Experiments with the real time clock module.
 */

#include <math.h>

#include "./include/rtc-demo.h"

#include "lpc-rtc.h"

#include "types.h"
#include "olimex.h"
#include "helpers.h"
#include "lpc214x.h"
#include "pll.h"
#include "interrupts.h"
#include "hwsys.h"
#include "conio.h"


int main() {

    // Initialize the system
    initialize();

    LED1_ON;
    LED2_OFF;
    DBG("Hello: rtc-demo.\n");

    rtc_init(SOURCE_RTCX);

    /*
     * rtc_setTime
     * -----------------------------------------------
     * sets the Time counter group
     * SEC, MIN, HOUR, DOM, DOW, DOY, MONTH, YEAR
     * This function will calculate the DOW and DOY from
     * the DOM, MONTH, YEAR, and set the DOW and DOY 
     * counters.
     */
    /* void rtc_setTime(U8  sec,
     *                 U8  min,
     *                 U8  hour,
     *                 U8  dom,
     *                 U8  month,
     *                 U16 year) {
     */

    rtc_setTime(0,25,17,16,8,2009);

    //    CIIR = (0x2); // interrupt every minute.
    CIIR = (0x1); // interrupt every second.

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
    hwSysInit(FOURTYEIGHT_MHZ);
    // hwSysInit(THIRTYSIX_MHZ);
    // hwSysInit(TWENTYFOUR_MHZ);
    // hwSysInit(TWELVE_MHZ);

    // enable interrupts in the CPSR
    enableIRQ();

    // enable the leds
    enable_leds();

    // 48Mhz PCLK, divisor is 0x1a for 115200
    console0Init(ONE_FIFTEEN_TWO_B);

    // enable the interrupt control register.
    enableRTC_INT(13, (unsigned) RTC_Handler );

}


/*
 * RTC_Handler
 * ------------------------
 * Code RTC interrupt
 */
void RTC_Handler (void) {
    U32 seconds, minutes, hours;
    U32 day, month, year;

    U32 dow, dofy;

    DBG("\nTime is: ");
    seconds = rtc_readSecs();
    minutes = rtc_readMins();
    hours = rtc_readHours();

    day = rtc_readDom();
    month = rtc_readMonth();
    year = rtc_readYear();

    dow = rtc_readDow();
    dofy= rtc_readDofY();
    //    dofy= CTIME2;

    DBG(itoa(hours,10));
    DBG(":");
    DBG(itoa(minutes,10));
    DBG(":");
    DBG(itoa(seconds,10));
    DBG(".\n");

    DBG("Date is: ");
    DBG(itoa(day,10));

    DBG("-");
    DBG(itoa(month,10));

    DBG("-");
    DBG(itoa(year,10));

    DBG(" : weekday ");
    DBG(itoa(dow,10));

    DBG(" : Ordinal day ");
    DBG(itoa(dofy,10));

    led1_invert();
    led2_invert();

    RESET_ILR; // interrupt location register

    EXIT_INTERRUPT;
}


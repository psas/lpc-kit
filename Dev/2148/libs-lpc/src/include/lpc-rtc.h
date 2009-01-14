
/* 
 * lpc-rtc.h
 * Functions to manage Real Time Clock (RTC) on LPC2148
 */


#ifndef _LPC_RTC_H
#define _LPC_RTC_H

#include "types.h"

// which clock should RTC use?
#define SOURCE_PCLK 0       
#define SOURCE_RTCX 1

// reset interrupt location register
#define RESET_ILR   ILR=ILR


/*
 * rtc_prescale
 * -----------------------------
 * set the prescale integer and fraction values
 * Reference: User Manual LPC214x Rev. 02--25 July 2006 p287
 * iScale: value for Integer Prescale PREINT
 * fScale: value for Fractional Prescale PREFRAC
 *
 * PREINT = (int) (PCLK/32768) -1
 * PREFRAC= PCLK - ((PREINT+1) x 32768)
 * Examples:

 PCLK     65,537        48MHz   10MHz
 PREINT   1             1463    304
 PREFRAC  1             27648   5760

*/
void rtc_prescale(U16 iScale, U16 fScale);


/*
 * rtc_init
 * ------------------------------------------------
 * setup the CCR-disable the clock
 * mask the alarms
 * clear the counter increment interrupt register (CIIR)
 * clear the IIR
 * reset and hold the clock tick register
 * enable the clock
 */
void rtc_init(U8 source);

/*
 * rtc_alarmMask
 * -------------------------------------------------
 * Write a value to the alarm mask register
 * Reference: User Manual LPC214x Rev. 02--25 July 2006 p 283
 */
void rtc_alarmMask(U8 amask);

/*
 * rtc_alarmSec
 * -------------------------------------------------
 * Write a value to the alarm ALSEC register
 * Reference: User Manual LPC214x Rev. 02--25 July 2006 p 286
 */
void rtc_alarmSec(U8 amask);

/*
 * rtc_alarmMin
 * -------------------------------------------------
 * Write a value to the alarm ALMIN register
 * Reference: User Manual LPC214x Rev. 02--25 July 2006 p 286
 */
void rtc_alarmMin(U8 amask);

/*
 * rtc_alarmHour
 * -------------------------------------------------
 * Write a value to the alarm ALHOUR register
 * Reference: User Manual LPC214x Rev. 02--25 July 2006 p 286
 */
void rtc_alarmHour(U8 amask);

/*
 * isLeapYear
 * ------------------------------------------------
 * Returns a bool if year is a leap year.
 */
bool isLeapYear(U16 year);

/*
 * ordinal_date
 * ----------------------------------------
 * returns the ordinal date from a m/d/y

 ordinal date from march 1:
 floor ( 30.6 ( m + 1 ) ) + d - 122

 The ordinal date from 1 January is:

 * for January: d
 * for February: d + 31
 * for the other months: the ordinal date from 1 March plus 59, or 60 in a leap year

 *
 */
U16 ordinal_date(U8 month, U8 day, U16 year);

/*
 * dow
 * ------------------------------------
 * Returns the day of the week (SUN=0..SAT=6)
 * http://en.wikipedia.org/wiki/Calculating_the_day_of_the_week#An_algorithm_to_calculate_the_day_of_the_week
 * Bounds checking forces month = 1 and year = 2008
 * for out of bounds month or year.
 */
U8 dow(U16 year, U8 month, U16 day);

/*
 * rtc_setTime
 * -----------------------------------------------
 * sets the Time counter group
 * SEC, MIN, HOUR, DOM, DOW, DOY, MONTH, YEAR
 * This function will calculate the DOW and DOY from
 * the DOM, MONTH, YEAR, and set the DOW and DOY 
 * counters.
 */
void rtc_setTime(U8  sec, 
                 U8  min, 
                 U8  hour, 
                 U8  dom,
                 U8  month,
                 U16 year);


/*
 * rtc_readCTC
 * ------------------------------------------------
 * Read the Clock Tick Counter
 */
U16 rtc_readCTC(void);

/*
 * rtc_readSecs
 * ------------------------------------------------
 * Read the Seconds from CTIME0
 */
U32 rtc_readSecs(void);

/*
 * rtc_readMins
 * ------------------------------------------------
 * Read the Mins from CTIME0
 */
U32 rtc_readMins(void);

/*
 * rtc_readHours
 * ------------------------------------------------
 * Read the Hours from CTIME0
 */
U32 rtc_readHours(void);

/*
 * rtc_readDow
 * ------------------------------------------------
 * Read the Day of week from CTIME0
 */
U32 rtc_readDow(void);

/*
 * rtc_readDom
 * ------------------------------------------------
 * Read the Day of Month from CTIME1
 */
U32 rtc_readDom(void);

/*
 * rtc_readMonth
 * ------------------------------------------------
 * Read the Month from CTIME1
 */
U32 rtc_readMonth(void);

/*
 * rtc_readYear
 * ------------------------------------------------
 * Read the Year from CTIME1
 */
U32 rtc_readYear(void);

/*
 * rtc_readDofY
 * ------------------------------------------------
 * Read the Ordinal Date from CTIME2
 */
U32 rtc_readDofY(void);



#endif

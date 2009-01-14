
/*
 * Functions to manage Real Time Clock (RTC) on LPC2148
 */



#include "math.h"

#include "./include/lpc-rtc.h"

#include "types.h"
#include "lpc214x.h"

#include "lpc-serial.h"
#include "helpers.h"

// declare prototype.
U8 _month_map(U16 year, U8 month);


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
void rtc_prescale(U16 iScale, U16 fScale) {
    U16 int_mask  = 0x1FFF;
    U16 frac_mask = 0x7FFF;

    PREINT = iScale & int_mask;
    PREFRAC= fScale & frac_mask;

}

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
void rtc_init(U8 source) {

    U8 bitmask = 0x1;


    source &= bitmask;                   // clean up source, just in case.


    CCR    = (source << 4);              // bit 4 is the CLKSRC

    AMR    = 0xFF;                       // mask all alarms.

    CIIR   = 0x0;                        // clear all increment interrupts

    ILR    = 0x3;                        // clear interrupt flags
    
    CCR    = CCR | (0x2);                // clear Counters (CTCR)
    CCR    = CCR & ~(0x2);               // clear Counters (CTCR)

    CCR    = CCR | (0x01);               // enable clock to counters.

}

/*
 * rtc_alarmMask
 * -------------------------------------------------
 * Write a value to the alarm mask register
 * Reference: User Manual LPC214x Rev. 02--25 July 2006 p 283
 */
void rtc_alarmMask(U8 amask) {
    
    AMR = amask;

}


/*
 * rtc_alarmSec
 * -------------------------------------------------
 * Write a value to the alarm ALSEC register
 * Reference: User Manual LPC214x Rev. 02--25 July 2006 p 286
 */
void rtc_alarmSec(U8 amask) {
    U8 alsec_mask = 0x3F;

    ALSEC = amask & alsec_mask;

}

/*
 * rtc_alarmMin
 * -------------------------------------------------
 * Write a value to the alarm ALMIN register
 * Reference: User Manual LPC214x Rev. 02--25 July 2006 p 286
 */
void rtc_alarmMin(U8 amask) {
    U8 almin_mask = 0x3F;

    ALMIN = amask & almin_mask;

}

/*
 * rtc_alarmHour
 * -------------------------------------------------
 * Write a value to the alarm ALHOUR register
 * Reference: User Manual LPC214x Rev. 02--25 July 2006 p 286
 */
void rtc_alarmHour(U8 amask) {
    U8 alhour_mask = 0x1F;

    ALHOUR = amask & alhour_mask;

}

/*
 * isLeapYear
 * ------------------------------------------------
 * Returns a bool if year is a leap year.
 */
bool isLeapYear(U16 year) {

    U8 yb2 = year % 100;

    if( ( ( yb2 == 0 ) && ((year % 100) == 0) && ((year % 400) == 0) ) ||
	( ( yb2 != 0 ) && ( year % 4    == 0)                        ) 
	) { 
	return(TRUE);
    } else {
	return(FALSE);
    }
}

               

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
U16 ordinal_date(U8 month, U8 day, U16 year) {

    U16 march_first_ord;

    // Bounds checking, a simple version.
    if( (year  < 1) || (year  > 4095) ) { year  = 2008; }
    if( (month < 1) || (month >   12) ) { month = 1;    }
    if( (day   < 1) || (day   >   31) ) { day   = 1;    }

    if      (month == 1) { return ((U16) day);                  }
    else if (month == 2) { return ((U16) (day + 31));           }
    else {
	march_first_ord =  floor ( 30.6 * ( month + 1 ) ) + day - 122;
	if(isLeapYear(year)) { return (U16)(march_first_ord + 60); }
	else                 { return (U16)(march_first_ord + 59); }
    }
}

/*
 * _month_map
 * ------------------------------------------
 * Returns a key for the month calculation DOW
 */
U8 _month_map(U16 year, U8 month) {

    U8 mkey;

    // Months: We refer to the months table below to work out on which day of the
    // week a month starts. Notice that January starts on day 0, which is simply
    // another way of saying that the year and January of that year start on the
    // same day

    // magic numbers:(non-leap){ Jan.................Dec }
    static U8 mkey_array[12] = { 0,3,3,6,1,4,6,2,5,0,3,5 };

    // leap year?
    if( isLeapYear(year) ) { 
	if      (month == 1 )  { mkey = 6;  }
	else if (month == 2 )  { mkey = 2;  }
	else {  mkey = mkey_array[month-1]; }
    }
    else { // not a leap year....
	mkey = mkey_array[month-1];
    }
    return(mkey);
}



/*
 * dow
 * ------------------------------------
 * Returns the day of the week (SUN=0..SAT=6)
 * http://en.wikipedia.org/wiki/Calculating_the_day_of_the_week#An_algorithm_to_calculate_the_day_of_the_week
 * Bounds checking forces month = 1 and year = 2008
 * for out of bounds month or year.
 */
U8 dow(U16 year, U8 month, U16 day) {
   
    U8  yt2, yb2, m;
    U16 c;
    U16 y;

    // Bounds checking, a simple version.
    if( (year  < 1) || (year  > 4095) ) { year  = 2008; }
    if( (month < 1) || (month > 12)   ) { month = 1;    }
    if( (day   < 1) || (day   > 31)   ) { day   = 1;    }

    // integer division always rounds to 0 in C standard for POSITIVE
    // numbers.
    // top 2 digits of year 2104 ==> 21
    yt2 =  year / 100;

    // bottom two digits of the year
    yb2 = year % 100;

    // calculate c
    c =  2 * (3 - ( yt2 % 4));

    // day of week year starts....prior to mod 7, we do that
    // at end.
    y = floor(yb2/4);

    // day of week month starts
    m = _month_map(year, month);
    
    // add and mod 7
    return ( (c + yb2 + y + m + day) % 7 );

}
  


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
                 U16 year) {
    // clean up inputs.
    U8  sec_mask  = 0x03F;
    U8  min_mask  = 0x03F;
    U8  hour_mask = 0x01F;
    U8  dom_mask  = 0x01F;
    U8  month_mask= 0x00F;
    U16 year_mask = 0xFFF;

    // clean up DOW and DOY
    U8  dow_mask  = 0x007;
    U16 doy_mask  = 0x1FF;

    SEC  = sec  & sec_mask;
    MIN  = min  & min_mask;
    HOUR = hour & hour_mask;
    DOM  = dom  & dom_mask;
   
    DOW = dow(year, month, dom) & dow_mask;
    DOY = ordinal_date(month, dom, year) & doy_mask;
 
    MONTH = month & month_mask;
    YEAR  = year & year_mask;

}


/*
 * rtc_readCTC
 * ------------------------------------------------
 * Read the Clock Tick Counter
 */
U16 rtc_readCTC(void) {

    U16 ctc_mask = 0x7FF;
    
    return(CTC & ctc_mask);

}


/*
 * rtc_readSecs
 * ------------------------------------------------
 * Read the Seconds from CTIME0
 */
U32 rtc_readSecs(void) {

    U32 sec_mask = 0x3F;
    
    return(CTIME0 & sec_mask);

}

/*
 * rtc_readMins
 * ------------------------------------------------
 * Read the Mins from CTIME0
 */
U32 rtc_readMins(void) {

    // bits 13:8
    U32 min_mask = 0x3F00;
    
    return( ((CTIME0 & min_mask) >> 8) );

}

/*
 * rtc_readHours
 * ------------------------------------------------
 * Read the Hours from CTIME0
 */
U32 rtc_readHours(void) {

    // bits 20:16
    U32 hour_mask = 0x3F0000;
    
    return( ((CTIME0 & hour_mask) >> 16) );

}


/*
 * rtc_readDow
 * ------------------------------------------------
 * Read the Day of week from CTIME0
 */
U32 rtc_readDow(void) {

    // bits 26:24
    U32 dow_mask = 0x07000000;
    
    return( ((CTIME0 & dow_mask) >> 24) );

}


/*
 * rtc_readDom
 * ------------------------------------------------
 * Read the Day of Month from CTIME1
 */
U32 rtc_readDom(void) {

    // bits 20:16
    U32 dom_mask = 0x1F;
    
    return( CTIME1 & dom_mask );

}

/*
 * rtc_readMonth
 * ------------------------------------------------
 * Read the Month from CTIME1
 */
U32 rtc_readMonth(void) {

    // bits 11:8
    U32 month_mask = 0xF00;
    
    return( ((CTIME1 & month_mask)>>8) );

}


/*
 * rtc_readYear
 * ------------------------------------------------
 * Read the Year from CTIME1
 */
U32 rtc_readYear(void) {

    // bits 27:16
    U32 year_mask = 0x0FFF0000;
    
    return( ((CTIME1 & year_mask)>>16) );

}


/*
 * rtc_readDofY
 * ------------------------------------------------
 * Read the Ordinal Date from CTIME2
 */
U32 rtc_readDofY(void) {

    // bits 11:0
    U32 dofy_mask = 0x0FFF;
    
    return( CTIME2 & dofy_mask );

}




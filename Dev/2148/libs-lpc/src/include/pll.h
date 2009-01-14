
/*
 * pll.h 
 */

#ifndef _PLL_H
#define _PLL_H

#include "./lpc214x.h"
#include "./types.h"


/*
 * pll0_feed
 * ----------------------
 * Turns off interrupts, feeds pll, then
 * restores interrupt functions.
 * Reference : User manual LPC214x p38
 */
void pll0_feed(void);




/*
 * pll0_run
 * ------------------------
 * Starts the pll, waits for lock and
 * connects the system clock.
 * psel is low 2 bits of U8
 * msel is low 5 bits of U8
 * other bits are ignored.
 * Reference: User manual LPC314x p 40
 *
 PLLCFG(0or1) PLL Configuration Register
 7 | 6  5 | 4 0  |
 0 | PSEL | MSEL | 

 Only write 0 to bit 7
 PSEL PLL Divider value
 PMUL PLL Multiplier value

 From the spec: LPC214x User Manual p39
 If Fosc is 12Mhz (Olimex lpc2148 uses 12Mhz)...
 Then only two settings will keep these restraints:

 10Mhz  <= CCLK <= 60Mhz
 156Mhz <= Fcco <= 320Mhz

 CCLK   P   M  PSEL  MSEL  Fcco   Fosc
 48Mhz  2   4  1     3     192Mhz  12Mhz
 12Mhz  8   1  3     0     192Mhz  12Mhz

 CCLK = M x Fosc
 Fcco = M x 2 x P

 Example:
 pll0_run(0x1,0x3)
 CCLK = 48Mhz, Fcco 192Mhz

 pll0_run(0x3, 0x0)
 CCLK = 12Mhz, Fcco 192Mhz

 APBDIV: 00 APB bus is 0.25 CCLK   (reset value)
         01 APB == CCLK
	 10 APB bus is 0.50 CCLK
	 11 NO EFFECT, SETTING UNCHANGED

*/
void pll0_run(U8 psel, U8 msel, U8 apbdiv);



/*
 * pll1_feed
 * ----------------------
 * Turns off interrupts, feeds pll, then
 * restores interrupt functions.
 * Reference : User manual LPC214x p38
 */
void pll1_feed(void);


/*
 * pll1_run
 * ------------------------
 * Starts the pll, waits for lock then
 * connects the USB clock.
 * psel is low 2 bits of U8
 * msel is low 5 bits of U8
 * other bits are ignored.
 * Reference: User manual LPC214x p 40 Example 2
 *
 PLLCFG(0or1) PLL Configuration Register
 7 | 6  5 | 4 0  |
 0 | PSEL | MSEL | 

 Only write 0 to bit 7
 PSEL PLL Divider value
 PMUL PLL Multiplier value
 Example:
 LPC2148 USB : pll1_run(0x1,0x3) 
 ( P=2 (12x4=48Mhz, M=4 (M-1=3) Fcco=192Mhz.)
*/
void pll1_run(U8 psel , U8 msel);


/*
 * read_PLL0STAT
 * bits [15:11] and bit 7 undefined on read.
 * may not agree with PLLCON and PLLCFG if no
 * feed has taken place.
 */
U16 read_PLL0STAT(void);


/*
 * read_PLL1STAT
 * bits [15:11] and bit 7 undefined on read.
 * may not agree with PLLCON and PLLCFG if no
 * feed has taken place.
 */
U16 read_PLL1STAT(void);


/*
 * write_APBDIV
 * ------------------------------------
 * Same as VPBDIV
 * Only low two bits are used.
 */
void write_APBDIV(U8 update);





#endif  /* _PLL_H */

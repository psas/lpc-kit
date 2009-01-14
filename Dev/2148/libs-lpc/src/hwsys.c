
/**********************************************************
                  Header files
 **********************************************************/

#include "lpc214x.h"
#include "types.h"
#include "debug.h"
#include "pll.h"
#include "mam.h"
#include "hwsys.h"

/* global static */
static int cclk_frequency_g;
static int pclk_frequency_g;


/*
 *  lpc214x user manual: page 21
 *  For system clock slower than 20 MHz, MAMTIM can be 001. For system clock between
 *  20 MHz and 40 MHz, Flash access time is suggested to be 2 CCLKs, while in systems
 *  with system clock faster than 40 MHz, 3 CCLKs are proposed.
 */
void hwSysInit(freq_t cclkFreq) { 
  switch(cclkFreq) {
    case SIXTY_MHZ:
      //  CCLK = 60Mhz, Fcco 240Mhz
      //  APBDIV = 01 => 60Mhz PCLK
      pll0_run(0x1,0x4, 0x01);
      cclk_frequency_g = pclk_frequency_g = 60000000;
      write_MAM(0x2,0x3);
      break;
    case FOURTYEIGHT_MHZ:
      //  CCLK = 48Mhz, Fcco 192Mhz
      //  APBDIV = 01 => 48Mhz PCLK
      pll0_run(0x1,0x3, 0x01);
      cclk_frequency_g = pclk_frequency_g = 48000000;
      write_MAM(0x2,0x3);
      break;
    case THIRTYSIX_MHZ:
      //  CCLK = 36Mhz, Fcco 280Mhz
      //  APBDIV = 01 => 36Mhz PCLK
      pll0_run(0x2,0x2, 0x01);
      cclk_frequency_g = pclk_frequency_g = 36000000;
      write_MAM(0x2,0x2);
      break;
    case TWENTYFOUR_MHZ:
      //  CCLK = 24Mhz, Fcco 192Mhz
      //  APBDIV = 01 => 24Mhz PCLK
      pll0_run(0x2,0x1, 0x01);
      cclk_frequency_g = pclk_frequency_g = 24000000;
      write_MAM(0x2,0x2);
      break;
    case TWELVE_MHZ:
      //  CCLK = 12Mhz, Fcco 192Mhz
      //  APBDIV = 01 => 12Mhz PCLK
      pll0_run(0x3,0x0, 0x01);
      cclk_frequency_g = pclk_frequency_g = 12000000;
      write_MAM(0x2,0x1);
      break;
    default:
      // no debug print statement yet available...no console
      break;
  }

}

inline int hwSysCclkVal(void)
{
  return cclk_frequency_g;
}

inline int hwSysPclkVal(void)
{
  return pclk_frequency_g;
}

/*
 *  Stubs for various interrupts 
 */

void IRQ_Routine (void) {
	while (1) ;	
}

void FIQ_Routine (void)  {
	while (1) ;	
}

void SWI_Routine (void)  {
	while (1) ;	
}

void UNDEF_Routine (void) {
	while (1) ;	
}

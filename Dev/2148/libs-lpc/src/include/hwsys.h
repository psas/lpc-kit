

#ifndef _HWSYS_H
#define _HWSYS_H

/* 
 * Attempt to limit settings of frequency by using
 * C's limited type checking abilities. (i.e. Don't
 * let someone try to program 42.334 Mhz as a frequency.)
 */
typedef enum {
  TWELVE_MHZ      = 12000000,
  TWENTYFOUR_MHZ  = 24000000, 
  THIRTYSIX_MHZ   = 36000000, 
  FOURTYEIGHT_MHZ = 48000000,
  SIXTY_MHZ       = 60000000 
} freq_t;

void	            hwSysInit(freq_t cclkFreq);

inline int	    hwSysCclkVal(void);

inline int          hwSysPclkVal(void);

void IRQ_Routine (void)   __attribute__ ((interrupt("IRQ")));
void FIQ_Routine (void)   __attribute__ ((interrupt("FIQ")));
void SWI_Routine (void)   __attribute__ ((interrupt("SWI")));
void UNDEF_Routine (void) __attribute__ ((interrupt("UNDEF")));

#endif


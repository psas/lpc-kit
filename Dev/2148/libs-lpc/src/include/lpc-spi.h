/*
 *  lpc-spi.h
 */


/* Don't write 1's to 15:12 and 1:0 */
#define S0SPCR_MASK 0x0FFC

#define SPIF_MASK 0x0080

#define M6675_DATA_MASK 0x7FF8
#define M6674_DATA_MASK 0x7FE0


inline void enable_spi0(void) ;


/*
 *  Functions to manage SPI (serial peripheral interface) module
 */


/* local includes */
#include "./include/lpc-spi.h"
#include "types.h"
#include "lpc214x.h"
#include "lpc-serial.h"
#include "helpers.h"



/*
 * enable_spi0
 *   Configure pins for SPI
 *   p0.4-01 9:8 of PINSEL0 SCK03
 *   p0.5-01 11:10 of PINSEL0 MISO0
 *   p0.6-01 13:12 of PINSEL0 MOSI0
 *   p0.7-01 15:14 of PINSEL0 SSEL0
 *   15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
 *    0  1  0  1  0  1 0 1 1 1 1 1 1 1 1 1
 *         5         5         f    f
 */
inline void enable_spi0(void) {
    PINSEL0 &= 0xFFFF55FF;      // SPI0 Configuration P0.[4,5,6,7]
}







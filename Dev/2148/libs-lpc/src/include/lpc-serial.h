
/*
 * lpc-serial.h
 */

#ifndef _LPC_SERIAL_H
#define _LPC_SERIAL_H


#include "lpc214x.h"
#include "types.h"

// (lcr)
// Control register values. p 102 LPC214x manual.
// 8 bit word, No parity, 1 stop bit
#define U0LCR_8N1            0x3  


// FIFO (fcr)                   trig 000 txreset/rxreset Enable
#define U0FCR_EN_ONE_CHAR        0x7    // 00 000 00 1
#define U0FCR_EN_FOUR_CHAR       0x41   // 01 000 00 1
#define U0FCR_EN_EIGHT_CHAR      0x81   // 10 000 00 1
#define U0FCR_EN_FOURTEEN_CHAR   0xC1   // 11 000 00 1



/*
 * serial0Init
 * ---------------------------------------
 * Initialize the serial port for UART0
 * sample divisor calculations:
 * PCLK       Baud            divisor        %error
 * 48Mhz      115200          26d or 0x1a    0.16
 * 48Mhz      19200          156d or 0x9c    0.16             
 * 12Mhz      19200           39d or 0x27    0.16
 * 12Mhz      115200           6  or 0x6     7.84
 * OR alternately: 
 *  (U16) (FLOOR(PCLK/16*BAUD_RATE)) 
 *
 * Example:
 * serial0Init(0x1a, U0LCR_8N1, U0FCR_EN_ONE_CHAR)  
 *
 * // 48Mhz PCLK, divisor is 0x9c for 19200
 *   serial0Init(0x9c, U0LCR_8N1, U0FCR_EN_ONE_CHAR);
 *
 */
void serial0Init(U16 divisor, U8 lcr, U8 fcr);

/*
 * serial_getline
 * --------------------------------------
 * receive chars from the serial port until
 * \r or 120chars, then return a string.
 * YOU SHOULD copy string to a new variable,
 * local to calling scope.
 */
char* serial_getline (void);

/*
 * serial_getchar
 * --------------------------------------
 * receive a char from the serial port
 * Return a char (U8).
 */
char serial_getchar (void);

/*
 * serial_putchar
 * -------------------------------------
 * put a char to the serial port
 *
 * '\n' transmitted as '\r'
 *
 * Non zero return implies error.
 * 
 */
U32 serial_putchar(char output);

/*
 * serial_putstring
 * -------------------------------------
 * put a string to the serial port
 *
 * non zero return implies error
 */
U32 serial_putstring(const char *s);



#endif


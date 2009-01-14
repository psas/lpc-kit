
/*
 * lpc-serial.h
 */

#ifndef _LPC_SERIAL_H
#define _LPC_SERIAL_H


#include <math.h>

#include "lpc214x.h"
#include "types.h"
#include "helpers.h"

typedef enum {
  TWELVE_B             = 1200,
  FOURTY_EIGHT_B       = 4800,
  NINETY_SIX_B         = 9600,
  NINETEEN_TWO_B       = 19200,
  THIRTY_EIGHT_FOUR_B  = 38400,
  FIFTY_SEVEN_SIX_B    = 57600,
  ONE_FIFTEEN_TWO_B    = 115200 
} baud_t;

#define MAXBUFFER                 120

// define these for readablity.
//
#define U0RBR_EMPTY               !(U0LSR & 0x01)
#define U0THRE_EMPTY               (U0LSR & 0x20) 

#define U1RBR_EMPTY               !(U0LSR & 0x01)
#define U1THRE_EMPTY               (U0LSR & 0x20) 
//
//
#define U0LCR_DLAB_MASK           0x7F  
#define U1LCR_DLAB_MASK           0x7F  
//
// /* bits 3:0 determine pin0.1 and pin0.0 functionality */
// /* pin0.0 is TXD(UART0), pin0.1 is RXD(UART0) */
#define TXD_RXD_0_PINSEL0_MASK    ~(0xF)
#define TXD_RXD_0_ENABLE            0x5

// /* bits 19:16 determine pin0.9 and pin0.8 functionality */
// /* pin0.9 is RxD(UART1), pin0.8 is TxD(UART1) */
#define TXD_RXD_1_PINSEL0_MASK    ~(0x000F0000)
#define TXD_RXD_1_ENABLE            0x00050000

//
//
#define U0FCR_TX_RESET       U0FCR = (U0FCR & 0xFB) | 0x04
#define U0FCR_RX_RESET       U0FCR = (U0FCR & 0xFD) | 0x02

#define U1FCR_TX_RESET       U1FCR = (U1FCR & 0xFB) | 0x04
#define U1FCR_RX_RESET       U1FCR = (U1FCR & 0xFD) | 0x02
//
//
// (lcr)
// Control register values. p 102 LPC214x manual.
// 8 bit word, No parity, 1 stop bit
#define U0LCR_8N1            0x3  
#define U1LCR_8N1            0x3  


// FIFO (fcr)                   trig 000 txreset/rxreset Enable
#define U0FCR_EN_ONE_CHAR        0x7    // 00 000 00 1
#define U0FCR_EN_FOUR_CHAR       0x41   // 01 000 00 1
#define U0FCR_EN_EIGHT_CHAR      0x81   // 10 000 00 1
#define U0FCR_EN_FOURTEEN_CHAR   0xC1   // 11 000 00 1

#define U1FCR_EN_ONE_CHAR        0x7    // 00 000 00 1
#define U1FCR_EN_FOUR_CHAR       0x41   // 01 000 00 1
#define U1FCR_EN_EIGHT_CHAR      0x81   // 10 000 00 1
#define U1FCR_EN_FOURTEEN_CHAR   0xC1   // 11 000 00 1



/*
 * console0Init
 * ---------------------------------------
 * Initialize the serial port for UART0
 *
 * Fix to 8N1 for simplicity.
 * Fix to one char for interrupt for simplicity.
 */
void console0Init(baud_t baudRate);

/*
 * console1Init
 * ---------------------------------------
 * Initialize the serial port for UART0
 *
 * Fix to 8N1 for simplicity.
 * Fix to one char for interrupt for simplicity.
 */
void console1Init(baud_t baudRate);


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
 * putchar
 * -------------------------------------
 * put a char to the serial port
 *
 * '\n' transmitted as '\r'
 *
 * Non zero return implies error.
 * 
 */
int putchar(int output);

/*
 * putchar_u1
 * -------------------------------------
 * put a char to the serial port uart1
 *
 * '\n' transmitted as '\r'
 *
 * Non zero return implies error.
 * 
 */
int putchar_u1(int output);


/*
 * serial_putstring
 * -------------------------------------
 * put a string to the serial port
 *
 * non zero return implies error
 */
int puts(const char *s);



#endif



/*
 * lpc-serial.c
 * -------------------------------------------
 * Simple serial port console routines 
 * using the LPC2148
 */

#include <string.h>

#include "./include/lpc-serial.h"

#include "./include/types.h"
#include "./include/lpc214x.h"

#define MAXBUFFER                 120

// define these for readablity.

#define U0RBR_EMPTY               !(U0LSR & 0x01)
#define U0THRE_EMPTY               (U0LSR & 0x20) 

#define U0LCR_DLAB_MASK           0x7F  

/* bits 3:0 determine pin0.1 and pin0.0 functionality */
/* pin0.0 is TXD(UART0), pin0.1 is RXD(UART0) */
#define TXD_RXD_0_PINSEL0_MASK    ~(0xF)
#define TXD_RXD_0_ENABLE          0x5


#define U0FCR_TX_RESET       U0FCR = (U0FCR & 0xFB) | 0x04
#define U0FCR_RX_RESET       U0FCR = (U0FCR & 0xFD) | 0x02


/*
 * serial0Init
 * ---------------------------------------
 * Initialize the serial port for UART0
 * sample divisor calculations:
 * PCLK       Baud            divisor        %error
 * 48Mhz      115200          26d or 0x1a    0.16
 * 12Mhz      19200           39d or 0x27    0.16
 * 12Mhz      115200          6   or 0x6     7.84
 * OR alternately: 
 * serial0Init( (U16)FLOOR(PCLK/16*BAUD_RATE) )
 */
void serial0Init(U16 divisor, U8 lcr, U8 fcr) {

    PINSEL0 = (PINSEL0 & TXD_RXD_0_PINSEL0_MASK) | TXD_RXD_0_ENABLE ;

    // uart control register. Set DLAB (lcr bit 7) 
    //   to access divisor latches, for fractional baud rate.
    U0LCR = lcr | ~U0LCR_DLAB_MASK; 

    // U0DLL is lower 8 bits of divisor.
    // baud rate CLOCK (must =) 16 x Baud rate goal.
    if(divisor == 0x0) 
	U0DLL = 0x1; // Don't divide by zero. Ever.
    else
	U0DLL = (divisor & 0xFF);

    // U16 is unsigned type, so this is a LOGICAL shift! 
    // No sign bit preservation....i.e. this is a + number. 
    U0DLM  = divisor >> 8; 

    // uart control. Clear DLAB, no more access needed.
    U0LCR = U0LCR & U0LCR_DLAB_MASK;

    // enable FIFO
    U0FCR = fcr;

}    

/*
 * serial_getline
 * --------------------------------------
 * receive chars from the serial port until
 * \n, then return a string.
 * Maximum length of line is 120 chars.
 * copy string from pointer to new variable,
 * static declaration here is to avoid global declaration.
 * HOWEVER, the pointer is globally accessed too...shrug.
 */
char* serial_getline (void)  {
 
    char current=' ';
    U16 index=0;
    static char _LINEBUFFER[MAXBUFFER];

//    while(current != '\r' && index < MAXBUFFER-1) {
    while(current != '\n' && index < MAXBUFFER-1) {
	current=serial_getchar();
	_LINEBUFFER[index]=current;
	++index;
    }
    _LINEBUFFER[index]='\0';

    return (_LINEBUFFER);
}


/*
 * serial_getchar
 * --------------------------------------
 * receive a char from the serial port
 * Return a char (U8).
 */
char serial_getchar (void)  {
  
    while (U0RBR_EMPTY);

    return (U0RBR);
}



/*
 * serial_putchar
 * -------------------------------------
 * put a char to the serial port
 *
 * Non zero return implies error.
 * 
 */
U32 serial_putchar(char output) {

    U32 error;

    if(output == '\n') output = '\r';

    while (!U0THRE_EMPTY) ; /* wait */

    U0THR = output;
    
    error = 0;
    return(error);
}



/*
 * serial_putstring
 * -------------------------------------
 * put a string to the serial port
 *
 * Assumes null termination of string.
 * 
 * non zero return implies error
 */
U32 serial_putstring(const char *s) {

    U32 error;
    int i, length;

    length = strlen(s);

    for (i=0; i < length; ++i) {
	serial_putchar(*s); // put the char
	s++;                // increment to next char
    }

    error = 0;
    return(error);
}



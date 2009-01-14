
/*
 * conio.c
 * -------------------------------------------
 * Simple serial port console routines 
 * using the LPC2148 UART0 and UART1
 */


#include "./include/conio.h"
#include "hwsys.h"

#include "types.h"
#include "lpc214x.h"
#include "helpers.h"

/*
 * console0Init
 * ---------------------------------------
 * Initialize the serial port for UART0
 *
 * Fix to 8N1 for simplicity.
 * Fix to one char for interrupt for simplicity.
 */
void console0Init(baud_t baudRate) {

    U16 divisor;

    if(hwSysPclkVal() == TWELVE_MHZ) {
        divisor = (U16) floor(hwSysPclkVal()/(16 * baudRate));
        if(baudRate == ONE_FIFTEEN_TWO_B) {
            // Set the Fractional divide register to get baud to 115384.6
            U0FDR = (12 << 4) | 1 ;
        }
    } else {
        divisor = (U16) floor(hwSysPclkVal()/(16 * baudRate));
    }

    PINSEL0 = (PINSEL0 & TXD_RXD_0_PINSEL0_MASK) | TXD_RXD_0_ENABLE ;

    PCONP |= 0x08;
    // uart control register. Set DLAB (lcr bit 7) 
    //   to access divisor latches, for fractional baud rate.
    U0LCR = U0LCR_8N1 | ~U0LCR_DLAB_MASK; 

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
    U0FCR = U0FCR_EN_ONE_CHAR;
}

/*
 * console1Init
 * ---------------------------------------
 * Initialize the serial port for UART1
 *
 * Fix to 8N1 for simplicity.
 * Fix to one char for interrupt for simplicity.
 */
void console1Init(baud_t baudRate) {

    U16 divisor;

    if(hwSysPclkVal() == TWELVE_MHZ) {
        divisor = (U16) floor(hwSysPclkVal()/(16 * baudRate));
        if(baudRate == ONE_FIFTEEN_TWO_B) {
            // Set the Fractional divide register to get baud to 115384.6
            U1FDR = (12 << 4) | 1 ;
        }
    } else {
        divisor = (U16) floor(hwSysPclkVal()/(16 * baudRate));
    }

    PINSEL0 = (PINSEL0 & TXD_RXD_1_PINSEL0_MASK) | TXD_RXD_1_ENABLE ;

    PCONP |= 0x10;
    // uart control register. Set DLAB (lcr bit 7) 
    //   to access divisor latches, for fractional baud rate.
    U1LCR = U1LCR_8N1 | ~U1LCR_DLAB_MASK; 

    // U1DLL is lower 8 bits of divisor.
    // baud rate CLOCK (must =) 16 x Baud rate goal.
    if(divisor == 0x0) 
        U1DLL = 0x1; // Don't divide by zero. Ever.
    else
        U1DLL = (divisor & 0xFF);

    // U16 is unsigned type, so this is a LOGICAL shift! 
    // No sign bit preservation....i.e. this is a + number. 
    U1DLM  = divisor >> 8; 

    // uart control. Clear DLAB, no more access needed.
    U1LCR = U1LCR & U1LCR_DLAB_MASK;

    // enable FIFO
    U1FCR = U1FCR_EN_ONE_CHAR;
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

    while((current != '\n' && current != '\r') && index < MAXBUFFER-1) {
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
 * putchar
 * -------------------------------------
 * put a char to the serial port
 * 
 */
int putchar(int ch) {

    if (ch == '\n') {
        while (!(U0LSR & 0x20));
        U0THR = '\r';
    } 
    while (!(U0LSR & 0x20));
    U0THR = ch;

    return ch;
}


/*
 * putchar_u1
 * -------------------------------------
 * put a char to the serial port
 * 
 */
int putchar_u1(int ch) {

    if (ch == '\n') {
        while (!(U1LSR & 0x20));
        U1THR = '\r';
    } 
    while (!(U1LSR & 0x20));
    U1THR = ch;

    return ch;
}




/*
 * puts
 * -------------------------------------
 * put a string to the serial port
 *
 * Assumes null termination of string.
 */
int puts(const char *s) {

    while(*s) {
        putchar(*s++); // put the char
    }
    putchar('\n');
    return(1);
}


/*
 * puts_u1
 * -------------------------------------
 * put a string to the serial port uart1
 *
 * Assumes null termination of string.
 */
int puts_u1(const char *s) {

    while(*s) {
        putchar_u1(*s++); // put the char
    }
    putchar_u1('\n');
    return(1);
}



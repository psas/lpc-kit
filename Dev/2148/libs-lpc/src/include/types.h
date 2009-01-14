
/* 
 * types.h
 * -----------------------------------------
 */ 



#ifndef _TYPES_H
#define _TYPES_H

/* Boolean type(s) */
typedef enum {FALSE, TRUE} bool;
typedef enum {NO   , YES } yesno;

/* From limits.h (gcc)
 *  Number of bits in a `char'.
 *  #  define CHAR_BIT      8
 *  The C standard says 'at least 1 byte'... 
 */
typedef        unsigned char          BYTE;  /* 8 bits  0-255           */
typedef        unsigned char          U8  ;  /* 8 bits  0-255           */
typedef        unsigned short int     U16 ;  /* 16 bits 0-65,535        */
typedef        unsigned int           U32 ;  /* 32 bits 0-4,294,967,295 */

#define        U32MAX                 0xFFFFFFFF;
#define        U32MIN                 0x00000000;

#define        U16MAX                 0xFFFF;
#define        U16MIN                 0x0000;

/* See  /usr/include/linux/stddef.h */
#ifndef NULL
#define NULL ((void *)0)

#endif




#endif /* _TYPES_H */

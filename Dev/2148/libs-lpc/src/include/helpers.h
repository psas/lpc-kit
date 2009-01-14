
/* 
 * helpers.h
 * -----------------------------------------------
 * Useful preprocessor macros and #defines 
 *
 */


#ifndef _HELPERS_H
#define _HELPERS_H

int printf(const char *format, ...);

#ifdef DEBUG
#define DBG     printf
#define ASSERT(x)       if(!(x)){DBG("\nAssertion '%s' failed in %s:%s#%d!\n",#x,__FILE__,__FUNCTION__,__LINE__);while(1);}
#else
#define DBG(x ...)
#define ASSERT(x)
#endif


/*
 * itoa
 * ------------------------------
 * integer to ascii code
 * Copy return to a local variable for long
 * term useage.
 */
char* itoa(int val, int base);

/*
 * ftoa
 * float to ascii code
 * Copy return to a local var for long term usage
 */
/*
char* ftoa(float val);
*/


#define MINOF(a,b) ( ( (a) < (b) ) ? (a) : (b) )
#define MAXOF(a,b) ( ( (a) > (b) ) ? (a) : (b) )






#endif /* _HELPERS_H */

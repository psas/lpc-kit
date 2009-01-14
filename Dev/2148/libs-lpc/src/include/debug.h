
/* 
 * helpers.h
 * -----------------------------------------------
 * Useful preprocessor macros and #defines 
 *
 */


#ifndef _DEBUG_H
#define _DEBUG_H


/*
    Cites:
    http://gcc.gnu.org/onlinedocs/cpp/Macros.html
    http://inst.eecs.berkeley.edu/~kennyto/useful_cpp_macros.html
*/

/* Assert */
#define USBASSERT(expr)\
{\
  if(!(expr))\
  {\
    fprintf(stderr, "USBASSERT failed on \"%s\" at %s:%d\n",\
                     #expr, __FILE__, __LINE__);\
    exit(-1);\
  }\
} 

/* Breadcrumb macro... */
#define REACHED printf("REACHED %s:%d\n", __FILE__, __LINE__); 







#endif /* _DEBUG_H */


/*
 * MAM (Memory Acceleration Module) functions for lpc2148
 */

#ifndef _MAM_H
#define _MAM_H

#include "./lpc214x.h"
#include "./mam.h"
#include "./types.h"


/*
 * write_MAM
 * ---------------------------------------
 * values for MAMCR and mamtim are correctly
 * updated using this function.
 * Reference: User manual lpc214x p 21
 * MAMCR: only two lower bits are used [1:0]
 * MAMTIM: only three lower bits are used [2:0]
 */
void write_MAM(U8 mamcr, U8 mamtim);

/*
 * read_MAMCR
 * ------------------------------
 * read the MAMCR register. 
 * Upper bits undefined for read [7:2]
 */
U8 read_MAMCR(void);


/*
 * read_MAMTIM
 * ------------------------------
 * read the MAMTIM register. 
 * Upper bits undefined for read [7:3]
 */
U8 read_MAMTIM(void);




#endif // _MAM_H


/*
 * Mam.c : implementation for updating 
 *         MAM (Memory acceleration module)
 */

#include "./include/lpc214x.h"
#include "./include/mam.h"

/*
 * write_MAM
 * ---------------------------------------
 * values for mamcr and mamtim are correctly
 * updated using this function.
 * Reference: User manual lpc214x p 21
 */
void write_MAM(U8 mamcr, U8 mamtim) {

    U8 mamcrMask  = 0x3;   // only write 0's to reserved bits 
    U8 mamtimMask = 0x7;   // only write 0's to reserved bits 

    // prior to changing write 0's to MAMCR
    MAMCR  = 0x0;


    // update in this order per manual description.
    MAMTIM = (mamtim & mamtimMask);

    MAMCR  = (mamcr  & mamcrMask);
}

/*
 * read_MAMCR
 * ------------------------------
 * read the mamcr register. 
 * Upper bits undefined for read [7:2]
 */
U8 read_MAMCR(void) {
    return(MAMCR);
}

/*
 * read_MAMTIM
 * ------------------------------
 * read the mamcr register. 
 * Upper bits undefined for read [7:3]
 */
U8 read_MAMTIM(void) {
    return(MAMTIM);
}


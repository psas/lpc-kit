
/*
 * helpers.c
 * ---------------------------
 * sometimes useful code.
 */

// #include <stdio.h>

/*
 * itoa
 * ------------------------------
 * integer to ascii code
 * Copy return to a local variable for long
 * term useage.
 */
char* itoa(int val, int base){

    static char ibuf[32] = {0};

    int i = 30;

    if(val == 0) { return "0"; }
    else {
	for(; val && i ; --i, val /= base)

	    ibuf[i] = "0123456789abcdef"[val % base];

	return &ibuf[i+1];
    }
}


/*
 * ftoa
 * float to ascii code
 * Copy return to a local var for long term usage
 */
/* char* ftoa(float val)
{
	static char fbuf[20];

	snprintf(fbuf, 19, "%f", val);

//	printf("Here is the result: %s\n", halfpie);
	return &fbuf[0];
}
*/





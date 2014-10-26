#ifndef PLSTR_H
#define PLSTR_H

#include <stdlib.h>


/*****************************************************************
 *                  FUNCTION DEFINITIONS                         *
 *****************************************************************/


char *pl_cpy(char *source, char *destination);

char *pl_slice(char *source, int offset, int limit);

char *pl_cat(char *destination, char *source);

#endif /* PLSTR_H */

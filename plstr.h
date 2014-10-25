#ifndef PLSTR_H
#define PLSTR_H

#include <stdlib.h>


/*****************************************************************
 *                  FUNCTION DEFINITIONS                         *
 *****************************************************************/


char *pl_strcpy(char *destination, char *source, size_t dest_size);

char *pl_slice(char *source, int offset, int limit);


#endif /* PLSTR_H */

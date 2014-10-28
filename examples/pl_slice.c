#include "plstr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main() {
    char the_string[] = "subdermatoglyphic";
    char *sliced;

    sliced = pl_slice(the_string, 3, 10);
    if (sliced != NULL) {
        printf("the_string: %s\nsliced 3, 10: %s\n", the_string, sliced);
        free(sliced);
    }

    sliced = pl_slice(the_string, 0, -10);
    if (sliced != NULL) {
        printf("sliced 0,-10: %s\n", sliced);
        free(sliced);
    }

    return 0;
}

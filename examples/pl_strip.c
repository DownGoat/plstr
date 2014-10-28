#include "plstr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main() {
    char whitespace[] = "   lots of space   ";
    char cool_nick[] = "xxxxTheDude99xxxx";
    char *ret_val = NULL;

    ret_val = pl_strip(whitespace, NULL);
    if (ret_val != NULL) {
        printf("Original: %s\nStriped: %s\n", whitespace, ret_val);
        free(ret_val);
    }

    ret_val = pl_strip(cool_nick, "x");
    if (ret_val != NULL) {
        printf("Striped: %s\n", ret_val);
        free(ret_val);
    }


    return 0;
}

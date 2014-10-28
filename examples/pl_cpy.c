#include "plstr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main() {
    char the_string[] = "This is a short string.";
    char *copied_str, *pre_alloc;

    copied_str = pl_cpy(the_string, NULL);
    if (copied_str != NULL) {
        printf("copied_str: %s\n", copied_str);
    }

    pre_alloc = (char *) calloc(strlen(the_string) + 1, sizeof(char));
    if (pre_alloc == NULL) {
        return 0;
    }

    if(pl_cpy(the_string, pre_alloc) != NULL) {
        printf("pre_alloc: %s\n", pre_alloc);
    }

    free(copied_str);
    free(pre_alloc);

    return 0;
}

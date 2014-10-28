#include "plstr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main() {
    char foo[] = "foo";
    char bar[] = "bar";
    char *concatenated = NULL;

    concatenated = pl_cat(foo, bar);
    if (concatenated != NULL) {
        printf("foo:%s\nbar:%s\nconcatenated:%s\n", foo, bar, concatenated);
    }

    free(concatenated);

    return 0;
}

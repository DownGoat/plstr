#include "plstr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main() {
    char the_string[] = "This is a short string.";
    char **splitted;
    int size;

    splitted = pl_split(the_string, " ", &size);

    if (splitted != NULL) {
        int i;

        printf("The size is %d\n", size);
        for (i = 0; i < size; i++) {
            printf("splitted[%d] = %s\n", i, splitted[i]);
        }

        for (i = 0; i < size; i++) {
            free(splitted[i]);
        }

        free(splitted);
    }

    return 0;
}

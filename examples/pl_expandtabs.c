#include "plstr.h"
#include <stdio.h>
#include <stdlib.h>


int main() {
    char *ret_val;

    ret_val = pl_expandtabs("tabs\tinstead\tof\tspace", 1);
    if (ret_val != NULL) {
        printf("tabs\tinstead\tof\tspace\n%s\n", ret_val);
    }

    free(ret_val);

    return 0;
}

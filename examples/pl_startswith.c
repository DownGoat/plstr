#include "plstr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main() {
    char the_string[] = "http://google.com";
    int ret_val;

    ret_val = pl_startswith(the_string, "http://");
    if (ret_val) {
        printf("the_string, does indeed start with http://\n");
    }

    ret_val = pl_startswith(the_string, "ftp://");
    if (!ret_val) {
        printf("And it does not start with ftp://\n");
    }

    return 0;
}

#include "plstr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main() {
    char the_string[] = "http://google.com";
    int ret_val;

    ret_val = pl_endswith(the_string, ".com");
    if (ret_val) {
            printf("the_string, does indeed end with .com\n");
    }

    ret_val = pl_endswith(the_string, ".net");
    if (!ret_val) {
            printf("And it does not end with .net\n");
    }

    return 0;
}

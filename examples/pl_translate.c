#include "plstr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main() {
    char the_string[] = "read this short text";
    char *ret_val = NULL;

    ret_val = pl_translate(the_string, NULL, "aeiou");
    if (ret_val != NULL) {
        printf("the_string: %s\nno table:%s\n", the_string, ret_val);
        free(ret_val);
    }

    ret_val = pl_translate(the_string, "aeiou", "xxxxx");
    if (ret_val != NULL) {
        printf("with table: %s\n", ret_val);
        free(ret_val);
    }

    return 0;
}

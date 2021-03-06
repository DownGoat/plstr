/* The MIT License (MIT)
 *
 * Copyright (c) <2014> <Sindre Smistad>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
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

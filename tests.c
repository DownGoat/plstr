/**
 * The MIT License (MIT)
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <execinfo.h>


#define KRED    "\x1B[31m"
#define KGRN    "\x1B[32m"
#define KYLW    "\x1B[33m"
#define KWHT    "\x1B[37m"


void assert_equal_str(char *x, char *y, char *func_name, char *msg) {
    if (x == NULL && y == NULL) {
        printf("%s[PASSED] %s: passed\n", KGRN, func_name);

        goto exit;
    }

    if (x == NULL || y == NULL) {
        printf("%s[FAILED] %s: %s\n", KRED, func_name, msg);
        printf("%sGot: %s\n", KWHT, y);
        printf("Expected: %s\n", x);

        goto exit;
    }

    else if (!strcmp(x, y)) {
        printf("%s[PASSED] %s: passed\n", KGRN, func_name);
    }

    else {
        printf("%s[FAILED] %s: %s\n", KRED, func_name, msg);
        printf("%sGot: %s\n", KWHT, y);
        printf("Expected: %s\n", x);
    }

exit:
    printf("%s", KWHT);
}


void assert_equal_int(int x, int y, char * func_name, char *msg) {
    if(x == y ) {
        printf("%s[PASSED] %s: passed\n", KGRN, func_name);
    }

    else {
        printf("%s[FAILED] %s: %s\n", KRED, func_name, msg);
        printf("%sGot: %d\n", KWHT, y);
        printf("Expected: %d\n", x);
    }

    printf("%s", KWHT);
}


void test_slice_positive_sub_str() {
    char the_string[] = "spam, eggs, and ham";
    char *ret_val;

    ret_val = pl_slice(the_string, 3, 9);

    assert_equal_str(
            "m, egg",
            ret_val,
            "test_slice_positive_sub_str",
            "The substring is not equal to the sliced string."
            );
}


void test_slice_negative_offset() {
    char the_string[] = "spam, eggs, and ham";
    char *ret_val;

    ret_val = pl_slice(the_string, -3, 9);

    assert_equal_str(
            NULL,
            ret_val,
            "test_slice_negative_offset",
            "Test 1: The ret_val is not empty."
            );

    ret_val = pl_slice(the_string, -1, 9);

    assert_equal_str(
            NULL,
            ret_val,
            "test_slice_negative_offset",
            "Test 2: The ret_val is not empty."
            );

    ret_val = pl_slice(the_string, -9, 9);

    assert_equal_str(
            NULL,
            ret_val,
            "test_slice_negative_offset",
            "Test 3: The ret_val is not empty."
            );

    ret_val = pl_slice(the_string, -1, 3);

    assert_equal_str(
            NULL,
            ret_val,
            "test_slice_negative_offset",
            "Test 4: The ret_val is not empty."
            );

    ret_val = pl_slice(the_string, -4, 1);

    assert_equal_str(
            NULL,
            ret_val,
            "test_slice_negative_offset",
            "Test 5: The ret_val is not empty."
            );
}


void test_slice_negative_limit() {
    char the_string[] = "spam, eggs, and ham";
    char *ret_val;

    ret_val = pl_slice(the_string, 1, -3);

    assert_equal_str(
                "pam, eggs, and ",
                ret_val,
                "test_slice_negative_limit",
                "Test 1: The strings are not equal"
            );

    ret_val = pl_slice(the_string, 1, -10);

    assert_equal_str(
                "pam, egg",
                ret_val,
                "test_slice_negative_limit",
                "Test 2: The strings are not equal"
            );

    ret_val = pl_slice(the_string, 9, -10);

    assert_equal_str(
                NULL,
                ret_val,
                "test_slice_negative_limit",
                "Test 3: The strings are not equal"
            );

    ret_val = pl_slice(the_string, 0, -5);

    assert_equal_str(
                "spam, eggs, an",
                ret_val,
                "test_slice_negative_limit",
                "Test 4: The strings are not equal"
            );
}


void test_slice_equal_offlim() {
    char the_string[] = "spam, eggs, and ham";
    char *ret_val;

    ret_val = pl_slice(the_string, 3, 3);

    assert_equal_str(
                NULL,
                ret_val,
                "test_slice_equal_offlim",
                "The strings are not equal."
            );
}


void test_slice_both_negative() {
    char the_string[] = "spam, eggs, and ham";
    char *ret_val;

    ret_val = pl_slice(the_string, -13, -4);

    assert_equal_str(
                "eggs, and",
                ret_val,
                "test_slice_both_negative",
                "Test 1: The function did not return the correct substring."
            );

    ret_val = pl_slice(the_string, -5, -5);

    assert_equal_str(
                NULL,
                ret_val,
                "test_slice_both_negative",
                "Test 2: The function did not return the correct substring."
            );

    ret_val = pl_slice(the_string, -5, -10);

    assert_equal_str(
                NULL,
                ret_val,
                "test_slice_both_negative",
                "Test 3: The function did not return the correct substring."
            );
}


void test_slice_limit_oor() {
    char the_string[] = "spam, eggs, and ham";
    char *ret_val;

    ret_val = pl_slice(the_string, 0, 99);

    assert_equal_str(
            NULL,
            ret_val,
            "test_slice_limit_oor",
            "The ret_val was not NULL."
            );
}


void test_slice_offset_oor() {
    char the_string[] = "spam, eggs, and ham";
    char *ret_val;

    ret_val = pl_slice(the_string, 99, 199);

    assert_equal_str(
                NULL,
                ret_val,
                "test_slice_offset_oor",
                "The ret_val was not NULL"
            );
}


void test_strcpy_no_alloc() {
    char the_string[] = "spam, eggs, and ham";
    char *ret_val;

    ret_val = pl_cpy(the_string, NULL);

    assert_equal_str(
                "spam, eggs, and ham",
                ret_val,
                "test_strcpy_no_alloc",
                "The strings are not equal."
            );
}


void test_strcpy_pre_alloc() {
    char the_string[] = "spam, eggs, and ham";
    char *ret_val;

    ret_val = (char *) calloc(strlen(the_string) + 1, sizeof(char));

    ret_val = pl_cpy(the_string, ret_val);

    assert_equal_str(
                "spam, eggs, and ham",
                ret_val,
                "test_strcpy_pre_alloc",
                "The strings are not equal."
            );
}


void test_strcpy_not_empty() {
    char the_string[] = "spam, eggs, and ham";
    char *ret_val, tmp[100];

    strcpy(tmp, "foobar");

    ret_val = pl_cpy(the_string, NULL);

    assert_equal_str(
                "spam, eggs, and ham",
                ret_val,
                "test_strcpy_not_empty",
                "The strings are not equal"
            );
}


void test_strcat() {
    char foo[] = "foo";
    char bar[] = "bar";
    char *ret_val;

    ret_val = pl_cat(foo, bar);

    assert_equal_str(
                "foobar",
                ret_val,
                "test_strcat",
                "The strings are not equal."
            );
}


void test_strcat_empty_source() {
    char empty[5] = {0};
    char bar[] = "bar";
    char *ret_val;

    ret_val = pl_cat(empty, bar);

    assert_equal_str(
                "bar",
                ret_val,
                "test_strcat_empty_source",
                "The strings are not equal."
            );
}


void test_strcat_empty_destination() {
    char empty[5] = {0};
    char foo[] = "foo";
    char *ret_val;

    ret_val = pl_cat(foo, empty);

    assert_equal_str(
                "foo",
                ret_val,
                "test_strcat_empty_destination",
                "The strings are not equal."
            );
}


void test_split() {
    int size = 0;
    char the_string[] = "fooasdbar";
    char **ret_val;

    ret_val = pl_split(the_string, "asd", &size);

    assert_equal_str(
                "foo",
                ret_val[0],
                "test_split",
                "Test 1: The strings are not equal."
            );

    assert_equal_str(
                "bar",
                ret_val[1],
                "test_split",
                "Test 2: The strings are not equal."
            );

    assert_equal_int(
                2,
                size,
                "test_split",
                "Test 3: Size is not correct."
            );
}


void test_split_multiple_delims() {
    int size = 0;
    char the_string[] = "fooasdbarasdmagic";
    char **ret_val;

    ret_val = pl_split(the_string, "asd", &size);

    assert_equal_str(
                "foo",
                ret_val[0],
                "test_split_multiple_delims",
                "Test 1: Strings are not equal."
            );

    assert_equal_str(
                "bar",
                ret_val[1],
                "test_split_multiple_delims",
                "Test 2: Strings are not equal."
            );

    assert_equal_str(
                "magic",
                ret_val[2],
                "test_split_multiple_delims",
                "Test 3: Strings are not equal"
            );

    assert_equal_int(
                3,
                size,
                "test_split_multiple_delims",
                "Test 4: Size is not correct."
            );
}


void test_split_delim_not_found() {
    int size = 0;
    char the_string[] = "fooasdbarasdmagic";
    char **ret_val;

    ret_val = pl_split(the_string, "x", &size);

    assert_equal_str(
                NULL,
                ret_val,
                "test_split_delim_not_found",
                "Test 1: NULL not returned."
            );

    assert_equal_int(
                0,
                size,
                "test_split_delim_not_found",
                "Test 2: Size is not correct."
            );
}


void test_split_empty_delim() {
    int size = 0;
    char the_string[] = "fooasdbarasdmagic";
    char **ret_val;

    ret_val = pl_split(the_string, "", &size);

    assert_equal_str(
                NULL,
                ret_val,
                "test_split_empty_delim",
                "Test 1: NULL not returned."
            );

    assert_equal_int(
                0,
                size,
                "test_split_empty_delim",
                "Test 2: Size is not right."
            );
}


void test_split_single_delim() {
    int size = 0;
    char the_string[] = "abc def ghi";
    char **ret_val;

    ret_val = pl_split(the_string, " ", &size);

    assert_equal_str(
                "abc",
                ret_val[0],
                "test_split_single_delim",
                "Test 1: Strings not equal."
            );

    assert_equal_str(
                "def",
                ret_val[1],
                "test_split_single_delim",
                "Test 2: Strings not equal."
            );

    assert_equal_str(
                "ghi",
                ret_val[2],
                "test_split_single_delim",
                "Test 3: Strings not equal."
            );

    assert_equal_int(
                3,
                size,
                "test_split_single_delim",
                "Test 4: Size of the returned array is not right."
            );
}


void test_startswith() {
    char the_string[] = "http://google.com";

    int ret_val = pl_startswith(the_string, "http://");

    assert_equal_int(
                1,
                ret_val,
                "test_startswith",
                "Test 1: The string does not start with http://"
            );

    ret_val = pl_startswith(the_string, "https://");

    assert_equal_int(
                0,
                ret_val,
                "test_startswith",
                "Test 2: The string starts with https://"
            );
}


void test_startwith_empy_string() {
    char the_string[] = "";

    int ret_val = pl_startswith(the_string, "asd");

    assert_equal_int(
                -1,
                ret_val,
                "test_startwith_empy_string",
                "The function did not return the correct error value."
            );
}


void test_startwith_empy_prefix() {
    char the_string[] = "http://bing.com";

    int ret_val = pl_startswith(the_string, "");

    assert_equal_int(
            -1,
            ret_val,
            "test_startwith_empy_prefix",
            "The function did not return the correct error value."
        );
}


void test_endswith() {
    char the_string[] = "http://yahoo.com";

    int ret_val = pl_endswith(the_string, ".com");

    assert_equal_int(
                1,
                ret_val,
                "test_endswith",
                "Test 1: The strings does not end with .com"
            );

    ret_val = pl_endswith(the_string, ".net");

    assert_equal_int(
                0,
                ret_val,
                "test_endswith",
                "Test 2: The string does end with .net"
            );
}


void test_endswith_empty_string() {
    char the_string[] = "";

    int ret_val = pl_endswith(the_string, "asd");

    assert_equal_int(
                -1,
                ret_val,
                "test_endswith_empty_string",
                "Error code not returned."
            );
}


void test_endswith_empty_postfix() {
    char the_string[] = "http://duckduckgo.com";

    int ret_val = pl_endswith(the_string, "");

    assert_equal_int(
                -1,
                ret_val,
                "test_endswith_empty_postfix",
                "Error code not returned."
            );
}


int main () {

    test_slice_positive_sub_str();
    test_slice_equal_offlim();
    test_slice_negative_offset();
    test_slice_negative_limit();
    test_slice_both_negative();
    test_slice_limit_oor();
    test_slice_offset_oor();
    test_strcpy_no_alloc();
    test_strcpy_pre_alloc();
    test_strcpy_not_empty();
    test_strcat();
    test_strcat_empty_source();
    test_strcat_empty_destination();
    test_split();
    test_split_multiple_delims();
    test_split_delim_not_found();
    test_split_empty_delim();
    test_split_single_delim();
    test_startswith();
    test_startwith_empy_string();
    test_startwith_empy_prefix();
    test_endswith();
    test_endswith_empty_string();
    test_endswith_empty_postfix();

    return 0;
}

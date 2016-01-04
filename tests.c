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


void assert_equal_pointers(void *x, void *y, char *func_name, char *msg) {
    if(x == y ) {
        printf("%s[PASSED] %s: passed\n", KGRN, func_name);
    }

    else {
        printf("%s[FAILED] %s: %s\n", KRED, func_name, msg);
        printf("%sGot: %p\n", KWHT, y);
        printf("Expected: %p\n", x);
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


void test_slice_empty_string() {
    char *ret_val;

    ret_val = pl_slice("", 1, 3);
    assert_equal_str(
                NULL,
                ret_val,
                "test_slice_empty_string",
                "Test 1: NULL not returned."
            );

    ret_val = pl_slice(NULL, 1, 3);
    assert_equal_str(
                NULL,
                ret_val,
                "test_slice_empty_string",
                "Test 2: NULL not returned."
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


void test_strcpy_empty_paras() {
    char *ret_val;

    ret_val = pl_cpy(NULL, NULL);
    assert_equal_str(
                NULL,
                ret_val,
                "test_strcpy_empty_paras",
                "Test 2: NULL not returned."
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


void test_strcat_empty_params() {
    char *ret_val;

    ret_val = pl_cat(NULL, "asd");
    assert_equal_str(
                NULL,
                ret_val,
                "test_strcat_empty_params",
                "Test 1: NULL not returned."
            );

    ret_val = pl_cat("asd", NULL);
    assert_equal_str(
                NULL,
                ret_val,
                "test_strcat_empty_params",
                "Test 2: NULL not returned."
            );

    ret_val = pl_cat(NULL, NULL);
    assert_equal_str(
                NULL,
                ret_val,
                "test_strcat_empty_params",
                "Test 3: NULL not returned."
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
                (char *) ret_val,
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
                (char *) ret_val,
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


void test_split_empty_paras() {
    char **ret_val;
    int size = 0;

    ret_val = pl_split("", " ", &size);
    assert_equal_str(
                NULL,
                (char *) ret_val,
                "test_split_empty_paras",
                "Test 1: NULL not returned."
            );

    ret_val = pl_split(NULL, " ", &size);
    assert_equal_str(
                NULL,
                (char *) ret_val,
                "test_split_empty_paras",
                "Test 2: NULL not returned"
            );

    ret_val = pl_split("asd asd", "", &size);
    assert_equal_str(
                NULL,
                (char *) ret_val,
                "test_split_empty_paras",
                "Test 3: NULL not returned."
            );

    ret_val = pl_split("asd asd", NULL, &size);
    assert_equal_str(
                NULL,
                (char *) ret_val,
                "test_split_empty_paras",
                "Test 4: NULL not returned."
            );

    ret_val = pl_split("asd asd", " ", NULL);
    assert_equal_str(
                NULL,
                (char *) ret_val,
                "test_split_empty_paras",
                "Test 5: NULL not returned."
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


void test_startwith_empty_params() {
    int ret_val;

    ret_val = pl_startswith(NULL, "asd");
    assert_equal_int(
                -1,
                ret_val,
                "test_startwith_empty_params",
                "Test 1: -1 Not returned."
            );

    ret_val = pl_startswith("www.vg.no", NULL);
    assert_equal_int(
                -1,
                ret_val,
                "test_startwith_empty_params",
                "Test 2: -1 Not returned."
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


void test_endswith_empty_params() {
    int ret_val;

    ret_val = pl_endswith(NULL, ".com");
    assert_equal_int(
                -1,
                ret_val,
                "test_endswith_empty_params",
                "Test 1: -1 Not returned."
            );

    ret_val = pl_endswith("asd.com", NULL);
    assert_equal_int(
                -1,
                ret_val,
                "test_endswith_empty_params",
                "Test 1: -1 not returned."
            );
}


void test_strip_no_chars() {
    char the_string[] = "   aaabbbccc";
    char str2[] = "aaabbbccc   ";
    char str3[] = "   aaabbbccc   ";
    char str4[] = "\n\r\t\v\f aaabbbccc    \f\v\r\n\t";
    char *ret_val;

    ret_val = pl_strip(the_string, NULL);
    assert_equal_str(
                "aaabbbccc",
                ret_val,
                "test_strip_no_chars",
                "Test 1: Strings are not equal."
            );

    ret_val = pl_strip(str2, NULL);
    assert_equal_str(
                "aaabbbccc",
                ret_val,
                "test_strip_no_chars",
                "Test 2: Strings are not equal."
            );

    ret_val = pl_strip(str3, NULL);
    assert_equal_str(
                "aaabbbccc",
                ret_val,
                "test_strip_no_chars",
                "Test 3: Strings are not equal."
            );

    ret_val = pl_strip(str4, NULL);
    assert_equal_str(
                "aaabbbccc",
                ret_val,
                "test_strip_no_chars",
                "Test 4: Strings are not equal."
            );
}


void test_strip_with_chars() {
    char *ret_val;
    char str1[] = "aaabbbccc";
    char str2[] = "bbbcccaaa";
    char str3[] = "aaabbbcccaaa";
    char str4[] = "adadbbbcccadad";

    ret_val = pl_strip(str1, "a");
    assert_equal_str(
                "bbbccc",
                ret_val,
                "test_strip_with_chars",
                "Test 1: Strings are not equal."
            );

    ret_val = pl_strip(str2, "a");
    assert_equal_str(
                "bbbccc",
                ret_val,
                "test_strip_with_chars",
                "Test 2: Strings are not equal."
            );

    ret_val = pl_strip(str3, "a");
    assert_equal_str(
                "bbbccc",
                ret_val,
                "test_strip_with_chars",
                "Test 3: Strings are not equal."
            );

    ret_val = pl_strip(str4, "ad");
    assert_equal_str(
                "bbbccc",
                ret_val,
                "test_strip_with_chars",
                "Test 4: Strings are not equal."
            );
}


void test_strip_empty_params() {
    char *ret_val;

    ret_val = pl_strip("", "a");
    assert_equal_str(
                NULL,
                ret_val,
                "test_strip_empty_params",
                "Test 1: NULL not returned."
            );

    ret_val = pl_strip("   asd   ", "");
    assert_equal_str(
                "asd",
                ret_val,
                "test_strip_empty_params",
                "Test 2: Strings not equal."
            );

    ret_val = pl_strip("   asd   ", NULL);
    assert_equal_str(
                "asd",
                ret_val,
                "test_strip_empty_params",
                "Test 3: Strings not equal."
            );

    ret_val = pl_strip(NULL, NULL);
    assert_equal_str(
                NULL,
                ret_val,
                "test_strip_empty_params",
                "Test 4: NULL not returned"
            );

    ret_val = pl_strip(NULL, "asd");
    assert_equal_str(
                NULL,
                ret_val,
                "test_strip_empty_params",
                "Test 5: NULL not returned."
            );
}


void test_translate() {
    char *ret_val;

    ret_val = pl_translate("read this short text", NULL, "aeiou");
    assert_equal_str(
                "rd ths shrt txt",
                ret_val,
                "test_translate",
                "Test 1: Strings are not equal."
            );

    ret_val = pl_translate("read this short text", "aeiou", "xxxxx");
    assert_equal_str(
                "rxxd thxs shxrt txxt",
                ret_val,
                "test_translate",
                "Test 2: Strings are not equal."
            );

    ret_val = pl_translate("read this short text", "zzzzz", "aeiou");
    assert_equal_str(
                "read this short text",
                ret_val,
                "test_translate",
                "Test 3: Strings are not equal"
            );
}


void test_translate_empty_params() {
    char *ret_val;

    ret_val = pl_translate("", "123", "321");
    assert_equal_pointers(
                NULL,
                ret_val,
                "test_translate_empty_params",
                "Test 1: NULL not returned."
            );

    ret_val = pl_translate(NULL, "123", "321");
    assert_equal_pointers(
                NULL,
                ret_val,
                "test_translate_empty_params",
                "Test 2: NULL not returned."
            );

    ret_val = pl_translate("spam eggs and ham", "", "321");
    assert_equal_pointers(
                NULL,
                ret_val,
                "test_translate_empty_params",
                "Test 3: NULL not returned."
            );

    ret_val = pl_translate("spam eggs and ham", "ega", "");
    assert_equal_pointers(
                NULL,
                ret_val,
                "test_translate_empty_params",
                "Test 4: NULL not returned."
            );

    ret_val = pl_translate("spam eggs and ham", "ega", NULL);
    assert_equal_pointers(
                NULL,
                ret_val,
                "test_translate_empty_params",
                "Test 5: NULL not returned."
            );

    ret_val = pl_translate("", "", "");
    assert_equal_pointers(
                NULL,
                ret_val,
                "test_translate_empty_params",
                "Test 6: NULL not returned."
            );

    ret_val = pl_translate(NULL, NULL, NULL);
    assert_equal_pointers(
                NULL,
                ret_val,
                "test_translate_empty_params",
                "Test 7: Null not returned."
            );
}


void test_splitlines() {
    char **ret_val = NULL;
    int size = 0;

    ret_val = pl_splitlines("asd\ndsa\n\rqwe", 0, &size);
    assert_equal_str(
                "asd",
                ret_val[0],
                "test_splitlines",
                "Test 1: Strings are not equal"
            );

    assert_equal_str(
                "dsa",
                ret_val[1],
                "test_splitlines",
                "Test 2: Strings are not equal."
            );

    assert_equal_str(
                "",
                ret_val[2],
                "test_splitlines",
                "Test 3: String is not empty."
            );

    assert_equal_str(
                "qwe",
                ret_val[3],
                "test_splitlines",
                "Test 4: Strings are not equal."
            );

    assert_equal_int(
                4,
                size,
                "test_splitlines",
                "Test 5: Size not right;"
            );
}



void test_splitlines_keepends() {
    char **ret_val = NULL;
    int size = 0;

    ret_val = pl_splitlines("asd\ndsa\n\rqwe", 1, &size);
    assert_equal_str(
                "asd\n",
                ret_val[0],
                "test_splitlines_keepends",
                "Test 1: Strings not equal."
            );

    assert_equal_str(
                "dsa\n",
                ret_val[1],
                "test_splitlines_keepends",
                "Test 2: Strings not equal."
            );

    assert_equal_str(
                "\r",
                ret_val[2],
                "test_splitlines_keepends",
                "Test 3: Strings not equal."
            );

    assert_equal_str(
                "qwe",
                ret_val[3],
                "test_splitlines_keepends",
                "Test 4: Strings not equal."
            );

    assert_equal_int(
                4,
                size,
                "test_splitlines_keepends",
                "Test 5: Size not right."
            );
}


void test_splitlines_empty_params() {
    char **ret_val;
    int size = 0;

    ret_val = pl_splitlines("", 0, &size);
    assert_equal_pointers(
                NULL,
                ret_val,
                "test_splitlines_empty_params",
                "Test 1: NULL not returned."
            );

    ret_val = pl_splitlines(NULL, 0, &size);
    assert_equal_pointers(
                NULL,
                ret_val,
                "test_splitlines_empty_params",
                "Test 2: NULL not returned."
            );

    ret_val = pl_splitlines("shortstringwithnolines", 0, &size);
    assert_equal_pointers(
                NULL,
                ret_val,
                "test_splitlines_empty_params",
                "Test 3: NULL not returned."
            );
}


void test_count() {
    int ret_val = 0;

    ret_val = pl_count("one one two three one two four", "one");
    assert_equal_int(
                3,
                ret_val,
                "test_count",
                "Test 1: Count not right"
            );

    ret_val = pl_count("one one two three one two four", "six");
    assert_equal_int(
                0,
                ret_val,
                "test_count",
                "Test 2: Count not right"
            );
}


void test_count_empty_params() {
    int ret_val = 0;

    ret_val = pl_count("", "One");
    assert_equal_int(
                -1,
                ret_val,
                "test_count_empty_params",
                "Test 1: Error code not returned."
            );

    ret_val = pl_count(NULL, "one");
    assert_equal_int(
                -1,
                ret_val,
                "test_count_empty_params",
                "Test 2: Error code not returned."
            );

    ret_val = pl_count("one three three seven", "");
    assert_equal_int(
                -1,
                ret_val,
                "test_count_empty_params",
                "Test 3: Error code not returned."
            );

    ret_val = pl_count("one three three seven", NULL);
    assert_equal_int(
                -1,
                ret_val,
                "test_count_empty_params",
                "Test 4: Error code not returned."
            );
}


void test_expandtabs() {
    char *ret_val;

    ret_val = pl_expandtabs("this is a\ttabbed\t\tstring", 4);
    assert_equal_str(
                "this is a   tabbed      string",
                ret_val,
                "test_expandtabs",
                "Test 1: Strings are not equal."
            );

    ret_val = pl_expandtabs("'\t'", 10);
    assert_equal_str(
                "'         '",
                ret_val,
                "test_expandtabs",
                "Test 2: Strings are not equal."
            );

    ret_val = pl_expandtabs("asd\tdsa", 0);
    assert_equal_str(
                "asddsa",
                ret_val,
                "test_expandtabs",
                "Test 3: Strings are not equal."
            );
}


void test_expandtabs_no_params() {
    char *ret_val;

    ret_val = pl_expandtabs("", 5);
    assert_equal_pointers(
                NULL,
                ret_val,
                "test_expandtabs_no_params",
                "Test 1: NULL not retruned."
            );

    ret_val = pl_expandtabs(NULL, 0);
    assert_equal_pointers(
                NULL,
                ret_val,
                "test_expandtabs_no_params",
                "Test 2: NULL not returned."
            );

    ret_val = pl_expandtabs("asd\t", -1);
    assert_equal_pointers(
                NULL,
                ret_val,
                "test_expandtabs_no_params",
                "Test 3: NULL not returned."
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
    test_slice_empty_string();
    test_strcpy_no_alloc();
    test_strcpy_pre_alloc();
    test_strcpy_not_empty();
    test_strcpy_empty_paras();
    test_strcat();
    test_strcat_empty_source();
    test_strcat_empty_destination();
    test_strcat_empty_params();
    test_split();
    test_split_multiple_delims();
    test_split_delim_not_found();
    test_split_empty_delim();
    test_split_single_delim();
    test_split_empty_paras();
    test_startswith();
    test_startwith_empy_string();
    test_startwith_empy_prefix();
    test_startwith_empty_params();
    test_endswith();
    test_endswith_empty_string();
    test_endswith_empty_postfix();
    test_endswith_empty_params();
    test_strip_no_chars();
    test_strip_with_chars();
    test_strip_empty_params();
    test_translate();
    test_translate_empty_params();
    test_splitlines();
    test_splitlines_keepends();
    test_splitlines_empty_params();
    test_count();
    test_count_empty_params();
    test_expandtabs();
    test_expandtabs_no_params();

    return 0;
}

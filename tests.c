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

        return;
    }

    if (x == NULL || y == NULL) {
        printf("%s[FAILED] %s: %s\n", KRED, func_name, msg);
        printf("%sGot: %s\n", KWHT, y);
        printf("Expected: %s\n", x);

        return;
    }

    else if (!strcmp(x, y)) {
        printf("%s[PASSED] %s: passed\n", KGRN, func_name);
    }

    else {
        printf("%s[FAILED] %s: %s\n", KRED, func_name, msg);
        printf("%sGot: %s\n", KWHT, y);
        printf("Expected: %s\n", x);
    }
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

    return 0;
}

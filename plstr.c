/*
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
/**
 * @file plstr.c
 *
 * \mainpage
 * \section  Introduction
 * This library implements some of Python's string methods to make working with
 * strings in C a little less annoying. Not all of the methods are implemented
 * at this stage, but more are in the process of being added. Working with
 * strings have generally been dangerous in C, buffer overflows and the likes
 * can happen easily if the programmer is not very careful.
 *
 * The goal of this library is to implement Python's string methods in a manner
 * that makes it hard for the users of the library to make security errors.
 * The allocation of buffers for the strings are in most of the functions done
 * internally, so the user does not need to allocate the memory needed. This
 * also means that the return values for most of the functions is pointers to
 * buffers allocated on the heap, and the user does then need to free this
 * memory after use.
 *
 * The project homepage is hosted at
 * <a href="https://github.com/DownGoat/plstr">GitHub</a>.
 *
 * \section Examples
 * At this time there is example code for how to use all the functions of the
 * library, see \ref plstr.c for the examples of how to use the different
 * functions.
 */

#include <string.h>
#include <stdlib.h>


/**
 * @brief This function is a wrapper around \a strcpy, it copies a string into a
 * buffer. If the \a destination argument is \b NULL a new buffer is allocated,
 * and if it is not \b NULL the string passed in \a source is copied into the
 * destination buffer.
 *
 * If the \a source parameter is \b NULL you will need to free the returned
 * buffer after use.
 *
 * @param source This is string you want to copy into the buffer.
 *
 * @param destination This is the optional \a destination buffer, if \b NULL is
 * passed a new buffer is allocated and the \a source is copied into it. If it
 * is not \b NULL the source will be copied into the destination buffer.
 *
 * @return If successful a pointer to buffer is returned. If the function fails
 * \b NULL is returned.
 *
 * \b Example
 *
 * \code{.c}
#include "plstr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main() {
    char the_string[] = "This is a short string.";
    char *copied_str, *pre_alloc;

    copied_str = pl_cpy(the_string, NULL);
    if (copied_str != NULL) {
        printf("copied_str: %s\n", copied_str);
    }

    pre_alloc = (char *) calloc(strlen(the_string) + 1, sizeof(char));
    if (pre_alloc == NULL) {
        return 0;
    }

    if(pl_cpy(the_string, pre_alloc) != NULL) {
        printf("pre_alloc: %s\n", pre_alloc);
    }

    free(copied_str);
    free(pre_alloc);

    return 0;
}
\endcode
 *
 * \b Output
\code{.unparsed}
copied_str: This is a short string.
pre_alloc: This is a short string.
\endcode
 */
char *pl_cpy(char *source, char *destination) {
    char *ret_val = NULL, *tmp = destination;

    if (source == NULL) {
        goto error_exit;
    }

    if (destination == NULL) {
        tmp = calloc(strlen(source) + 1, sizeof(char));
        if (tmp == NULL) {
            goto error_exit;
        }
    }

    ret_val = strcpy(tmp, source);

    return ret_val;

error_exit:
    free(tmp);
    return ret_val;
}


/**
 * @brief This function slices an string using an offset and a limit and returns a
 * substring. The original string is not manipulated in any way. The function
 * supports the use of both a negative offset and limit. When using a negative
 * value it is offsetted from the end of the string.
 *
 * You need to free the returned buffer after use.
 *
 * @param source The string you want to slice.
 *
 * @param offset The offset you want to slice from.
 *
 * @param limit The limit you want to slice too.
 *
 * @return The function returns a pointer to an allocated buffer containing the
 * substring if successful. In cases where the limit is smaller than the
 * offset, or if they are equal a \b NULL pointer is returned. The returned
 * \b NULL pointer can also be returned if the call to calloc fails, or if the
 * offset or limit is outside the range of the string.
 *
\b Example
\code{.c}
#include "plstr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main() {
    char the_string[] = "subdermatoglyphic";
    char *sliced;

    sliced = pl_slice(the_string, 3, 10);
    if (sliced != NULL) {
        printf("the_string: %s\nsliced 3, 10: %s\n", the_string, sliced);
        free(sliced);
    }

    sliced = pl_slice(the_string, 0, -10);
    if (sliced != NULL) {
        printf("sliced 0,-10: %s\n", sliced);
        free(sliced);
    }

    return 0;
}
\endcode
 *
 * \b Output
\code{.unparsed}
the_string: subdermatoglyphic
sliced 3, 10: dermato
sliced 0,-10: subderm
\endcode
 */
char *pl_slice(char *source, int offset, int limit) {
    char *ret_val = NULL, *tmp = NULL;
    int i, tmp_count = 0, new_limit, new_offset;
    int source_length = 0;


    if (source == NULL) {
        goto error_exit;
    }

    source_length = strlen(source);

    if (source_length == 0) {
        goto error_exit;
    }

    // Get the right limit if limit is negative
    if (limit < 0) {
        new_limit = source_length + limit;
    }

    else {
        new_limit = limit;
    }

    // Same shit but with the offset.
    if (offset < 0) {
        new_offset = source_length + offset;
    }

    else {
        new_offset = offset;
    }

    /*
     * Exit if limit is less than offset or if they are equal.
     * If the limit is less than the offset there is no characters to slice,
     * same if they are equal.
     */
    if (new_limit < new_offset || (new_limit - new_offset) == 0) {
        goto error_exit;
    }

    /*
     * Exit if limit or offset points to somewhere outside of the string.
     */
    if (new_limit > source_length || new_offset > source_length) {
        goto error_exit;
    }

    tmp = (char *) calloc((new_limit - new_offset) + 1, sizeof(char));
    if (tmp == NULL) {
        goto error_exit;
    }

    for(i = new_offset; i < new_limit; i++) {
        tmp[tmp_count] = source[i];
        tmp_count++;
    }

    ret_val = tmp;

    return ret_val;

error_exit:
    free(tmp);

    return ret_val;
}


/**
 * @brief This function creates a new buffer that is the size of the two
 * arguments, and both arguments are copied into it. The resulting string
 * is \a destination+source.
 *
 * You need to free the returned buffer after use.
 *
 * @param destination The first string you want to concatenation to.
 *
 * @param source The string you want concatenation destination with.
 *
 * @return Returns a pointer to the new buffer of the concatenation strings.
 * If the function fails \b NULL is returned.
 *
 * \b Example
\code{.c}
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
\endcode
 *
 * \b Output
\code
foo:foo
bar:bar
concatenated:foobar
\endcode
 */
char *pl_cat(char *destination, char *source) {
    char *ret_val = NULL, *tmp = NULL;

    if (destination == NULL || source == NULL) {
        goto error_exit;
    }

    tmp = (char *) calloc(strlen(source) + strlen(destination) + 1, sizeof(char));
    if (tmp == NULL) {
        goto error_exit;
    }

    tmp = pl_cpy(destination, tmp);
    if (tmp == NULL) {
        goto error_exit;
    }

    strcat(tmp, source);

    ret_val = tmp;

    return ret_val;

error_exit:
    free(tmp);

    return ret_val;
}


/**
 * @brief This function splits the string using a delimiter, and puts the
 * results in array of strings. The function differs from the behaviour of
 * strtok, if the delimiter is longer than a single character it splits the
 * string where where the delimiter is found. If the string is
 * \a "fooasdbarasdmagic" and the delimiter is \a "asd" the result will be
 * ["foo", "bar", "magic"]. If the delimiter is not found, or it is empty string
 * NULL is returned instead. The original string is not modified.
 *
 * You need to free the returned buffer after use, remember to iterate over the
 * returned buffer and free every element in it before you free the returned
 * buffer.
 *
 * @param string The string you want to split up.
 *
 * @param delim The delimiter you want to use.
 *
 * @param size This will be set to the size of the returned array.
 *
 * @return Returns an array of strings with the different sub strings if
 * successful. The size argument is set to the size of the returned array.
 * If the function fails \b NULL is returned.
 *
\b Example
\code{.c}
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
\endcode
 *
 * \b Output
\code{.unparsed}
The size is 5
splitted[0] = This
splitted[1] = is
splitted[2] = a
splitted[3] = short
splitted[4] = string.
\endcode
 */
char **pl_split(char *string, char *delim, int *size) {
    char **ret_val = NULL, **tmp = NULL, *pch = string, *offset = string;
    int i = 0, delims = 0;
    int delim_length = 0;

    if (string == NULL || strlen(string) == 0) {
        return NULL;
    }

    if (delim == NULL) {
        return NULL;
    }

    delim_length = strlen(delim);

    if (delim_length == 0) {
        return NULL;
    }

    if (size == NULL) {
        return NULL;
    }

    // Count the number of occurences of the sub str.
    do {
        pch = strstr(pch, delim);

        if (pch != NULL) {
            delims++;

            pch = pch + delim_length;
        }
    } while (pch != NULL);

    if (delims == 0) {
        return NULL;
    }

    tmp = (char **) calloc(delims + 1, sizeof(char *));
    if (tmp == NULL) {
        goto error_exit;
    }

    pch = string;
    offset = string;

    for (i = 0; i < delims; i++) {
        char *sub_str = NULL;

        pch = strstr(pch, delim);

        sub_str = (char *) calloc((pch - offset) + 1, sizeof(char));
        if (sub_str == NULL) {
            goto error_exit;
        }

        strncpy(sub_str, offset, pch - offset);

        pch += delim_length;
        offset = pch;
        tmp[i] = sub_str;
    }

    pch = (char *) calloc(strlen(offset) + 1, sizeof(char));
    if (pch == NULL) {
        goto error_exit;
    }

    strncpy(pch, offset, strlen(offset));

    tmp[i] = pch;
    ret_val = tmp;

    *size = i + 1;

    return ret_val;

error_exit:
    for(i = 0; i < (delims + 1); i++) {
        free(tmp[i]);
    }

    free(tmp);

    return ret_val;
}


/**
 * @brief Checks if the string starts with a prefix. Does not modify the string.
 *
 * @param string The string you want to check.
 *
 * @param prefix The substring you want to check if the string starts with.
 *
 * @return Returns \b 1 if the string starts with the prefix, returns \b 0 if it
 * does not. If the function fails \b -1 is returned.
 *
\code{.c}
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
\endcode
 *
 * \b Output
\code{.unparsed}
the_string, does indeed start with http://
And it does not start with ftp://
\endcode
 */
int pl_startswith(char *string, char *prefix) {
    char *tmp = NULL;
    int ret_val = -1;
    int prefix_length = 0;

    if (string == NULL || prefix == NULL) {
        goto error_exit;
    }

    if (strlen(string) == 0) {
        goto error_exit;
    }

    prefix_length = strlen(prefix);

    if (prefix_length == 0) {
        goto error_exit;
    }

    tmp = (char *) calloc(prefix_length + 1, sizeof(char));
    if (tmp == NULL) {
        goto error_exit;
    }

    tmp = strncpy(tmp, string, prefix_length);

    if (!strcmp(tmp, prefix)) {
        ret_val = 1;
    }

    else {
        ret_val = 0;
    }

    free(tmp);

    return ret_val;

error_exit:
    free(tmp);

    return ret_val;
}


/**
 * @brief Checks if a strings ends with a postfix. The function does not alter
 * the string.
 *
 * @param string The string you want to check if ends with the postfix.
 *
 * @param postfix The substring you want to check if the string ends with.
 *
 * @return The function returns \b 1 if the string ends with the postfix. If the
 * string does not end with the postfix \b 0 is returned. If the function fails
 * \b -1 is returned.
 *
 * \b Example
\code{.c}
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
\endcode
 *
 * \b Output
\code{.unparsed}
the_string, does indeed end with .com
And it does not end with .net
\endcode
 */
int pl_endswith(char *string, char *postfix) {
    char *pch = NULL;
    int ret_val = -1;
    int string_length = 0, postfix_length = 0;

    if (string == NULL || postfix == NULL) {
        goto error_exit;
    }

    string_length = strlen(string);
    postfix_length = strlen(postfix);

    if (string_length == 0 || postfix_length == 0) {
        goto error_exit;
    }

    pch = string + string_length - postfix_length;

    if (!strcmp(pch, postfix)) {
        ret_val = 1;
    }

    else {
        ret_val = 0;
    }

    return ret_val;

error_exit:

    return ret_val;
}


/**
 * @brief This function handls the logic for the pl_split function for the cases
 * where the chars parameter is empty.
 */
static char *strip_empty_chars(char *string) {
    char *offset = NULL, *limit = NULL, *ret_val;
    int i;
    int string_length = 0;

    if (string == NULL)  {
        goto error_exit;
    }

    string_length = strlen(string);

    if (string_length == 0) {
        goto error_exit;
    }

    for (i = 0; i < string_length; i++) {
        switch ((int) string[i]) {
            case 9:
                break;
            case 10:
                break;
            case 11:
                break;
            case 12:
                break;
            case 13:
                break;
            case 32:
                break;
            default:
                offset = string + i;
                i = string_length;
                break;
        }
    }

    // Cannot start comparison at the null terminator.
    for (i = string_length - 1; i >= 0; i--) {
        switch ((int) string[i]) {
            case 9:
                break;
            case 10:
                break;
            case 11:
                break;
            case 12:
                break;
            case 13:
                break;
            case 32:
                break;
            default:
                limit = string + i;
                i = 0;
                break;
        }
    }

    ret_val = (char *) calloc(limit - offset + 2, sizeof(char));
    if (ret_val == NULL) {
        goto error_exit;
    }

    strncpy(ret_val, offset, limit - offset + 1);

    return ret_val;

error_exit:
    free (ret_val);

    return ret_val;
}


/**
 * @brief This is function handels logic for the pl_strip function when the
 * chars parameter is not empty.
 */
static char *strip_with_chars(char *string, char *chars) {
    char *ret_val = NULL, *offset = NULL, *limit = NULL;
    int i, x, found = 0;
    int string_length = 0, chars_length = 0;

    if (string == NULL || chars == NULL) {
        goto error_exit;
    }

    string_length = strlen(string);
    chars_length = strlen(chars);

    if (string_length == 0 || chars_length == 0) {
        goto error_exit;
    }

    for (i = 0; i < string_length; i++) {
        for (x = 0; x < chars_length; x++) {
            if (string[i] == chars[x]) {
                found = 1;
            }
        }

        if (!found) {
            offset = string + i;
            break;
        }

        found = 0;
    }

    found = 0;
    // Cant start comparing the null terminator as it is not striped.
    for (i = string_length - 1; i >= 0; i--) {
        for (x = 0; x < chars_length; x++) {
            if (string[i] == chars[x]) {
                found = 1;
            }
        }

        if (!found) {
            limit = string + i;
            break;
        }

        found = 0;
    }

    ret_val = (char *) calloc(limit - offset + 2, sizeof(char));
    if (ret_val == NULL) {
        goto error_exit;
    }

    strncpy(ret_val, offset, limit - offset + 1);

    return ret_val;

error_exit:
    return ret_val;
}


/**
 * @brief This function strips characters from a string. The default behaviour
 * if chars is \b NULL or empty string is to strip whitespace characters from
 * either end of the string. The characters removed is '\\n \\r \\t \\v \\f' and
 * the space character. If the chars parameter contains characters those
 * characters are removed from either side of the string.
 *
 * You need to free the returned buffer after use.
 *
 * @param string The string you want to split.
 *
 * @param chars The characters you want to strip from the string. If the
 * parameter is empty or \b NULL whitespace is removed from either side.
 *
 * @return The function returns a pointer to the string with the characters
 * removed. If the function fails \b NULL is returned.
 *
 * \b Example
\code{.c}
#include "plstr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main() {
    char whitespace[] = "   lots of space   ";
    char cool_nick[] = "xxxxTheDude99xxxx";
    char *ret_val = NULL;

    ret_val = pl_strip(whitespace, NULL);
    if (ret_val != NULL) {
        printf("Original: %s\nStriped: %s\n", whitespace, ret_val);
        free(ret_val);
    }

    ret_val = pl_strip(cool_nick, "x");
    if (ret_val != NULL) {
        printf("Striped: %s\n", ret_val);
        free(ret_val);
    }


    return 0;
}
\endcode
 *
 * \b Output
\code{.unparsed}
Original:    lots of space
Striped: lots of space
Striped: TheDude99
\endcode
 */
char *pl_strip(char *string, char *chars) {
    if (string == NULL || strlen(string) == 0) {
        return NULL;
    }

    if (chars == NULL || strlen(chars) == 0) {
        return strip_empty_chars(string);
    }

    return strip_with_chars(string, chars);
}


/**
 * @brief This function handels the cases for pl_translate where there is no
 * table. It should not be called directly, call pl_translate with the table
 * parameter set as NULL instead.
 */
static char *translate_no_table(char *string, char *deletechars) {
    char *tmp = NULL, *ret_val = NULL;
    int i, x, found = 0, idx = 0;
    int string_length = 0, deletechars_length = 0;

    if (string == NULL || deletechars == NULL) {
        goto error_exit;
    }

    string_length = strlen(string);
    deletechars_length = strlen(deletechars);

    if (string_length == 0 || deletechars_length == 0) {
        goto error_exit;
    }

   for (i = 0; i < string_length; i++) {
        for (x = 0; x < deletechars_length; x++) {
            if (string[i] == deletechars[x]) {
                found++;
            }
        }
    }

    tmp = (char *) calloc(string_length - found + 1, sizeof(char));
    if (tmp == NULL) {
        goto error_exit;
    }


    for (i = 0; i < string_length; i++) {
        int delete = 0;

        for (x = 0; x < deletechars_length; x++) {
            if (string[i] == deletechars[x]) {
                delete = 1;
            }
        }

        if (!delete) {
            tmp[idx] = string[i];
            idx++;
        }
    }

    ret_val = tmp;

    return ret_val;

error_exit:
    free(tmp);

    return ret_val;
}


/**
 * @brief This function handels the logic for the pl_translate function in the
 * cases where the table parameter is not empty. Do not call this function
 * directly, call pl_translate instead.
 */
static char *translate_with_table(char *string, char *table, char *deletechars) {
    char *tmp = NULL, *ret_val = NULL;
    char swap_table[256];
    int i;

    if (string == NULL || table == NULL || deletechars == NULL) {
        goto error_exit;
    }

    if (strlen(string) == 0 || strlen(table) == 0 || strlen(deletechars) == 0) {
        goto error_exit;
    }

    if (strlen(table) != strlen(deletechars)) {
        goto error_exit;
    }

    for (i = 0; i < 256; i++) {
        swap_table[i] = i;
    }

    for (i = 0; i < strlen(table); i++) {
        swap_table[(int) table[i]] = deletechars[i];
    }

    tmp = pl_cpy(string, NULL);
    if (tmp == NULL) {
        goto error_exit;
    }

    for (i = 0; i < strlen(tmp); i++) {
        tmp[i] = swap_table[(int) tmp[i]];
    }

    ret_val = tmp;

    return ret_val;

error_exit:
    free(tmp);

    return ret_val;
}


/**
 * @brief This function implements the behaviour of python string translate
 * method. This function has two different behaviours. It either deletes
 * characters from the string, or switches out the characters with the ones
 * set in the table.
 *
 * If the table parameter, is \b NULL the characters passed in the deletechars
 * parameter are removed from the string. If you pass it the string
 * <a> 'read this short text'</a>, and deletechars is \a 'aeiou', the returned
 * string is <a>'rd ths shrt txt'</a>.
 *
 * If the table parameter is not empty, every occurrence of one of the table
 * characters is replaced with the character in deletechars at the same index.
 * So if the same string is passed <a>'read this short text'</a>, and the table
 * is \a 'xxxxx', and deletechars is \a 'aeiou' the returned string is
 * <a>'rxxd thxs shxrt txxt'</a>. The length of the table and deletechars needs
 * to be of the same length, or else the function will return \b NULL.
 *
 * You need to free the returned buffer after use.
 *
 * @param string The string you want to translate.
 *
 * @param table Optional parameter, if set it is used to swap the characters
 * passed in the deletechars parameter. If not every occurrence of the characters
 * passed in deletechars is removed from the string.
 *
 * @param deletechars The characters in the parameter is removed or swapped out
 * from the string. If the table parameter is used this parameter and table
 * needs to be of equal size.
 *
 * @return Returns a pointer to the new string with the deleted/swapped out
 * characters. If the function fails \b NULL is returned.
 *
 * \b Example
\code{.c}
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
\endcode
 *
 * \b Output
\code{.unparsed}
the_string: read this short text
no table:rd ths shrt txt
with table: rxxd thxs shxrt txxt
\endcode
 */
char *pl_translate(char *string, char *table, char *deletechars) {
    if (string == NULL || deletechars == NULL) {
        return NULL;
    }

    if (strlen(string) == 0 || strlen(deletechars) == 0) {
        return NULL;
    }

    if (table != NULL && strlen(table) == 0) {
        return NULL;
    }

    if (table == NULL) {
        return translate_no_table(string, deletechars);
    }

    return translate_with_table(string, table, deletechars);
}


/**
 * @brief This function splits up a string when a newline character is found.
 *
 * The function is very similar to the split function but the results are a bit
 * different. It splits up a string whenever it finds the \a \\n or \a \\r character.
 * If the keepends parameter is set the function will not remove the newline
 * from the string.
 *
 * If the string <a>"first\nsecond\n\nfourth"</a> is passed the results will be:
 * <a>["first", "second", "", "fourth"] </a>. If the keepends parameter is set
 * the results of the same string would be:
 * <a>["first\n", "second\n", "\n", "fourth\n"]</a>
 *
 * This function allocates memory, the returned pointer should be freed after
 * use.
 *
 * @param the_string The string you want to split.
 *
 * @param keepends If set to \a 1 the newlines will be appended, if not the
 * will be removed.
 *
 * @param size This parameter will be changed to the size of the returned
 * string array.
 *
 * @return The function will return a array of strings if successful, if the
 * function fails \b NULL will be returned.
 *
 * \b Example
\code{.c}
#include "plstr.h"
#include <stdio.h>
#include <stdlib.h>


int main() {
    char **ret_val = NULL;
    int size = 0;

    ret_val = pl_splitlines("first\nsecond\n\nfourth", 0, &size);
    if (ret_val != NULL) {
        int i;

        for (i = 0; i < size; i++) {
            printf("%d: %s ", i, ret_val[i]);
            free(ret_val[i]);
        }

        free(ret_val);
        printf("\n\n");
    }

    ret_val = pl_splitlines("first\nsecond\n\nfourth", 1, &size);
    if (ret_val != NULL ) {
        int i;

        for (i = 0; i < size; i++) {
            printf("%d: %s\n", i, ret_val[i]);
            free(ret_val[i]);
        }

        printf("\n");
        free(ret_val);
    }

    return 0;
}
\endcode
 *
 * \b Output
\code{.unparsed}
0: first 1: second 2:  3: fourth

0: first

1: second

2:

3: fourth

\endcode
 */
char **pl_splitlines(char *the_string, int keepends, int *size) {
    char **ret_val = NULL, *pch = NULL, *tmp = NULL;
    int delims = 0, i, idx = 0;
    int string_length = 0;

    if (the_string == NULL) {
        goto error_exit;
    }

    string_length = strlen(the_string);

    if (string_length == 0) {
        goto error_exit;
    }

    // Count the number of lines.
    for (i = 0; i < string_length; i++) {
        // 10 and 13 is the int value of \n and \r
        if ((int) the_string[i] == 10 || (int) the_string[i] == 13) {
            delims++;
        }
    }

    // Nothing todo.
    if (delims == 0) {
        goto error_exit;
    }

    ret_val = (char **) calloc(delims + 1, sizeof(char *));
    if (ret_val == NULL) {
        goto error_exit;
    }


    pch = the_string;
    for (i = 0; i < string_length; i++) {
        if ((int) the_string[i] == 10 || (int) the_string[i] == 13) {
            int len = (the_string + i) - pch;

            if (keepends) {
                len++;
            }

            tmp = (char *) calloc(len + 1, sizeof(char));
            if (tmp == NULL) {
                goto error_exit;
            }

            strncpy(tmp, pch, len);
            pch = the_string + i + 1;
            ret_val[idx] = tmp;
            idx++;
        }
    }

    tmp = (char *) calloc(strlen(pch) + 1, sizeof(char));
    if (tmp == NULL) {
        goto error_exit;
    }

    tmp = strncpy(tmp, pch, strlen(pch));
    ret_val[delims] = tmp;

    *size = delims + 1;

    return ret_val;

error_exit:
    if (delims) {
        for (i = 0; i < delims; i++) {
            free(ret_val[i]);
        }
    }

    free(ret_val);
    free(tmp);

    return ret_val;
}


/**
 * @brief Counts the number of occurences of a word in a string. The word can
 * does not need to be a word in the english langauge sense, just any form of
 * sub string can be counted.
 *
 * @param the_string The string you wnat to search.
 *
 * @param word The sub string you want to search the string for.
 *
 * @return Returns the number of occurences of the sub string. If the function
 * fails \b -1 is returned.
 *
 * \b Example
\code{.c}
#include "plstr.h"
#include <stdio.h>
#include <stdlib.h>


int main() {
    int ret_val = 0;

    ret_val = pl_count("one three three seven", "three");
    if (ret_val != -1) {
        printf("The word 'three' was found %d times.\n", ret_val);
    }

    ret_val = pl_count("What do you get if you multiply six by nine?", "42");
    if (ret_val == 0) {
        printf("Apparantly not 42. I always knew something was wrong with the universe.\n");
    }

    return 0;
}
\endcode
 * \b Output
\code{.unparsed}
The word 'three' was found 2 times.
Apparantly not 42. I always knew something was wrong with the universe.
\endcode
 */
int pl_count(char * the_string, char *word) {
    char *pch = the_string;
    int count = 0;

    if (the_string == NULL || word == NULL) {
        return -1;
    }

    if (strlen(the_string) == 0 || strlen(word) == 0) {
        return -1;
    }

    do {
        pch = strstr(pch, word);
        if (pch != NULL) {
            pch += strlen(word);
            count++;
        }
    } while(pch != NULL);

    return count;
}


/**
 * @brief This function replaces tabs with spaces.
 *
 * The tabs are replaced with the number of spaces in specified in the
 * \a tabsize parameter. There is no default value for the tabsize, so a value
 * needs to be passed. The resulting string is put in a heap allocated buffer,
 * the buffer should be freed after use.
 *
 * @param the_string The string with tabs.
 *
 * @param tabsize The number of spaces you want to replace each tab with.
 *
 * @return The function returns a heap allocated buffer of the string with tabs
 * replaced. If the function fails \b NULL is returned.
 *
 * \b Example
\code{.c}
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

\endcode
 *
 * \b Output
\code{.unparsed}
tabs    instead of      space
tabs instead of space
\endcode
 */

char *pl_expandtabs(char *the_string, int tabsize) {
    char *tmp = NULL, *ret_val = NULL;
    int i, tabcount = 0, str_size, idx;
    int string_length = 0;

    if (the_string == NULL || tabsize < 0) {
        goto error_exit;
    }

    string_length = strlen(the_string);

    if (string_length == 0) {
        goto error_exit;
    }

    for (i = 0; i < string_length; i++) {
        // 9 is tab int value.
        if (the_string[i] == 9) {
            tabcount++;
        }
    }

    if (!tabcount) {
        goto error_exit;
    }

    str_size = (tabcount * tabsize) + string_length + 1;
    tmp = (char *) calloc(str_size, sizeof(char));
    if (tmp == NULL) {
        goto error_exit;
    }

    idx = 0;
    for (i = 0; i < string_length; i++) {
        if (the_string[i] == 9) {
            int x;
            for (x = 0; x < tabsize; x++) {
                tmp[idx] = ' ';
                idx++;
            }
        }

        else {
            tmp[idx] = the_string[i];
            idx++;
        }
    }

    ret_val = tmp;

    return ret_val;

error_exit:
    free(tmp);

    return ret_val;
}

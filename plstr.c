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
/**
 * @file plstr.c
 *
 * @brief asd
 *
 * @author Sindre Smistad <sindre@downgoat.net>
 *
 * @date 2014.10.25
 */

#include <string.h>
#include <stdlib.h>


/**
 * @brief This function is a wrapper around strcpy, it copies a string into a
 * buffer. If the destionation argument is NULL a new buffer is allocated,
 * and if it is not NULL the string passed in source is copyed into the
 * destionation buffer.
 *
 * @param source This is string you want to copy into the buffer.
 *
 * @param destionation This is the optional destination buffer, if NULL is
 * passed a new buffer is allocated and the source is copied into it. If it
 * is not NULL the source will be copied into the destionation buffer.
 *
 * @return If successful a pointer to buffer is returned. If the function fails
 * NULL is returned.
 */
char *pl_cpy(char *source, char *destination) {
    char *ret_val = NULL, *tmp = destination;

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
 * @brief This function slices a string using a offset and a limit and returns a
 * substring. The original string is not manipulated in any way. The function
 * supports the use of both a negative offset and limit. When using a negative
 * value it is offseted from the end of the string.
 *
 * @param source The string you want to slice.
 *
 * @param offset The offset you want to slice from.
 *
 * @param limit The limit you want to slice too.
 *
 * @return The function returns a pointer to a allocated buffer containing the
 * substring if successful. In cases where the limit is smaller than the
 * offset, or if they are equal a NULL pointer is returned. The returned
 * NULL pointer can also be returned if the call to calloc fails, or if the
 * offset or limit is outside the range of the string.
 */
char *pl_slice(char *source, int offset, int limit) {
    char *ret_val = NULL, *tmp = NULL;
    int i, tmp_count = 0, new_limit, new_offset;

    // Get the right limit if limit is negative
    if (limit < 0) {
        new_limit = strlen(source) + limit;
    }

    else {
        new_limit = limit;
    }

    // Same shit but with the offset.
    if (offset < 0) {
        new_offset = strlen(source) + offset;
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
    if (new_limit > strlen(source) || new_offset > strlen(source)) {
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
 * is destination+source.
 *
 * @param destination The first string you want to concatenation to.
 *
 * @param source The string you want concatenation destination with.
 *
 * @return Returns a pointer to the new buffer of the concatenation strings.
 * If the function fails NULL is returned.
 */
char *pl_cat(char *destination, char *source) {
    char *ret_val = NULL, *tmp = NULL;

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
 * @breif This function splits the string using a delimiter, and puts the
 * results in array of strings. The function differs from the behaviour of
 * strtok, if the delimiter is longer than a single character it splits the
 * string where where the delimeter is found. If the string is
 * "fooasdbarasdmagic" and the delimiter is "asd" the result will be
 * ["foo", "bar", "magic"]. If the delimter is not found, or it is empty string
 * NULL is returned instead. The orignal string is not modified.
 *
 * @param string The string you want to split up.
 *
 * @param delim The delimiter you want to use.
 *
 * @param size This will be set to the size of the returned array.
 *
 * @return Returns a array of strings whith the different sub strings if
 * successful. The size argument is set to the size of the returned array.
 * If the function fails NULL is returned.
 */
char **pl_split(char *string, char *delim, int *size) {
    char **ret_val = NULL, **tmp = NULL, *pch = string, *offset = string;
    int i = 0, delims = 0;

    if (strlen(delim) == 0) {
        return NULL;
    }

    // Count the number of occurences of the sub str.
    do {
        pch = strstr(pch, delim);

        if (pch != NULL) {
            delims++;

            pch = pch + strlen(delim);
        }
    } while (pch != NULL);

    if (delims == 0) {
        return NULL;
    }

    tmp = (char **) calloc(delims + 1, sizeof(char));
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

        pch += strlen(delim);
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
 * @return Returns 1 if the string starts with the prefix, returns 0 if it does
 * not. If the function fails -1 is returned.
 */
int pl_startswith(char *string, char *prefix) {
    char *tmp = NULL;
    int ret_val = -1;

    if (strlen(string) == 0 || strlen(prefix) == 0) {
        goto error_exit;
    }

    tmp = (char *) calloc(strlen(prefix) + 1, sizeof(char));
    if (tmp == NULL) {
        goto error_exit;
    }

    tmp = strncpy(tmp, string, strlen(prefix));

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
 * @return The function returns 1 if the string ends with the postfix. If the
 * string does not end with the postfix 0 is returned. If the function fails
 * -1 is returned.
 */
int pl_endswith(char *string, char *postfix) {
    char *pch = NULL;
    int ret_val = -1;

    if (strlen(string) == 0 || strlen(postfix) == 0) {
        goto error_exit;
    }

    pch = string + strlen(string) - strlen(postfix);

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

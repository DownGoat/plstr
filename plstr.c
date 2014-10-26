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
    char *ret_val = NULL, *tmp;

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

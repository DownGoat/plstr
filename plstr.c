#include <string.h>
#include <stdlib.h>


/**
 *  @brief This is a wrapper around the standard strcpy function, it checks if
 *  the size of source string is less or equal to the destination string.
 *
 *  @param destination This is a pointer to the buffer the string will be copied
 *  into.
 *
 *  @param source This is the pointer to the buffer containing the string you
 *  want to copy.
 *
 *  @param dest_size This is the size of the destination buffer.
 *
 *  @return This function returns a NULL pointer if the source string is longer
 *  then the destination buffer.
 */
char *pl_strcpy(char *destination, char *source, size_t dest_size) {
    char *ret_val = NULL;

    if (dest_size >= strlen(source)) {
        ret_val = strcpy(destination, source);
    }

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

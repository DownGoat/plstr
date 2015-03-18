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

#ifndef PLSTR_H
#define PLSTR_H

#include <stdlib.h>


/*****************************************************************
 *                  FUNCTION DEFINITIONS                         *
 *****************************************************************/


char    *pl_cpy(char *, char *);
char    *pl_slice(char *, int, int);
char    *pl_cat(char *, char *);
char    **pl_split(char *, char *, int *);
int     pl_startswith(char *, char *);
int     pl_endswith(char *, char *);
char    *pl_strip(char *, char *);
char    *pl_translate(char *, char *, char *);
char    **pl_splitlines(char *, int , int *);
int     pl_count(char *, char *);
char    *pl_expandtabs(char *, int);

#endif /* PLSTR_H */

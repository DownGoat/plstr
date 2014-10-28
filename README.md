Introduction
============
This library implements some of Python's string methods to make working with
strings in C a little less annoying. Not all of the methods are implemented at
this stage, but more are in the process of being added. Working with strings
have generally been dangerous in C, buffer overflows and the likes can happen
easily if the programmer is not very careful.

The goal of this library is to implement Python's string methods in a manner
that makes it hard for the users of the library to make security errors. The
allocation of buffers for the strings are in most of the functions done
internally, so the user does not need to allocate the memory needed. This also
means that the return values for most of the functions is pointers to buffers
allocated on the heap, and the user does then need to free this memory after
use.

The project homepage is hosted at GitHub.

Examples
========
At this time there is example code for how to use all the functions of the
library, see documentation for plstr.c or the examples directory for the 
examples of how to use the different functions.



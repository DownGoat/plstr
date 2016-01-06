#!/bin/sh
make clean
make
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes --log-file="logfile.out" -v ./plstr
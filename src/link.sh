#!/bin/bash
gcc -Wl,--wrap,getchar -m32 $1 runtime.c

#!/bin/sh

LD_PRELOAD=./libmalloc.so ls
LD_PRELOAD=./libmalloc.so ip a
LD_PRELOAD=./libmalloc.so cat Makefile
LD_PRELOAD=./libmalloc.so factor 10 20 30 40

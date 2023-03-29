CC = gcc
CPPFLAGS = -D_DEFAULT_SOURCE
CFLAGS = -Wall -Wextra -Werror -std=c99 -fPIC -fno-builtin -Wvla
LDFLAGS = -shared
VPATH = src

TARGET_LIB = libmalloc.so
OBJS = malloc.o

#new
OBJS2 = my_malloc.o

all: library

library: $(TARGET_LIB)
$(TARGET_LIB): CFLAGS += -pedantic -fvisibility=hidden
$(TARGET_LIB): LDFLAGS += -Wl,--no-undefined
$(TARGET_LIB): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

#new
test: $(OBJS2) tests/tests.o
	$(CC) -lcriterion $^ -o $@
check: library
	./tests/tests.sh

debug: CFLAGS += -g
debug: clean $(TARGET_LIB)

clean:
	$(RM) $(TARGET_LIB) $(OBJS)

.PHONY: all $(TARGET_LIB) clean

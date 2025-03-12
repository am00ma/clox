.PHONY: all test init clean objs vars

CC       = clang
CFLAGS   = -Wall -O0 -g -std=c23

LIBS     = -lm
INCLUDES = -Iinclude

HDRS = $(wildcard include/*.h)
SRCS = $(wildcard src/*.c)

TESTS = $(wildcard tests/*.c)
TESTBINS = $(patsubst tests/%.c, build/tests/%, $(TESTS))

all: test

init:
	mkdir -p src
	mkdir -p include
	mkdir -p tests
	mkdir -p build/obj
	mkdir -p build/tests

test: $(TESTBINS)

clean:
	$(RM) -r ./build

objs: $(OBJS)

OBJS    = $(patsubst src/%.c, build/obj/%.o, $(SRCS))
.PRECIOUS: $(OBJS)

build/obj/%.o: src/%.c include/%.h
	$(CC) $(CFLAGS) -c $< $(INCLUDES) -o $@

build/tests/%: tests/%.c $(OBJS)
	$(CC) $(CFLAGS) $< $(OBJS) $(LIBS) $(INCLUDES) -o $@

vars:
	@echo "SRCS      : " $(SRCS)
	@echo "HDRS      : " $(HDRS)
	@echo "OBJS      : " $(OBJS)
	@echo "LIBS      : " $(LIBS)
	@echo "INCLUDES  : " $(INCLUDES)
	@echo "TESTS     : " $(TESTS)
	@echo "TESTBINS  : " $(TESTBINS)

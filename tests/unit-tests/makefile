CC=gcc
CXX=g++

COMMONS=../../src
COMMONS_SRCS=$(shell find $(COMMONS)/commons -iname "*.c" -or -iname "*.h")
COMMONS_BIN=$(COMMONS)/build
COMMONS_SO=$(COMMONS_BIN)/libcommons.so

DOCTEST=lib/doctest.h

# main.cpp must go first
SRCS=$(shell find tests -iname "main.cpp") $(shell find tests -iname "test_*.cpp") $(shell find tests -iname "*.c")
DEPS=$(shell find tests -iname "*.h")
OBJS=$(SRCS:%=build/%.o)
DIRS=$(sort $(dir $(OBJS)))

BIN=build/commons-unit-test

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) -L"$(COMMONS_BIN)" -Wl,-rpath,"$(COMMONS_BIN)" -o "$@" $(OBJS) -lcommons

build/%.cpp.o: %.cpp $(DEPS) $(COMMONS_SO) | $(DIRS) lib/doctest.h
	$(CXX) -I"$(COMMONS)" -Ilib -c -Wno-write-strings -o "$@" "$<"

build/%.c.o: %.c $(DEPS) $(COMMONS_SO) | $(DIRS) lib/doctest.h
	$(CC) -I"$(COMMONS)" -Ilib -c -o "$@" "$<"

lib/doctest.h: | lib
	wget -qO- https://raw.githubusercontent.com/doctest/doctest/v2.4.9/doctest/doctest.h | tee $@ > /dev/null

$(DIRS) lib:
	mkdir -p $@

$(COMMONS_SO): $(COMMONS_SRCS)
	$(MAKE) -C $(COMMONS) all

debug: CC += -DDEBUG -g
debug: CXX += -DDEBUG -g
debug: all

clean:
	rm -rf build

test: all
	./$(BIN)

valgrind: debug
	valgrind --error-exitcode=42 --leak-check=full -v ./$(BIN)

.PHONY: all debug clean test valgrind

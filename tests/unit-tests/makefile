RM=rm -rf
CPP=g++
CC=gcc

BASE_DIR=../..

DOCTEST=/usr/local/include/doctest.h

COMMONS=$(BASE_DIR)/src
COMMONS_BIN=$(COMMONS)/build
COMMONS_SO=$(COMMONS_BIN)/libcommons.so
COMMONS_SRCS=$(patsubst ./%,$(COMMONS)/%,$(shell make --no-print-directory -C $(COMMONS) sources))

C_SRCS=$(shell find src -iname "*.c")
CPP_SRCS=$(shell find tests -iname "*.cpp")
OBJS=$(C_SRCS:src/%.c=build/%.o) $(CPP_SRCS:tests/%.cpp=build/%.o)

BIN_DIR=build
BIN=$(BIN_DIR)/commons-unit-test

all: $(BIN)

$(BIN_DIR):
	mkdir -p $@

$(BIN): $(DOCTEST) $(COMMONS_SO) $(BIN_DIR) $(OBJS)
	$(CPP) -L"$(COMMONS_BIN)" -Wl,-rpath,"$(COMMONS_BIN)" -o "$@" src/main.cpp $(OBJS) -lcommons

build/%.o: tests/%.cpp
	$(CPP) -std=gnu++11 -I"$(COMMONS)" -c -Wno-write-strings -o "$@" "$<"

build/%.o: src/%.c
	$(CC) -I"$(COMMONS)" -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"

debug: CC += -DDEBUG -g
debug: all

clean:
	$(RM) build

test: all
	LD_LIBRARY_PATH="$(COMMONS_BIN)" ./$(BIN)

$(COMMONS_SO): $(COMMONS_SRCS)
	$(MAKE) -C $(COMMONS) all

$(DOCTEST):
	wget -qO- https://raw.githubusercontent.com/doctest/doctest/v2.4.9/doctest/doctest.h | sudo tee $@ > /dev/null

valgrind: debug
	LD_LIBRARY_PATH="$(COMMONS_BIN)/" valgrind --error-exitcode=42 --leak-check=full -v ./$(BIN)

.PHONY: all debug clean test valgrind

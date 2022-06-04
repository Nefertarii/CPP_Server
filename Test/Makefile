# Test
DIR_NAME = CPPServer

#list of source files (with whitespace between them)
SRC_FILES := test.cpp test2.cpp test3.cpp test4.cpp test5.cpp \
	test6.cpp test7.cpp test8.cpp test9.cpp test10.cpp Taccount.cpp
#Object files
OBJECTS = $(patsubst %.cpp, %.o, $(SRC_FILES))
#Target files
TARGET = $(patsubst %.cpp,%,$(SRC_FILES))
LIB = $(patsubst %.cpp, %.o, $(SRC_FILES))
BUILD = ../build
SRC = ../Http/$(DIR_NAME)
VPATH = $(BUILD)
#Sub object
BASE = ../Base
LOG = ../Log
NET = ../Net

CC = g++
HEADFILE = $(shell pwd)/../
LIBFILE = $(shell pwd)/../build/lib
FLAGS = -I $(HEADFILE) -pthread -std=c++17 -Og -Wall -g
MAKEFLAGS = --no-print-directory
GREEN = "\\033[32m"
ORANGE  = "\\033[33m"
BLUE = "\\033[34m"
RESET = "\\033[0m"
export CC FLAGS MAKEFLAGS GREEN ORANGE BLUE RESET

# compile all object file
all: BASE LOG NET PACKAGE

BASE:
	@cd $($@) && make all $(MAKEFLAGS)
LOG:
	@cd $($@) && make all $(MAKEFLAGS)
NET:
	@cd $($@) && make all $(MAKEFLAGS)
PACKAGE:
	@echo "$(ORANGE)ar package: all static library$(RESET)"
	@cd $(BUILD) && ar -cr lib$(DIR_NAME).a $(shell find $(shell pwd)/$(BUILD) -name '*.o')
	@cd $(BUILD) && mv lib$(DIR_NAME).a lib/lib$(DIR_NAME).a
	@echo "$(GREEN)All compile done.$(RESET)"

test: test.cpp
	$(CC) -c -I $(HEADFILE) $(FLAGS) $< -o $(BUILD)/$@.o
test2: test2.cpp
	$(CC) -c $(FLAGS) $< -o $(BUILD)/$@.o
test3: test3.cpp
	$(CC) -c $(FLAGS) $< -o $(BUILD)/$@.o
test4: test4.cpp
	$(CC) -c $(FLAGS) $< -o $(BUILD)/$@.o
test5: test5.cpp
	$(CC) -c $(FLAGS) $< -o $(BUILD)/$@.o
test6: test6.cpp
	$(CC) -c $(FLAGS) $< -o $(BUILD)/$@.o
test7: test7.cpp
	$(CC) -c $(FLAGS) $< -o $(BUILD)/$@.o
test8: test8.cpp
	$(CC) -c $(FLAGS) $< -o $(BUILD)/$@.o
test9: test9.cpp
	$(CC) -c $(FLAGS) $<
	$(CC) $(FLAGS) -L$(shell pwd)/../build/lib -o test9 test9.o -l$(DIR_NAME)
test10: test10.cpp
	$(CC) -c $(FLAGS) $< -o $(BUILD)/$@.o

Taccount: Taccount.cpp $(shell pwd)/../build/lib/lib$(DIR_NAME).a
	$(CC) -c $(FLAGS) $<
	$(CC) $(FLAGS) -L$(shell pwd)/../build/lib -o $@ $@.o -l$(DIR_NAME)

.PHONY : clean
clean:
	@echo "$(ORANGE)Dir($(DIR_NAME)) delete file: $(OBJECTS) lib$(DIR_NAME).a$(RESET)"
	@echo "$(ORANGE)Dir($(DIR_NAME)) delete file: $(TARGET)$(RESET)"
	@rm -f $(OBJECTS) $(TARGET)
	@cd $(BUILD)/lib && rm -f lib$(DIR_NAME).a
	@cd $(BASE) && make clean
	@cd $(LOG) && make clean
	@cd $(NET) && make clean
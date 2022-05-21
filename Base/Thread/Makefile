# Base/Thread
DIR_NAME = Thread

#list of source files (with whitespace between them)
SRC_FILES := thread.cpp threadpool.cpp

#Object files
OBJECTS = $(patsubst %.cpp, %.o, $(SRC_FILES))
BUILD = ../../build
SRC = ../Base/$(DIR_NAME)
VPATH = $(BUILD)

CC ?= g++
FLAGS ?= -I -pthread -std=c++17 -Og -Wall -g

all: $(OBJECTS)

# compile all object file
$(OBJECTS): %.o: %.cpp
	@echo "Compile $(notdir $<)"
	$(CC) -c $(FLAGS) $< -o $(BUILD)/$@
	
.PHONY : clean
clean:
	@echo "Dir($(DIR_NAME)) delete file: $(OBJECTS)"
	@cd $(BUILD) && rm -f $(OBJECTS)

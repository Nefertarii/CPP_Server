# Base
DIR_NAME = Base

#list of source files (with whitespace between them)
SRC_FILES := buffer.cpp eventloop.cpp eventloopthread.cpp eventloopthreadpool.cpp \
	filehandler.cpp filestat.cpp latch.cpp taskqueue.cpp

#Object files
OBJECTS = $(patsubst %.cpp, %.o, $(SRC_FILES))
BUILD = ../build
SRC = ../$(DIR_NAME)
VPATH = $(BUILD)
#Sub object
NET = Net
THREAD = Thread
TIMER = Timer

CC ?= g++
FLAGS ?= -pthread -std=c++17 -Og -Wall -g
export CC FLAGS

all: $(OBJECTS) NET THREAD TIMER

# compile all object file
$(OBJECTS): %.o: %.cpp
	@echo "Compile $(notdir $<)"
	@$(CC) -c $(FLAGS) $< -o $(BUILD)/$@
NET:
	@cd $($@) && make all
THREAD:
	@cd $($@) && make all
TIMER:
	@cd $($@) && make all

.PHONY : clean
clean:
	@echo "Dir($(DIR_NAME)) delete file: $(OBJECTS)"
	@cd $(BUILD) && rm -f $(OBJECTS)
	@cd $(NET) && make clean
	@cd $(THREAD) && make clean
	@cd $(TIMER) && make clean

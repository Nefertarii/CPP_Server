# Base/Eventloop
DIR_NAME = Poll

#list of source files (with whitespace between them)
SRC_FILES := eventloop.cpp eventloopthread.cpp eventloopthreadpool.cpp poller.cpp channel.cpp timerqueue.cpp

#Object files
OBJECTS = $(patsubst %.cpp, %.o, $(SRC_FILES))
BUILD = ../../build
SRC = ../Base/$(DIR_NAME)
VPATH = $(BUILD)

CC ?= g++
FLAGS ?= -I  -pthread -std=c++17 -Og -Wall -g
MAKEFLAGS ?= --no-print-directory
export CC FLAGS MAKEFLAGS

all: $(OBJECTS) LIB

# compile all object file
$(OBJECTS): %.o: %.cpp %.h 
	@echo "$(BLUE)Compile $(notdir $<)$(RESET)"
	@$(CC) $(FLAGS) -c $< -o $(BUILD)/$@
LIB:
	@echo "$(ORANGE)ar package: libstatic$(DIR_NAME).a$(RESET)"
	@cd $(BUILD) && ar -cr libstatic$(DIR_NAME).a $(OBJECTS)
	@cd $(BUILD) && mv libstatic$(DIR_NAME).a lib/libstatic$(DIR_NAME).a
	@echo "$(GREEN)$(DIR_NAME) compile done.$(RESET)"

.PHONY : clean
clean:
	@echo "$(ORANGE)Dir($(DIR_NAME)) delete file: $(OBJECTS) libstatic$(DIR_NAME).a$(RESET)"
	@cd $(BUILD) && rm -f $(OBJECTS)
	@cd $(BUILD)/lib && rm -f libstatic$(DIR_NAME).a

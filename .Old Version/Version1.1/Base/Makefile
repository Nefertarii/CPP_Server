# Base
DIR_NAME = Base

#list of source files (with whitespace between them)
SRC_FILES := buffer.cpp filehandler.cpp filestat.cpp latch.cpp taskqueue.cpp

#Object files
OBJECTS = $(patsubst %.cpp, %.o, $(SRC_FILES))
BUILD = ../build
SRC = ../$(DIR_NAME)
VPATH = $(BUILD)
#Sub object
POLL = Poll
NET = Net
THREAD = Thread
TIMER = Timer

CC ?= g++
FLAGS ?= -I  -pthread -std=c++17 -Og -Wall -g
MAKEFLAGS ?= --no-print-directory
export CC FLAGS MAKEFLAGS

all: POLL NET THREAD TIMER $(OBJECTS) LIB

POLL:
	@cd $($@) && make all
NET:
	@cd $($@) && make all
THREAD:
	@cd $($@) && make all
TIMER:
	@cd $($@) && make all
# compile all object file
$(OBJECTS): %.o: %.cpp %.h
	@echo "$(BLUE)Compile $(notdir $<)$(RESET)"
	@$(CC) $(FLAGS) -c  $< -o $(BUILD)/$@
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
	@cd $(POLL) && make clean
	@cd $(NET) && make clean
	@cd $(THREAD) && make clean
	@cd $(TIMER) && make clean
	
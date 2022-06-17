# CPP_Server
DIR_NAME = CPP_Server

#list of source files (with whitespace between them)
SRC_FILES := CPP_Server.cpp
#Object files
OBJECTS = $(patsubst %.cpp, %.o, $(SRC_FILES))
#Target files
TARGET = $(patsubst %.cpp,%,$(SRC_FILES))
LIB = $(patsubst %.cpp, %.o, $(SRC_FILES))
BUILD = build
VPATH = $(BUILD)
#Sub object
BASE = Base
LOG = Log
NET = Net
CC = g++
HEADFILE = $(shell pwd)
LIBFILE = $(shell pwd)/$(BUILD)/lib
MODE = 
ifeq ($(MODE), DEBUG)
FLAGS = -I $(HEADFILE) -pthread -std=c++17 -O2 -Wall -g
else
FLAGS = -I $(HEADFILE) -pthread -std=c++17 -Og -DNOPRINTDEBUG -Wall -g
endif
MAKEFLAGS = --no-print-directory
GREEN = "\\033[32m"
ORANGE  = "\\033[33m"
BLUE = "\\033[34m"
RESET = "\\033[0m"
PUSH_COMMENT = 
export CC FLAGS MAKEFLAGS GREEN ORANGE BLUE RESET

# compile all object file
all: PRINT BASE LOG NET PACKAGE CPP_Server

PRINT:
ifeq ($(MODE), DEBUG)
	@echo "$(GREEN)\t\t\t\tMAKE MODE=DEBUG$(RESET)"
else
	@echo "$(GREEN)\t\t\t\tMAKE MODE=NORMAL$(RESET)"
endif
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
	
CPP_Server: CPP_Server.cpp $(shell pwd)/$(BUILD)/lib/lib$(DIR_NAME).a
	@echo "$(BLUE)Compile $(notdir $@)$(RESET)"
	@$(CC) -c $(FLAGS) $<
	@$(CC) $(FLAGS) -L$(shell pwd)/$(BUILD)/lib -o $@ $@.o -l$(DIR_NAME)


.PHONY : clean
clean:
	@echo "$(ORANGE)Dir($(DIR_NAME)) delete file: $(OBJECTS) lib$(DIR_NAME).a$(RESET)"
	@echo "$(ORANGE)Dir($(DIR_NAME)) delete file: $(TARGET)$(RESET)"
	@rm -f $(OBJECTS) $(TARGET)
	@cd $(BUILD)/lib && rm -f lib$(DIR_NAME).a
	@cd $(BASE) && make clean
	@cd $(LOG) && make clean
	@cd $(NET) && make clean
clean_test:
	@echo "$(ORANGE)Dir($(DIR_NAME)) delete file: $(TARGET)$(RESET)"
	@rm -f $(OBJECTS) $(TARGET)

git_push_all : 
	git add .
	git commit -m "normal update(see ChangeLog)"
	git push -u origin master

git_push_all_commit :
	git add .
	git commit -m $(PUSH_COMMENT)
	git push -u origin master
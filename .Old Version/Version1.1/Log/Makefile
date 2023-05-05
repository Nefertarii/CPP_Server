# Log
DIR_NAME = Log

#list of source files (with whitespace between them)
SRC_FILES := logformatter.cpp logger.cpp logging.cpp loglevel.cpp logmsg.cpp

#Object files
OBJECTS = $(patsubst %.cpp, %.o, $(SRC_FILES))
BUILD = ../build
SRC = ../$(DIR_NAME)
VPATH = $(BUILD)
#Sub object
SINK = Sink

CC ?= g++
FLAGS ?= -I  -pthread -std=c++17 -Og -Wall -g
MAKEFLAGS ?= --no-print-directory
export CC FLAGS MAKEFLAGS

# compile all object file
all: SINK $(OBJECTS) LIB

SINK:
	@cd $($@) && make all
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
	@cd $(SINK) && make clean
	@cd $(BUILD)/lib && rm -f libstatic$(DIR_NAME).a
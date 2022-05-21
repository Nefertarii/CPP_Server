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
FLAGS ?= -pthread -std=c++17 -Og -Wall -g
export CC FLAGS

all: $(OBJECTS) SINK

# compile all object file
$(OBJECTS): %.o: %.cpp
	@echo "Compile $(notdir $<)"
	@$(CC) -c $(FLAGS) $< -o $(BUILD)/$@
SINK:
	@cd $($@) && make all

.PHONY : clean
clean:
	@echo "Dir($(DIR_NAME)) delete file: $(OBJECTS)"
	@cd $(BUILD) && rm -f $(OBJECTS)
	@cd $(SINK) && make clean
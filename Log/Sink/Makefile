# Log/Sink
DIR_NAME = Sink

#list of source files (with whitespace between them)
SRC_FILES := filesink.cpp stdsink.cpp

#Object files
OBJECTS = $(patsubst %.cpp, %.o, $(SRC_FILES))
BUILD = ../../build
SRC = ../Log/$(DIR_NAME)
VPATH = $(BUILD)

CC ?= g++
FLAGS ?= -I  -pthread -std=c++17 -Og -Wall -g
export ($(CC) $(FLAGS))

all: $(OBJECTS)

# compile all object file
$(OBJECTS): %.o: %.cpp
	@echo "Compile $(notdir $<)"
	@$(CC) -c $(FLAGS) $< -o $(BUILD)/$@

.PHONY : clean
clean:
	@echo "Dir($(DIR_NAME)) delete file: $(OBJECTS)"
	@cd $(BUILD) && rm -f $(OBJECTS)

# Net
DIR_NAME = Net

#list of source files (with whitespace between them)
#

#Object files
#
BUILD = ../build
SRC = ../$(DIR_NAME)
VPATH = $(BUILD)
#Sub object
HTTP = Http
TCP = Tcp

CC ?= g++
FLAGS ?= -I  -pthread -std=c++17 -Og -Wall -g
MAKEFLAGS ?= --no-print-directory
export CC FLAGS MAKEFLAGS

all: HTTP TCP

# compile all object file
HTTP:
	@cd $($@) && make all
TCP:
	@cd $($@) && make all

.PHONY : clean
clean:
	@cd $(BUILD) && rm -f $(OBJECTS)
	@cd $(HTTP) && make clean
	@cd $(TCP) && make clean
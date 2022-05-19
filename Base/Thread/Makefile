# Thread
objects_thread = thread.o threadpool.o

MODE = DEBUG
CC = g++
ifeq ($(MODE),DEBUG)
FLAGS = -pthread -std=c++17 -Og -Wall -g 
else
FLAGS = -pthread -std=c++17 -O2 -Wall -DNOPRINTDEBUG
endif

output = ../../build

#$(objects_net): %.h: %.cpp
#	$(CC) $(FLAGS) $< -c $@
all: $(objects_thread)

$(objects_thread): %.o: %.h
	$(CC) -c $(FLAGS) $< -o $(output)/$@

.PHONY : clean
clean :
	-cd $(output) && rm $(objects_thread)

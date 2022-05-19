# Net
objects_net = acceptor connector channel poller socket socketapi timerqueue

MODE = DEBUG
CC = g++
ifeq ($(MODE),DEBUG)
FLAGS = -pthread -std=c++17 -Og -Wall -g
else
FLAGS = -pthread -std=c++17 -O2 -Wall -DNOPRINTDEBUG
endif

output = ../../build

all: $(objects_net)
$(objects_net): %: %.cpp %.h
	$(CC) $(FLAGS) $< -c $(output)/$@

.PHONY : clean
clean :
	rm *.gch
	-cd $(output) && rm $(objects_net)
# Net
objects_net = acceptor.o connector.o channel.o poller.o socket.o socketapi.o timerqueue.o
dir_net = Net
# Thread
objects_thread = thread.o threadpool.o
dir_thread = Thread
# Timer
objects_timer = clock.o timer.o timerid.o timers.o
dir_timer = Timer
# Base
objects_base = buffer.o eventloop.o eventloopthread.o eventloopthreadpool.o filehandler.o filestat.o latch.o

MODE = DEBUG
CC = g++
ifeq ($(MODE),DEBUG)
FLAGS = -pthread -std=c++17 -Og -Wall -g 
else
FLAGS = -pthread -std=c++17 -O2 -Wall -DNOPRINTDEBUG
endif

output = ../build

#$(objects_net): %.h: %.cpp
#	$(CC) $(FLAGS) $< -c $@
all: $(objects_net) $(objects_thread) $(objects_timer) $(objects_base)

$(objects_net): %.o: %.h
	$(CC) -c $(FLAGS) $< -o $(output)/$@

$(objects_thread): %.o: %.h
	$(CC) -c $(FLAGS) $< -o $(output)/$@

$(objects_timer): %.o: %.h
	$(CC) -c $(FLAGS) $< -o $(output)/$@

$(objects_base): %.o: %.h
	$(CC) -c $(FLAGS) $< -o $(output)/$@


.PHONY : clean
clean :
	-cd $(output) && rm $(objects_net)
	-cd $(output) && rm $(objects_thread)
	-cd $(output) && rm $(objects_timer)
	-cd $(output) && rm $(objects_base)
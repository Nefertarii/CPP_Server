TARGET = test
CC = g++
TARGET = test
OBJECTS = clock.o timestamp.o timer.o timerid.o timerqueue.o \
	formatter.o sink.o record.o core.o netlog.o \
	poller.o eventloop.o channel.o test.o 
FLAGS = -pthread -std=c++20 -Og -Wall -g
NET = ../Headfile/Net
THREAD = ../Headfile/Thread
TIMER = ../Headfile/Timer
LOG = ../Log

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) -o $(TARGET) $(OBJECTS)

clock.o : $(TIMER)/Head/clock.h
	$(CC) $(FLAGS) -c $(TIMER)/clock.cpp

timer.o : $(TIMER)/Head/timer.h
	$(CC) $(FLAGS) -c $(TIMER)/timer.cpp

timestamp.o : $(TIMER)/Head/timestamp.h
	$(CC) $(FLAGS) -c $(TIMER)/timestamp.cpp

timerid.o : $(TIMER)/Head/timerid.h
	$(CC) $(FLAGS) -c $(TIMER)/timerid.cpp

timerqueue.o : $(TIMER)/Head/timerqueue.h
	$(CC) $(FLAGS) -c $(TIMER)/timerqueue.cpp

formatter.o : $(LOG)/Head/formatter.h
	$(CC) $(FLAGS) -c $(LOG)/formatter.cpp

sink.o : $(LOG)/Head/sink.h
	$(CC) $(FLAGS) -c $(LOG)/sink.cpp

record.o : $(LOG)/Head/record.h
	$(CC) $(FLAGS) -c $(LOG)/record.cpp

core.o : $(LOG)/Head/core.h
	$(CC) $(FLAGS) -c $(LOG)/core.cpp

netlog.o : $(NET)/Head/netlog.h
	$(CC) $(FLAGS) -c $(NET)/netlog.cpp

poller.o : $(NET)/Poll/Head/poller.h
	$(CC) $(FLAGS) -c $(NET)/Poll/poller.cpp

eventloop.o : $(NET)/Poll/Head/eventloop.h
	$(CC) $(FLAGS) -c $(NET)/Poll/eventloop.cpp

channel.o : $(NET)/Poll/Head/channel.h
	$(CC) $(FLAGS) -c $(NET)/Poll/channel.cpp

test.o : test.cpp
	$(CC) $(FLAGS) -c test.cpp

.PHONY : clean
clean : 
	-rm *.o
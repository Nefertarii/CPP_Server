TARGET1 = test
TARGET2 = test2
TARGET3 = test3
TARGET4 = test4
CC = g++
OBJECTS1 = clock.o timestamp.o timer.o timerid.o timerqueue.o \
	formatter.o sink.o record.o core.o netlog.o \
	poller.o eventloop.o channel.o thread.o eventloopthread.o \
	latch.o socketapi.o acceptor.o socket.o inetaddress.o test.o 
OBJECTS2 = test2.o
OBJECTS3 = thread.o test3.o
OBJECTS4 = test4.o 
FLAGS = -pthread -std=c++17 -Og -Wall -g
BASE = ../Net/Base
HTTP = ../Net/Http
INSPECT = ../Net/Inspect
POLL = ../Net/Poll
SOCKETS = ../Net/Sockets
THREAD = ../Thread
TIMER = ../Timer
LOG = ../Log
#test
test: $(OBJECTS1)
	$(CC) $(FLAGS) -o $(TARGET1) $(OBJECTS1)

test2: $(OBJECTS2)
	$(CC) $(FLAGS) -o $(TARGET2) $(OBJECTS2)

test3: $(OBJECTS3)
	$(CC) $(FLAGS) -o $(TARGET3) $(OBJECTS3)

test4: $(OBJECTS4)
	$(CC) $(FLAGS) -o $(TARGET4) $(OBJECTS4)
#timer
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
#logging
formatter.o : $(LOG)/Head/formatter.h
	$(CC) $(FLAGS) -c $(LOG)/formatter.cpp

sink.o : $(LOG)/Head/sink.h
	$(CC) $(FLAGS) -c $(LOG)/sink.cpp

record.o : $(LOG)/Head/record.h
	$(CC) $(FLAGS) -c $(LOG)/record.cpp

core.o : $(LOG)/Head/core.h
	$(CC) $(FLAGS) -c $(LOG)/core.cpp
#base
thread.o : $(BASE)/Head/thread.h
	$(CC) $(FLAGS) -c $(BASE)/thread.cpp

latch.o : $(BASE)/Head/latch.h
	$(CC) $(FLAGS) -c $(BASE)/latch.cpp

netlog.o : $(BASE)/Head/netlog.h
	$(CC) $(FLAGS) -c $(BASE)/netlog.cpp
#poll
poller.o : $(POLL)/Head/poller.h
	$(CC) $(FLAGS) -c $(POLL)/poller.cpp

eventloop.o : $(POLL)/Head/eventloop.h
	$(CC) $(FLAGS) -c $(POLL)/eventloop.cpp

channel.o : $(POLL)/Head/channel.h
	$(CC) $(FLAGS) -c $(POLL)/channel.cpp

eventloopthread.o : $(POLL)/Head/eventloopthread.h
	$(CC) $(FLAGS) -c $(POLL)/eventloopthread.cpp
#sockets
socket.o : $(SOCKETS)/Head/socket.h
	$(CC) $(FLAGS) -c $(SOCKETS)/socket.cpp 

inetaddress.o : $(SOCKETS)/Head/inetaddress.h
	$(CC) $(FLAGS) -c $(SOCKETS)/inetaddress.cpp

socketapi.o : $(SOCKETS)/Head/socketapi.h
	$(CC) $(FLAGS) -c $(SOCKETS)/socketapi.cpp

acceptor.o : $(SOCKETS)/Head/acceptor.h
	$(CC) $(FLAGS) -c $(SOCKETS)/acceptor.cpp
#test
test.o : test.cpp
	$(CC) $(FLAGS) -c test.cpp

test2.o : test2.cpp
	$(CC) $(FLAGS) -c test2.cpp

test3.o : test3.cpp
	$(CC) $(FLAGS) -c test3.cpp

clean : 
	-rm *.o

cleanall:
	-rm *.o $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4)
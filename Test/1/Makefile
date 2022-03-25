TARGET1 = test
TARGET2 = test2
TARGET3 = test3
TARGET4 = test4
TARGET5 = test5
TARGET6 = test7
CC = g++
OBJECTS1 = clock.o timestamp.o timer.o timerid.o timerqueue.o \
	poller.o eventloop.o channel.o thread.o eventloopthread.o \
	latch.o socketapi.o acceptor.o connector.o socket.o inetaddress.o \
	buffer.o tcpconnection.o tcpserver.o tcpclient.o test.o 
OBJECTS2 = test2.o
OBJECTS3 = buffer.o test3.o
OBJECTS4 = buffer.o test4.o 
OBJECTS5 = clock.o timestamp.o timer.o timerid.o timerqueue.o \
	poller.o eventloop.o channel.o thread.o eventloopthread.o \
	latch.o socketapi.o acceptor.o connector.o socket.o inetaddress.o \
	buffer.o tcpconnection.o tcpserver.o tcpclient.o test5.o 
OBJECTS6 = filehandler.o logformatter.o loglevel.o logmsg.o \
	stdsink.o clock.o timestamp.o test7.o
FLAGS = -pthread -std=c++17 -Og -Wall -g
BASE = ../Net/Base
HTTP = ../Net/Http
INSPECT = ../Net/Inspect
POLL = ../Net/Poll
SOCKETS = ../Net/Sockets
SERVER = ../Net/Server
THREAD = ../Thread
TIMER = ../Timer
LOG = ../Log/Base
SINK = ../Log/Sink

#test
test: $(OBJECTS1)
	$(CC) $(FLAGS) -o $(TARGET1) $(OBJECTS1)

test2: $(OBJECTS2)
	$(CC) $(FLAGS) -o $(TARGET2) $(OBJECTS2)

test3: $(OBJECTS3)
	$(CC) $(FLAGS) -o $(TARGET3) $(OBJECTS3)

test4: $(OBJECTS4)
	$(CC) $(FLAGS) -o $(TARGET4) $(OBJECTS4)

test5: $(OBJECTS5)
	$(CC) $(FLAGS) -o $(TARGET5) $(OBJECTS5)

test7: $(OBJECTS6)
	$(CC) $(FLAGS) -o $(TARGET6) $(OBJECTS6)

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
#log
filehandler.o : $(LOG)/Head/filehandler.h
	$(CC) $(FLAGS) -c $(LOG)/filehandler.cpp

logformatter.o : $(LOG)/Head/logformatter.h $(LOG)/Head/fontcolor.h
	$(CC) $(FLAGS) -c $(LOG)/logformatter.cpp

loglevel.o : $(LOG)/Head/loglevel.h
	$(CC) $(FLAGS) -c $(LOG)/loglevel.cpp

logmsg.o : $(LOG)/Head/logmsg.h
	$(CC) $(FLAGS) -c $(LOG)/logmsg.cpp

logsink.o : $(LOG)/Head/logsink.h
	$(CC) $(FLAGS) -c $(LOG)/logsink.cpp
#sink
stdsink.o : $(SINK)/Head/stdsink.h
	$(CC) $(FLAGS) -c $(SINK)/stdsink.cpp
#base
thread.o : $(BASE)/Head/thread.h
	$(CC) $(FLAGS) -c $(BASE)/thread.cpp

latch.o : $(BASE)/Head/latch.h
	$(CC) $(FLAGS) -c $(BASE)/latch.cpp

netlog.o : $(BASE)/Head/netlog.h
	$(CC) $(FLAGS) -c $(BASE)/netlog.cpp

buffer.o : $(BASE)/Head/buffer.h
	$(CC) $(FLAGS) -c $(BASE)/buffer.cpp
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

connector.o : $(SOCKETS)/Head/connector.h
	$(CC) $(FLAGS) -c $(SOCKETS)/connector.cpp	
#server
tcpconnection.o : $(SERVER)/Head/tcpconnection.h
	$(CC) $(FLAGS) -c $(SERVER)/tcpconnection.cpp

tcpserver.o : $(SERVER)/Head/tcpserver.h
	$(CC) $(FLAGS) -c $(SERVER)/tcpserver.cpp

tcpclient.o : $(SERVER)/Head/tcpclient.h
	$(CC) $(FLAGS) -c $(SERVER)/tcpclient.cpp
#test
test.o : test.cpp
	$(CC) $(FLAGS) -c test.cpp

test2.o : test2.cpp
	$(CC) $(FLAGS) -c test2.cpp

test3.o : test3.cpp
	$(CC) $(FLAGS) -c test3.cpp

test4.o : test4.cpp
	$(CC) $(FLAGS) -c test4.cpp

test5.o : test5.cpp
	$(CC) $(FLAGS) -c test5.cpp

test7.o : test7.cpp
	$(CC) $(FLAGS) -c test7.cpp

.PHONY : clean cleanall

clean : 
	-rm *.o

cleanall:
	-rm *.o $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6)
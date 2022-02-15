TARGET = test
CC = g++
TARGET = test
OBJECTS = timer.o formatter.o sink.o record.o core.o netlog.o test.o 
FLAGS = -pthread -std=c++17 -Og -Wall
SOCKET = ../Headfile/Socket
THREAD = ../Headfile/Thread
TIMER = ../Headfile/Clock
LOG = ../Log

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) -o $(TARGET) $(OBJECTS)

timer.o : $(TIMER)/Head/timer.h
	$(CC) $(FLAGS) -c $(TIMER)/timer.cpp

formatter.o : $(LOG)/Head/formatter.h
	$(CC) $(FLAGS) -c $(LOG)/formatter.cpp

sink.o : $(LOG)/Head/sink.h
	$(CC) $(FLAGS) -c $(LOG)/sink.cpp

record.o : $(LOG)/Head/record.h
	$(CC) $(FLAGS) -c $(LOG)/record.cpp

core.o : $(LOG)/Head/core.h
	$(CC) $(FLAGS) -c $(LOG)/core.cpp

netlog.o : $(SOCKET)/Net/Head/netlog.h
	$(CC) $(FLAGS) -c $(SOCKET)/Net/netlog.cpp

test.o : test.cpp
	$(CC) $(FLAGS) -c test.cpp

.PHONY : clean
clean : 
	-rm *.o
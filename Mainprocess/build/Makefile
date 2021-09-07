MODE = NORMAL
TARGET1 = Server
cc = g++
ifeq ($(MODE),DEBUG)
CPPFLAGS = -pthread -Wall -std=c++17 -DDEBUG -Og
else
CPPFLAGS = -pthread -Wall -std=c++17 -O2
endif
OBJECTS1 = Glog.o Gjson.o Gstring.o Gthread.o Gtime.o \
			Serverrno.o Servfunc.o Httprocess.o Httpconnect.o Httprespone.o\
			Servprocess.o Protocol.o \
			Servcontrol.o \
			Server.o

$(TARGET1): $(OBJECTS1)
	$(cc) $(CPPFLAGS) -o $(TARGET1) $(OBJECTS1)
Glog.o : ../../Important/Glog.h
	$(cc) $(CPPFLAGS) -c ../../Important/Glog.cpp
Gjson.o : ../../Important/Gjson.h 
	$(cc) $(CPPFLAGS) -c ../../Important/Gjson.cpp
Gstring.o : ../../Important/Gstring.h 
	$(cc) $(CPPFLAGS) -c ../../Important/Gstring.cpp
Gthread.o : ../../Important/Gthread.h 
	$(cc) $(CPPFLAGS) -c ../../Important/Gthread.cpp
Gtime.o : ../../Important/Gtime.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gtime.cpp
Serverrno.o : ../Httprocess/Serverrno.h
	$(cc) $(CPPFLAGS) -c ../Httprocess/Serverrno.cpp
Servfunc.o : ../Httprocess/Servfunc.h 
	$(cc) $(CPPFLAGS) -c ../Httprocess/Servfunc.cpp
Httprocess.o : ../Httprocess/Httprocess.h
	$(cc) $(CPPFLAGS) -c ../Httprocess/Httprocess.cpp
Httpconnect.o : ../Httprocess/Httpconnect.h
	$(cc) $(CPPFLAGS) -c ../Httprocess/Httpconnect.cpp
Httprespone.o : ../Httprocess/Httprespone.h
	$(cc) $(CPPFLAGS) -c ../Httprocess/Httprespone.cpp
Servprocess.o : ../Servcontrol.h
	$(cc) $(CPPFLAGS) -c ../Servprocess.cpp
Protocol.o : ../Protocol.h
	$(cc) $(CPPFLAGS) -c ../Protocol.cpp
headfile.o : ../headfile.h
	$(cc) $(CPPFLAGS) -c ../headfile.cpp
Servcontrol.o : ../Servcontrol.h
	$(cc) $(CPPFLAGS) -c ../Servcontrol.cpp
Server.o : ../Server.cpp
	$(cc) $(CPPFLAGS) -c ../Server.cpp

.PHONY:clean
clean:
	-rm *.o
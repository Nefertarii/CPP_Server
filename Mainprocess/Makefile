MODE = NORMAL
TARGET1 = Server
OBJECTS1 = Glog.o Gjson.o Gstring.o Gthread.o Gtime.o \
			Serverrno.o Servfunc.o Httprocess.o \
			Servprocess.o Protocol.o \
			Servcontrol.o \
			Server.o
ifeq ($(MODE),DEBUG)
$(TARGET1): $(OBJECTS1)
	g++ -pthread -Wall -std=c++17 -O2 -DDEBUG -o $(TARGET1) $(OBJECTS1)
else
$(TARGET1): $(OBJECTS1)
	g++ -pthread -Wall -std=c++17 -O2 -o $(TARGET1) $(OBJECTS1)	
endif

ifeq ($(MODE),DEBUG)
$(TARGET1): $(OBJECTS1)
Glog.o : ../Important/Glog.h
	g++ -pthread -Wall -std=c++17 -O2 -DDEBUG -c ../Important/Glog.cpp
else
$(TARGET1): $(OBJECTS1)
Glog.o : ../Important/Glog.h
	g++ -pthread -Wall -std=c++17 -O2 -c ../Important/Glog.cpp	
endif

Gjson.o : ../Important/Gjson.h 
	g++ -pthread -Wall -std=c++17 -O2 -c ../Important/Gjson.cpp
Gstring.o : ../Important/Gstring.h 
	g++ -pthread -Wall -std=c++17 -O2 -c ../Important/Gstring.cpp
Gthread.o : ../Important/Gthread.h 
	g++ -pthread -Wall -std=c++17 -O2 -c ../Important/Gthread.cpp
Gtime.o : ../Important/Gtime.h
	g++ -pthread -Wall -std=c++17 -O2 -c ../Important/Gtime.cpp


Serverrno.o : Httprocess/Serverrno.h
	g++ -pthread -Wall -std=c++17 -O2 -c Httprocess/Serverrno.cpp
	
ifeq ($(MODE),DEBUG)
$(TARGET1): $(OBJECTS1)
Servfunc.o : Httprocess/Servfunc.h 
	g++ -pthread -Wall -std=c++17 -O2 -DDEBUG -c Httprocess/Servfunc.cpp
else
$(TARGET1): $(OBJECTS1)
Servfunc.o : Httprocess/Servfunc.h 
	g++ -pthread -Wall -std=c++17 -O2 -c Httprocess/Servfunc.cpp
endif

Httprocess.o : Httprocess/Httprocess.h
	g++ -pthread -Wall -std=c++17 -O2 -c Httprocess/Httprocess.cpp



Servprocess.o : Servcontrol.h
	g++ -pthread -Wall -std=c++17 -O2 -c Servprocess.cpp
Protocol.o : Protocol.h
	g++ -pthread -Wall -std=c++17 -O2 -c Protocol.cpp
headfile.o : headfile.h
	g++ -pthread -Wall -std=c++17 -O2 -c headfile.cpp



Servcontrol.o : Servcontrol.h
	g++ -pthread -Wall -std=c++17 -O2 -c Servcontrol.cpp

Server.o : Server.cpp
	g++ -pthread -Wall -std=c++17 -O2 -c Server.cpp


.PHONY:clean
clean:
	-rm *.o
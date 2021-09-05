MODE = NORMAL
TARGET1 = Server
OBJECTS1 = Glog.o Gjson.o Gstring.o Gthread.o Gtime.o \
			Serverrno.o Servfunc.o Httprocess.o Httpconnect.o Httprespone.o\
			Servprocess.o Protocol.o \
			Servcontrol.o \
			Server.o
ifeq ($(MODE),DEBUG)
$(TARGET1): $(OBJECTS1)
	g++ -pthread -Wall -std=c++17 -Og -DDEBUG -o $(TARGET1) $(OBJECTS1)

Glog.o : ../Important/Glog.h
	g++ -pthread -Wall -std=c++17 -Og -DDEBUG -c ../Important/Glog.cpp
Gjson.o : ../Important/Gjson.h 
	g++ -pthread -Wall -std=c++17 -Og -DDEBUG -c ../Important/Gjson.cpp
Gstring.o : ../Important/Gstring.h 
	g++ -pthread -Wall -std=c++17 -Og -DDEBUG -c ../Important/Gstring.cpp
Gthread.o : ../Important/Gthread.h 
	g++ -pthread -Wall -std=c++17 -Og -DDEBUG -c ../Important/Gthread.cpp
Gtime.o : ../Important/Gtime.h
	g++ -pthread -Wall -std=c++17 -Og -DDEBUG -c ../Important/Gtime.cpp
Serverrno.o : Httprocess/Serverrno.h
	g++ -pthread -Wall -std=c++17 -Og -DDEBUG -c Httprocess/Serverrno.cpp
Servfunc.o : Httprocess/Servfunc.h 
	g++ -pthread -Wall -std=c++17 -Og -DDEBUG -c Httprocess/Servfunc.cpp
Httprocess.o : Httprocess/Httprocess.h
	g++ -pthread -Wall -std=c++17 -Og -DDEBUG -c Httprocess/Httprocess.cpp
Httpconnect.o : Httprocess/Httpconnect.h
	g++ -pthread -Wall -std=c++17 -Og -DDEBUG -c Httprocess/Httpconnect.cpp
Httprespone.o : Httprocess/Httprespone.h
	g++ -pthread -Wall -std=c++17 -Og -DDEBUG -c Httprocess/Httprespone.cpp
Servprocess.o : Servcontrol.h
	g++ -pthread -Wall -std=c++17 -Og -DDEBUG -c Servprocess.cpp
Protocol.o : Protocol.h
	g++ -pthread -Wall -std=c++17 -Og -DDEBUG -c Protocol.cpp
headfile.o : headfile.h
	g++ -pthread -Wall -std=c++17 -Og -DDEBUG -c headfile.cpp
Servcontrol.o : Servcontrol.h
	g++ -pthread -Wall -std=c++17 -Og -DDEBUG -c Servcontrol.cpp
Server.o : Server.cpp
	g++ -pthread -Wall -std=c++17 -Og -DDEBUG -c Server.cpp

else
$(TARGET1): $(OBJECTS1)
	g++ -pthread -Wall -std=c++17 -O2 -o $(TARGET1) $(OBJECTS1)	

Glog.o : ../Important/Glog.h
	g++ -pthread -Wall -std=c++17 -O2 -c ../Important/Glog.cpp	
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
Servfunc.o : Httprocess/Servfunc.h 
	g++ -pthread -Wall -std=c++17 -O2 -c Httprocess/Servfunc.cpp
Httprocess.o : Httprocess/Httprocess.h
	g++ -pthread -Wall -std=c++17 -O2 -c Httprocess/Httprocess.cpp
Httpconnect.o : Httprocess/Httpconnect.h
	g++ -pthread -Wall -std=c++17 -O2 -c Httprocess/Httpconnect.cpp
Httprespone.o : Httprocess/Httprespone.h
	g++ -pthread -Wall -std=c++17 -O2 -c Httprocess/Httprespone.cpp
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
endif

.PHONY:clean
clean:
	-rm *.o
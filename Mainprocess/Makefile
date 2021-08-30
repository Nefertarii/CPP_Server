MODE = NORMAL
TARGET1 = Server
OBJECTS1 = Glog.o Gjson.o Gstring.o Gthread.o Gtime.o \
			Serverrno.o Servfunc.o Httprocess.o \
			Servprocess.o Protocol.o \
			Servcontrol.o \
#			Server.o
ifeq ($(MODE),DEBUG)
$(TARGET1): $(OBJECTS1)
	g++ -pthread -std=c++17 -O2 -DEBUG -o $(TARGET1) $(OBJECTS1)
else
$(TARGET1): $(OBJECTS1)
	g++ -pthread -std=c++17 -O2 -o $(TARGET1) $(OBJECTS1)	
endif


Glog.o : ../Important/Glog.h
	g++ -pthread -std=c++17 -O2 -c ../Important/Glog.cpp
Gjson.o : ../Important/Gjson.h 
	g++ -pthread -std=c++17 -O2 -c ../Important/Gjson.cpp
Gstring.o : ../Important/Gstring.h 
	g++ -pthread -std=c++17 -O2 -c ../Important/Gstring.cpp
Gthread.o : ../Important/Gthread.h 
	g++ -pthread -std=c++17 -O2 -c ../Important/Gthread.cpp
Gtime.o : ../Important/Gtime.h
	g++ -pthread -std=c++17 -O2 -c ../Important/Gtime.cpp


Serverrno.o : Httprocess/Serverrno.h
	g++ -pthread -std=c++17 -O2 -c Httprocess/Serverrno.cpp
Servfunc.o : Httprocess/Servfunc.h 
	g++ -pthread -std=c++17 -O2 -c Httprocess/Servfunc.cpp
Httprocess.o : Httprocess/Httprocess.h
	g++ -pthread -std=c++17 -O2 -c Httprocess/Httprocess.cpp



Servprocess.o : Servcontrol.h
	g++ -pthread -std=c++17 -O2 -c Servprocess.cpp
Protocol.o : Protocol.h
	g++ -pthread -std=c++17 -O2 -c Protocol.cpp
headfile.o : headfile.h
	g++ -pthread -std=c++17 -O2 -c headfile.cpp



Servcontrol.o : Servcontrol.h
	g++ -pthread -std=c++17 -O2 -c Servcontrol.cpp

#Server.o : Server.cpp
	#g++ -pthread -std=c++17 -O2 -c Server.cpp


.PHONY:clean
clean:
	-rm *.o
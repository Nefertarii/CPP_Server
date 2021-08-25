TARGET1 = Server
OBJECTS1 = Gjson.o Glog.o Gstring.o Gthread.o Gtime.o  \
			Httprocess.o Httprespone.o \
			Protocol.o Servercontrol.o Headfile.o Server.o

$(TARGET1): $(OBJECTS1)
	g++ -pthread -std=c++17 -O2 -o $(TARGET1) $(OBJECTS1)

Gtime.o: ../Important/Gtime.h 
	g++ -pthread -std=c++17 -O2 -c ../Important/Gtime.cpp

Gjson.o: ../Important/Gjson.h
	g++ -pthread -std=c++17 -O2 -c ../Important/Gjson.cpp

Gstring.o: ../Important/Gstring.h
	g++ -pthread -std=c++17 -O2 -c ../Important/Gstring.cpp

Glog.o: ../Important/Glog.h ../Important/Gtime.h
	g++ -pthread -std=c++17 -O2 -c ../Important/Glog.cpp

Gthread.o: ../Important/Gthread.h
	g++ -pthread -std=c++17 -O2 -c ../Important/Gthread.cpp

Protocol.o: Protocol.h
	g++ -pthread -std=c++17 -O2 -c Protocol.cpp

Httprocess.o: Httprocess/Httprocess.h
	g++ -pthread -std=c++17 -O2 -c Httprocess/Httprocess.cpp

Httprespone.o: Httprocess/Httprespone.h
	g++ -pthread -std=c++17 -O2 -c Httprocess/Httprespone.cpp

Servercontrol.o : Servercontrol.h
	g++ -pthread -std=c++17 -O2 -c Servercontrol.cpp

Headfile.o: Headfile.h 
	g++ -pthread -std=c++17 -O2 -c Headfile.cpp

Server.o: Server.cpp
	g++ -pthread -std=c++17 -O2 -c Server.cpp

.PHONY:clean
clean:
	-rm *.o
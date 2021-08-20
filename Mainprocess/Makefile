TARGET1 = Server
OBJECTS1 = Httpprocess.o Httprespone.o \
			Glog.o Gstring.o Gthread.o Gtime.o Gjson.o \
			Protocol.o Servprocess.o Headfile.o Server.o

$(TARGET1): $(OBJECTS1)
	g++ -pthread -std=c++17 -O2 -o $(TARGET1) $(OBJECTS1)

Httpprocess.o: Httpsprocess/Httpprocess.h
	g++ -pthread -std=c++17 -O2 -c Httpsprocess/Httpprocess.cpp

Httprespone.o: Httpsprocess/Httprespone.h
	g++ -pthread -std=c++17 -O2 -c Httpsprocess/Httprespone.cpp

Glog.o: ../Important/Glog.h
	g++ -pthread -std=c++17 -O2 -c ../Important/Glog.cpp

Gstring.o: ../Important/Gstring.h
	g++ -pthread -std=c++17 -O2 -c ../Important/Gstring.cpp

Gthread.o: ../Important/Gthread.h
	g++ -pthread -std=c++17 -O2 -c ../Important/Gthread.cpp

Gtime.o: ../Important/Gtime.h 
	g++ -pthread -std=c++17 -O2 -c ../Important/Gtime.cpp

Gjson.o: ../Important/Gjson.h
	g++ -pthread -std=c++17 -O2 -c ../Important/Gjson.cpp

Protocol.o: Protocol.h
	g++ -pthread -std=c++17 -O2 -c Protocol.cpp

Servprocess.o: Servprocess.h
	g++ -pthread -std=c++17 -O2 -c Servprocess.cpp

Headfile.o: Headfile.h 
	g++ -pthread -std=c++17 -O2 -c Headfile.cpp

Server.o: Server.cpp
	g++ -pthread -std=c++17 -O2 -c Server.cpp

.PHONY:clean
clean:
	-rm *.o
TARGET = Gmanage
OBJECTS = Gmanage.o Database.o \
			Httpprocess.o Httprespone.o \
			Glog.o Gstring.o Gthread.o Gtime.o \
			Protocol.o Server.o Servprocess.o Headfile.o 

$(TARGET): $(OBJECTS)
	g++ -pthread -std=c++17 -O2 -o $(TARGET) $(OBJECTS)
Database.o: Database/Database.h
	g++ -pthread -std=c++17 -O2 -c Database/Database.cpp

Httpprocess.o: Httpsprocess/Httpprocess.h
	g++ -pthread -std=c++17 -O2 -c Httpsprocess/Httpprocess.cpp

Httprespone.o: Httpsprocess/Httprespone.h
	g++ -pthread -std=c++17 -O2 -c Httpsprocess/Httprespone.cpp

Glog.o: Important/Glog.h
	g++ -pthread -std=c++17 -O2 -c Important/Glog.cpp

Gstring.o: Important/Gstring.h
	g++ -pthread -std=c++17 -O2 -c Important/Gstring.cpp

Gthread.o: Important/Gthread.h
	g++ -pthread -std=c++17 -O2 -c Important/Gthread.cpp

Gtime.o: Important/Gtime.h 
	g++ -pthread -std=c++17 -O2 -c Important/Gtime.cpp

Protocol.o: Mainprocess/Protocol.h
	g++ -pthread -std=c++17 -O2 -c Mainprocess/Protocol.cpp

Server.o: Mainprocess/Server.h
	g++ -pthread -std=c++17 -O2 -c Mainprocess/Server.cpp

Servprocess.o: Mainprocess/Servprocess.h
	g++ -pthread -std=c++17 -O2 -c Mainprocess/Servprocess.cpp

Headfile.o: Mainprocess/Headfile.h Important/Gstring.h Important/Gthread.h Important/Gtime.h Important/Glog.h
	g++ -pthread -std=c++17 -O2 -c Mainprocess/Headfile.cpp




.PHONY:clean
clean:
	-rm *.o
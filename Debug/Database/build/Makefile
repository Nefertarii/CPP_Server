MODE = NORMAL
TARGET1 = Gmeta
TARGET2 = Graph
cc = g++
ifeq ($(MODE),DEBUG)
CPPFLAGS = -pthread -Wall -std=c++17 -DDEBUG -Og
else
CPPFLAGS = -pthread -Wall -std=c++17 -O2
endif
OBJECTS1 = Glog.o Gjson.o Gstring.o Gtime.o \
			Accountconfirm.o Metahttp.o Metalocal.o Gmeta.o 
OBJECTS2 = Graph.o GQLparse.o Graphserv.o 

$(TARGET1): $(OBJECTS1)
	$(cc) $(CPPFLAGS) -o $(TARGET1) $(OBJECTS1)
Glog.o : ../../Important/Glog.h
	$(cc) $(CPPFLAGS) -c ../../Important/Glog.cpp
Gjson.o : ../../Important/Gjson.h 
	$(cc) $(CPPFLAGS) -c ../../Important/Gjson.cpp
Gstring.o : ../../Important/Gstring.h 
	$(cc) $(CPPFLAGS) -c ../../Important/Gstring.cpp
Gtime.o : ../../Important/Gtime.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gtime.cpp
Accountconfirm.o : ../Meta/Accountconfirm.h
	$(cc) $(CPPFLAGS) -c ../Meta/Accountconfirm.cpp
Metahttp.o : ../Meta/Metahttp.h
	$(cc) $(CPPFLAGS) -c ../Meta/Metahttp.cpp
Metalocal.o : ../Meta/Metalocal.h
	$(cc) $(CPPFLAGS) -c ../Meta/Metalocal.cpp
Gmeta.o : ../Meta/Gmeta.cpp
	$(cc) $(CPPFLAGS) -c ../Meta/Gmeta.cpp
	
$(TARGET2): $(OBJECTS2)
	$(cc) $(CPPFLAGS) -o $(TARGET2) $(OBJECTS2)
Graph.o : ../Graph/Graph.cpp
	$(cc) $(CPPFLAGS) -c ../Graph/Graph.cpp
GQLparse.o : ../Graph/GQLparse.h
	$(cc) $(CPPFLAGS) -c ../Graph/GQLparse.cpp
Graphserv.o : ../Graph/Graphserv.h
	$(cc) $(CPPFLAGS) -c ../Graph/Graphserv.cpp


.PHONY:clean
clean:
	-rm *.o
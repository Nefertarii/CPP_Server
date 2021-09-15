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
			metaconfirm.o Gmeta_http.o Gmeta_local.o Gmeta.o 
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
metaconfirm.o : ../Meta/metaconfirm.h
	$(cc) $(CPPFLAGS) -c ../Meta/metaconfirm.cpp
Gmeta_http.o : ../Meta/Gmeta_http.h
	$(cc) $(CPPFLAGS) -c ../Meta/Gmeta_http.cpp
Gmeta_local.o : ../Meta/Gmeta_local.h
	$(cc) $(CPPFLAGS) -c ../Meta/Gmeta_local.cpp
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
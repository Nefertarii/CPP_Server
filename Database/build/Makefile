MODE = NORMAL
TARGET1 = Meta
TARGET2 = Graph
cc = g++
ifeq ($(MODE),DEBUG)
CPPFLAGS = -pthread -Wall -std=c++17 -DDEBUG -Og
else
CPPFLAGS = -pthread -Wall -std=c++17 -O2
endif
OBJECTS1 = Meta.o Metalocal.o Accountconfirm.o \
			Gfilefunc.o Glog.o Gsocketctrl.o Gsocketfunc.o\
			Gstring.o Gtime.o
OBJECTS2 = Graph.o GQLparse.o Graphserver.o \
			Gepollctrl.o Gsocketctrl.o Gsocketfunc.o Glog.o \
			Gstring.o Gtime.o

$(TARGET1): $(OBJECTS1)
	$(cc) $(CPPFLAGS) -o $(TARGET1) $(OBJECTS1)
$(TARGET2): $(OBJECTS2)
	$(cc) $(CPPFLAGS) -o $(TARGET2) $(OBJECTS2)
Glog.o : ../../Important/Glog.h
	$(cc) $(CPPFLAGS) -c ../../Important/Glog.cpp
Gstring.o : ../../Important/Gstring.h 
	$(cc) $(CPPFLAGS) -c ../../Important/Gstring.cpp
Gtime.o : ../../Important/Gtime.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gtime.cpp
Accountconfirm.o : ../Meta/Accountconfirm.h
	$(cc) $(CPPFLAGS) -c ../Meta/Accountconfirm.cpp
Metalocal.o : ../Meta/Metalocal.h
	$(cc) $(CPPFLAGS) -c ../Meta/Metalocal.cpp
Meta.o : ../Meta/Meta.cpp
	$(cc) $(CPPFLAGS) -c ../Meta/Meta.cpp
Graphserver.o : ../Graph/Graphserver.h
	$(cc) $(CPPFLAGS) -c ../Graph/Graphserver.cpp

GQLparse.o : ../Graph/GQLparse.h
	$(cc) $(CPPFLAGS) -c ../Graph/GQLparse.cpp
Graph.o : ../Graph/Graph.cpp
	$(cc) $(CPPFLAGS) -c ../Graph/Graph.cpp
Gepollctrl.o : ../../Important/Gepollctrl.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gepollctrl.cpp
Gfilefunc.o : ../../Important/Gfilefunc.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gfilefunc.cpp
Gsocketctrl.o : ../../Important/Gsocketctrl.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gsocketctrl.cpp
Gsocketfunc.o : ../../Important/Gsocketfunc.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gsocketfunc.cpp

.PHONY:clean
clean:
	-rm *.o
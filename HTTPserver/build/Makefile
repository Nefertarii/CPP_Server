MODE = NORMAL
TARGET = HTTPserver
cc = g++
ifeq ($(MODE),DEBUG)
CPPFLAGS = -pthread -Wall -std=c++17 -DDEBUG -Og
else
CPPFLAGS = -pthread -Wall -std=c++17 -O2
endif
OBJECTS = Httpserver.o Servctrl.o \
			Gsocketctrl.o Gepollctrl.o Httphandler.o \
			Httprocess.o Httprespone.o Httperrno.o \
			Gsocketfunc.o Glog.o Gstring.o Gtime.o

$(TARGET): $(OBJECTS)
	$(cc) $(CPPFLAGS) -o $(TARGET) $(OBJECTS)

Glog.o : ../../Important/Glog.h
	$(cc) $(CPPFLAGS) -c ../../Important/Glog.cpp
Gstring.o : ../../Important/Gstring.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gstring.cpp
Gtime.o : ../../Important/Gtime.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gtime.cpp

Gepollctrl.o : ../../Important/Gepollctrl.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gepollctrl.cpp
Gsocketctrl.o :	../../Important/Gsocketctrl.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gsocketctrl.cpp
Gsocketfunc.o :	../../Important/Gsocketfunc.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gsocketfunc.cpp

Httperrno.o : ../../Important/Httprocess/Httperrno.h
	$(cc) $(CPPFLAGS) -c ../../Important/Httprocess/Httperrno.cpp
Httphandler.o : ../../Important/Httprocess/Httphandler.h
	$(cc) $(CPPFLAGS) -c ../../Important/Httprocess/Httphandler.cpp
Httprespone.o : ../../Important/Httprocess/Httprespone.h
	$(cc) $(CPPFLAGS) -c ../../Important/Httprocess/Httprespone.cpp
Httprocess.o : ../../Important/Httprocess/Httprocess.h
	$(cc) $(CPPFLAGS) -c ../../Important/Httprocess/Httprocess.cpp

Servctrl.o : ../Servctrl.h
	$(cc) $(CPPFLAGS) -c ../Servctrl.cpp
Httpserver.o : ../Httpserver.cpp
	$(cc) $(CPPFLAGS) -c ../Httpserver.cpp

.PHONY:clean
clean:
	-rm *.o
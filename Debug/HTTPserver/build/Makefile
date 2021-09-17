MODE = NORMAL
TARGET = HTTPserver
cc = g++
ifeq ($(MODE),DEBUG)
CPPFLAGS = -pthread -Wall -std=c++17 -DDEBUG -Og
else
CPPFLAGS = -pthread -Wall -std=c++17 -O2
endif
OBJECTS = Httpserver.o \
			Servctrl.o \
			Httphandler.o Httprocess.o Httprespone.o Gsocketctrl.o \
			Httperrno.o
			Gepollctrl.o Gsocketctrl.o \
			

			

		

$(TARGET): $(OBJECTS)
	$(cc) $(CPPFLAGS) -o $(TARGET) $(OBJECTS)
Gjson.o : ../../Important/Gjson.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gjson.cpp
Glog.o : ../../Important/Glog.h
	$(cc) $(CPPFLAGS) -c ../../Important/Glog.cpp
Gsort.o : ../../Important/Gsort.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gsort.cpp
Gstring.o : ../../Important/Gstring.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gstring.cpp
Gthread.o : ../../Important/Gthread.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gthread.cpp
Gtime.o : ../../Important/Gtime.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gtime.cpp

Gepollcontrol.o : ../../Important/Gepollcontrol.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gepollcontrol.cpp
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

Servcontrol.o : ../Servcontrol.h
	$(cc) $(CPPFLAGS) -c ../Servcontrol.cpp
Httpserver.o : ../Httpserver.cpp
	$(cc) $(CPPFLAGS) -c ../Httpserver.cpp

.PHONY:clean
clean:
	-rm *.o
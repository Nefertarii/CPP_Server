MODE = NORMAL
TARGET = HTTPserver
cc = g++
ifeq ($(MODE),DEBUG)
CPPFLAGS = -pthread -Wall -std=c++17 -DDEBUG -pg -Og
else
CPPFLAGS = -pthread -Wall -std=c++17 -O2
endif
OBJECTS = Gstring.o Gtime.o Glog.o Gjson.o Gfilefunc.o \
			Gsocketfunc.o Gepollctrl.o \
			Gsocketctrl_client.o Gsocketctrl_server.o \
			Httperrno.o Accountconfirm.o \
			Httprocess.o Httprespone.o Httphandler.o \
			Servctrl.o Httpserver.o \

$(TARGET): $(OBJECTS)
	$(cc) $(CPPFLAGS) -o $(TARGET) $(OBJECTS)

Gstring.o : ../../Important/Gstring.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gstring.cpp
Gtime.o : ../../Important/Gtime.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gtime.cpp
Glog.o : ../../Important/Glog.h
	$(cc) $(CPPFLAGS) -c ../../Important/Glog.cpp
Gjson.o : ../../Important/Gjson.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gjson.cpp
Gfilefunc.o : ../../Important/Gfilefunc.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gfilefunc.cpp
			 
Gsocketfunc.o : ../../Important/Gsocketfunc.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gsocketfunc.cpp
Gepollctrl.o : ../../Important/Gepollctrl.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gepollctrl.cpp
Gsocketctrl_client.o : ../../Important/Gsocketctrl_client.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gsocketctrl_client.cpp
Gsocketctrl_server.o : ../../Important/Gsocketctrl_server.h
	$(cc) $(CPPFLAGS) -c ../../Important/Gsocketctrl_server.cpp

Httperrno.o : ../../Important/Httprocess/Httperrno.h
	$(cc) $(CPPFLAGS) -c ../../Important/Httprocess/Httperrno.cpp
Accountconfirm.o : ../../Important/Httprocess/Accountconfirm.h
	$(cc) $(CPPFLAGS) -c ../../Important/Httprocess/Accountconfirm.cpp
Httprocess.o : ../../Important/Httprocess/Httprocess.h
	$(cc) $(CPPFLAGS) -c ../../Important/Httprocess/Httprocess.cpp
Httprespone.o : ../../Important/Httprocess/Httprespone.h
	$(cc) $(CPPFLAGS) -c ../../Important/Httprocess/Httprespone.cpp
Httphandler.o : ../../Important/Httprocess/Httphandler.h
	$(cc) $(CPPFLAGS) -c ../../Important/Httprocess/Httphandler.cpp

Servctrl.o : ../Servctrl.h
	$(cc) $(CPPFLAGS) -c ../Servctrl.cpp
Httpserver.o : ../Httpserver.cpp
	$(cc) $(CPPFLAGS) -c ../Httpserver.cpp

.PHONY:clean
clean:
	-rm *.o
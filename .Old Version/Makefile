#TARGET2 = Database
#TARGET3 = Gmanage
#OBJECTS2 = Database.pp
#OBJECTS3 = Gmanage.cpp
			
subsystem:
	cd Mainprocess/build && $(MAKE)
	



#$(TARGET3): $(OBJECTS3)
#	g++ -pthread -std=c++17 -O2 -o Database/Database.cpp
#	g++ -pthread -std=c++17 -O2 -o Gmanage.cpp

.PHONY:clean
clean:
	cd Mainprocess/build && rm *.o
	#cd ../../Database && rm *.o
	#cd ../../ && rm *.o







#.PHONY:clean
#clean:
	
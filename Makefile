CPP= g++
CPPFLAGS= -c -g -Wextra -Wvla -Wall -std=c++11 -DNDEBUG
TAR_FILES= Makefile README osm.cpp
# All Target
all: osm

#Library
osm: osm.o
	ar rcs libosm.a osm.o


# Object Files
osm.o: osm.h osm.cpp
	$(CPP) $(CPPFLAGS) osm.cpp -o osm.o

tar:
	tar cvf ex1.tar $(TAR_FILES)
 
# Other Targets
clean:
	-rm -f *.o libosm.a 

CPP= g++
CPPFLAGS= -c -g -Wextra -Wvla -Wall -std=c++11 -DNDEBUG

# All Target
all: osm

#Library
osm: osm.o
	ar rcs libosm.a osm.o


# Object Files
osm.o: osm.h osm.cpp
	$(CPP) $(CPPFLAGS) osm.cpp -o osm.o

 
# Other Targets
clean:
	-rm -f *.o libosm.a 

CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp functions.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=trimfile

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
CC = g++

#compiler flags
CFLAGS = -std=c++11 -g -Wall

# the build target executable
TARGET = biomapper

all: compile
#	$(CC) $(CFLAGS) -o bin/$(TARGET) src/main.cpp

compile: main.o biomapper.o
	$(CC) $(CFLAGS) -o bin/$(TARGET) main.o biomapper.o

main.o: src/main.cpp 
	$(CC) $(CFLAGS) -c src/main.cpp

biomapper.o: src/biomapper.cpp src/biomapper.hpp
	$(CC) $(CFLAGS) -c src/biomapper.cpp	

clean: 
	$(RM) bin/$(TARGET) src/*.o src/*~

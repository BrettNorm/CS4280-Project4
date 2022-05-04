CC	= g++
CFLAGS	= -g -std=c++11
TARGET	= statSem
OBJS	= main.o parser.o scanner.o statSem.o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

parser.o: parser.cpp
	$(CC) $(CFLAGS) -c parser.cpp

makeTree.o: makeTree.cpp
	$(CC) $(CFLAGS) -c makeTree.cpp

scanner.o: scanner.cpp
	$(CC) $(CFLAGS) -c scanner.cpp

testScanner.o: testScanner.cpp
	$(CC) $(CFLAGS) -c testScanner.cpp

statSem.o: statSem.cpp
	$(CC) $(CFLAGS) -c statSem.cpp


clean:
	rm -rf *.o statSem

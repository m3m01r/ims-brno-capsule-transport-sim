TARGET=capsules
CC=g++
CFLAGS= -std=c++20 -Wall -pedantic
LIBS= -lsimlib -lm
SRC= main.cpp passenger.cpp tram_stop.cpp capsule.cpp

all: clean $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

run: $(TARGET)
	./$(TARGET) -a 7 -c 11 -l 15

clean:
	rm -rf $(TARGET) *.out *.png

CC = g++
CFLAGS = -g -Wall 
SOURCES = httpServer.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = httpServer

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

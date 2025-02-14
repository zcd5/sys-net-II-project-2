CC = g++
CFLAGS = -g -Wall 
SOURCES = httpServer.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = httpServer
TARGET2 = httpClient

all: $(TARGET) $(TARGET2)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

$(TARGET2): httpClient.o
	$(CC) $(CFLAGS) -o $(TARGET2) httpClient.o

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET2) httpClient.o

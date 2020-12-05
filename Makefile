
CC=g++
CFLAGS=-Wall -Wextra
TARGET=peak

$(TARGET): $(TARGET).o
	@ $(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o 

$(TARGET).o: $(TARGET).cpp
	@ $(CC) $(CFLAGS) -c $(TARGET).cpp

.PHONY : clean
clean:
	@ rm -f $(TARGET) $(TARGET).o


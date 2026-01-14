CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = pz1

all: $(TARGET)

$(TARGET): pz1.c
	$(CC) $(CFLAGS) -o $(TARGET) pz1.c

clean:
	rm -f $(TARGET)

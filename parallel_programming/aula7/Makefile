CC  := gcc

CFLAGS  := -O3
LDFLAGS := -pthread

TARGET := prodcon-1.x prodcon-2.x prodcon-3.x prodcon-4.x

SOURCE_FIRST := prodcon-1.c buffer.c

SOURCE_SECOND := prodcon-2.c buffer.c

SOURCE_THIRD := prodcon-3.c buffer.c

SOURCE_FOURTH := prodcon-4.c buffer.c

ALL: $(TARGET)

prodcon-1.x: $(SOURCE_FIRST)
	$(CC) $(CFLAGS) $(SOURCE_FIRST) -o prodcon-1.x $(LDFLAGS)

prodcon-2.x: $(SOURCE_SECOND)
	$(CC) $(CFLAGS) $(SOURCE_SECOND) -o prodcon-2.x $(LDFLAGS)

prodcon-3.x: $(SOURCE_THIRD)
	$(CC) $(CFLAGS) $(SOURCE_THIRD) -o prodcon-3.x $(LDFLAGS)

prodcon-4.x: $(SOURCE_FOURTH)
	$(CC) $(CFLAGS) $(SOURCE_FOURTH) -o prodcon-4.x $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o

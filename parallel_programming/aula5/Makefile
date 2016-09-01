CC  := gcc

CFLAGS  := -O3
LDFLAGS := -pthread

TARGET = intset-seq intset-coarse

SOURCE_SEQ = intset.c linkedlist.c

SOURCE_COARSE = intset.c linkedlist-coarse.c

ALL: $(TARGET)

intset-seq: $(SOURCE_SEQ)
	$(CC) $(CFLAGS) $(SOURCE_SEQ) -o intset-seq $(LDFLAGS)

intset-coarse: $(SOURCE_COARSE)
	$(CC) $(CFLAGS) $(SOURCE_COARSE) -o intset-coarse $(LDFLAGS)



clean:
	rm -f $(TARGET) *.o

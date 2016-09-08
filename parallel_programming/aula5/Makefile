CC  := gcc

CFLAGS  := -O3
LDFLAGS := -pthread

TARGET = intset-seq intset-coarse intset-fine

SOURCE_SEQ = intset.c linkedlist.c

SOURCE_COARSE = intset.c linkedlist-coarse.c

SOURCE_FINE = intset.c linkedlist-fine.c

ALL: $(TARGET)

intset-seq: $(SOURCE_SEQ)
	$(CC) $(CFLAGS) $(SOURCE_SEQ) -o intset-seq $(LDFLAGS)

intset-coarse: $(SOURCE_COARSE)
	$(CC) $(CFLAGS) $(SOURCE_COARSE) -o intset-coarse $(LDFLAGS)

intset-fine: $(SOURCE_FINE)
	$(CC) $(CFLAGS) $(SOURCE_FINE) -o intset-fine $(LDFLAGS)



clean:
	rm -f $(TARGET) *.o

CC  := gcc

CFLAGS  := -O3
LDFLAGS := -pthread

TARGET = intset-seq 

SOURCE_SEQ = intset.c linkedlist.c

ALL: $(TARGET)

intset-seq: $(SOURCE_SEQ)
	$(CC) $(CFLAGS) $(SOURCE_SEQ) -o intset-seq $(LDFLAGS)



clean:
	rm -f $(TARGET) *.o 

CC  := gcc

CFLAGS  := -O3
LDFLAGS := -pthread

TARGET = intset-seq intset-global intset-rw

SOURCE_SEQ = intset.c hashtable.c

SOURCE_GLOBAL = intset.c hashtable-global.c

SOURCE_RW = intset.c hashtable-rw.c

ALL: $(TARGET)

intset-seq: $(SOURCE_SEQ)
	$(CC) $(CFLAGS) $(SOURCE_SEQ) -o intset-seq $(LDFLAGS)

intset-global: $(SOURCE_GLOBAL)
	$(CC) $(CFLAGS) $(SOURCE_GLOBAL) -o intset-global $(LDFLAGS)

intset-rw: $(SOURCE_RW)
	$(CC) $(CFLAGS) $(SOURCE_RW) -o intset-rw $(LDFLAGS)


clean:
	rm -f $(TARGET) *.o

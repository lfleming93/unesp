CC  := gcc

CFLAGS  := -O1
LDFLAGS := -pthread

TARGET := roupas-seq.x roupas-par.x

SOURCE_SEQ := roupas-seq.c clothing.c

SOURCE_PAR := roupas-par.c clothing.c buffer.c

ALL: $(TARGET)

roupas-seq.x: $(SOURCE_SEQ)
	$(CC) $(CFLAGS) $(SOURCE_SEQ) -o $(TARGET) $(LDFLAGS)

roupas-par.x: $(SOURCE_PAR)
	$(CC) $(CFLAGS) $(SOURCE_PAR) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o

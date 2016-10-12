CC  := gcc

CFLAGS  := -O1
LDFLAGS := -pthread

TARGET_PAR := roupas-par.x

TARGET_SEQ := roupas-seq.x

SOURCE_SEQ := roupas-seq.c clothing.c

SOURCE_PAR := roupas-par.c clothing.c buffer.c

ALL: $(TARGET_PAR) $(TARGET_SEQ)

roupas-seq.x: $(SOURCE_SEQ)
	$(CC) $(CFLAGS) $(SOURCE_SEQ) -o $(TARGET_SEQ) $(LDFLAGS)

roupas-par.x: $(SOURCE_PAR)
	$(CC) $(CFLAGS) $(SOURCE_PAR) -o $(TARGET_PAR) $(LDFLAGS)

clean:
	rm -f $(TARGET_PAR) $(TARGET_SEQ) *.o

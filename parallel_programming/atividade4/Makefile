CC  := gcc

CFLAGS  := -O1
LDFLAGS := -pthread

TARGET := roupas-seq.x 

SOURCE_SEQ := roupas-seq.c clothing.c

ALL: $(TARGET)

roupas-seq.x: $(SOURCE_SEQ)
	$(CC) $(CFLAGS) $(SOURCE_SEQ) -o $(TARGET) $(LDFLAGS)


clean:
	rm -f $(TARGET) *.o 

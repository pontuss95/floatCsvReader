CC=gcc
CFLAGS=-I. -g
DEPS = floatCsvReader.h
OBJ = floatCsvReader.o test.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

floatCsvReader: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


clean:
	rm -f *.o floatCsvReader
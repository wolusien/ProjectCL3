CC=gcc
CFLAGS=-Wall
EXEC= main tfs_create tfs_format tfs_partition
HEADERS= $(wildcard *.h)
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))


all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~
	rm -f $(EXEC)

proper:	
	rm disk.tfs

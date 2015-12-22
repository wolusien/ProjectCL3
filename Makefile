CC=gcc
CFLAGS=-Wall
EXEC= main tfs_create tfs_format tfs_partition
HEADERS= $(wildcard *.h)
OBJECTS = little.o manipdisk.o interne.o


all: $(EXEC)

main: $(OBJECTS) main.o

tfs_create :$(OBJECTS) tfs_create.o

tfs_format  : $(OBJECTS) tfs_format.o

tfs_partition : $(OBJECTS) tfs_partition.o

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~
proper:	
	rm disk.tfs
clean:
	rm -f *.o
	rm -f *~
	rm -f $(EXEC)

proper:	
	rm disk.tfs

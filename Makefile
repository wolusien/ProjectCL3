CC=gcc
CFLAGS=-Wall
EXEC= test main tfs_create tfs_format tfs_partition tfs_analyse
HEADERS= $(wildcard *.h)
OBJECTS = little.o manipdisk.o interne.o manipvolume.o

all: $(EXEC)

test : $(OBJECTS) test.o
main :$(OBJECTS) main.o

tfs_create :$(OBJECTS) tfs_create.o

tfs_format  : $(OBJECTS) tfs_format.o

tfs_partition : $(OBJECTS) tfs_partition.o

tfs_analyse : $(OBJECTS) tfs_analyse.o



%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~

proper:	
	rm -f $(EXEC)
	rm disk.tfs








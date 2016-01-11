CC=gcc
CFLAGS=-Wall
EXEC= main tfs_create tfs_format tfs_partition tfs_analyse tfs_mkdir tfs_mv
HEADERS= $(wildcard *.h)
OBJECTS = little.o manipdisk.o interne.o  
OBJECTS2 = iter.o manipvolume.o manipsystem.o
all: $(EXEC)


main :$(OBJECTS) $(OBJECTS2) main.o

tfs_create :$(OBJECTS) tfs_create.o

tfs_format  : $(OBJECTS) tfs_format.o

tfs_partition : $(OBJECTS) tfs_partition.o

tfs_analyse : $(OBJECTS) tfs_analyse.o

tfs_mkdir : $(OBJECTS) $(OBJECTS2) tfs_mkdir.o

tfs_mv :  $(OBJECTS) $(OBJECTS2) tfs_mv.o

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~
proper:	
	rm -f $(EXEC)
	rm disk.tfs








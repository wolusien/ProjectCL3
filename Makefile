CC=gcc
CFLAGS=-Wall
EXEC= test tfs_create tfs_format tfs_partition tfs_analyse tfs_mkdir test.c
HEADERS= $(wildcard *.h)
OBJECTS = little.o manipdisk.o interne.o  
OBJECTS2 = iter.o manipvolume.o manipsystem.o
all: $(EXEC)


test :$(OBJECTS) $(OBJECTS2) test.o

tfs_create :$(OBJECTS) tfs_create.o

tfs_format  : $(OBJECTS) tfs_format.o

tfs_partition : $(OBJECTS) tfs_partition.o

tfs_analyse : $(OBJECTS) tfs_analyse.o

tfs_mkdir : $(OBJECTS) $(OBJECTS2) tfs_mkdir.o

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~
proper:	
	rm -f $(EXEC)
	rm disk.tfs








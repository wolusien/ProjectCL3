#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "error.h"
#include "little.h"

#define SIZEBLOCK 1024
#define MINBLOCK 3
#define BUFSIZE 256
#define MAX_DISQUE 100
#define DEFAULT_SIZE_TFS 50
#define MAX_PART 254
typedef struct{
    int id;
    char *name;
    int fd;
    int nbBlock;
    int nbPart;
    int taillePart[MAX_PART];
  //FILE* fd;
}disk_id;

disk_id* disque_ouvert[MAX_DISQUE];

typedef struct{
  char buff[1024];
  int id;
}block;
error read_physical_block(disk_id id,block* b,uint32_t num);
error write_physical_block(disk_id id,block b,uint32_t num);
error string_to_uint (char *s, uint32_t u);
error start_disk(char *name,disk_id *id);
error stop_disk(disk_id id);

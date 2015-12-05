#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "error.h"

typedef struct{
  char *name;
  int id;
  char* buffer;
}disk_id;

typedef struct{
  char b[1024];
  int id;
}block;
error read_physical_block(disk_id id,block b,uint32_t num);
error write_physical_block(disk_id id,block b,uint32_t num);

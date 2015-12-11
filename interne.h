#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "error.h"

#define SIZEBLOCK 1024
#define MINBLOCK 3;
#define BUFSIZE 256;
#define MAX_DISQUE 100

char *disque_ouvert[MAX_DISQUE];

typedef struct{
  int id;
}disk_id;

typedef struct{
  char b[1024];
  int id;
}block;
error read_physical_block(disk_id id,block b,uint32_t num);
error write_physical_block(disk_id id,block b,uint32_t num);
error string_to_uint (char *s, uint32_t u);

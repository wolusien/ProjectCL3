#include "error.h"

typedef struct{
  char *name;
}disk_id;

typedef struct{
  char b[1024];
  int id;
}block;
error read_physical_block(disk_id id,block b,uint32_t num);
error write_physical_block(disk_id id,block b,uint32_t num);

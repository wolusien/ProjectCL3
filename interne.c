#include "interne.h"

error read_physical_block(disk_id id,block b,uint32_t num){
  FILE *f = fopen(id.name,"r");
  fseek(f,num*1024,SEEK_SET);
  fgets(b.b,1024,f);
  error e;
  return e;
}

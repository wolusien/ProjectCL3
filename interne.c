#include "interne.h"

error read_physical_block(disk_id id,block b,uint32_t num){
  FILE *f = fopen(id.name,"r");
  fseek(f,num*1024,SEEK_SET);
  int i;
  for(i=0; i<1024; i++){
      b.b[i]=fgetc(f);
  }
  error e;
  return e;
}

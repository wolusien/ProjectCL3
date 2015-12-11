#include "interne.h"

int main (int argc, char * argv[]){
  string_to_uint("00000000000000000000000000002246");

    int i;
    disk_id disk;
  disk.name="disk.tfs";
  FILE *f=fopen(disk.name,"w+");
  printf("nombre de bloc que vous voulez creez :\n");
  int d;
  scanf("%d",&d);
  for(i=0;i<d*1024;i++){
    fputc('0',f);
  }
  block b;
  for(i=0;i<1016;i++){
    b.b[i]='A';
  }
  for(i=1016;i<1024;i++){
    b.b[i]='1';
  }
  write_physical_block(disk,b,00000000000000000000000000000001);
  block bl;
  read_physical_block(disk,bl,00000000000000000000000000000001);
  exit(0);
}



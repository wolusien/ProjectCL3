#include "interne.h"

int main (int argc, char * argv[]){
  int taille = 1024*atoi(argv[1]);
  char bytes[taille];
  FILE *f = fopen("disk.tfs","w+");
  int i;
  for(i=0;i<taille;i++){
    bytes[i]='0';
  }
  fputs(bytes,f);
  fclose(f);
  disk_id disk;
  disk.name="disk.tfs";
  block b;
  read_physical_block(disk,b,00000000000000000000000000000001);
  printf("Voici le message%s il estfini\n",b.b);
  exit(0);
}



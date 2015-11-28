#include "interne.h"

int main (int argc, char * argv[]){
  int taille = 1024*atoi(argv[1]);
  char bytes[taille];
  FILE *f = fopen("disk.tfs","w+");
  int i;
  for(i=0; i<taille; i++){
      bytes[i]='0';
  }
  fputs(bytes,f);
  fclose(f);
  disk_id disk;
  disk.name="disk.tfs";
  block b;
  int tab[1024];
  read_physical_block(disk,b,00000000000000000000000000000001);
  for(i=0; i<1024; i++){
      printf("\nb contient %c\n",b.b[i]);
      //tab[i]=(b.b[i]-'0');
      //printf("%d",tab[i]);
  } 
  printf("\n");
 exit(0);
}



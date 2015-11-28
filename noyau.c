#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
int main (int argc, char * argv[]){
  int taille = 1024*atoi(argv[1]);
  char bytes[taille];
  FILE *f = fopen("disk.tfs","w+");
  for(int i=0;i<taille;i++){
    bytes[i]=0;
  }
  fputs(bytes,f);
  fclose(f);
  exit(0);
}



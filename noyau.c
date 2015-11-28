#include "interne.h"

int main (int argc, char * argv[]){
  disk_id disk;
  disk.name="fic.txt";
  block b;
  read_physical_block(disk,b,00000000000000000000000000000001);
  printf("%s\n", b.b);
  exit(0);
}



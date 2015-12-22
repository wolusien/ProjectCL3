#include "interne.h"

/*
int main(int argc,char *argv[]){
    int fichier;
    int size;
    char *name;
    printf("%d",argc);
  switch(argc){
  case 1 :
      name = "disk.tfs";
      size = DEFAULT_SIZE_TFS;
      break;
  case 2 :
      name = argv[1];
      size = DEFAULT_SIZE_TFS;
      break;
  case 4 :
    if(strcmp(argv[1],"-s")==0){
      name = argv[3];
      size = atoi(argv[2]);
    }
    else{
        printf("option %s inconnue changez pour -s",argv[1]);
        exit(-1);
    }
    break;
  default:
    printf("mauvais nombre d'argument");
    exit(-1);
    break;
  }
  fichier = open(name,O_CREAT | O_EXCL | O_RDWR | S_IRUSR | S_IWUSR);
  if(fichier != -1){
    block first;
    uint32_t u = int_to_little(size);
    unsigned char *tab =(unsigned char *)&u;
    int i;
    for(i=0;i<4;i++){
      first.buff[i]=tab[i];
    }
    for(i=4;i<1024;i++){
      first.buff[i]='0';
    }
    uint32_t position = int_to_little(0);
    disk_id disk;
    disk.id=0;
    disk.fd=fichier;
    disque_ouvert[0]=&disk;
    write_physical_block(disk,first,position);
    block rest;
    for(i=0;i<1024;i++){
      rest.buff[i]='0';
    }
    for(i=1;i<size;i++){
      position=int_to_little(i);
      write_physical_block(disk,rest,position);
    }
    disque_ouvert[0]=NULL;
    close(fichier);

  }
  else{
      printf("le nom choisi existe déjà veuillez en entrez un nouveau");
  }
}
*/

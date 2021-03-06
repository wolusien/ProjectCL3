#include "manip.h"


int main(int argc,char *argv[]){
  int fichier;
  int size;
  char *name;
  switch(argc){
  case 1 :
    name = "disk.tfs";
    size = DEFAULT_SIZE_TFS;
    break;
  case 2 :
    name = argv[1];
    size = DEFAULT_SIZE_TFS;
    break;
  case 3:
    if(strcmp(argv[1],"-s")==0){
      size = atoi(argv[2]);
      name = "disk.tfs";
      if(size==0){
        fprintf(stderr,"%s n'est pas un entier\n",argv[2]);
        exit(-1);
      }
    }
  case 4 :
    if(strcmp(argv[1],"-s")==0){
      name = argv[3];
      size = atoi(argv[2]);
      if(size==0){
        fprintf(stderr,"%s n'est pas un entier\n",argv[2]);
        exit(-1);
      }
    }
    else{
      fprintf(stderr,"option %s inconnue changez pour -s\n",argv[1]);
      exit(-1);
    }
    break;
  default:
    fprintf(stderr,"mauvais nombre d'argument\n");
    exit(-1);
    break;
  }
  fichier = open(name,O_CREAT | O_EXCL | O_RDWR ,00700);
  if(fichier != -1){
    block first;
    fill_block(&first,size,0);
    fill_block(&first,0,4);
    int i;
    for(i=8;i<1024;i++){
      first.buff[i]='\0';
    }

    uint32_t position = int_to_little(0);
    disk_id disk;
    disk.id=0;
    disk.fd=fichier;
    disk.nbBlock = size;
    disque_ouvert[0]=&disk;
    write_physical_block(disk,first,position);
    block rest;
    for(i=0;i<1024;i++){
      rest.buff[i]='\0';
    }
    for(i=1;i<size;i++){
      position=int_to_little(i);
      write_physical_block(disk,rest,position);
    }
    disque_ouvert[0]=NULL;
    close(fichier);
    
  }
  else{
    fprintf(stderr,"le nom choisi existe déjà veuillez en entrez un nouveau\n");
    exit(-1);
  }
  exit(0);
}

#include "interne.h"




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
  case 4 :
    if(strcmp(argv[1],"-s")==0){
      name = argv[3];
      size = argv[2];
    }
    else{
      printf("option %s inconnue changez pour -s",argv[1]);
      exit(-1);
    }
    break;
  default:
    printf("mauvais nombre d'argument");
    exit(-1);
  }
  int fichier = open("disk.tfs",O_CREAT | O_EXCL);
  if(fichier != -1){
    block first;
    unint32_t u = int_to_little(size);
    unsigned *char tab =&u;
    int i;
    for(i=0;i<4;i++){
      first.buff[i]=tab[i];
    }
    for(i=4;i<1024;i++){
      first.buff[i]='0';
    }
    unint32_t position = int_to_little(0);
    
  }
  else{
    printf("le nom choisi existe déjà veuillez en entrez un nouveau");
  }
}



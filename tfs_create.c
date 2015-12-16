#include "interne.h"

<<<<<<< HEAD
int main(int argc ,char *argv){
  if(argc == 1 || argc == 3 || argc == 4){
    int size = DEFAULT_SIZE_TFS;
    if(argc==2){
      if(strcmp(argv[1],"-s")==0){
     if(argc == 3){ 
	  if()
      }else{
	
      }
    }else{
      error e;
      e.id = -1;
      printf("add option -s");
=======
int main(int argc,char *argv[]){
  int fichier;
  switch(argc){
  case 1 :
    int err = open("disk.tfs",O_CREAT | O_EXCL);
    if(err !=-1){
      fichier =err;
      
    }
    else{
      printf("le nom par défaut existe déjà veuillez preciser un nouveau nom de fichier");
      exit(0);
>>>>>>> modif
    }
  }else{
    error e;
    e.id = -1;
    printf("wrong numbre of arguments : %d aguments", argc);
  }
}


#include "interne.h"


error start_disk(char *name,disk_id *id){
  if(name != NULL){
      int i;
      int boolean=0;
      for(i=0;i<MAX_DISQUE;i++){
          if(disque_ouvert[i]!=NULL){
              if(strcmp(disque_ouvert[i]->name,name)==0){
                  boolean =1;
                  break;
              }
          }
      }
      if(boolean == 0){
          FILE* f = fopen(name,"r+");
          if(f!=NULL){
              printf("Je suis avt la boucle while\n");
              while(disque_ouvert[i]!=NULL && i!=MAX_DISQUE){
                  i++;
                  printf("Je suis ds la boucle while\n");
              }
              if(i!=MAX_DISQUE){
                  printf("Je suis ds le if\n");
                  if(id==NULL){printf("Je suis null id\n");}
                  id->id=i;
                  printf("Je suis avt le malloc de name\n");
                  id->name= malloc(sizeof(char)*strlen(name));
                  printf("Je suis avt le malloc de disk\n");
                  disque_ouvert[id->id]->fd = f;
              }
              else{
                  printf("Je suis ds le else\n");
                  error e;
                  e.errnb=-1;
                  printf("vous ne pouvez plus ouvrir de disque \n");
                  return e;
              }
              printf("Je suis après les conditions\n");
          }
      }
      else{
          error erreur;
          return erreur;
      }  
  }
  else{
      error erreur;
      return erreur;
  }
  error e;
  return e;
}
error read_block(disk_id id,block b,uint32_t num){
  return read_physical_block(id,b,num);
}

error write_bloc(disk_id id,block b,uint32_t num){
    return write_physical_block(id,b,num);
}

error sync_disk(disk_id id){
    error e;
    return e;
}

error stop_disk(disk_id id){
    if(disque_ouvert[id.id]!=NULL){
        FILE *nom=disque_ouvert[id.id];
        int e = fclose(nom);
        if(e != -1){
            disque_ouvert[id.id]=NULL;
            free(id.name);
        }
    }
    error er;
    return er;
}

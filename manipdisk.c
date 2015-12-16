#include "interne.h"


error start_disk(char *name,disk_id *id){
  if(name != NULL){
    FILE* f = fopen(name,"r+");
    if(f!=NULL){
      int i=0;
      while(disque_ouvert[i]!=NULL || i==MAX_DISQUE){
	i++;
      }
      if(i!=MAX_DISQUE){
	id->id=i;
	id->name= malloc(sizeof(char)*strlen(name));
	disque_ouvert[id->id]= f;
      }
      else{
	error e;
	e.errnb=-1;
	printf("vous ne pouvez plus ouvrir de disque \n");
	return e;
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

#include "interne.h"


error start_disk(char *name,disk_id *id){
  if(name != NULL){
    char *type = strchr(name,'.');
    if(strcmp(type,".tfs")!=-1){
      FILE* f = fopen(name,"w+");
      if(f!=NULL){
	disque_ouvert[id->id]= malloc(sizeof(char)*strlen(name));
	disque_ouvert[id->id]= name;
      }
      else{
	error erreur;
	return erreur;
      }  
    }
  }
  else{
    error erreur;
    return erreur;
  }
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
    char *nom=disque_ouvert[id.id];
    int e = fclose(disque_ouvert[id.id]);
    if(e != -1){
      disque_ouvert[id.id]=NULL;
      free(nom);
    }
  }
  error er;
  return er;
}

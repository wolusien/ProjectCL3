#include "interne.h"
int e = fopen(name,+w);

error start_disk(char *name,disk_id *id){
  if(name != NULL){
    char *type = strchr(name,'.');
    if(strcmp(type,".tfs")!=-1){
      int e = fopen(name,"w+");
      if(e!=-1){
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

error write_bloc(disk_id id,block b,unint32_t num){
  return write_physical_block(id,b,num);
}

error sync_disk(disk_id id){
  error e;
  return e;
}

error stop_disk(disk_id id){
  if(disque_ouvert[id.id]!=NULL){
    int e = fclose(disque_ouvert[id]);
    if(e != -1){
      disque_ouvert[id.id]=NULL;
      free(disque_ouvert[id.id]);
    }
  }
  error er;
  return er;
}

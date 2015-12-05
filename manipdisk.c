#include "interne.h"
int e = fopen(name,+w);

error start_disk(char *name,disk_id *id){
  if(name != NULL){
    char *type = strchr(name,'.');
    if(strcmp(type,".tfs")!=-1){
      int e = fopen(name,"w+");
      if(e!=-1){
	disque_ouver[disk_id]= malloc(sizeof(char)*strlen(name));
	disque_ouvert[disk_id.id]= name;
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

#include "interne.h"


error start_disk(char *name,disk_id *id) {
  error e;
  if(name != NULL){
    int boolean =0;
    for(int i=0;i<MAX_DISQUE;i++){
      if(disque_ouvert[i]!=NULL){
	if(strcmp(disque_ouvert[i]->name,name)==0){
	  boolean =1;
	  break;
	}
      }
    }
    printf("checkpoint1\n");
    if(boolean==0){
      FILE *f =fopen(name,"r+");
      printf("checkpoint2\n");
      if(f != NULL){
	int nbrcurs =0;
	while(disque_ouvert[nbrcurs]!=NULL && nbrcurs!=MAX_DISQUE){
	  nbrcurs++;
	}
	printf("checkpoint3\n");
	if(nbrcurs!=MAX_DISQUE){
	  if(id !=NULL){
	    id->name = malloc(strlen(name)*sizeof(char));
  	    id->id = nbrcurs;
	    id->fd = f;
	    id->name= name;
	    disque_ouvert[nbrcurs]=id;
	    e.errnb = 0;
	  }
	  else{
	    e.errnb = -1;
	    printf("le pointeur de disk_id passé en argument est NULL\n");
	  }
	}
	else{
	  e.errnb =-1;
	  printf("il y a trop de disque ouvert\n");
	}
      }
      else{
	e.errnb = -1;
	printf("le disque ne peut pas s'ouvrir\n");
      }
    }
    else{
      e.errnb = -1;
      printf("le disque est déjà ouvert\n");
    }
  }
  else{
    e.errnb = -1;
    printf("ajouter un nom de disque\n");
  }
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
        FILE *nom=disque_ouvert[id.id]->fd;
        int e = fclose(nom);
        if(e != -1){
            free(id.name);
	    free(disque_ouvert[id.id]);
	    disque_ouvert[id.id]=NULL;
        }
    }
    error er;
    return er;
}

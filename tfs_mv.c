/**
 *\file tfs_mv.c
 **/
#include "manipsystem.h"

/**
 *\fn  main (int argc, char * argv[]){
 *\brief si on prend en argumen le chemin d'un fichier et un nom, on renome le fichier avec le nom et on retourne o, sinon on retournt -1;
 **/
int main (int argc, char * argv[]){
  if(argc==3){
    iter iter=decomposition(strdup(argv[1]));
    if(strcmp(iter->name, "FILE:")==0){
      if(iter->next!=NULL){
	iter=iter->next;
	if(strcmp(iter->name,"HOST")==0){
	  return 1; 
	}else{
	  if(iter->next!=NULL){
	    iter=iter->next;
	    disk_id *id=malloc(sizeof(disk_id));
	    error err = start_disk(iter->name, id);
	    if(err.errnb!=-1){
	      if(tfs_rename(argv[1], argv[2])!=-1){
		return 0;
	      }
	    }
	  }
	}
      }
    }
  }
  return -1;
}

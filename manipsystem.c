#include "manipdisk.h"
int isNumber(char *buf){
  int boolean;
  int i;
  for(i=0;i<strlen(buf);i++){
    if(isdigit((int)buf[i])==0){
      boolean = -1;
      break;
    }
  }
  return boolean;
}
int tfs_mkdir(const char *path, mode_t mode){
    iter i = decomposition(path);
    
}

int tfs_rename(const char *old, const char *new){
  error e;
  iter iter=decomposition(old);
  if(strcmp(iter->name, "FILE:")==0){
    if(iter->next!=NULL){
      iter=iter->next;
      if(strcmp(iter->name,"HOST")==0){
	e.errnb=1;  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	return e;   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      }else{
	if(iter->next!=NULL){
	  iter=iter->next;
	  int i;
	  for(i=0;i<MAX_DISQUE;i++){
	    if(disque_ouvert[i]!=NULL){
	      if(strcmp((disque_ouvert[i])->name,iter.name)==0){
		
	      }
	    }
	  }
	  e.errnb=-1;
	  fprintf(stderr, "tfs_rename : no disk open");
	  return e;
	}
      }
    }else{
      e.errnb=-1;
      fprintf(stderr, "tfs_rename : NULL pointeur");
      return e;
    }
  }
}

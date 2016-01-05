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
  char *separateur("//");
  char *token;
  token = strtok(path,separateur);
  if(pointeur != NULL){
    poiteur = strtok(NULL,separateur);
    if(pointeur != NULL && strcmp(pointeur,"FILE:")==0){
      pointeur = strtok(NULL,separateur);
      if(pointeur != NULL){
	if(strcmp(pointeur,"HOST") ==0){
	}
	else{
	  disk_id *id = malloc(sizeof(disk_id));
	  error e = startdisk(pointeur,id);
	  if(e.errnb != -1){
	    pointeur = strtok(NULL,separateur);
	    if(pointeur != NULL){
	      if(isNumber(pointeur)!=-1){
		int volume = atoi(pointeur);
		if(pos<id->nbPart){
		  int pos = 1;
		  int i;
		  for(i=0;i<=volume;i++){
		    pos = pos+id->taillePart[i];
		  }
		  block *b;
		  read_block(*id,b,int_to_little(pos));
		  uint32_t n;
		  unsigned char *tab=(unsigned char *)(&n);
		  for(i=0;i<4;i++){
		    tab[i]=first.buff[i];
		  }
		  
		}
		else{
		  printf("volume %d doesn't exist on %s",volume,id->name);
		}
	      }
	      else{
		printf("\'%s\' is not a good path",path);
		return -1;
	      }
	    }
	    else{
	      printf("\'%s\' is not a good path",path);
	      return -1;
	    }
	  }
	  else{
	    printf(" disk %s doesn't exist " pointeur);
	    return -1
	  }
	}
      }
      else{
	printf("\'%s\' is not a good path",path);
	return -1;
      }
    }
    else{
      printf("\'%s\' is not a good path",path);
      return -1;
    }
  }
  else{
    printf("\'%s\' is not a good path",path);
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

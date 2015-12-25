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
}  

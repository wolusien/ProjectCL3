#include "manivolume.h"

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
  int length=strlength(new);
  if(length<28){
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
		  disk_id *disk=disk_ouvert[i];
		  i=i->next;
		  if(i->next!=NULL){
		    i=i->next;
		    int part = atoi(i->name);
		    if(part>=0 && part<disk->nbPart){
		      Part here=disk->tabPart[part];
		      go_end(iter);
		      char *oldname=iter.name;
		      iter=iter.pred;
		      iter.next=NULL;
		      go_start(iter);
		      int place;
		      error e=find_name(iter,*disk,part, &place);
		      if(e.errnb==0){
			int numblock;
			int pos;
			if(name_in_dir(*disk,part,place,oldname,&numblock, &pos)!=-1){
			  block b;
			  read_block(*disk, &b, numblock+here.num_first_block);
			  int j;
			  for(j=0; j<length; j++){
			    b.buff[pos+4+j]=new[j];
			  }
			  for(j=length; j<28; j++){
			    b.buff[j]='\0';
			  }
			}
		      }
		    }
		  }
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
}

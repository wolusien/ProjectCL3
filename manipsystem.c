#include "manipsystem.h"
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
  return 0;
}

int tfs_rename(const char *old, const char *new){
  int length=strlen(new);
  if(length<28){
    iter iter=decomposition(strdup(old));
    if(strcmp(iter->name, "FILE:")==0){
      if(iter->next!=NULL){
	iter=iter->next;
	if(strcmp(iter->name,"HOST")==0){
	  return 1;  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}else{
	  if(iter->next!=NULL){
	    iter=iter->next;
	    int i;
	    for(i=0;i<MAX_DISQUE;i++){
	      if(disque_ouvert[i]!=NULL){
		if(strcmp((disque_ouvert[i])->name,iter->name)==0){
		  disk_id *disk=disque_ouvert[i];
		  iter=iter->next;
		  if(iter->next!=NULL){
		    iter=iter->next;
		    int part = atoi(iter->name);
		    if(part>=0 && part<disk->nbPart){
		      Part here=disk->tabPart[part];
		      go_end(iter);
		      char *oldname=iter->name;
		      iter=iter->prec;
		      iter->next=NULL;
		      go_start(iter);
		      int place;
		      error e=find_name(iter,*disk,part, &place);
		      if(e.errnb==0){
			int numblock;
			int pos;
			if(name_in_dir(*disk,part,place,oldname,&numblock, &pos).errnb!=-1){
			  block b;
			  read_block(*disk, &b, numblock+here.num_first_block);
			  int j;
			  for(j=0; j<length; j++){
			    b.buff[pos+4+j]=new[j];
			  }
			  for(j=length; j<28; j++){
			    b.buff[j]='\0';
			  }
			  return 0;
			}
		      }
		    }else{
		       fprintf(stderr, "tfs_rename : no part \n");
		    }
		  }else{
		     fprintf(stderr, "tfs_rename : wrong path \n");
		  }
		}
	      }
	      return -1;
	    }
	    fprintf(stderr, "tfs_rename : no disk open \n");
	  }else{
	    fprintf(stderr, "tfs_rename : wrong path \n");		     
	  }
	}
      }else{
	fprintf(stderr, "tfs_rename : NULL pointeur");
      }
    }else{
      fprintf(stderr, "tfs_rename : wrong path, miss FILE: \n");		     
    }
  }else{
    fprintf(stderr, "tfs_rename : char new too long, max length=28 \n");		     
  }
  return -1;
}

int tfs_open(const char *name,int oflag){
  iter iter=decomposition(strdup(name));
  if(strcmp(iter->name, "FILE:")==0){
    if(iter->next!=NULL){
      iter=iter->next;
      if(strcmp(iter->name,"HOST")==0){
	return 1;   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      }else{
	int i;
	for(i=0;i<MAX_DISQUE;i++){
	  if(disque_ouvert[i]!=NULL){
	    if(strcmp((disque_ouvert[i])->name,iter->name)==0){
	      disk_id *disk=disque_ouvert[i];
	      if(iter->next!=NULL){
		iter=iter->next;
		int part = atoi(iter->name);
		if(part>=0 && part<disk->nbPart){
		  Part here=disk->tabPart[part];
		  int place;
		  error e=find_name(iter,*disk,part, &place);
		  if(e.errnb==0){
		    block b;
		    read_block(*disk,&b, here.num_first_block+1+(place-1)/16);
		    int a;
		    readint_block(&b, &a,place%16+4);
		    if(a==0){ //si num correspond a un fichier
		      tfs_fd f;
		      f.pdisk=disk;
		      f.partition=part;
		      f.pos=place;
		      f.flag=oflag;
		      f.path=strdup(name);
		      int j;
		      for(j=0; j<MAX_FICHIERS;j++){
			if(fichiers_ouverts[i]==NULL){
			  fichiers_ouverts[i]=&f;
			  return i;
			}
		      }
		      fprintf(stderr, "tfs_open : fichiers_ouverts is full \n");
		    }else{
		      fprintf(stderr, "tfs_open : nom correspond a un dossier \n");
		    }
		  }else{
		    fprintf(stderr, "tfs_open : wrong path \n");
		  }
		}else{
		  fprintf(stderr, "tfs_open : no part \n");
		}
	      }else{
		fprintf(stderr, "tfs_open : wrong path \n");
	      }
	    }else{
	      fprintf(stderr, "tfs_open : wrong path \n");
	    }
	  }else{
	    fprintf(stderr, "tfs_open : wrong path \n");
	  }
	  return -1;
	}
	fprintf(stderr, "tfs_open : no disk open \n");
      }
    }else{
      fprintf(stderr, "tfs_open : NULL pointeur");
    }
  }else{
    fprintf(stderr, "tfs_open : wrong path, miss FILE: \n");		     
  }
  return -1;
}

// prend en argument un indice d'un tfs_fd dans le tableau de fichiers_ouverts
int tfs_close(int num){
  if(num>=0 && num<MAX_FICHIERS){
    free(fichiers_ouverts[num]->pdisk);
    free(fichiers_ouverts[num]->path);
    free(fichiers_ouverts[num]);
    return 0;
  }else{
    return -1;	
  }
}



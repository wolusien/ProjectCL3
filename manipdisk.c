#include "manip.h"

error fill_block(block *b, int a,int loc){ //loc est l'endroit du bloc ou on veut écrire l'entier a en little indian.
  if(loc>=0 && loc<=1020){
    uint32_t p=int_to_little(a);
    unsigned char *c=(unsigned char *)(&p);
    int i;
    for(i=0; i<4; i++){
      b->buff[i+loc]=c[i];
    }
    error e;
    e.errnb=0;
    return e;
  }else{
    error e;
    e.errnb=-1;
    fprintf(stderr,"fill_block : wrong argument loc : %d", loc);
    return e;
  }
}

error readint_block(block *b, int *a,int loc){ //loc est l'endroit du bloc ou on veut lire l'entier que l'on écrit dans a.
  int i;
  if(loc>=0 && loc<=1020){
    uint32_t n;
    unsigned char *tab=(unsigned char *)(&n);
    for(i=0;i<4;i++){
      tab[i]=b->buff[i+loc];
    }
    *a = little_to_int(n); 
    error e;
    e.errnb=0;
    return e;
  }else{
    error e;
    e.errnb=-1;
    fprintf(stderr,"fill_block : wrong argument loc : %d", loc);
    return e;
  }
}


error start_disk(char *name,disk_id *id) {
  error e;
  if(name != NULL){
    int boolean =0;
    int i;
    for(i=0;i<MAX_DISQUE;i++){
      if(disque_ouvert[i]!=NULL){
        if(strcmp((disque_ouvert[i])->name,name)==0){
            boolean =1;
            break;
          }
      }
    }
    if(boolean==0){
      int f = open(name,O_RDWR);
      if(f != -1){
        int nbrcurs =0;
        while(disque_ouvert[nbrcurs]!=NULL && nbrcurs!=MAX_DISQUE){
          nbrcurs++;
        }
        if(nbrcurs!=MAX_DISQUE){
          if(id !=NULL){
            disque_ouvert[nbrcurs]=id;
            id->id = nbrcurs;
            id->fd=f;
            id->nbBlock=1;
            block first;
            read_block((*id),&first,0);
            uint32_t n;
            unsigned char *tab=(unsigned char *)(&n);
            for(i=0;i<4;i++){
              tab[i]=first.buff[i];
            }
            id->nbBlock = little_to_int(n);
            for(i=0;i<4;i++){
              tab[i]=first.buff[i+4];
            }
            id->nbPart=little_to_int(n);
            if(id->nbPart !=0){
	      uint32_t id_first=1;
              for(i=0;i<id->nbPart;i++){
                int j;
                for(j=0;j<4;j++){
                  tab[j]=first.buff[j+8+4*i];
                }
		Part p;
		p.taille=little_to_int(n);
		p.num_first_block=id_first;
		id_first+=p.taille;
		block firstPart;
		read_block(*id,&firstPart,p.num_first_block);
		int free_block_count;  
		readint_block(&firstPart,&free_block_count,12);
		p.free_block_count=free_block_count;
		int first_free_block;
		readint_block(&firstPart,&first_free_block,16);
		p.first_free_block= first_free_block;
		int max_file_count;
		readint_block(&firstPart,&max_file_count,20);
		p.max_file_count=max_file_count;
		int free_file_count;
		readint_block(&firstPart,&free_file_count,24);
		p.free_file_count=free_file_count;
		p.file_table_size = ((max_file_count-1)/16)+1;
		int first_free_file;
		readint_block(&firstPart,&first_free_file,28);
		p.first_free_file=first_free_file;
		p.file_table_size = (max_file_count/16)+1;
                id->tabPart[i]=p;
              }
            }
            id->name = malloc(strlen(name)*sizeof(char));
            id->name= name;
            disque_ouvert[nbrcurs]=id;
            e.errnb = 0;
          }
          else{
            e.errnb = -1;
            fprintf(stderr,"le pointeur de disk_id passé en argument est NULL\n");
          }
        }
        else{
          e.errnb =-1;
          fprintf(stderr,"il y a trop de disque ouvert\n");
        }
      }
      else{
        e.errnb = -1;
        fprintf(stderr,"le disque ne peut pas s'ouvrir\n");
      }
    }
    else{
      e.errnb = -1;
      fprintf(stderr,"le disque est déjà ouvert\n");
    }
  }
  else{
    e.errnb = -1;
    fprintf(stderr,"ajouter un nom de disque\n");
  }
  return e;
}
error read_block(disk_id id,block *b,uint32_t num){
  return read_physical_block(id,b,num);
}

error write_block(disk_id id,block b,uint32_t num){
  return write_physical_block(id,b,num);
}

error sync_disk(disk_id id){
  error e;
  return e;
}

error stop_disk(disk_id id){
  error er;
  if(disque_ouvert[id.id]!=NULL){
    er.errnb = close(id.fd);
    if(er.errnb != -1){
      id.name = NULL;
      free(id.name);
      disque_ouvert[id.id]=NULL;
    }
  }
  return er;
}

#include "interne.h"


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
    printf("checkpoint1\n");
    if(boolean==0){
        //FILE *f =fopen(name,"r+");
        int f = open(name,O_RDWR);
        printf("checkpoint2\n");
        if(f != -1){
            int nbrcurs =0;
            while(disque_ouvert[nbrcurs]!=NULL && nbrcurs!=MAX_DISQUE){
                nbrcurs++;
            }
            printf("checkpoint3 nbrcurs %d\n",nbrcurs);
            if(nbrcurs!=MAX_DISQUE){
                if(id !=NULL){
                    printf("Je suis dans la boucle d'allocation d'id\n");
                    id->name = malloc(strlen(name)*sizeof(char));
                    id->id = nbrcurs;
                    printf("Je suis dans la boucle d'allocation d'id id  %d\n",id->id);
                    id->fd = f;
                    printf("Je suis dans la boucle d'allocation d'id fd %d\n",id->fd);
                    id->name= name;
                    printf("Je suis dans la boucle d'allocation d'id name %s\n",id->name);
                    disque_ouvert[nbrcurs]=id;
                    printf("disque_ouvert[nbrcurs] -> fd %d\n",(*disque_ouvert[nbrcurs]).fd);
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
    error er;
    if(disque_ouvert[id.id]!=NULL){
        //id.file = disque_ouvert[id.id]->file;
        printf("Je vais close\n");
        er.errnb = close(id.fd);
        printf("J'ai close\n");
        if(er.errnb != -1){
            /*printf("Je vais free name\n");
            //free(id.name);
            printf("Je vais free disque_ouvert[id.id]\n");
            free(disque_ouvert[id.id]);
            printf("Je vais disque_ouvert[id.id]=NULL\n");*/
            disque_ouvert[id.id]=NULL;
        }
    }
    return er;
}

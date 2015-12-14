#include "interne.h"

error string_to_uint (char *s, uint32_t u){
  if(strlen(s)!=4){
    error e;
    return e;
  }else{
    u=s[3]+s[2]*256+s[1]*65636+s[0]*16777216;
    error e;
    return e;
  }
}

error read_physical_block(disk_id id,block b,uint32_t num){
  if(id.id>0 && id.id<MAX_DISQUE){
    if(disque_ouvert[id.id]!=NULL){
      FILE *f = fopen(disque_ouvert[id.id],"r");
      if (f!=NULL){
	uint32_t nb_block;
	unsigned char* u=(unsigned char*)(&nb_block);
	fseek(f,0,SEEK_SET);
	u[0]=fgetc(f);
	u[1]=fgetc(f);
	u[2]=fgetc(f);
	u[3]=fgetc(f);
	if(num<nb_block){
	  fseek(f,num*1024,SEEK_SET);
	  int i;
	  for(i=0; i<1024; i++){
	    b.b[i]=fgetc(f);
	  }
	  fclose(f);
	  error e;
	  e.errnb=0;
	  return e;
	}
	error e;
	printf("wrong argument num : %d \n", num ) ;
	e.errnb=-1;
	return e;
      }
    }
  }
  error e;
  printf("no disk with id %d \n", id.id);
  e.errnb=-1;
  return e;
}


error write_physical_block(disk_id id,block b,uint32_t num){
  if(id.id>0 && id.id<MAX_DISQUE){
    if(disque_ouvert[id.id]!=NULL){
      FILE *f = fopen(disque_ouvert[id.id],"w+");
      if (f!=NULL){
          //Recupère le nb de block dans le disk
          uint32_t nb_block;
          unsigned char* o = (unsigned char*)(&nb_block);
          fseek(f,0,SEEK_SET);
          o[0]=fgetc(f);
          o[1]=fgetc(f);
          o[2]=fgetc(f);
          o[3]=fgetc(f);
          if(num<nb_block){
              fseek(f,num*1024,SEEK_SET);
              fputs(b.b,f);
              fclose(f);
              error e;
              e.errnb=0;
              return e;
          }
          error e;
          printf("wrong argument num : %d \n", num ) ;
          e.errnb=-1;
          return e;
      }
    }
  }
  error e;
  printf("no disk with id %d \n", id.id);
  e.errnb=-1;
  return e;
}

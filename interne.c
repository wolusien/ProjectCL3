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
	uint32_t nb_block=(fgetc(f)|fgetc(f)|fgetc(f)|fgetc(f)); // !!! CONVERSION LITTLE INDIAN
	if(num<nb_block){
	  fseek(f,num*1024,SEEK_SET);
	  int i;
	  for(i=0; i<1024; i++){
	    b.b[i]=fgetc(f);
	    //printf("%c",b.b[i]);
	  }
	  fclose(f);
	  error e;
	  return e;
	}
      }
    }
    error e;
    return e;
  }
  error e;
  return e;
}
error write_physical_block(disk_id id,block b,uint32_t num){
  FILE *f = fopen(disque_ouvert[id.id],"w+");
  fseek(f,num*1024,SEEK_SET);
  fputs(b.b,f);
  fclose(f);
  error e;
  return e;
}

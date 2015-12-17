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
    error e;
    if(id.id>=0 && id.id<MAX_DISQUE){
        if(disque_ouvert[id.id]!=NULL){
            int f = disque_ouvert[id.id]->fd;
            if (f!=-1){
                uint32_t nb_block;
                unsigned char* u = (unsigned char*)(&nb_block);
                lseek(f,0,SEEK_SET);
                /*u[0]=fgetc(f);
                u[1]=fgetc(f);
                u[2]=fgetc(f);
                u[3]=fgetc(f);*/
                read(f,u,4);
                if(num<=50//nb_block
                    ){
                    lseek(f,num*1024,SEEK_SET);
                    read(f,b.buff,1024);
                    e.errnb=0;
                    return e;
                }else{
                    error e;
                    printf("wrong argument num : %d \n", num ) ;
                    e.errnb=-1;
                    return e;
                }
            }
        }
    }
    printf("no disk with id %d \n", id.id);
    e.errnb=-1;
    return e;
}


error write_physical_block(disk_id id,block b,uint32_t num){
    error e;
    if(id.id>=0 && id.id<MAX_DISQUE){
        if(disque_ouvert[id.id]!=NULL){
            int f = id.fd;//disque_ouvert[id.id]->fd;
            if (f!=-1){
                //Recupère le nb de block dans le disk
                uint32_t nb_block;
                unsigned char* o = (unsigned char*)(&nb_block);
                lseek(f,0,SEEK_SET);
                /*o[0]=fgetc(f);
                o[1]=fgetc(f);
                o[2]=fgetc(f);
                o[3]=fgetc(f);*/
                read(f,o,4);
                if(num<=50//nb_block
                    ){
                    lseek(f,num*1024,SEEK_SET);
                    write(f,b.buff,f);
                    e.errnb=0;
                    return e;
                }else{
                    printf("wrong argument num : %d \n", num ) ;
                    e.errnb=-1;
                    return e;
                }
            }
        }
        printf("no disk with id %d open \n", id.id);
        e.errnb=-1;
        return e;
    }else{
        e.errnb=-1;
        return e;
    }
}

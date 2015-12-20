#include "interne.h"

error read_physical_block(disk_id id,block b,uint32_t num){
    error e;
    if(id.id>=0 && id.id<MAX_DISQUE){
        if(disque_ouvert[id.id]!=NULL){
            int f = disque_ouvert[id.id]->fd;
            if (f!=-1){
                //Recupère le nb de block dans le disk
                uint32_t nb_block;
                unsigned char* u = (unsigned char*)(&nb_block);
                lseek(f,0,SEEK_SET);
                read(f,u,4);
                if(num<=nb_block){
                    lseek(f,num*1024,SEEK_SET);
                    read(f,b.buff,1024);
                    e.errnb=0;
                    return e;
                }else{
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
            int f = id.fd;
            if (f!=-1){
                //Recupère le nb de block dans le disk
                uint32_t nb_block;
                unsigned char* o = (unsigned char*)(&nb_block);
                lseek(f,0,SEEK_SET);
                read(f,o,4);
                if(num<=nb_block){
                    lseek(f,num*1024,SEEK_SET);
                    write(f,b.buff,1024);
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

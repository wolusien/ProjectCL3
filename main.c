#include "interne.h"

int main (int argc, char * argv[]){
    printf("Je suis au début du code du main\n");
    disk_id *id;
    char *path = "disk.tfs";
    id = malloc(sizeof(disk_id));
    printf("Je vais exécuter startdisk\n"); 
    start_disk(path,id);
    int i;
    for(i=0; i<5;i++){
        printf("tableau disque_ouvert %d\n",disque_ouvert[i]);
    }
    block b;
    uint32_t num = int_to_little(1);
    uint32_t n = int_to_little(1708);
    unsigned char* o = (unsigned char*)(&n);
    for(i=0; i<4; i++){
        b.buff[i]=o[i];
    }
    for(i=4; i<1024; i++){
        b.buff[i]='M';
    }
    b.id=0;
    printf("Je vais exécuter write_physical\n");
    printf("fd contient %d et le vrai fd est %d\n",(*id).fd, open("disk.tfs",O_CREAT,744));
    write(3,b.buff,1024);
    //write_physical_block((*id),b,num);
    printf("J'ai exécuter write_physical\n");
    printf("Je vais exécuter stopdisk\n");
//    stop_disk((*id));
    /*printf("Je vais exécuter startdisk\n"); 
    start_disk(path,id);
    block bis;
    bis.id=1;
    printf("Je vais exécuter read_physical\n");
    read_physical_block((*id),bis,num);
    printf("J'ai exécuter read_physical\n");
    uint32_t nread;
    unsigned char* oread = (unsigned char*)(&nread);
    for(i=0; i<4; i++){
        oread[i]=bis.buff[i];
    }
    printf("Voici votre nombre %d\n",int_to_little(nread));
    printf("Je vais exécuter stopdisk\n");
    stop_disk((*id));*/
    exit(0);
}


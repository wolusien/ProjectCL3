#include "interne.h"

int main (int argc, char * argv[]){
    disk_id *id;
    int i;
    char *path = "disk.tfs";
    id = malloc(sizeof(disk_id)); 
    start_disk(path,id);
    block b;
    uint32_t num = int_to_little(2);
    uint32_t n;
    unsigned char* o = (unsigned char*)(&n);
    n = int_to_little(178);
    for(i=0; i<4; i++){
        b.buff[i]=o[i];
    }
    b.id=0;
    write_physical_block((*id),b,num);
    block bis;
    bis.id=1;
    read_physical_block((*id),&bis,num);
    uint32_t nread;
    unsigned char* oread = (unsigned char*)(&nread);
    for(i=0; i<4; i++){
            oread[i]=bis.buff[i];
    }
    printf("Voici votre nombre %d\n",int_to_little(nread));
    printf("Je vais exécuter stopdisk\n");
    stop_disk((*id));
    exit(0);
}

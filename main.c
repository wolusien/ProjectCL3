#include "interne.h"

int main (int argc, char * argv[]){
    printf("Je suis au début du code du main\n");
    disk_id *id;
    char *path = "disk.tfs";
    id = malloc(sizeof(disk_id));
    printf("Je vais exécuter startdisk\n"); 
    error start=start_disk(path,id);
    block b;
    uint32_t num = int_to_little(0);
    uint32_t n = int_to_little(1708);
    unsigned char* o = (&n);
    int i;
    for(i=0; i<4; i++){
        b.b[i]=o[i];
    }
    b.id=0;
    printf("Je vais exécuter write_physical\n");
    write_physical_block((*id),b,num);
    printf("J'ai exécuter write_physical\n");
    block bis;
    bis.id=1;
    printf("Je vais exécuter read_physical\n");
    read_physical_block((*id),bis,num);
    printf("J'ai exécuter read_physical\n");
    uint32_t nread;
    unsigned char* oread = (&nread);
    for(i=0; i<4; i++){
        oread[i]=bis.b[i];
    }
    printf("Voici votre nombre %d\n",int_to_little(nread));
    printf("Je vais exécuter stopdisk\n");
    error stop=stop_disk((*id));
    exit(0);
}


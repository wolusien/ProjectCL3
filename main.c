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
    uint32_t n = int_to_little(17);
    unsigned char* o = (&num);
    int i;
    for(i=0; i<4; i++){
        b.b[i]=o[i];
    }
    b.id=0;
    printf("Je vais exécuter write_physical\n");
    write_physical_block((*id),b,num);
    printf("Je vais exécuter read_physical\n");
    read_physical_block((*id),b,num);
    for(i=0; i<4; i++){
        o[i]=b.b[i];
    }
    printf("Voici votre nombre %d\n",int_to_little(n));
    printf("Je vais exécuter stopdisk\n");
    error stop=stop_disk((*id));
    exit(0);
}


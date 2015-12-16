#include "interne.h"

int main (int argc, char * argv[]){
    printf("Je suis au début du codedu main\n");
    disk_id *id;
    id = malloc(sizeof(disk_id));
    char *path = "disk.tfs"; 
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
    //error stop=stop_disk((*id));
    exit(0);
}


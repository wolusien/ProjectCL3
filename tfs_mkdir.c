#include "manipsystem.h"

int main(int argc,char *argv[]){
    if(argc==2){
        char *path = argv[1];
        iter i = decomposition(path);
        if(strcmp(i->name,"FILE:")!=-1){//on verifie que le chemin commence par path
            if(i->next != NULL){
                i=i->next;
                if(strcmp(i->name,"HOST")==-1){
                    char *name = i->name;
                    free_iter(i);
                    disk_id *id = malloc(sizeof(disk_id));
                    error er =start_disk(name,id);
                    if(er.errnb != -1){
                        int a = mkdir(path,O_CREAT | O_RDWR);
                        stop_disk(*id);
                        return a;
                    }
                    else{
                        free(id);
                        printf("can't start disk");
                        return -1;
                    }
                }
            }
            else{
                printf("wrong path");
            }
        }
        else{
            printf("wrong path");
            exit(-1);
        }
    }
}
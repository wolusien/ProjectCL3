/**
 * \file mkdir.c
 * \brief fonction mkdir systeme et bloc(pas completement fonctionnel)
 * \author DJEBALI LABADENS HUMBERT
 */
#include "manipsystem.h"
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc == 2) {
        //printf("%s",argv[1]);
        char path[strlen(argv[1])]; 
        int i;
        for(i=0;i<strlen(argv[1]);i++){
            path[i]=argv[1][i];
        }
        iter it = decomposition(path);
        if (strcmp(it->name, "FILE:") != -1) {//on verifie que le chemin commence par path
            if (it->next != NULL) {
                it = it->next;
                if (strcmp(it->name, "HOST") !=0) {
                    char *name = it->name;
                    disk_id *id = malloc(sizeof (disk_id));
                    error er = start_disk(name, id);
                    if (er.errnb != -1) {
                        int a = tfs_mkdir(argv[1], O_CREAT | O_RDWR);
                        stop_disk(*id);
                        return a;
                    } else {
                        free(id);
                        printf("can't start disk\n");
                        return -1;
                    }
                    
                } else {
                    
                    if(it->next != NULL) {
                        it = it->next;
                        while (it->next != NULL) {
                            if (chdir(it->name) != -1) {
                                it = it->next;
                            } else {
                                fprintf(stderr,"can't\n");
                                return -1;
                            }
                        }
                        int a = mkdir(it->name, 0777);
                        if(a==-1){
                            printf("name %s already exist \n",it->name);
                        }
                        return a;
                    }
                }

            } else {
                fprintf(stderr,"wrong path\n");
            }
        } else {
            fprintf(stderr,"wrong path\n");
            exit(-1);
        }
    }
    fprintf(stderr,"wrong number arguments\n");
    return -1;
}

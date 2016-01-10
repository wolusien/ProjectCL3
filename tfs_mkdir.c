#include "manipsystem.h"
#include <sys/stat.h>
int main(int argc, char *argv[]) {
    if (argc == 2) {
        char *path = argv[1];
        iter i = decomposition(path);
        if (strcmp(i->name, "FILE:") != -1) {//on verifie que le chemin commence par path
            if (i->next != NULL) {
                i = i->next;
                if (strcmp(i->name, "HOST") == -1) {
                    char *name = i->name;
                    free_iter(i);
                    disk_id *id = malloc(sizeof (disk_id));
                    error er = start_disk(name, id);
                    if (er.errnb != -1) {
                        int a = mkdir(path, O_CREAT | O_RDWR);
                        stop_disk(*id);
                        return a;
                    } else {
                        free(id);
                        printf("can't start disk");
                        return -1;
                    }
                } else {
                    
                    if (i->next != NULL) {
                        i = i->next;
                        while (i->next != NULL) {
                            if (chdir(i->name) != -1) {
                                i = i->next;
                            } else {
                                fprintf(stderr,"can't");
                                return -1;
                            }
                        }
                        int a = mkdir(i->name, 0777);
                        if(a==-1){
                            printf("name %s already exist \n",i->name);
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
    fprintf(stderr,"wrong number arguments");
    return -1;
}

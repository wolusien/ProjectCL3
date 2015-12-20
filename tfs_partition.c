#include "interne.h"

int disk_exist(char* disk){
    char* buffer = malloc(1024*sizeof(char));
    struct stat* buffer_Stat = malloc(sizeof(struct stat));
    DIR* directory_Parent = opendir(getcwd(buffer,1024));
    struct dirent* directory_Fils=NULL;
    while((directory_Fils=readdir(directory_Parent))){
        char* name_File=(*directory_Fils).d_name; 
        printf("%s\n", name_File);    
    }
    return closedir(opendir(getcwd(buffer,1024)));
}


int main(int argc, char* argv[]){
    /*int i;
    int open = -10;
    int count=0;
    int partition[argc - 4];
    if(argc>=5){
        //Test the argument of the cmd tfs_partition
        if(strcmp("tfs-partition",argv[0])==0){
            if(strcmp("-p",argv[1])==0){
                if(strcmp("size",argv[2])==0){
                    char* disk = argv[argc-1];
                    int l = strlen(disk);
                    //Test disk syntax name
                    if(l>=5){
                        if(disk[l-4]='.' && disk[l-3]='t' && disk[l-2]='f'&& disk[l-1]='s'){
                            //Search if the disk given by argv[argc-1] is open or not
                            for(i=1; i<MAX_DISQUE; i++){
                                if(strcmp((*disque_ouvert[i]).name,argv[argc-1])==0){
                                    open=i;
                                    break;
                                }
                                if(open==-10){
                                    disk_id* disk = malloc(sizeof(disk_id));
                                    startdisk(argv[argc-1],disk);
                                }
                                switch(argc){
                                case 5 : partition[count]=atoi(argv[3]); 
                                
                                }
                            }
                        } 
                    }
                }
            }
        } 
    }else{
        
        exit(-1);
        }*/
    disk_exist("Nul");
    exit(0);
}

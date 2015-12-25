#include "manip.h"

int main(int argc, char* argv[]){
  error e;
  int i;
  int used_space=0;
  if(argc==2){
    disk_id* disk = malloc(sizeof(disk_id));
    e = start_disk(argv[argc-1],disk);
    if(e.errnb!=-1){
      fprintf(stdout,"\n\t\t\t\t\tDisk Configuration : %s\n",argv[argc-1]);
      if((*disk).nbPart>0){
        fprintf(stdout,"Partition number : %d\n\n\tList of partitions : \n",(*disk).nbPart);
        for(i=0; i<(*disk).nbPart;i++){
          used_space += (*disk).taillePart[i];
          fprintf(stdout,"Partition %d : \t%dB\n",(i+1),(*disk).taillePart[i]);
        }
      }
      int free_space = (*disk).nbBlock - used_space;
      fprintf(stdout,"\nDisk : %s\n\t\tBlocks number : %d\tUsed blocks : %d\t\tFree blocks : %d\n",argv[argc-1],(*disk).nbBlock,used_space,free_space);
      stop_disk(*disk);
      exit(0);
    }else{
      exit(-1);
    }
  }else{
    fprintf(stderr,"tfs_analyse : Wrong arguments number\n");
    exit(-1);
  }
}

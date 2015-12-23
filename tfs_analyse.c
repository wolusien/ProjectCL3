#include "manip.h"

int main(int argc, char* argv[]){
  error e;
  int i;
  int used_space=0;
  if(argc==2){
    disk_id* disk = malloc(sizeof(disk_id));
    e = start_disk(argv[argc-1],disk);
    if(e.errnb!=-1){
      fprintf(stdin,"Disk Configuration : %s\n",argv[argc-1]);
      if((*disk).nbPart>0){
        fprintf(stdin,"\t\t\tPartition number : %d\nList of partitions : \n",(*disk).nbPart);
        for(i=0; i<(*disk).nbPart;i++){
          used_space += (*disk).taillePart[i];
          fprintf(stdin,"Partition %d : \t%dB\n",i,(*disk).taillePart[i]);
        }
      }
      int free_space = (*disk).nbBlock - used_space;
      fprintf(stdin,"\nDisk : %s\n\t\tBlocks number : %d\tUsed blocks : %d\tFree blocks : %d\n",argv[argc-1],(*disk).nbBlock,used_space,free_space);
      e = stop_disk(*disk);
    }else{
      exit(-1);
    }
  }
  exit(0);
}

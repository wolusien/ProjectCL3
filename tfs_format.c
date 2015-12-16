#include "interne.h"

int main(int argc, char *argv[]){
  if(argc==6){
    disk_id *id=malloc(sizeof(disk_id));
    error e = start_disk(argv[argc-1], id);
    if(e.ernum==0){
      
    }else{
      return e;
    }
  }else{
    error e;
    e.ernum=-1;
    printf("tfs format : wrong arguments number %d", (argc-1));
    return e;
  }
}



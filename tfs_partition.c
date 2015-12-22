#include "interne.h"


int main(int argc, char* argv[]) {
  error e;
  int i;
  int boolean = 0;
  int nbBlock = 0;
  int count = 0;
  int partition[argc - 4];
  
  if (argc >= 5) {
    //Test the argument of the cmd tfs_partition
    if (strcmp("-p", argv[1]) == 0) {
      if (strcmp("size", argv[2]) == 0) {          
        //Search if the disk given by argv[argc-1] is open or not
        disk_id* disk = malloc(sizeof (disk_id));
        //Test if the disk given by argv[argc-1] is open or not
        e = start_disk(argv[argc - 1], disk);
        if (e.errnb == 0) {
          //Read the first block of the disk
          block b;
          uint32_t num = int_to_little(0);
          read_physical_block((*disk), (&b), num);
          //Get size of partitions and fill the tab partition
          for (i = 2; i < argc - 1; i++) {
            if(strcmp("-p",argv[i])==0){
              boolean = 1;
            }else{
              if(boolean = 1){
                int psize = atoi(argv[i]);
                if (psize != 0 && psize > 4 && psize < ((*disk).nbBlock-1) && nbBlock < ((*disk).nbBlock-1)){
                  nbBlock = nbBlock + psize; 
                  partition[count] = psize;
                  count++;
                  boolean=0;
                }else{
                  fprintf(stderr,"Wrong arguments for partition(s) size(s) %d\n",psize);
                  exit(-1);
                }
				
              }
            }
          }
          if (count > 0) {
            //Case where we have good partition
            (*disk).nbPart = count;
            for (i = 0; i < count; i++) {
              (*disk).taillePart[i] = partition[i];
            }
            uint32_t nbPart = int_to_little(count);
            unsigned char* nb = (unsigned char*) (&nbPart);
            //File the block b with the number of partition
            for (i = 0; i < 4; i++) {
              b.buff[i+4] = nb[i];
            }
            int j = 8;
            for (i = 0; i < count; i++) {
              int k;
              uint32_t p = int_to_little(partition[i]);
              unsigned char* pc = (unsigned char*) (&p);
              for (k = 0; k < 4; k++) {
                b.buff[j] = pc[k];
                j++;
              }
            }
            write_physical_block((*disk), b, num);
            stop_disk(*disk);
          } else {
            fprintf(stderr,"Aucun partitionnage du disque possible, mauvaise dimension de partition!\n");
            exit(-1);
          }
        } else {
          exit(-1);
        }
      } else {
        fprintf(stderr,"tfs_partition : Wrong syntax\n");
        exit(-1);
      }
    } else {
      fprintf(stderr,"tfs_partition : Wrong syntax\n");
      exit(-1);
    }
  } else {
    fprintf(stderr,"tfs_partition : Wrong arguments number\n");
    exit(-1);
  }
  exit(0);
}


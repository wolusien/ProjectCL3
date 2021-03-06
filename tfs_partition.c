#include "manip.h"

/**
 *\fn int main(int argc, char* argv[])
 *\brief Command tfs_partition Create partitions on disk given on tab argv 
 *\param argc Integer represents strings number on argv 
 *\param argv Array of string
 *\return  Integer represents the result of the command
 **/
int main(int argc, char* argv[]) {
  error e;
  int i;
  int boolean = 0;
  int nbBlock = 0;
  int count = 0;
  int ininbPart;
  int partition[argc - 4];
  
  if (argc >= 3 && argc%2==0) {
    //Test the argument of the cmd tfs_partition
    if (strcmp("-p", argv[1]) == 0) {
      //Search if the disk given by argv[argc-1] is open or not
      disk_id* disk = malloc(sizeof (disk_id));
      //Test if the disk given by argv[argc-1] is open or not
      e = start_disk(argv[argc - 1], disk);
      if (e.errnb == 0) {
        //Read the first block of the disk
        block b;
        uint32_t num = int_to_little(0);
        read_physical_block((*disk),&b, num);
        //Get size of partitions and fill the tab partition
        for (i = 1; i < argc - 1; i++) {
          if(strcmp("-p",argv[i])==0){
            boolean = 1;
          }else{
            if(boolean == 1){
              int psize = atoi(argv[i]);
              if (psize != 0 && psize > 4 && psize < ((*disk).nbBlock-1) && nbBlock < ((*disk).nbBlock-1)){
                nbBlock = nbBlock + psize; 
                partition[count] = psize;
                count++;
                boolean=0;
              }else{
                fprintf(stderr,"tfs_partition : Wrong size for partition size %d\n",psize);
                exit(-1);
              }
              
            }else{
              fprintf(stderr,"tfs_partition : Wrong arguments for partition(s) size(s) %s\n",argv[i]);
              exit(-1);
            }		
          }
        }
        if (count > 0) {
          ininbPart = (*disk).nbPart;
          //Case where we have good partition
          (*disk).nbPart = count + ininbPart;
          for (i = 0; i < count; i++) {
	    Part p;
	    p.taille=partition[i];
            (*disk).tabPart[i + ininbPart] = p;
          }
          nbBlock = 0;
          for (i = 0; i < (*disk).nbPart; i++) {
            nbBlock += (*disk).tabPart[i].taille;
          }
          if(nbBlock < (*disk).nbBlock-1){
            uint32_t nbPart = int_to_little(count + ininbPart);
            unsigned char* nb = (unsigned char*) (&nbPart);
            //File the block b with the number of partition
            for (i = 0; i < 4; i++) {
              b.buff[i+4] = nb[i];
            }
            int j = (ininbPart*4) + 8;
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
            free(disk);
          }else{
            fprintf(stderr,"tfs_partition : No enough free blocks avalaible in %s\n",argv[argc -1]);
            exit(-1);
          }
        } else {
          fprintf(stderr,"tfs_partition : No size for partition \n");
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
    fprintf(stderr,"tfs_partition : Wrong arguments number\n");
    exit(-1);
  }
  exit(0);
}

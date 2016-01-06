#include "manip.h"

int main(int argc, char* argv[]){
  error e;
  int i;
  int used_space=0;
  if(argc >= 2){
    disk_id* disk = malloc(sizeof(disk_id));
    e = start_disk(argv[argc-1],disk);
    if(e.errnb!=-1){
			switch(argc){
				case 2 : 
					fprintf(stdout,"\n\t\t\t\t\tDisk Configuration : %s\n",argv[argc-1]);
					if((*disk).nbPart>0){
						fprintf(stdout,"Partition number : %d\n\n\tList of partitions : \n",(*disk).nbPart);
						for(i=0; i<(*disk).nbPart;i++){
							used_space += (*disk).tabPart[i].taille+1;
							fprintf(stdout,"Partition %d : \t%dB\n",i,(*disk).tabPart[i].taille);
						}
					}
					int free_space = (*disk).nbBlock - used_space;
					fprintf(stdout,"\nDisk : %s\n\t\tBlocks number : %d\tUsed blocks : %d\t\tFree blocks : %d\n",argv[argc-1],(*disk).nbBlock,used_space,free_space);
					break;
				case 4 :
					if(strcmp(argv[1],"-p")==0){
						int pnum = atoi(argv[argc-2]);
						if((*disk).nbPart>0 && pnum<(*disk).nbPart && pnum>-1 ){
							fprintf(stdout,"\n\t\t\t\t\tPartition %d Configuration on %s : \n",pnum,argv[argc-1]);
							Part p = (*disk).tabPart[pnum];
							fprintf(stdout,"Size of partition : %d B\n\n",p.taille);
							fprintf(stdout,"\tNumber of the description block of the partition on disk : Block N° %d\n\n",p.num_first_block);
							fprintf(stdout,"\tSize of free blocks in partition : %dB\n\n",p.free_block_count);
							fprintf(stdout,"\tNumber of the first free block in the partition : Block N° %d\n\n",p.first_free_block);
							fprintf(stdout,"\t\tSize of the files Table in partiton : %dB\n",p.file_table_size);
							fprintf(stdout,"\t\tMaximum number of file in partition : %d\n",p.max_file_count);
							fprintf(stdout,"\t\tNumber of free file in partition : %d\n",p.free_file_count);
							fprintf(stdout,"\t\tNumber of the first free file in partition : %d\n\n",p.first_free_file);
						}
					}else{
						fprintf(stderr,"tfs_analyse : Wrong arguments for partition option. Do you mean -p instead of %s\n",argv[1]);
						exit(-1);
					}
					break;
			}
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

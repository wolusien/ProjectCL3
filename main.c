//#include "manip.h"
#include "manipvolume.h"
int main (int argc, char * argv[]){

  disk_id *id = malloc(sizeof(disk_id));
  error e = start_disk("disk.tfs",id);
  if(e.errnb != -1){
	error e1 = test_file(id,0,"pd");
	if(e1.errnb != -1){
		//printf("La création du disk a réussi\n");
		error e3 = remove_file_block(id,0,1);
			if(e3.errnb != -1){
				printf("Le remove_file block a réussi\n");	
			}else{
				printf("Le remove_file block a échoué\n");	
			}	   
	}else{
		printf("La création du disk a échoué\n");
	}
  }else{
	printf("Le start_disk a échoué\n");  
  }
  // use_block(disque_ouvert[0], 14,  0);
  //use_block(disque_ouvert[0], 6,  0);
  //use_block(disque_ouvert[0], 8,  0);
  //use_block(disque_ouvert[0], 14,  0);
  //free_block(disque_ouvert[0], 14,  0);
  /*printf("huhuhuhuh : \n");
  disk_id *id = malloc(sizeof(disk_id));  
  start_disk("t.tfs",id);
  printf("taille : %d \n",id->tabPart[0].max_file_count);
  use_block(disque_ouvert[0], 3 ,  0);
  use_block(disque_ouvert[0], 14,  0);
  use_block(disque_ouvert[0], 6,  0);
  use_block(disque_ouvert[0], 8,  0);
  use_block(disque_ouvert[0], 14,  0);
  free_block(disque_ouvert[0], 14,  0);
  free_block(disque_ouvert[0], 15,  0);
  free_block(disque_ouvert[0], 14,  0);
  */
  // int f = open("tii.tfs",O_RDWR);
  // read(f,b->buff,1024);
  //printf("kjb %d \n", b->buff[0]);
  // printf("kjb %d \n", b->buff[0]);
  //disk_id *id = malloc(sizeof(disk_id));
  //start_disk("t.tfs",id);
  //printf("dudu : %d \n", id->tabPart[1].max_file_count);
  //stop_disk(*id);
  exit(0);
  }
   

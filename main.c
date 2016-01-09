//#include "manip.h"
#include "manipsystem.h"
int main (int argc, char * argv[]){

    disk_id *id = malloc(sizeof(disk_id));
    start_disk("disk.tfs",id);
    mkdir("FILE://disk.tfs//0//lalallala",O_CREAT);
    stop_disk(*id);
}
  /*
  char path[100]="je//suis//pas";
  iter i= decomposition(path);
  go_start(i);
  go_end(i);
  free_iter(i);
  exit(0);
  }*/

  

/*

  if(e.errnb != -1){
	error er =test_file(id,0,"pd");

	  block bl;
	  read_block(*id, &bl,int_to_little(6));
	  char aa[28];
	  readname_rep(bl, aa, 0 );
	  printf("ggggg  %s \n",aa);

		const char * a="FILE://t.tfs/0/pj";
	const char * b="OOOOOOOOOOOOO";
	tfs_rename(a, b);

  }else{
	printf("Le start_disk a échoué\n");  
  }
}*/
  // use_block(disque_ouvert[0], 14,  0);
  //use_block(disque_ouvert[0], 6,  0);
  //use_block(disque_ouvert[0], 8,  0);
  //use_block(disque_ouvert[0], 14,  0);
  //free_block(disque_ouvert[0], 14,  0);
  
  /*printf("huhuhuhuh : \n");
  printf("huhuhuhuh : \n");
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
  //disk_id *id;
  /*int i;
  char *path = "disk.tfs";
  id = malloc(sizeof(disk_id)); 
  start_disk(path,id);
  block b;
  uint32_t num = int_to_little(2);
  uint32_t n;
  unsigned char* o = (unsigned char*)(&n);
  n = int_to_little(178);
  for(i=0; i<4; i++){
    b.buff[i]=o[i];
  }
  b.id=0;
  write_physical_block((*id),b,num);
  block bis;
  bis.id=1;
  read_physical_block((*id),bis,num);
  uint32_t nread;
  unsigned char* oread = (unsigned char*)(&nread);
  for(i=0; i<4; i++){
    oread[i]=bis.buff[i];
  }
  printf("Voici votre nombre %d\n",int_to_little(nread));
  printf("Je vais exécuter stopdisk\n");
  stop_disk((*id));
  exit(0);

  stop_disk((*id));
}
 */  

#include "interne.h"

int main(int argc, char *argv[]){
  if(argc==6){
    if(strcmp(argv[1], "-p")==0){
      if(strcmp(argv[1], "-mf")==0){
	disk_id *id=malloc(sizeof(disk_id));
	error err = start_disk(argv[argc-1], id);
	if(err.errnb==0){
	  int partition =atoi(argv[2]);
	  if(0<partition && partition<=id->nbPart){
	    int file_count=atoi(argv[4]);
	    int taille=id->taillePart[file_count-1];
	    int taille_descripteur=file_count/16;
	    if(0<file_count && taille>(1+taille_descripteur+file_count)){ //on vérifie qu'il y a bien assez de blocs dans la partition pour y mettre file_count fichiers.
	      block b;
	      b.id=0;
	     
	      fill_block(&b, 31534654, 0);
	      fill_block(&b, SIZEBLOCK, 4);
	      fill_block(&b, taille, 8);
	      fill_block(&b,taille-1-taille_descripteur , 12);
	      fill_block(&b, taille_descripteur+1, 16);
	      fill_block(&b, taille, 20);
	      fill_block(&b, file_count, 24);
	      //   fill_block(&b, , 28);
	 
  // todo chainage blocs libres. Chainage files libres. dossier racine
	    }else{
	      printf("tfs format : wrong arguments, too much files : %d", file_count);
	    }
	  }else{
	    printf("tfs format : wrong arguments, no partition %d", partition);
	  }
	}
      }else{
	printf("tfs format : wrong arguments, miss -mf");
      }
    }else{
      printf("tfs format : wrong arguments, miss -p");
    }
  }else{
    printf("tfs format : wrong arguments number %d", (argc-1));

  }
  return 0;
}

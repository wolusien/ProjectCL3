#include "manip.h"

/**
 *\fn main(int argc, char *argv[]){
 *\brief on initialise une partition, c'est a dire on remplie son premier block avec toutes les informations nécessaires, puis on construit le file table, avec le chainage des fichiers libres, puis on construit le dossier racine(on ajoute aussi son entrée dans la table des fichiers), et enfin on initialise le chainage des blocks libres.
 **/
int main(int argc, char *argv[]){
  int i; // i pour les for
  if(argc==6){
    if(strcmp(argv[1], "-p")==0){
      if(strcmp(argv[3], "-mf")==0){
	disk_id *id=malloc(sizeof(disk_id));
	error err = start_disk(argv[argc-1], id);
	if(err.errnb==0){
	  int partition =atoi(argv[2]); //la première partition à l'id 0
	  if(0<=partition && partition<=id->nbPart){
	    int file_count=atoi(argv[4]);
	    int taille=id->tabPart[partition].taille;
	    int taille_descripteur=((file_count-1)/16)+1;
	    if(0<file_count && taille>(1+taille_descripteur+file_count)){ //on vérifie qu'il y a bien assez de blocs dans la partition pour y mettre file_count fichiers.
	      //tout les tests sont faits, on commence a initialiser le premier bloc de la partition.
	      block b;
	     	     
	      fill_block(&b, MAGIC_NUMBER, 0);
	      fill_block(&b, SIZEBLOCK, 4);
	      fill_block(&b, taille, 8);
	      fill_block(&b,taille-2-taille_descripteur , 12);
	      fill_block(&b, taille_descripteur+2, 16);
	      fill_block(&b, file_count, 20);
	      fill_block(&b, file_count-1, 24);
	      fill_block(&b,1, 28); //Il y a un fichier 0, le dossier racine.
	      
	      //trouver l'indice du premier block de la partition.
	      uint32_t id_first=1;
	      for(i=0;i<partition; i++){
		id_first+=id->tabPart[i].taille;
	      }
	      write_block(*id, b,int_to_little(id_first)); 
	 	      
	      //initialisation file table
	      block file_table;
	      int k;
	      for(k=0;k<1024;k++){
		file_table.buff[k]='\0';
	      }
		printf("1 %d \n", taille_descripteur);
		printf("2 %d \n", id_first);
	      for(i=1; i<=taille_descripteur; i++){
		if(i==1){
		  fill_block(&file_table, 64, 0);
		  fill_block(&file_table, 1, 4);
		  fill_block(&file_table, 0, 8);
		  fill_block(&file_table, taille_descripteur+1, 12);
		}else{
		  fill_block(&file_table, 16*(i-1)+1, 60);
		}
		int j;
		for(j=1; j<=15; j++){
		  if((16*(i-1)+j)<file_count){
		    fill_block(&file_table, (i-1)*16+j+1, j*64+60);
		  }else{
		    if(16*(i-1)+j==file_count){
		       fill_block(&file_table, (i-1)*16+j, j*64+60);
		    }

		  }
		}
		write_physical_block(*id, file_table,i+id_first);
		printf("aa %d \n", i+id_first);
		for(k=0;k<1024;k++){
		  file_table.buff[k]='\0';
		}
	      }
	      //initialisation du dossier racine
	      block racine;
	      fill_block(&racine,0,0);
	      racine.buff[4]='.';
	      racine.buff[5]='0';
	      fill_block(&racine,0,32);
	      racine.buff[36]='.';
	      racine.buff[37]='.';
	      racine.buff[38]='0';
	      write_block(*id, racine,int_to_little(id_first+taille_descripteur+1));

	      //initialisation des blocs restants (chainage)
	      block reste;
	      for(i= taille_descripteur+2; i<taille;i++){
		int k;
                for(k=0;k<1024;k++){
                  reste.buff[k]='\0';
                }
		if(i!=(taille-1)){
		  fill_block(&reste,i+1, 1020);
		} else{
		  fill_block(&reste,i, 1020);
		}
		write_block(*id, reste,int_to_little(id_first+i));
		fill_block(&reste,0, 1020);
	      }
	      
 	    }else{
	      printf("tfs format : wrong arguments, too much files : %d \n", file_count);
	    }
	  }else{
	    printf("tfs format : wrong arguments, no partition %d \n", partition);
	  }
	}
	stop_disk(*id);
      }else{
	printf("tfs format : wrong arguments, miss -mf \n");
      }
    }else{
      printf("tfs format : wrong arguments, miss -p \n");
    }
  }else{
    printf("tfs format : wrong arguments number %d \n", (argc-1));

  }
  return 0;
}

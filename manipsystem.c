#include "manipsystem.h"

int isNumber(char *buf){
  int boolean=0;
  int i;
  for(i=0;i<strlen(buf);i++){
    if(isdigit((int)buf[i])==0){
      boolean = -1;
      break;
    }
  }
  return boolean;
}

int tfs_mkdir(const char *path, mode_t mode) {
    iter i = decomposition((char *) path);
    if (strcmp("FILE:", i->name) == 0) {
        if (i->next != NULL) {
            i = go_next(i);
            if (strcmp("HOST", i->name)==0) {//dans ce cas on fait les opérations sur le systeme
                return 1;
            }
            char *name = i->name;
            disk_id *id = NULL;
            int j;
            for (j = 0; j < MAX_DISQUE; j++) {//on verifie que le disque est bien entrain de tourner
                if (disque_ouvert[j] != NULL) {
                    if (strcmp(disque_ouvert[j]->name, name) == 0) {
                        id = disque_ouvert[j];
                        break;
                    }
                }
            }
            if (id != NULL) {//le disque est bien ouvert
                if (i->next != NULL) {
                    i = go_next(i);
                    if (isNumber(i->name) == 0) {
                        int volume = atoi(name);
                        if (volume < id->nbPart) {//on verifie si le volume existe sur le disque
                            if (id->tabPart[volume].free_file_count > 0) {//on verifie si on peut ajouter un autre fichier sur ce volume
                                if (id->tabPart[volume].free_block_count != 0) {
                                    i = go_end(i);
                                    char *namedir = i->name; //on recupere le nom du dossier a creer
                                    if (strlen(namedir) <= 27) {
                                        iter ni = go_prec(i);
                                        printf("%s\n",ni->name);
                                        printf("%s\n",ni->prec->name);
                                        ni->next = NULL;
                                  
                                        ni = go_start(ni);
                                        printf("ici");
                                        ni = go_next(ni);
                                        ni = go_next(ni);
                                        int posbloc;
                                        int posinbloc;
                                        error e1;
                                        int place;
                                        if (ni->next != NULL) {// on regarge si le dossier sur lequel on doit creer notre dossier n'est pas le dossier racine
                                            ni = go_next(ni);
                                            printf("la\n");
                                            e1 = find_name(i, *id, volume, &place); //on  recherche le dossier sur lequel on doit creer le fichier
                                            posbloc = id->tabPart[volume].num_first_block + (place - 1) / 16 + 1;
                                            posinbloc = (place % 16)*64;
                                        } else {
                                            posbloc = id->tabPart[volume].num_first_block + 1;
                                            place = 0;
                                            posinbloc = 0;
                                            e1.errnb = 0;
                                        }
                                        if (e1.errnb != -1) {//on regarde si le nom a été trouve
                                            block filet;
                                            read_block(*id, &filet, int_to_little(posbloc));
                                            int a;
                                            readint_block(&filet, &a, posinbloc + 4);
                                            if (a == 1) {//on regarde si il s'agit d'un repertoire
                                                int free_place_dir;
                                                int free_pos_in_bloc;
                                                char libre[32];
                                                for (j = 0; j < 32; j++) {
                                                    libre[j] = '\0';
                                                }
                                                e1 = name_in_dir(*id, volume, place,libre,&free_place_dir, &free_pos_in_bloc);//verifie si il reste de la place pour écrire le nom sur le répertoire
                                                block b2;
                                                if (e1.errnb != -1) {//si c'est le cas on on écris à a cette endroit
                                                    read_block(*id, &b2, id->tabPart[volume].num_first_block + free_place_dir);
                                                    fill_block(&b2, id->tabPart[volume].first_free_file, free_pos_in_bloc);
                                                    for (j = 0; j < strlen(namedir); j++) {
                                                        b2.buff[j + free_pos_in_bloc + 4] = namedir[j];
                                                    }
                                                    b2.buff[free_pos_in_bloc + 4 + strlen(name)] = '0';
                                                    write_block(*id, b2, id->tabPart[volume].num_first_block + free_place_dir);

                                                } else if (id->tabPart[volume].free_block_count >= 2) {//sinon on regarde si il y assez de bloc pour en rajouter un au repertoire et en crée un 
                                                    free_place_dir = id->tabPart[volume].first_free_block;
                                                    use_block(id, free_place_dir, volume);//on utilise un nouveau bloc
                                                    add_file_block(id, volume, place, free_place_dir);//on ajoute ce bloc au repertoire sur lequel on crée un nouveau repertoire
                                                    fill_block(&b2, id->tabPart[volume].first_free_file, 0);//on écrit le numéro du repertoire crée
                                                    for (j = 0; j < strlen(namedir); j++) {
                                                        b2.buff[j + 4] = namedir[j];
                                                    }
                                                    b2.buff[4 + strlen(name)] = '0';
                                                    write_block(*id, b2, id->tabPart[volume].num_first_block + free_place_dir);
                                                } else {
                                                    fprintf(stderr, "volume number %d is full", volume);
                                                    return -1;
                                                }
                                                free_place_dir = id->tabPart[volume].first_free_block;
                                                block rep;
                                                fill_block(&rep,id->tabPart[volume].first_free_file,0);
                                                rep.buff[4]='.';
                                                rep.buff[5]='0';
                                                for(j=6;j<32;j++){
                                                    rep.buff[j]='\0';
                                                }
                                                fill_block(&rep,place,32);
                                                rep.buff[36]='.';
                                                rep.buff[37]='.';
                                                rep.buff[38]='0';
                                                for(j=39;j<1024;j++){
                                                    rep.buff[j]='\0';
                                                }
                                                use_block(id,free_place_dir,volume);
                                                write_block(*id,rep,id->tabPart[volume].num_first_block+free_place_dir);
                                                int num_new_dir = id->tabPart[volume].first_free_file;
                                                remove_free_file(*id,volume,num_new_dir);
                                                posbloc = (num_new_dir-1)/16+id->tabPart[volume].num_first_block+1;
                                                read_block(*id,&filet,int_to_little(posbloc));
                                                fill_block(&filet,64,(num_new_dir%16)*64);
                                                fill_block(&filet,1,(num_new_dir%16)*64+4);
                                                fill_block(&filet,0,(num_new_dir%16)*64+8);
                                                fill_block(&filet,free_place_dir,(num_new_dir%16)*64+12);
                                                for(j=16;j<64;j++){
                                                    filet.buff[j+(num_new_dir%16)*64]='\0';
                                                }
                                                write_block(*id,filet,int_to_little(posbloc));
                                                return 0;
                                                
                                            } else {
                                                fprintf(stderr, "you must create you're new directory on a directory");
                                            }
                                        } else {
                                            fprintf(stderr, "wrong path\n");
                                            return -1;
                                        }

                                    } else {
                                        fprintf(stderr, "name of new directory couldn't be longer than 27 characters");
                                        return -1;
                                    }
                                } else {
                                    fprintf(stderr, "volume number %d is full\n", volume);
                                    return -1;
                                }
                            } else {
                                fprintf(stderr, "you can't add  another file on volume number %d unless you delete a file\n", volume);
                                return -1;
                            }
                        } else {
                            fprintf(stderr, "volume %d doesn't exist\n", volume);
                            return -1;
                        }
                    } else {
                        fprintf(stderr, "volume must be a number\n");
                        return -1;
                    }

                } else {
                    fprintf(stderr, "you must give a name to your new directory\n");
                    return -1;
                }
            } else {
                fprintf(stderr, "disk isn't running \n");
                return -1;
            }
        } else {
            fprintf(stderr, "wrong path\n");
        }
    } else {
        fprintf(stderr, "Wrong path, miss \'FILE:\'\n");
        return -1;
    }
    return -1;
}

/**
 * \fn tfs_rename(const char *old, const char *new){
 * \brief tfs_rename renome le cichier désigné par le chenin old en lui donnant le nom new si c'est possible.
 * \param old est le chemin du fichier
 * \param new est le nouveau nom
 * \return on retourne 0 si tout c'est bien passé  et -1 si il y a eu une erreur.
 **/
int tfs_rename(const char *old, const char *new){
  int length=strlen(new);
  if(length<28){
    iter iter=decomposition(strdup(old));
    if(strcmp(iter->name, "FILE:")==0){
      if(iter->next!=NULL){
	iter=go_next(iter);
	if(strcmp(iter->name,"HOST")==0){
	  return 1; 
	}else{
	  if(iter->next!=NULL){
	    iter=go_next(iter);
	    int i;
	    disk_id *disk=NULL;
	    for(i=0;i<MAX_DISQUE;i++){
	      if(disque_ouvert[i]!=NULL){
		if(strcmp((disque_ouvert[i])->name,iter->name)==0){
		  disk=disque_ouvert[i];
		  break;
		}
	      }
	    }
	    if(disk!=NULL){
	      iter=go_next(iter);
	      if(iter->next!=NULL){
		iter=go_next(iter);
		int part = atoi(iter->name);
		if(part>=0 && part<disk->nbPart){
		  Part here=disk->tabPart[part];
		  go_end(iter);
		  char *oldname=iter->name;
		  free(iter->name);
		  iter=go_prec(iter);
		  iter->next=NULL;
		  free(iter->next);
		  go_start(iter);
		  iter=go_next(iter);
		  iter=go_next(iter);
		  iter=go_next(iter);
		  int place;
		  error e=find_name(iter,*disk,part, &place);
		  free_iter(iter);
		  if(e.errnb==0){
		    int numblock;
		    int pos;
		    if(name_in_dir(*disk,part,place,oldname,&numblock, &pos).errnb!=-1){
		      block b;
		      read_block(*disk, &b,int_to_little( numblock+here.num_first_block));
		      int j;
		      for(j=0; j<length; j++){
			b.buff[pos+4+j]=new[j];
		      }
		      for(j=length; j<28; j++){
			b.buff[j]='\0';
		      }
		      return 0;
		    }
		  }else{
		    fprintf(stderr, "tfs_rename : wrong path \n");
		  }
		}else{
		  fprintf(stderr, "tfs_rename : no part \n");
		}
	      }else{
		fprintf(stderr, "tfs_rename : wrong path \n");
	      }			      
	    }else{
	      fprintf(stderr, "tfs_rename : no disk open \n");
	    }
	  }else{
	    fprintf(stderr, "tfs_rename : wrong path \n");		     
	  }
	}
      }else{
	fprintf(stderr, "tfs_rename : NULL pointeur");
      }
    }else{
      fprintf(stderr, "tfs_rename : wrong path, miss FILE: \n");		     
    }
  }else{
    fprintf(stderr, "tfs_rename : new name too long\n");		     
  }
  return -1;
}

/**
 * \fn tfs_open(const char *name,int oflag){
 * \brief tfs_open retourne l'indice dans la table des fichiers du fichier dont le path est donné en argument.
 * \param name est le chemin du fichier
 * \param oflag
 * \return on retourne 0 si tout c'est bien passé  et -1 si il y a eu une erreur.
 **/
int tfs_open(const char *name,int oflag){
  iter iter=decomposition(strdup(name));
  if(strcmp(iter->name, "FILE:")==0){
    if(iter->next!=NULL){
      iter=go_next(iter);
      if(strcmp(iter->name,"HOST")==0){
	return 1;  
      }else{
	int i;
	for(i=0;i<MAX_DISQUE;i++){
	  if(disque_ouvert[i]!=NULL){
	    if(strcmp((disque_ouvert[i])->name,iter->name)==0){
	      disk_id *disk=disque_ouvert[i];
	      if(iter->next!=NULL){
		iter=go_next(iter);
		int part = atoi(iter->name);
		if(part>=0 && part<disk->nbPart){
		  Part here=disk->tabPart[part];
		  int place;
		  error e=find_name(iter,*disk,part, &place);
		  if(e.errnb==0){
		    block b;
		    read_block(*disk,&b,int_to_little( here.num_first_block+1+(place-1)/16));
		    int a;
		    readint_block(&b, &a,place%16+4);
		    if(a==0){ //si num correspond a un fichier
		      tfs_fd f;
		      f.pdisk=disk;
		      f.partition=part;
		      f.pos=place;
		      f.flag=oflag;
		      f.path=strdup(name);
		      int j;
		      for(j=0; j<MAX_FICHIERS;j++){
			if(fichiers_ouverts[i]==NULL){
			  fichiers_ouverts[i]=&f;
			  return i;
			}
		      }
		      fprintf(stderr, "tfs_open : fichiers_ouverts is full \n");
		    }else{
		      fprintf(stderr, "tfs_open : nom correspond a un dossier \n");
		    }
		  }else{
		    fprintf(stderr, "tfs_open : wrong path \n");
		  }
		}else{
		  fprintf(stderr, "tfs_open : no part \n");
		}
	      }else{
		fprintf(stderr, "tfs_open : wrong path \n");
	      }
	    }else{
	      fprintf(stderr, "tfs_open : wrong path \n");
	    }
	  }else{
	    fprintf(stderr, "tfs_open : wrong path \n");
	  }
	  return -1;
	}
	fprintf(stderr, "tfs_open : no disk open \n");
      }
    }else{
      fprintf(stderr, "tfs_open : NULL pointeur");
    }
  }else{
    fprintf(stderr, "tfs_open : miss FILE: ");
  }
  return -1;
}

// prend en argument un indice d'un tfs_fd dans le tableau de fichiers_ouverts

int tfs_close(int num) {
    if (num >= 0 && num < MAX_FICHIERS) {
        free(fichiers_ouverts[num]->pdisk);
        free(fichiers_ouverts[num]->path);
        free(fichiers_ouverts[num]);
        return 0;
    } else {
        return -1;
    }
}

/**
 * \fn ssize_t tfs_write(int fildes, void* buff, size_t numbytes)
 * \brief Write a buffer into a normal file 
 * \param fildes Integer index of file on the tab of open files
 * \param buff Pointer characters which will be written on the file
 * \param nbytes Size_t Number of character which must be written 
 * \return Integer number of characters written
 * */
 
ssize_t tfs_write(int fildes, void* buff, size_t numbytes){
	int nbytes = (int)(numbytes);
	char* vuff = buff;
	if(fildes>-1){
		int i = 0;
		int j = 0;
		int k = 0;
		int str_length = strlen(vuff);
		tfs_fd* f = fichiers_ouverts[fildes];
		disk_id* disk = (*f).pdisk;
		int id_part = (*f).partition;		
		Part p = (*disk).tabPart[id_part];
		int fpos = (*f).pos;
		printf("Val of (*f).pos %d\n",(*f).pos);
		int fflag = (*f).flag;
		printf("Val of (*f).flag %d\n",(*f).flag);
		switch(fflag){
			case TFS_READ : 
				fprintf(stderr,"tfs_write : You can't use this flags for this function\n");
				return -1;
				break;
																								
			case TFS_WRITE : 
				if(nbytes>0){
					int nb_wblock = (nbytes/1024);
					if(nbytes%1024 > 0){
						nb_wblock += 1;
					}
					printf("Val of nb_wblock %d\n",nb_wblock);
					int fpos_block = (fpos*64);
					printf("Val of fpos_block %d\n",fpos_block);
					int fblock = (fpos_block/1024);
					if(fpos_block%1024 > 0){
						fblock += 1;
					}
					fblock += p.num_first_block;
					printf("Val of fblock %d\n",fblock);
					block finfo;
					read_block((*disk),&finfo,int_to_little(fblock));
					int f_posf;
					if(fpos_block % 16 > 0){
						f_posf = 	fpos_block % 16;
					}else{
						f_posf = fpos_block / 16;
					}
					printf("Val of f_posf %d\n",f_posf);
					int fsize;
					readint_block(&finfo, &fsize, (f_posf-1)*64);
					printf("Val of fsize %d\n",fsize);
					int nb_bfsize = fsize/1024;
					if(fsize%1024 > 0){
						nb_bfsize += 1;
					}
					if(nb_bfsize>=0){
						if(nb_bfsize < 11){
							if(nb_wblock<11){
								//Case where vuff which we write is bigger than the size file
								if(nb_bfsize < nb_wblock){
									for(i=0; i< nb_wblock-nb_bfsize; i++){
										//We add blocks in order to write vuff
										error e1 = add_file_block(disk,id_part,fpos,p.first_free_block);
										if(e1.errnb == -1){
											fprintf(stderr,"tfs_write : Problem with the udpate of size file (increase size)\n");
											return -1;
										}
									} 
									int tab_block[nb_wblock];
									for(i=0; i<nb_wblock; i++){
										fill_block(&finfo, tab_block[i], f_posf+12+(i*4));
										printf("Val of tab_block[%d] %d\n",i,tab_block[i]);
									}
									block bw;
									for(k=0; k<nb_wblock; k ++){
										if(tab_block[i] != 0){
											for(i=0; i<str_length ; i++){
												bw.buff[j] = vuff[i];
												j++;
												if(j == 1023){
													write_block((*disk),bw,int_to_little(tab_block[k] + p.num_first_block));
													j=0;
													printf("Val of tab_block[%d] + p.num_first_block %d\n",k,tab_block[k] + p.num_first_block);
												}
											}
										}
									}
									set_size_file(disk,id_part,fpos,(nbytes));
									return (nbytes);
								}else{
									int tab_block[nb_wblock];
									for(i=0; i<nb_wblock; i++){
										fill_block(&finfo, tab_block[i], f_posf+12+(i*4));
										printf("Val of tab_block[%d] %d\n",i,tab_block[i]);
									}
									for(i=0; i<	nb_bfsize-nb_wblock; i++){
										error e1 = remove_file_block(disk,id_part,fpos);
										if(e1.errnb == -1){
											fprintf(stderr,"tfs_write : Problem with the udpate of size file\n");
											return -1;
										}  
									}
									block bw;
									for(k=0; k<nb_wblock; k ++){
										if(tab_block[k] != 0){
											for(i=0; i<str_length ; i++){
												bw.buff[j] = vuff[i];
												j++;
												if(j == 1023){
													write_block((*disk),bw,int_to_little(tab_block[k] + p.num_first_block));
													j=0;
													printf("Val of tab_block[%d] + p.num_first_block %d\n",k,tab_block[k] + p.num_first_block);
												}
											}
										}
									}
									set_size_file(disk,id_part,fpos,(nbytes));
									return (nbytes);
								}
							}else{
								if(nb_wblock <266){
									for(i = 0; i < nb_wblock - nb_bfsize; i++){
										error e1 = add_file_block(disk, id_part, fpos, p.first_free_block); 
										if(e1.errnb == -1){
											return -1;
										}
									}
									int tab_block[nb_wblock];
									for(i=0; i<10; i++){
										fill_block(&finfo, tab_block[i], f_posf+12+(i*4));
										printf("Val of tab_block[%d] %d\n",i,tab_block[i]);
									}
									int* tab = get_indirect1(disk,id_part,fpos);
									for(i=10; i<nb_wblock; i++){
										if(tab[i-10] >0){
											tab_block[i] = tab[i-10];
										}
									}
									block bw;
									for(k=0; k<nb_wblock; k ++){
										if(tab_block[i] != 0){
											for(i=0; i<str_length ; i++){
												bw.buff[j] = vuff[i];
												j++;
												if(j == 1023){
													write_block((*disk),bw,int_to_little(tab_block[k] + p.num_first_block));
													j=0;
													printf("Val of tab_block[%d] + p.num_first_block %d\n",k,tab_block[k] + p.num_first_block);
												}
											}
										}
									}
									set_size_file(disk,id_part,fpos,(nbytes));
									return (nbytes);
								}else{
									fprintf(stderr, "tfs_write : Your nbytes is too high compare to system capacity\n");	
								}
							}	
						}else if(nb_bfsize <266){
							if(nb_wblock<nb_bfsize){
								if(nb_wblock <11){
									int tab_block[nb_wblock];
									for(i=0; i<nb_wblock; i++){
										fill_block(&finfo, tab_block[i], f_posf+12+(i*4));
										printf("Val of tab_block[%d] %d\n",i,tab_block[i]);
									}
									for(i=0; i<	nb_bfsize-nb_wblock; i++){
										error e1 = remove_file_block(disk,id_part,fpos);
										if(e1.errnb == -1){
											fprintf(stderr,"tfs_write : Problem with the udpate of size file\n");
											return -1;
										}  
									}
									block bw;
									for(k=0; k<nb_wblock; k ++){
										if(tab_block[k] != 0){
											for(i=0; i<str_length ; i++){
												bw.buff[j] = vuff[i];
												j++;
												if(j == 1023){
													write_block((*disk),bw,int_to_little(tab_block[k] + p.num_first_block));
													j=0;
													printf("Val of tab_block[%d] + p.num_first_block %d\n",k,tab_block[k] + p.num_first_block);
												}
											}
										}
									}
									set_size_file(disk,id_part,fpos,(nbytes));
									return (nbytes);
								}else if(nb_wblock <266){
									int block_to_remove = nb_bfsize - nb_wblock;
									for(i=0; i<block_to_remove; i++){
										error e1 = remove_file_block(disk,id_part,fpos);
										if(e1.errnb == -1){
											fprintf(stderr,"tfs_write : Problem with the udpate of size file\n");
											return -1;
										}
									}
									int tab_block[nb_wblock];
									for(i=0; i<10; i++){
										fill_block(&finfo, tab_block[i], f_posf+12+(i*4));
										printf("Val of tab_block[%d] %d\n",i,tab_block[i]);
									}
									int* tab = get_indirect1(disk,id_part,fpos);
									for(i=10; i<nb_wblock; i++){
										if(tab[i-10] >0){
											tab_block[i] = tab[i-10];
										}
									}
									block bw;
									for(k=0; k<nb_wblock; k ++){
										if(tab_block[i] != 0){
											for(i=0; i<str_length ; i++){
												bw.buff[j] = vuff[i];
												j++;
												if(j == 1023){
													write_block((*disk),bw,int_to_little(tab_block[k] + p.num_first_block));
													j=0;
													printf("Val of tab_block[%d] + p.num_first_block %d\n",k,tab_block[k] + p.num_first_block);
												}
											}
										}
									}
									set_size_file(disk,id_part,fpos,(nbytes));
									return (nbytes);
								}else{
									fprintf(stderr,"tfs_write : Your nbytes is too high compare to system capacity\n");
									return -1;
								}
							}else{
								if(nb_wblock < 266){
									int nbblock_to_add = nb_wblock-nb_bfsize;
									for(i=0; i<nbblock_to_add; i++){
										error e1 = add_file_block(disk,id_part,fpos,p.first_free_block);
										if(e1.errnb == -1){
											fprintf(stderr,"tfs_write : Problem with the udpate of size file (increase size)\n");
											return -1;
										}
									}
									int tab_block[nb_wblock];
									for(i=0; i<10; i++){
										fill_block(&finfo, tab_block[i], f_posf+12+(i*4));
										printf("Val of tab_block[%d] %d\n",i,tab_block[i]);
									}
									int* tab = get_indirect1(disk,id_part,fpos);
									for(i=10; i<nb_wblock; i++){
										if(tab[i-10] >0){
											tab_block[i] = tab[i-10];
										}
									}
									block bw;
									for(k=0; k<nb_wblock; k ++){
										if(tab_block[i] != 0){
											for(i=0; i<str_length ; i++){
												bw.buff[j] = vuff[i];
												j++;
												if(j == 1023){
													write_block((*disk),bw,int_to_little(tab_block[k] + p.num_first_block));
													j=0;
													printf("Val of tab_block[%d] + p.num_first_block %d\n",k,tab_block[k] + p.num_first_block);
												}
											}
										}
									}
									set_size_file(disk,id_part,fpos,(nbytes));
									return (nbytes);
								}else{
									fprintf(stderr,"tfs_write : Your nbytes are too higher comparte to system capacity\n");
									return -1;
								}
							}
						}else{
							fprintf(stderr,"tfs_write : Your nbytes are too higher comparte to system capacity\n");
							return -1;								
						}
					}else{
						fprintf(stderr,"tfs_write : Wrong argument for the file\n");
						return -1;
					} 
				}else{
					fprintf(stderr,"tfs_write : Wrong argument for nbytes %d\n",nbytes);
					return -1;
				}
			case TFS_APPEND :
				if(nbytes>0){
					int nb_wblock = (nbytes/1024);
					if(nbytes%1024 > 0){
						nb_wblock += 1;
					}
					printf("Val of nb_wblock %d\n",nb_wblock);
					int fpos_block = (fpos*64);
					printf("Val of fpos_block %d\n",fpos_block);
					int fblock = (fpos_block/1024);
					if(fpos_block%1024 > 0){
						fblock += 1;
					}
					fblock += p.num_first_block;
					printf("Val of fblock %d\n",fblock);
					block finfo;
					read_block((*disk),&finfo,int_to_little(fblock));
					int f_posf;
					if(fpos_block % 16 > 0){
						f_posf = 	fpos_block % 16;
					}else{
						f_posf = fpos_block / 16;
					}
					printf("Val of f_posf %d\n",f_posf);
					int fsize;
					readint_block(&finfo, &fsize, (f_posf-1)*64);
					printf("Val of fsize %d\n",fsize);
					int nb_bfsize = fsize/1024;
					if(fsize%1024 > 0){
						nb_bfsize += 1;
					}
					if(nb_wblock > nb_bfsize){
						for(i=0; i< nb_bfsize - nb_wblock; i++){
							error e1 = add_file_block(disk,id_part,fpos,p.first_free_block);
							if(e1.errnb == -1){
								fprintf(stderr,"tfs_write : Problem with the udpate of size file (increase size)\n");
								return -1;
							}
						}
					}
					int u = 0;
					while(u<nbytes){
						int bck = lastbf(disk,id_part,fpos);
						if(bck!=-1 && bck!= 0){
							block bloc;
							read_block((*disk),&bloc, int_to_little(p.num_first_block + bck));
							for(i=0;i<1024; i++){
								bloc.buff[i]=vuff[u];
								u++;
							}
							write_block((*disk),bloc, int_to_little(p.num_first_block + bck));
						}
					}
					set_size_file(disk,id_part,fpos,(nbytes+fsize));
					return (nbytes);
				}else{
					fprintf(stderr,"tfs_write : Wrong argument for nbytes %d\n",nbytes);
					return -1;
				}
		}
		return 0;
	}else{
		return fildes;
	}
}

#include "manipvolume.h"

error free_block(disk_id *id, int numblock, int volume) {
    error e;
    if (id != NULL) {
        if (0 <= volume && volume < id->nbPart) {
            Part here = id->tabPart[volume];
            if (here.file_table_size != 0) {
                if (numblock > 1 + here.file_table_size && numblock < here.taille) {
                    int prec = here.first_free_block; //on va chercher le dernier block du chainage pour le racorder.
                    if (prec != 0) { // si first_free_block vaut 0 alors il n'y a plus de block libres.
                        block b;
                        read_block(*id, &b, prec + here.num_first_block);
                        int suiv;
                        readint_block(&b, &suiv, 1020);

                        while (prec != suiv) {
                            if (prec == numblock) {
                                e.errnb = -1;
                                printf("free_block : block déja libre \n");
                                return e;
                            }
                            prec = suiv;
                            read_block(*id, &b, prec + here.num_first_block);
                            readint_block(&b, &suiv, 1020);
                        } // on a trouvé le dernier block du chainage
                        fill_block(&b, numblock, 1020);
                        write_block((*id), b, prec + here.num_first_block);
                        write_block(*id, b, numblock + here.num_first_block);
                        here.free_block_count += 1;
                        block first;
                        read_block(*id, &first, here.num_first_block);
                        fill_block(&first, here.free_block_count, 12);
                        write_block((*id), first, here.num_first_block);
                    } else { // cas ou il n'y avait plus de blocks libres; on doit réinitialiser le chainage. 
                        block libre;
                        int i;
                        for (i = 0; i < 1020; i++) {
                            libre.buff[i] = '\0';
                        }
                        fill_block(&libre, numblock, 1020);
                        write_block(*id, libre, numblock + here.num_first_block);
                        here.free_block_count += 1;
                        here.first_free_block = numblock;
                        block first;
                        read_block(*id, &first, here.num_first_block);
                        fill_block(&first, here.free_block_count, 12);
                        fill_block(&first, here.first_free_block, 16);
                        write_block((*id), first, here.num_first_block);
                    }
                    e.errnb = 0;
                    return e;
                } else {
                    e.errnb = -1;
                    fprintf(stderr, "free_block : wrong numblock : %d \n", numblock);
                    return e;
                }
            } else {
                e.errnb = -1;
                fprintf(stderr, "free_block : volume hasn't been format  \n");
                return e;
            }
        } else {
            e.errnb = -1;
            fprintf(stderr, "free_block : no volume  : %d \n", volume);
            return e;
        }
    } else {
        e.errnb = -1;
        fprintf(stderr, "free_block : id NULL \n");
        return e;
    }
}

error use_block(disk_id *id, int numblock, int volume) {
    error e;
    if (id != NULL) {
        if (0 <= volume && volume < id->nbPart) {
            Part here = id->tabPart[volume];
            if (here.file_table_size != 0) {
                if (numblock > 1 + here.file_table_size && numblock < here.taille) {
                    int prec = here.first_free_block; //on va chercher le block "numblock" pour le dechainer.
                    if (prec != 0) {
                        block p;
                        read_block(*id, &p, prec + here.num_first_block);
                        int suiv;
                        readint_block(&p, &suiv, 1020);
                        block s;
                        block first;
                        read_block(*id, &first, here.num_first_block);
                        if (suiv == numblock) { //Cas ou numbloc est le premier bloc libre
                            if (suiv == prec) { //Cas ou c'est le seul bloc libre
                                fill_block(&first, 0, 12);
                                fill_block(&first, 0, 16);
                                write_block((*id), first, here.num_first_block);
                                here.free_block_count = 0;
                                here.first_free_block = 0;
                            } else {
                                fill_block(&first, (here.free_block_count - 1), 12);
                                fill_block(&first, suiv, 16);
                                write_block((*id), first, here.num_first_block);
                                here.free_block_count -= 1;
                                here.first_free_block = suiv;
                            }
                            e.errnb = 0;
                            return e;
                        } else { //sinon on cherche le prédécesseur de numbloc dans le chainage.
                            while (prec != suiv) {
                                if (suiv == numblock) { //on a trouvé numblock  	  
                                    int a;
                                    read_block(*id, &s, suiv + here.num_first_block);
                                    readint_block(&s, &a, 1020);
                                    if (a == suiv) { //cas ou le block "numblock" est le dernier du chainage.
                                        fill_block(&s, prec, 1020);
                                    }
                                    write_block((*id), s, here.num_first_block + prec);
                                    fill_block(&first, here.free_block_count + 1, 12);
                                    write_block((*id), first, here.num_first_block);
                                    here.free_block_count -= 1;
                                    e.errnb = 0;
                                    return e;
                                }
                                prec = suiv;
                                read_block(*id, &p, prec + here.num_first_block);
                                readint_block(&p, &suiv, 1020);
                            }
                            e.errnb = -1;
                            printf("use_block : this block isn't free  : %d \n", numblock);
                            return e;
                        }
                    } else {
                        e.errnb = -1;
                        printf("use_block : volume is full  : %d \n", numblock);
                        return e;
                    }
                } else {
                    e.errnb = -1;
                    printf("use_block : wrong numblock : %d \n", numblock);
                    return e;
                }
            } else {
                e.errnb = -1;
                printf("use_block : volume hasn't been format  \n");
                return e;
            }
        } else {
            e.errnb = -1;
            printf("use_block : no volume  : %d \n", volume);
            return e;
        }
    } else {
        e.errnb = -1;
        printf("use_block : id NULL \n");
        return e;
    }
}

error add_free_file(disk_id id, int volume, int file) {
    error e;
    block filetable;
    block description_block;
    if (volume < id.nbPart) {
        if (file != 0) {
            if (id.tabPart[volume].free_file_count != id.tabPart[volume].max_file_count) {
                int position = id.tabPart[volume].num_first_block;
                if (id.tabPart[volume].first_free_file == 0) {
                    read_block(id, &description_block, int_to_little(position));
                    fill_block(&description_block, file, 28);
                    fill_block(&description_block, 1, 24);
                    id.tabPart[volume].free_file_count = 1;
                    id.tabPart[volume].first_free_file = file;
                    write_block(id, description_block, int_to_little(position));
                    position = position + (file) / 16 + 1;
                    read_block(id, &filetable, int_to_little(position));
                    id.tabPart[volume].first_free_file = file;
                    fill_block(&filetable, file, (((file * 64) % 1024) - 4));
                    write_block(id, filetable, position);
                    e.errnb = 0;
                    return e;
                } else {
                    int next;
                    int current = id.tabPart[volume].first_free_file;
                    int pos_bloc = ((current / 16) + 1);
                    read_block(id, &filetable, int_to_little(pos_bloc + position));
                    readint_block(&filetable, &next, (((current * 64) % 1024) - 4));
                    while (next == current) {
                        if (current != file) {
                            current = next;
                            pos_bloc = ((next / 16) + 1);
                            read_block(id, &filetable, int_to_little(pos_bloc + position));
                            readint_block(&filetable, &next, (((current * 64) % 1024) - 4));
                        } else {
                            e.errnb = -1;
                            fprintf(stderr, "this file are already free");
                            return e;
                        }
                    }
                    fill_block(&filetable, current, (((file * 64) % 1024) - 4));
                    write_block(id, filetable, position);
                    pos_bloc = ((file / 16) + 1);
                    read_block(id, &filetable, int_to_little(pos_bloc + position));
                    fill_block(&filetable, file, (((file * 64) % 1024) - 4));
                    write_block(id, filetable, int_to_little(pos_bloc + position));
                    read_block(id, &description_block, int_to_little(position));
                    id.tabPart[volume].free_file_count++;
                    fill_block(&description_block, id.tabPart[volume].free_file_count, 24);
                    e.errnb = 0;
                }
            } else {
                e.errnb = -1;
                printf("all the files in this volume are already free");
                return e;
            }
        } else {
            e.errnb = -1;
            fprintf(stderr, "vous ne pouvez pas liberez le repertoire racine");
            return e;

        }
        return e;
    }
    e.errnb = -1;
    return e;
}

error remove_free_file(disk_id id, int volume, int file) {
    error e;
    block filetable;
    block description_block;
    if (volume < id.nbPart) {
        int position = id.tabPart[volume].num_first_block;
        if (id.tabPart[volume].free_file_count != 0) {
            if (id.tabPart[volume].free_file_count != 1) {
                int current = id.tabPart[volume].first_free_block;
                int past;
                int pos_bloc = ((current / 16) + 1);
                int tour = 0;
                while (current != file && current != past && tour < id.tabPart[volume].free_file_count) {
                    past = current;
                    read_block(id, &filetable, int_to_little(pos_bloc + position));
                    readint_block(&filetable, &current, (((past * 64) % 1024) - 4));
                    pos_bloc = ((current / 16) + 1);
                    tour++;
                }
                if (current == file) {
                    int next;
                    readint_block(&filetable, &next, (((current * 64) % 1024) - 4));
                    if (past == 0) {
                        fill_block(&description_block, next, 28);
                        write_block(id, description_block, int_to_little(position));
                        id.tabPart[volume].first_free_file = next;
                    } else if (next == current) {
                        pos_bloc = ((past / 16) + 1);
                        read_block(id, &filetable, int_to_little(pos_bloc + position));
                        fill_block(&filetable, past, (((past * 64) % 1024) - 4));
                        write_block(id, filetable, (pos_bloc + position));
                    } else {
                        pos_bloc = ((past / 16) + 1);
                        read_block(id, &filetable, int_to_little(pos_bloc + position));
                        fill_block(&filetable, next, (((past * 64) % 1024) - 4));
                        write_block(id, filetable, (pos_bloc + position));
                    }
                    id.tabPart[volume].free_file_count--;
                    fill_block(&filetable, id.tabPart[volume].free_file_count, 24);
                    e.errnb = 0;
                } else {
                    e.errnb = -1;
                    fprintf(stderr, "file number %d isn't free", file);
                    return e;
                }
            } else {
                read_block(id, &description_block, int_to_little(position));
                fill_block(&description_block, 0, 28);
                fill_block(&description_block, 0, 24);
                id.tabPart[volume].free_file_count = 0;
                id.tabPart[volume].first_free_file = 0;
                write_block(id, description_block, int_to_little(position));
                e.errnb = 0;
            }
        } else {
            e.errnb = -1;
            fprintf(stderr, "there is no free files");
            return e;
        }
        return e;
    }
    e.errnb = -1;
    return e;
}

iter decomposition(char *path) {
    char *separateur = "//";
    char *token = strtok(path, separateur);
    iter current = malloc(sizeof (iter));
    current->name = token;
    token = strtok(NULL, separateur);
    iter it = current;
    while (token != NULL) {
        iter next = malloc(sizeof (iter));
        next->name = token;
        next->prec = it;
        it->next = next;
        it = it->next;
        token = strtok(NULL, separateur);
    }
    return current;
}

error readname_rep(block b, char *a, int loc) { //loc est l'endroit du bloc ou on veut lire le nom que l'on écrit dans a.
    int i;
    if (loc >= 0 && loc <= 992) {
        char nom[28];
        for (i = 4; i < 32; i++) {
            if (b.buff)
                nom[i] = b.buff[loc + i];
        }
        a = nom;
        error e;
        e.errnb = 0;
        return e;
    } else {
        error e;
        e.errnb = -1;
        fprintf(stderr, "readname_rep : wrong argument loc : %d", loc);
        return e;
    }
}

int name_in_block(disk_id id, int volume, int num_block, char *name) {
    if (id.nbPart > volume) {
        int pos = id.tabPart[volume].num_first_block + num_block;
        block b;
        read_block(id, &b, int_to_little(pos));
        char namefile;
        int i;
        for (i = 0; i < 1024; i = i + 32) {
            readname_rep(b, &namefile, i + 4);
            if (strcmp(&namefile, name) != -1) {
                readint_block(&b, &pos, i);
                return pos;
            }
        }
        fprintf(stderr, "%s is not on this block", name);
        return -1;
    } else {
        fprintf(stderr, "volume %d doesn't exist on %s", volume, id.name);
        return -1;
    }
}

int name_in_dir(disk_id id, int volume, int dir, char *name) {
    if (id.nbPart > volume) {
        if (dir < id.tabPart[volume].max_file_count) {
            int pos = id.tabPart[volume].num_first_block + (dir / 16) + 1;
            block b1;
            read_block(id, &b1, pos);
            int i;
            int numfic;
            int numb;
            for (i = 0; i < 10; i++) {
                readint_block(&b1, &numb, (dir % 16)*64 + 12 + i); //début des numéros de blocs contenant des données du fichier
                if(numb==0)
		  return -1;
		if ((numfic = name_in_block(id, volume, numb, name)) != -1)
                    return numfic;
            }
            readint_block(&b1, &numb, (dir % 16)*64 + 52);
            if(numb==0) return -1;
	    pos = id.tabPart[volume].num_first_block + numb;
            block b2;
            read_block(id, &b2, pos);
            for (i = 0; i < 1024; i = i + 4) {
                readint_block(&b2, &numb, i);
                if(numb==0) return -1;
		if ((numfic = name_in_block(id, volume, numb, name))) {
                    return numfic;
                }
            }
            readint_block(&b1, &numb, (dir % 16)*64 + 56);
            if(numb == 0)return -1;
	    pos = id.tabPart[volume].num_first_block+numb;
            read_block(id,&b2,pos);
            int pos2;
            block b3;
            for(i=0;i<1024;i=i+4){
                readint_block(&b2,&numb,i);
		if(numb==0) return -1;
                pos2 = id.tabPart[volume].num_first_block+numb;
                read_block(id,&b3,numb);
                int j;
                for(j=0;i<1024;j++){
                    readint_block(&b3,&numb,i);
		    if(numb==0) return -1;
                    if((numfic = name_in_block(id,volume,numb,name)))
                        return numfic;
                }
            }
            return numfic;
        } else {
            fprintf(stderr, "dir number %d doesn't exist on volume number %d", dir, volume);
            return -1;
        }


    } else {
        fprintf(stderr, "volume %d doesn't exist on %s", volume, id.name);
        return -1;
    }
}

void file_tableau(int *tab[16], disk_id id, int volume) {
    if (volume < id.nbPart) {
        int position = id.tabPart[volume].num_first_block;
        int maxf = id.tabPart[volume].max_file_count;
        tab = malloc(sizeof (int)*maxf);
        int fts = id.tabPart[volume].file_table_size;
        int i;
        block ft;
        for (i = 0; i < fts; i++) {
            read_block(id, &ft, position + 1 + i);
            int j;
            int max;
            if (i == fts - 1) {
                max = maxf - 16 * i;
            } else {
                max = 16;
            }
            for (j = 0; j < max; j++) {
                int k;
                for (k = 0; k < 16; k++) {
                    int nombre;
                    readint_block(&ft, &nombre, k * 4);
                    tab[j + i * 16][k] = nombre;
                }
            }
        }

    } else {
        printf("le volume n'existe pas");
    }

}



error find_name(iter i, disk_id *disk, int *volume, int *place){
  error e;
  i=i->next;
  if(i->next!=NULL){
    i=i->next;
    int part = atoi(i->name);
    if(part>=0 && part<disk->nbPart){
      Part here=disk->tabPart[part];
      if(i->next!=NULL){
	i=i->next;
	char *nom=i->name;
	int idtable=0;
	int bool =1;
	while(bool==1){
	  if(i->next!=NULL){
	    block b;
	    read_block(*disk, &b, here.num_first_block+(idtable-1)/16+1);
	    int a;
	    readint_block(&b, &a, 64*(idtable%16)+4);
	    if(a==1){  //on test si l'entrée correspondante a idtable représente bien un repertoire
	      if(name_in_dir(*disk,part,idtable,nom)!=-1){ //on rebarde si nom est bien une entrée du repertoire idtable 
		idtable=name_in_dir(*disk,part,idtable,nom);
		i=i->next;
		nom=i->name;
	      }else{
		e.errnb=-1;
		fprintf(stderr,"wrong path");
		return e;
	      }
	    }else{
	      e.errnb=-1;
	      fprintf(stderr,"wrong path");
	      return e;
	    }
	  }else{
	    idtable=name_in_dir(*disk,part,idtable,nom);
	    if(idtable!=-1){
	      *place=idtable;
	      e.errnb=0;
	      return e;
	    }else{
	      e.errnb=-1;
	      fprintf(stderr,"wrong path");
	      return e;
	    }
	  } 
	}
      }
    }
  }
  e.errnb=-1;
  fprintf(stderr,"wrong path");
  return e;
}

error free_file_blocks(disk_id* disk, int volume, int id_f) {
    error e;
    if (disk != NULL) {
        if (0 <= volume && volume < disk->nbPart) {
            Part here = (*disk).tabPart[volume];
            if (id_f > 0 && id_f < here.max_file_count) {
                error err;
                int i = 1;
                err = remove_file_block(disk, volume, id_f, i);
                while (err.errnb == 0) {
                    err = remove_file_block(disk, volume, id_f, i);
                    i += 1;
                }
                if (err.errnb == -2) {
                    e.errnb = 0;
                    return e;
                } else {
                    return err;
                }
            } else {
                e.errnb = -1;
                fprintf(stderr, "free_file_blocks : no file %d \n", id_f);
                return e;
            }
        } else {
            e.errnb = -1;
            fprintf(stderr, "free_block : no volume  : %d \n", volume);
            return e;
        }
    } else {
        e.errnb = -1;
        fprintf(stderr, "free_block : id NULL \n");
        return e;
    }
}

/*
disk: disk wich will be used
id_f : num file on file Table  
id_block : num of the block on partition
id_part : num of the partition in tabPart
 */
error add_file_block(disk_id* disk, int id_part, int id_f, int id_block) {
    error e;
    error e1;

    if (id_part < (*disk).nbPart && id_part>-1) {
        Part p = (*disk).tabPart[id_part];
        if (id_block > 3 && (p.taille / 1024) > id_block) {
            if (p.max_file_count > id_f) {
                //position of the file on file_Table on disk
                int posf_fTab = (64 * id_f) / 1024;
                if ((64 * id_f) % 1024 > 0) {
                    posf_fTab += 1;
                }
                int posf_part = posf_fTab + p.num_first_block;
                block finfo;
                read_block((*disk), (&finfo), int_to_little(posf_part));
                int pos;
                if (id_f % 16 > 0) {
                    pos = ((id_f % 16) - 1)*64;
                } else {
                    pos = 15 * 64;
                }
                printf("Val of pos %d_n", pos);
                int ftfs_size = 0;
                readint_block((&finfo), (&ftfs_size), pos);
                printf("Val of ftfs_size %d\n", ftfs_size);
                int nbBlock_size = (ftfs_size / 1024);
                if (ftfs_size % 1024 > 0) {
                    nbBlock_size += 1;
                }
                printf("Val of nbBlock_size %d\n", nbBlock_size);
                e1 = free_block(disk, id_block, id_part);
                if (nbBlock_size < 10) {
                    if (e1.errnb == 0) {
                        fill_block((&finfo), id_block, pos + 11 + (nbBlock_size * 4));
                        printf("Val of  (((id_f-1)*64)+11+(nbBlock_size*4)) %d\n", pos + 11 + (nbBlock_size * 4));
                        write_block((*disk), finfo, int_to_little(posf_part));
                        e.errnb = 0;
                        return e;
                    } else {
                        e.errnb = -1;
                        return e;
                    }
                } else if (nbBlock_size < 266) {
                    if (e1.errnb == 0) {
                        int ind1;
                        readint_block((&finfo), (&ind1), pos + 52);
                        printf("Val of ind1 %d\n", ind1);
                        block indirect1;
                        read_block((*disk), (&indirect1), int_to_little(p.num_first_block + ind1));
                        int pos_ind1 = nbBlock_size - 10;
                        printf("Val of pos_ind1*4 %d\n", pos_ind1 * 4);
                        fill_block((&indirect1), id_block, (pos_ind1 * 4));
                        write_block((*disk), indirect1, int_to_little(p.num_first_block + ind1));
                        e.errnb = 0;
                        return e;
                    } else {
                        e.errnb = -1;
                        return e;
                    }
                } else {
                    if (e1.errnb == 0) {
                        //on the block indirect2 which indirect2.indirect block is used
                        int useb = (nbBlock_size - 266) / 256;
                        if ((nbBlock_size - 266) % 256 > 0) {
                            useb = useb + 1;
                        }
                        int ind2;
                        readint_block((&finfo), (&ind2), pos + 56);
                        printf("Val of ind1 %d\n", ind2);
                        /*We read block indirect2*/
                        block indirect2;
                        read_block((*disk), (&(indirect2)), (int_to_little(p.num_first_block + ind2)));
                        int indi_indirect;
                        readint_block(&(indirect2), (&indi_indirect), useb);
                        printf("Val of indi_indirect2 %d\n", indi_indirect);
                        //We read block indirect2.indirect
                        read_block((*disk), &(indirect2), (int_to_little(p.num_first_block + indi_indirect)));
                        int pos_block;
                        if (((nbBlock_size - 266) % 256) > 0) {
                            pos_block = (nbBlock_size - 266) % 256;
                            fill_block(&indirect2, id_block, (pos_block * 4));
                        } else {
                            fill_block(&indirect2, id_block, 0);
                        }
                        write_block((*disk), indirect2, int_to_little(p.num_first_block + indi_indirect));
                        e.errnb = 0;
                    } else {
                        e.errnb = -1;
                    }
                }
            } else {
                fprintf(stderr, "Wrong argument for id_f of file\n");
                e.errnb = -1;
            }
        } else {
            fprintf(stderr, "Wrong argument for id_block of block\n");
            e.errnb = -1;
        }
    } else {
        fprintf(stderr, "Wrong argument for id_part of partition\n");
        e.errnb = -1;
    }
    return e;

}


//test if id_block contains the number id_blocktest

int have_block(disk_id* disk, int id_block, int id_blocktest) {
    int result = -1;
    block b;
    read_block((*disk), (&b), int_to_little(id_block));
    int i;
    int num;
    for (i = 0; i < 1024; i = i + 4) {
        readint_block(&b, &num, i);
        if (num == id_blocktest) {
            result = i;
        }
        break;
    }
    return result;
}

/*
disk: disk wich will be used
id_f : num file on file Table  
id_block : num of the block on file between 1 and 522
id_part : num of the partition in tabPart

 */
error remove_file_block(disk_id* disk, int id_part, int id_f) {
    error e;
    error e1;
    error e2;
    int i;
	if((*disk).nbPart>id_part){
		Part p = (*disk).tabPart[id_part];
		if(id_f > -1){
			int posf_block = (64*id_f)/1024;
			if((64*id_f)%1024 > 0){
				posf_block += 1;
			}
			int posf_id;
			if(posf_block%16 > 0){
				posf_id = posf_block%16;	
			} else{
				posf_id = 16;	
			}
			block finfo;
			e1 = read_block((*disk),&finfo,int_to_little(p.num_first_block + p.file_table_size));
			if(e1.errnb != -1){
				int tfs_size;
				readint_block(&finfo, (&tfs_size), (posf_id-1)*64);
				printf("Val of tfs_size %d\n",tfs_size);
				if(tfs_size > 0){
					
				}else{
					e.errnb = 0;
					fprintf(stdout,"remove_file_block : File have 0 blocks\n");	
				}	
			}else{
				e.errnb = -1;
				fprintf(stderr,"remove_file_block : Problem with read_block\n");
			}
		}else{
			e.errnb = -1;
			fprintf(stderr,"remove_file_block : Problem with id_f of file %d\n",id_f);	
		}
	}else{
		e.errnb = -1;
		fprintf(stderr,"remove_file_block : Problem with id_part %d\n",id_part);
	}
    return e;

}

error test_file(disk_id* disk, int id_part, char* name){
		error e;
			
		if((*disk).nbPart > id_part && id_part > 0){
			Part p = (*disk).tabPart[id_part];
			printf("Val of p.max_file_count dans test %d\n",p.max_file_count);
			block b;
			error e1 = read_block((*disk),&b,int_to_little(p.num_first_block + 1));
			if(e1.errnb != -1){
				int  fbloc = p.num_first_block + p.file_table_size + 2;
				block c;
				c.buff[0]='E';
				c.buff[1]='n';
				c.buff[2]='f';
				c.buff[3]='o';
				c.buff[4]='i';
				c.buff[5]='r';
				c.buff[6]='e';
				c.buff[7]='!';
				int i;
				error e2 = write_block((*disk),c,int_to_little(fbloc));
				if(e2.errnb != -1){
					int tab[16];
					i=0;
					tab[i] = 1024;i++;
					tab[i] = 0;i++;
					tab[i] = 0;i++;
					tab[i] = fbloc;
					for(i= 5; i<16; i++){
						tab[i] = 0;
					} 
					for(i = 0; i< 16; i++){
						fill_block(&b,tab[i],64+(i*4));
					}
					block racine;
					error e3 = read_block((*disk),&racine,int_to_little(p.num_first_block + p.file_table_size + 1));
					if(e3.errnb != -1){
						fill_block(&racine, 2, 0);
						for(i=4; i<6; i++){
							racine.buff[i] = name[i];
						}
						error e4 = write_block((*disk),racine, int_to_little(p.num_first_block + p.file_table_size + 1));
						if(e4.errnb != -1){
							//printf("L'écriture et la création du fichier a réussi\n");	
							printf("Val of p.max_file_count dans test %d\n",p.max_file_count);
							e.errnb=0;
						}
					}
				}
			}
		}else{
			e.errnb = -1;			
		}
		return e;
	}

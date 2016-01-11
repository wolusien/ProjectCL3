/**
 * \file manipvolume.c
 * \brief fichier contenant les fonctios manipulant le volume
 * \author Lucas.L Abel.H Wissam.D
 *
 * 
 *
 */
#include "manipvolume.h"

/**
 * \fn free_block(disk_id *id, int numblock, int volume) {
 * \brief on libère le block numblock(c'est à dire on l'ajoute dans la liste des blocks libres) de la partition volume du disk id.
 * \param id est le disk
 * \param numblock est le numéro du block
 * \param volume le numéro du volume
 * \return on retourne une erreur
 * */
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
                        read_block(*id, &b, int_to_little(prec + here.num_first_block));
                        int suiv;
                        readint_block(&b, &suiv, 1020);

                        while (prec != suiv) {
                            if (prec == numblock) {
                                e.errnb = -1;
                                fprintf(stderr, "free_block : block déja libre %d \n", numblock);
                                return e;
                            }
                            prec = suiv;
                            read_block(*id, &b,int_to_little( prec + here.num_first_block));
                            readint_block(&b, &suiv, 1020);
                        } // on a trouvé le dernier block du chainage
                        if (numblock != prec) {
                            fill_block(&b, numblock, 1020);
                            write_block((*id), b,int_to_little( prec + here.num_first_block));
                            write_block(*id, b,int_to_little( numblock + here.num_first_block));
                            printf("%d   %d :\n", prec, numblock);
                            here.free_block_count += 1;
                            block first;
                            read_block(*id, &first,int_to_little( here.num_first_block));
                            fill_block(&first, here.free_block_count, 12);
                            write_block((*id), first,int_to_little( here.num_first_block));
                        } else {
                            e.errnb = -1;
                            fprintf(stderr, "free_block : block déja libre %d \n", numblock);
                            return e;
                        }
                    } else { // cas ou il n'y avait plus de blocks libres; on doit réinitialiser le chainage. 
                        block libre;
                        int i;
                        for (i = 0; i < 1020; i++) {
                            libre.buff[i] = '\0';
                        }
                        fill_block(&libre, numblock, 1020);
                        write_block(*id, libre,int_to_little( numblock + here.num_first_block));
                        here.free_block_count += 1;
                        here.first_free_block = numblock;
                        block first;
                        read_block(*id, &first,int_to_little( here.num_first_block));
                        fill_block(&first, here.free_block_count, 12);
                        fill_block(&first, here.first_free_block, 16);
                        write_block((*id), first,int_to_little( here.num_first_block));
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

/**
 * \fn use_block(disk_id *id, int numblock, int volume) {
 * \brief on utilise le block numblock(c'est à dire on l'enlève dans la liste des blocks libres) du volume volume du disk id.
 * \param id est le disk
 * \param numblock est le numéro du block
 * \param volume le numéro du volume
 * \return on retourne une erreur
 * */
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
                        read_block(*id, &p,int_to_little( prec + here.num_first_block));
                        int suiv;
                        readint_block(&p, &suiv, 1020);
                        block s;
                        block first;
                        read_block(*id, &first,int_to_little( here.num_first_block));
                        if (suiv == numblock) { //Cas ou numbloc est le premier bloc libre
                            if (suiv == prec) { //Cas ou c'est le seul bloc libre
                                fill_block(&first, 0, 12);
                                fill_block(&first, 0, 16);
                                write_block((*id), first,int_to_little( here.num_first_block));
                                here.free_block_count = 0;
                                here.first_free_block = 0;
                            } else {
                                fill_block(&first, (here.free_block_count - 1), 12);
                                fill_block(&first, suiv, 16);
                                write_block((*id), first,int_to_little( here.num_first_block));
                                here.free_block_count -= 1;
                                here.first_free_block = suiv;
                            }
                            e.errnb = 0;
                            return e;
                        } else { //sinon on cherche le prédécesseur de numbloc dans le chainage.
                            while (prec != suiv) {
                                if (suiv == numblock) { //on a trouvé numblock  	  
                                    int a;
                                    read_block(*id, &s,int_to_little( suiv + here.num_first_block));
                                    readint_block(&s, &a, 1020);
                                    if (a == suiv) { //cas ou le block "numblock" est le dernier du chainage.
                                        fill_block(&s, prec, 1020);
                                    }
                                    write_block((*id), s,int_to_little( here.num_first_block + prec));
                                    fill_block(&first, here.free_block_count + 1, 12);
                                    write_block((*id), first,int_to_little( here.num_first_block));
                                    here.free_block_count -= 1;
                                    e.errnb = 0;
                                    return e;
                                }
                                prec = suiv;
                                read_block(*id, &p,int_to_little( prec + here.num_first_block));
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

/**
 * \fn error add_free_file(disk_id id, int volume, int file)
 * \brief rajoute un fichier dans la liste des fichier libres en modifiant le chainage
 * 
 * \param id descripteur de disk sur lequel on doit travailler
 * \param volume partition sur laquel on doit travailler
 * \param file le fichier qui vient d'être liberer
 * \return une erreur  
 */

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

/**
 * \fn error remove_free_file(disk_id id, int volume, int file)
 * \brief enleve un fichier dans la liste des fichier libres en modifiant le chainage
 * 
 * \param id descripteur de disk sur lequel on doit travailler
 * \param volume partition sur laquel on doit travailler
 * \param file le fichier qui vient d'être utiliser
 * \return une erreur  
 */
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
/**
 * \fn iter decomposition(char *path)
 * \brief decompose un chemin en itération en leur attribuant le nom des dossier du chemin 
 * 
 * \param path le chemin a décomposer
 * \return  une iteration correspondant au début du chemin
 */
iter decomposition(char *path) {
    char *separateur = "//";
    char *token = strtok(path, separateur);
    iter current = malloc(sizeof (iter));
    current->name = token;
    current->next = NULL;
    current->prec = NULL;
    token = strtok(NULL, separateur);
    iter it = current;
    while (token != NULL) {
        iter next = malloc(sizeof (iter));
        next->name = token;
        next->prec = it;
        next->next = NULL;
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
        for (i = 0; i < 28; i++) {
            nom[i] = b.buff[loc + i + 4];
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
/**
 * \fn int name_in_block(disk_id id, int volume, int num_block, char *name)
 * \brief cherche le nom d'un fichier dans un bloc
 * 
 * \param id descripteur de disque sur lequel on travaille
 * \param volume sur lequel on travaille
 * \param num_block bloc sur lequel on travaille
 * \param name nom du fichier 
 * \return la place dans le bloc -1 sinon
 */
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
                return i;
            }
        }
        fprintf(stderr, "%s is not on this block", name);
        return -1;
    } else {
        fprintf(stderr, "volume %d doesn't exist on %s", volume, id.name);
        return -1;
    }
}

/**
 * \fn error name_in_dir(disk_id id, int volume, int dir, char *name, int *numblock, int *posi) 
 * \param id descripteur de disque sur lequel on travaille
 * \param volume sur lequel on travaille
 * \param dir repertoire dans lequel on cherche
 * \param name nom de fichier recherché
 * \param numblock pointeur d'entier a remplir avec le numéro de bloc ou se trouve le fichier
 * \param posi pointeur d'entier a remplire avec la position du fichier dans le bloc
 * \return une erreur
 */

error name_in_dir(disk_id id, int volume, int dir, char *name, int *numblock, int *posi) {
    error e;
    if (id.nbPart > volume) {
        if (dir < id.tabPart[volume].max_file_count) {
            int pos;
            if (dir == 0)
                pos = 0;
            else
                pos = id.tabPart[volume].num_first_block + ((dir - 1) / 16) + 1;
            block b1;
            read_block(id, &b1, pos);
            int a;
            readint_block(&b1, &a, 64 * (dir % 16) + 4);
            if (a == 1) {
                int i;
                int numfic;
                int numb;
                for (i = 0; i < 10; i = i + 4) {
                    readint_block(&b1, &numb, (dir % 16)*64 + 12 + i); //début des numéros de blocs contenant des données du fichier
                    if (numb == 0) {
                        e.errnb = -1;
                        fprintf(stderr, "%s doesn't exit on directory number %d", name, dir);
                        return e;
                    }

                    if ((numfic = name_in_block(id, volume, numb, name)) != -1) {
                        *posi = numfic;
                        *numblock = numb;
                        e.errnb = 0;
                        return e;
                    }

                }
                readint_block(&b1, &numb, (dir % 16)*64 + 52);
                if (numb == 0) {
                    e.errnb = -1;
                    fprintf(stderr, "%s doesn't exit on directory number %d\n", name, dir);
                    return e;
                }
                pos = id.tabPart[volume].num_first_block + numb;
                block b2;
                read_block(id, &b2, pos);
                for (i = 0; i < 1024; i = i + 4) {
                    readint_block(&b2, &numb, i);
                    if (numb == 0) {
                        e.errnb = -1;
                        fprintf(stderr, "%s doesn't exit on directory number %d\n", name, dir);
                        return e;
                    }

                    if ((numfic = name_in_block(id, volume, numb, name)) != -1) {
                        *posi = numfic;
                        *numblock = numb;
                        e.errnb = 0;
                        return e;
                    }
                }
                readint_block(&b1, &numb, (dir % 16)*64 + 56);
                if (numb == 0) {
                    e.errnb = -1;
                    fprintf(stderr, "%s doesn't exit on directory number %d\n", name, dir);
                    return e;
                }
                pos = id.tabPart[volume].num_first_block + numb;
                read_block(id, &b2, pos);
                block b3;
                int pos2;
                for (i = 0; i < 1024; i = i + 4) {
                    readint_block(&b2, &numb, i);
                    if (numb == 0) {
                        e.errnb = -1;
                        fprintf(stderr, "%s doesn't exit on directory number %d\n", name, dir);
                        return e;
                    }
                    pos2 = id.tabPart[volume].num_first_block + numb;
                    read_block(id, &b3, pos2);
                    int j;
                    for (j = 0; i < 1024; j++) {
                        readint_block(&b3, &numb, i);
                        if (numb == 0) {
                            e.errnb = -1;
                            fprintf(stderr, "%s doesn't exit on directory number %d\n", name, dir);
                            return e;
                        }

                        if ((numfic = name_in_block(id, volume, numb, name)) != -1) {
                            *posi = numfic;
                            *numblock = numb;
                            e.errnb = 0;
                            return e;
                        }
                    }
                }
                return e;
            } else {
                e.errnb = -1;
                fprintf(stderr, "this is not a directory\n");
            }
        } else {
            fprintf(stderr, "dir number %d doesn't exist on volume number %d\n", dir, volume);
            e.errnb = -1;
        }


    } else {
        fprintf(stderr, "volume %d doesn't exist on %s\n", volume, id.name);
        e.errnb = -1;
    }
    return e;
}



/**
 * \fn find_name(iter i, disk_id disk, int part, int *place) {
 * \brief on cherche iter(qui correspond a un chemin) dans la partition part du disk , et on remplie place avec le numéro du fichier dans la table des fichiers.
 * \param disk est le disk
 * \param i est l'iterateur.
 * \param part le numéro du volume
 * \param place est l'entier qu'on va remplir.
 * \return on retourne une erreur
 * */


error find_name(iter i, disk_id disk, int part, int *place) {
    error e;
    char *nom = i->name;
    int idtable = 0;
    int bool = 1;
    while (bool == 1) {
        if (i->next != NULL) {
            int numblock;
            int pos;
            error err = name_in_dir(disk, part, idtable, nom, &numblock, &pos);
            if (err.errnb != -1) { //on regarde si nom est bien une entrée du repertoire idtable 
                block b;
                read_block(disk, &b,int_to_little( numblock));
                readint_block(&b, &idtable, pos);
                i = go_next(i);
                nom = i->name;
            } else {
                e.errnb = -1;
                fprintf(stderr, "wrong path");
                return e;
            }
        } else {
            int numblock;
            int pos;
            error err = name_in_dir(disk, part, idtable, nom, &numblock, &pos);
            if (err.errnb != -1) {
                block b;
                read_block(disk, &b,int_to_little( numblock));
                readint_block(&b, &idtable, pos);
                *place = idtable;
                e.errnb = 0;
                return e;
            } else {
                e.errnb = -1;
                fprintf(stderr, "wrong path");
                return e;
            }
        }
    }
    e.errnb = -1;
    fprintf(stderr, "wrong path");
    return e;
}

/**
 *\fn free_file_blocks(disk_id* disk, int volume, int id_f) {
 *\brief cette fonction permet de libérer tout les blocks d'un fichier.
 *\param disk
 *\param volume
 *\param id_f représente le numéro du fichier dans la table des fichiers dont on va libérer les blocks
 *\return on retourne une erreur
 **/
error free_file_blocks(disk_id* disk, int volume, int id_f) {
    error e;
    if (disk != NULL) {
        if (0 <= volume && volume < disk->nbPart) {
            Part here = (*disk).tabPart[volume];
            if (id_f > 0 && id_f < here.max_file_count) {
                error err;
                int i = 1;
                err = remove_file_block(disk, volume, id_f);
                while (err.errnb == 0) {
                    err = remove_file_block(disk, volume, id_f);
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

/**
 *\fn error add_file_block(disk_id* disk, int id_part, int id_f, int id_block) 
 *\brief Add block to a file by adding num block on the file Table for the file 
 *\param disk Disk_id* disk containing the file 
 *\param id_part Int represents the number of the partition on teh disk
 *\param id_f Int represents the number file on file Table
 *\param id_block
 *\return Error if the operation is a success e.errnb = 0 else -1 
 * */
error add_file_block(disk_id* disk, int id_part, int id_f, int id_block) {
    error e;
    error e1;

    if (id_part < (*disk).nbPart && id_part>-1) {
        Part p = (*disk).tabPart[id_part];
        if (id_block > 3 && p.taille > id_block) {
            if (p.max_file_count > id_f) {
                //position of the file on file_Table on disk
                int posf_fTab = (64 * id_f) / 1024;
                if ((64 * id_f) % 1024 > 0) {
                    posf_fTab += 1;
                }
                printf("Val of posf_fTab %d\n", posf_fTab);
                int posf_part = posf_fTab + p.num_first_block;
                printf("Val of posf_part %d\n", posf_part);
                block finfo;
                read_block((*disk), (&finfo), int_to_little(posf_part));
                int pos;
                if (id_f % 16 > 0) {
                    pos = (id_f % 16)*64;
                } else {
                    pos = 15 * 64;
                }
                printf("Val of pos %d\n", pos);
                int ftfs_size;
                readint_block((&finfo), (&ftfs_size), pos);
                printf("Val of ftfs_size %d\n", ftfs_size);
                int nbBlock_size = (ftfs_size / 1024);
                if (ftfs_size % 1024 > 0) {
                    nbBlock_size += 1;
                }
                printf("Val of nbBlock_size %d\n", nbBlock_size);
                e1 = use_block(disk, id_block, id_part);
                if (e1.errnb == 0) {
                    if (nbBlock_size < 10) {
                        fill_block((&finfo), id_block, pos + 11 + (nbBlock_size * 4));
                        printf("Val of  (((id_f-1)*64)+11+(nbBlock_size*4)) %d\n", pos + 11 + (nbBlock_size * 4));
                        write_block((*disk), finfo, int_to_little(posf_part));
                        e.errnb = 0;
                    } else if (nbBlock_size < 266) {
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

                    } else {
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
                    }
                } else {
                    e.errnb = -1;
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

/**
 *\fn error remove_file_block(disk_id* disk, int id_part, int id_f) 
 *\brief Remove the last block of file
 *\param disk Disk_id* disk wich will be used 
 *\param id_f Int num file on file Table 
 *\param id_part Int num of the partition in tabPart
 *\return Error Error if the operation is a success e.errnb = 0 else -1 
 **/
error remove_file_block(disk_id* disk, int id_part, int id_f) {
    error e;
    error e1;
    error e2;
    int i;
    if ((*disk).nbPart > id_part) {
        Part p = (*disk).tabPart[id_part];
        if (id_f > -1) {
            int posf_block = (64 * id_f) / 1024;
            if ((64 * id_f) % 1024 > 0) {
                posf_block += 1;
            }
            int posf_id;
            if (posf_block % 16 > 0) {
                posf_id = posf_block % 16;
            } else {
                posf_id = 16;
            }
            printf("Val of posf_id %d et posf_block %d\n", posf_id, posf_block);
            block finfo;
            e1 = read_block((*disk), &finfo, int_to_little(p.num_first_block + posf_block));
            if (e1.errnb != -1) {
                int tfs_size;
                readint_block(&finfo, (&tfs_size), (posf_id)*64);
                printf("Val of tfs_size %d\n", tfs_size);
                if (tfs_size > 0) {
                    int size = tfs_size / 1024;
                    if (tfs_size % 1024 > 0) {
                        size += 1;
                    }
                    printf("Val of size %d\n", size);
                    if (size < 11) {
                        printf("Val of (posf_id*64)+11+(4*(size-1)) %d\n", (posf_id * 64) + 12 + (4 * (size - 1)));
                        int numblock_to_use;
                        readint_block(&finfo, &numblock_to_use, (posf_id * 64) + 12 + (4 * (size - 1)));
                        printf("Val of numblock_to_use %d\n", numblock_to_use);
                        for (i = (posf_id * 64) + 11 + (4 * (size - 1)); i < ((posf_id - 1)*64) + 11 + (4 * (size - 1)); i++) {
                            finfo.buff[i] = '\0';
                        }
                        write_block((*disk), finfo, int_to_little(p.num_first_block + posf_block));
                        printf("Val of size %d\n", size);
                        e2 = free_block(disk, numblock_to_use, id_part);
                        if (e2.errnb != -1) {
                            e.errnb = 0;
                        } else {
                            e.errnb = -1;
                        }
                    } else if (size < 267) {
                        int ind1;
                        readint_block(&finfo, (&ind1), (posf_id - 1)*64 + 52);
                        printf("Val of ind1 %d\n", ind1);
                        block indirect1;
                        read_block((*disk), &indirect1, int_to_little(p.num_first_block + ind1));
                        int rsize = size - 10;
                        printf("Val of rsize %d\n", rsize);
                        int numblock_to_use;
                        readint_block(&indirect1, &numblock_to_use, (rsize - 1)*4);
                        for (i = (rsize - 1)*4; i < (rsize * 4); i++) {
                            indirect1.buff[i] = '\0';
                        }
                        printf("Val of numblock_to_use %d\n", numblock_to_use);
                        write_block((*disk), indirect1, int_to_little(p.num_first_block + ind1));
                        printf("Val of size %d\n", size);
                        e2 = free_block(disk, size, id_part);
                        if (e2.errnb != -1) {
                            e.errnb = 0;
                        } else {
                            e.errnb = -1;
                        }
                    } else {
                        int ind2 = -1;
                        readint_block(&finfo, (&ind2), (posf_id - 1)*64 + 56);
                        printf("Val of ind2 %d\n", ind2);
                        block indirect2;
                        read_block((*disk), &indirect2, int_to_little(p.num_first_block + ind2));
                        int rsize = size - 266;
                        int dsize = (rsize / 256) + (rsize % 256);
                        int ind1;
                        readint_block(&indirect2, (&ind1), (dsize - 1));
                        printf("Val of ind1 %d\n", ind1);
                        block indirect1;
                        read_block((*disk), &indirect1, int_to_little(p.num_first_block + ind1));
                        int ind_to_rem = dsize - (rsize / 256);
                        printf("Val of ind_to_rem %d\n", ind_to_rem);
                        int numblock_to_use;
                        if (ind_to_rem != 0) {
                            readint_block(&indirect1, &numblock_to_use, (ind_to_rem - 1)*4);
                            for (i = (ind_to_rem - 1)*4; i < ind_to_rem * 4; i++) {
                                indirect1.buff[i] = '\0';
                            }
                        } else {
                            readint_block(&indirect1, &numblock_to_use, (dsize - 1)*4);
                            for (i = (dsize - 1)*4; i < (dsize * 4); i++) {
                                indirect1.buff[i] = '\0';
                            }
                        }
                        write_block((*disk), indirect1, int_to_little(p.num_first_block + ind1));
                        printf("Val of size %d\n", size);
                        e2 = free_block(disk, numblock_to_use, id_part);
                        if (e2.errnb != -1) {
                            e.errnb = 0;
                        } else {
                            e.errnb = -1;
                        }
                    }
                } else {
                    e.errnb = 0;
                    fprintf(stderr, "remove_file_block : File have 0 blocks\n");
                }
            } else {
                e.errnb = -1;
                fprintf(stderr, "remove_file_block : Problem with read_block\n");
            }
        } else {
            e.errnb = -1;
            fprintf(stderr, "remove_file_block : Problem with id_f of file %d\n", id_f);
        }
    } else {
        e.errnb = -1;
        fprintf(stderr, "remove_file_block : Problem with id_part %d\n", id_part);
    }
    return e;
}

error test_file(disk_id* disk, int id_part, char* name) {
    error e;
    if ((*disk).nbPart > id_part && id_part > -1) {
        Part p = (*disk).tabPart[id_part];
        printf("Val of p.max_file_count dans test %d\n", p.max_file_count);
        block b;
        error e1 = read_block((*disk), &b, int_to_little(p.num_first_block + 1));
        if (e1.errnb != -1) {
            int fbloc = p.num_first_block + p.file_table_size + 2;
            block c;
            c.buff[0] = 'E';
            c.buff[1] = 'n';
            c.buff[2] = 'c';
            c.buff[3] = 'h';
            c.buff[4] = 'a';
            c.buff[5] = 'n';
            c.buff[6] = 'e';
            c.buff[7] = '!';
            int i;
            error e2 = write_block((*disk), c, int_to_little(fbloc));
            if (e2.errnb != -1) {
                int tab[16];
                i = 0;
                tab[i] = 1024;
                i++;
                tab[i] = 0;
                i++;
                tab[i] = 0;
                i++;
                tab[i] = fbloc;
                printf("Val of fbloc %d\n", fbloc);
                for (i = 5; i < 16; i++) {
                    tab[i] = 0;
                }
                for (i = 0; i < 16; i++) {
                    fill_block(&b, tab[i], 64 + (i * 4));
                }
                int erf;
                readint_block(&b, &erf, 64 + 12);
                printf("Val of erf %d\n", erf);
                write_block((*disk), b, int_to_little(p.num_first_block + 1));
                block racine;
                error e3 = read_block((*disk), &racine, int_to_little(p.num_first_block + p.file_table_size + 1));
                if (e3.errnb != -1) {
                    fill_block(&racine, 2, 64);
                    for (i = 68; i < 70; i++) {
                        racine.buff[i] = name[i];
			
                    }
                    error e4 = write_block((*disk), racine, int_to_little(p.num_first_block + p.file_table_size + 1));
                    if (e4.errnb != -1) {
                        //printf("L'écriture et la création du fichier a réussi\n");	
                        printf("Val of p.max_file_count dans test %d\n", p.max_file_count);
                        e.errnb = 0;
                    }
                }
            }
        }
    } else {
        e.errnb = -1;
    }
    return e;
}

/**
 * \fn int* get_indirect1(disk_id* disk, int id_part, int id_f)
 * \brief Get the indirect1 blocks of a file
 * \param disk Disk_id disk of the partition where the file is located
 * \param id_part Integer number of the partition on disk
 * \param id_f Integer number of the file on files Table on partition
 * \return Array of indirect1 blocks number
 * */
int* get_indirect1(disk_id* disk, int id_part, int id_f){
	int i;
	int* tab = malloc(sizeof(int)*256);
	if((*disk).nbPart > id_part && id_part > -1 && id_f>-1){
		Part p = (*disk).tabPart[id_part];
		block finfo;
		block indirect1;
		int indi1;
		int fpos = id_f*64;
		int fposf = (id_f*64)/1024;
		if(fpos%1024 > 0){
			fposf +=1;
		}
		read_block((*disk),&finfo,int_to_little(fposf + p.num_first_block));
		if(fpos%16 > 0){
			fpos = (fpos%16)-1;
		}else{
			fpos = 15;
		}
		readint_block(&finfo,&indi1,fpos*64+52);
		if(indi1>0){
			read_block((*disk),&indirect1,int_to_little(indi1 + p.num_first_block));
			for(i=0; i<256; i++){
				fill_block(&indirect1, tab[i],i*4);
			}
		}else{
			for(i=0; i<256; i++){
				tab[i] = 0;
			}	
		}
	}
	return tab;
}

/**
 * \fn error set_size_file(disk_id* disk, int id_part, int id_f, int no)
 * \brief Set the size of file to no 
 * \param disk
 * \param id_part
 * \param id_f
 * \param no new size file
 * \return Error
 * */
error set_size_file(disk_id* disk, int id_part, int id_f, int no){
	error e;
	if(id_part>-1 && id_part<(*disk).nbPart && id_f>-1){
		Part p = (*disk).tabPart[id_part];
		int pos = (id_f*64);
		int nbpos = (pos/1024);
		if(pos%1024 > 0){
			nbpos += 1;
		}
		block finfo;
		error e1 = read_block((*disk),&finfo,int_to_little(p.num_first_block+nbpos));
		if(e1.errnb != -1){
			int fpos;
			if(id_f%16 > 0){
				fpos = (id_f%16)-1;
			}else{
				fpos = 15;
			}
			error e2 = fill_block(&finfo, no, (fpos*64));
			if(e2.errnb != -1){
				e.errnb = 0;
			}else{
				e.errnb =-1;
			}
		}else{
			e.errnb = -1;	
		}
	}else{
		e.errnb = -1;
	}
	return e;
} 


/**
 * \fn int lastbf(disk_id* disk, int id_part, int id_f)
 * \brief Return the last block use by the file
 * \param disk Disk_id* which containing the file
 * \param id_part Int
 * */
int lastbf(disk_id* disk, int id_part, int id_f){
	if(id_part>-1 && id_part<(*disk).nbPart && id_f>-1){
		Part p = (*disk).tabPart[id_part];
		int fpos = id_f*64;
		int fbpos = (fpos/1024);
		if(fbpos % 1024 > 0){
			fbpos += 1;
		}
		block finfo;
		read_block((*disk),&finfo,int_to_little(p.num_first_block + fbpos));
		int pos;
		if(fpos % 16 >0){
			pos = (fpos % 16)-1; 
		}else{
			pos = 15;
		}
		int fsize = 0;
		fill_block(&finfo,fsize, pos*64);
		int tfsize = fsize /1024;
		if(fsize %1024 >0){
			tfsize += 1;
		}
		if(tfsize<11){
			int result = 0;
			fill_block(&finfo,result, pos*64+12+(tfsize-1)*4);
			if(result>0){
				return result;
			}
		}else if(tfsize<266){
			int indi1 = -1;
			fill_block(&finfo,indi1, (pos-1)*64+12+(10*4));
			block indirect1;
			read_block((*disk),&indirect1,int_to_little(p.num_first_block + indi1));
			tfsize -= 10;
			int result = 0;
			fill_block(&indirect1, result, (tfsize-1)*4);
			if(result>0){
				return result;
			}
		}else{
			int ind2;
			readint_block(&finfo, (&ind2), (pos-1)*64 + 56);
			printf("Val of ind2 %d\n",ind2);
			block indirect2;
			read_block((*disk), &indirect2, int_to_little(p.num_first_block + ind2));
			int rsize = tfsize - 266;
			int dsize = (rsize/256) + (rsize%256);
			int ind1;
			readint_block(&indirect2,(&ind1),(dsize-1));
			printf("Val of ind1 %d\n",ind1);
			block indirect1;
			read_block((*disk), &indirect1, int_to_little(p.num_first_block+ind1));
			int ind_to_rem = dsize - (rsize/256);
			printf("Val of ind_to_rem %d\n",ind_to_rem);
			int numblock_to_use;
			if(ind_to_rem != 0 ){
				readint_block(&indirect1,&numblock_to_use,ind_to_rem*4);
			}else{
				readint_block(&indirect1,&numblock_to_use,dsize*4);
			}
			return numblock_to_use;				
		
		}
	}else{
		return -1;
	}
	return -1;
}


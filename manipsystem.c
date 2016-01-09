#include "manipsystem.h"
#include <ctype.h>

int isNumber(char *buf) {
    int boolean = 0;
    int i;
    for (i = 0; i < strlen(buf); i++) {
        if (isdigit((int) buf[i]) == 0) {
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
            i = i->next;
            if (strcmp("HOST", i->name)) {//dans ce cas on fait les opérations sur le systeme
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
                    i = i->next;
                    if (isNumber(i->name) == 0) {
                        int volume = atoi(name);
                        if (volume < id->nbPart) {//on verifie si le volume existe sur le disque
                            if (id->tabPart[volume].free_file_count > 0) {//on verifie si on peut ajouter un autre fichier sur ce volume
                                if (id->tabPart[volume].free_block_count == 0) {
                                    go_end(i);
                                    char *namedir = i->name; //on recupere le nom du dossier a creer
                                    if (strlen(namedir) <= 27) {
                                        iter ni = i->prec;
                                        ni->next = NULL;
                                        i = NULL;
                                        free(i); //on supprime l'itération comprenant le nom
                                        go_start(i);
                                        i = i->next;
                                        i = i->next;
                                        int posbloc;
                                        int posinbloc;
                                        error e1;
                                        int place;
                                        if (i->next != NULL) {// on regarge si le dossier sur lequel on doit creer notre dossier n'est pas le dossier racine
                                            i = i->next;

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
                                    fprintf(stderr, "volume number %d is full", volume);
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
                    fprintf(stderr, "you must give a name to your new directory");
                    return -1;
                }
            } else {
                fprintf(stderr, "disk isn't running \n");
                return -1;
            }
        } else {
            fprintf(stderr, "wrong path");
        }
    } else {
        fprintf(stderr, "Wrong path, miss \'FILE:\'\n");
        return -1;
    }
    return -1;
}

int tfs_rename(const char *old, const char *new) {
    int length = strlen(new);
    if (length < 28) {
        iter iter = decomposition(strdup(old));
        if (strcmp(iter->name, "FILE:") == 0) {
            if (iter->next != NULL) {
                iter = iter->next;
                if (strcmp(iter->name, "HOST") == 0) {
                    return 1; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                } else {
                    if (iter->next != NULL) {
                        iter = iter->next;
                        int i;
                        for (i = 0; i < MAX_DISQUE; i++) {
                            if (disque_ouvert[i] != NULL) {
                                if (strcmp((disque_ouvert[i])->name, iter->name) == 0) {
                                    disk_id *disk = disque_ouvert[i];
                                    iter = iter->next;
                                    if (iter->next != NULL) {
                                        iter = iter->next;
                                        int part = atoi(iter->name);
                                        if (part >= 0 && part < disk->nbPart) {
                                            Part here = disk->tabPart[part];
                                            go_end(iter);
                                            char *oldname = iter->name;
                                            iter = iter->prec;
                                            iter->next = NULL;
                                            go_start(iter);
                                            int place;
                                            error e = find_name(iter, *disk, part, &place);
                                            if (e.errnb == 0) {
                                                int numblock;
                                                int pos;
                                                if (name_in_dir(*disk, part, place, oldname, &numblock, &pos).errnb != -1) {
                                                    block b;
                                                    read_block(*disk, &b, numblock + here.num_first_block);
                                                    int j;
                                                    for (j = 0; j < length; j++) {
                                                        b.buff[pos + 4 + j] = new[j];
                                                    }
                                                    for (j = length; j < 28; j++) {
                                                        b.buff[j] = '\0';
                                                    }
                                                    return 0;
                                                }
                                            }
                                        } else {
                                            fprintf(stderr, "tfs_rename : no part \n");
                                        }
                                    } else {
                                        fprintf(stderr, "tfs_rename : wrong path \n");
                                    }
                                }
                            }
                            return -1;
                        }
                        fprintf(stderr, "tfs_rename : no disk open \n");
                    } else {
                        fprintf(stderr, "tfs_rename : wrong path \n");
                    }
                }
            } else {
                fprintf(stderr, "tfs_rename : NULL pointeur");
            }
        } else {
            fprintf(stderr, "tfs_rename : wrong path, miss FILE: \n");
        }
    } else {
        fprintf(stderr, "tfs_rename : char new too long, max length=28 \n");
    }
    return -1;
}

int tfs_open(const char *name, int oflag) {
    iter iter = decomposition(strdup(name));
    if (strcmp(iter->name, "FILE:") == 0) {
        if (iter->next != NULL) {
            iter = iter->next;
            if (strcmp(iter->name, "HOST") == 0) {
                return 1; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            } else {
                int i;
                for (i = 0; i < MAX_DISQUE; i++) {
                    if (disque_ouvert[i] != NULL) {
                        if (strcmp((disque_ouvert[i])->name, iter->name) == 0) {
                            disk_id *disk = disque_ouvert[i];
                            if (iter->next != NULL) {
                                iter = iter->next;
                                int part = atoi(iter->name);
                                if (part >= 0 && part < disk->nbPart) {
                                    Part here = disk->tabPart[part];
                                    int place;
                                    error e = find_name(iter, *disk, part, &place);
                                    if (e.errnb == 0) {
                                        block b;
                                        read_block(*disk, &b, here.num_first_block + 1 + (place - 1) / 16);
                                        int a;
                                        readint_block(&b, &a, place % 16 + 4);
                                        if (a == 0) { //si num correspond a un fichier
                                            tfs_fd f;
                                            f.pdisk = disk;
                                            f.partition = part;
                                            f.pos = place;
                                            f.flag = oflag;
                                            f.path = strdup(name);
                                            int j;
                                            for (j = 0; j < MAX_FICHIERS; j++) {
                                                if (fichiers_ouverts[i] == NULL) {
                                                    fichiers_ouverts[i] = &f;
                                                    return i;
                                                }
                                            }
                                            fprintf(stderr, "tfs_open : fichiers_ouverts is full \n");
                                        } else {
                                            fprintf(stderr, "tfs_open : nom correspond a un dossier \n");
                                        }
                                    } else {
                                        fprintf(stderr, "tfs_open : wrong path \n");
                                    }
                                } else {
                                    fprintf(stderr, "tfs_open : no part \n");
                                }
                            } else {
                                fprintf(stderr, "tfs_open : wrong path \n");
                            }
                        } else {
                            fprintf(stderr, "tfs_open : wrong path \n");
                        }
                    } else {
                        fprintf(stderr, "tfs_open : wrong path \n");
                    }
                    return -1;
                }
                fprintf(stderr, "tfs_open : no disk open \n");
            }
        } else {
            fprintf(stderr, "tfs_open : NULL pointeur");
        }
    } else {
        fprintf(stderr, "tfs_open : wrong path, miss FILE: \n");
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


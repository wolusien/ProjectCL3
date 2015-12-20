#include "interne.h"

int main(int argc, char* argv[]) {
    error e;
    int i;
    int count = 0;
    int partition[argc - 4];
    
    if (argc >= 5) {
        //Test the argument of the cmd tfs_partition
        if (strcmp("-p", argv[1]) == 0) {
            if (strcmp("size", argv[2]) == 0) {
                char* strdisk = argv[argc - 1];
                int l = strlen(strdisk);
                //Test disk syntax name
                if (l >= 5) {
                    if (strdisk[l - 4] == '.' && strdisk[l - 3] == 't' 
                    && strdisk[l - 2] == 'f' && strdisk[l - 1] == 's') {
                        //Search if the disk given by argv[argc-1] is open or not
                        disk_id* disk = malloc(sizeof (disk_id));
                        //Test if the disk given by argv[argc-1] is open or not
                        e = start_disk(argv[argc - 1], disk);
                        if (e.errnb == 0) {
                            //Read the first block of the disk
                            block b;
                            uint32_t num = int_to_little(0);
                            read_physical_block((*disk), (&b), num);
                            //Get size of partitions and fill the tab partition
                            for (i = 3; i < argc - 1; i++) {
                                int psize = atoi(argv[i]);
                                if (psize != 0 
                                //tant que nbBlock n'est pas initialisé
                                //&& psize < (*disk).nbBlock - 1
                                ) {
                                    partition[count] = psize;
                                    count++;
                                }
                            }
                            if (count > 0) {
                                //Case where we have good partition
                                (*disk).nbPart = count - 1;
                                for (i = 0; i < count; i++) {
                                    (*disk).taillePart[i] = partition[i];
									printf("Valeur de la taille de la partition %d\n",partition[i]);
                                }
                                uint32_t nbPart = int_to_little(count - 1);
                                unsigned char* nb = (unsigned char*) (&nbPart);
                                //File the block b with the number of partition
                                for (i = 4; i < 8; i++) {
                                    b.buff[i] = nb[i];
                                }
								int j=8;
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
                            } else {
                                printf("Aucun partitionnage du disque possible, mauvaise dimension de partition!\n");
                            }
						}else {
							exit(-1);
						}
					} else {
						printf("tfs_partition : Wrong arguments for disk\n");
						exit(-1);
					}
				} else {
					printf("tfs_partition : Wrong arguments number\n");
					exit(-1);
                }
			} else {
                printf("tfs_partition : Wrong syntax\n");
                exit(-1);
			}
        } else {
            printf("tfs_partition : Wrong syntax\n");
            exit(-1);
        }
    } else {
        printf("tfs_partition : Wrong arguments number\n");
        exit(-1);
    }
    exit(0);
}

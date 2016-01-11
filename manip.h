/**
 * \file manip.h
 * \brief api de manipulation du disk
 * \author Lucas.L Abel.H Wissam.D
 *
 * 
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "error.h"
#include "little.h"

#define SIZEBLOCK 1024
#define MINBLOCK 3
#define BUFSIZE 256
#define MAX_DISQUE 5
#define DEFAULT_SIZE_TFS 50
#define MAX_PART 254
#define MAGIC_NUMBER 0x31534654

/**
 * \struct Part
 * \brief descripteur de partition
 **/

typedef struct{
    int taille;//number of blocks
    int num_first_block;//number of the first block in the disk
    int free_block_count;
    int first_free_block;
    int max_file_count;
    int free_file_count;
    int first_free_file;
    int file_table_size;//number of blocks
}Part;

/**
 * \struct disk_id
 * \brief descripteur de disk
 * 
 *  
 */
typedef struct{
    int id;
    char *name;
    int fd;
    int nbBlock;
    int nbPart;
    Part tabPart[MAX_PART];
}disk_id;

disk_id* disque_ouvert[MAX_DISQUE];

typedef struct{
  char buff[1024];
  int id;
}block;

error read_physical_block(disk_id id,block *b,uint32_t num);
error write_physical_block(disk_id id,block b,uint32_t num);
error start_disk(char *name,disk_id *id);
error stop_disk(disk_id id);
error read_block(disk_id id,block *b,uint32_t num);
error write_block(disk_id id,block b,uint32_t num);
error fill_block(block *b, int a,int loc);
error readint_block(block *b, int *a,int loc);





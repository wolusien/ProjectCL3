/**
 * \file manipvolume.h
 * \brief File contains functions and struct for partition and blocks
 * \author DJEBALI LABADENS HUMBERT
 **/

#include "manip.h"
#include "iter.h"


#define TFS_READ 1
#define TFS_WRITE 2
#define TFS_RWRITE 3
#define TFS_APPEND 4
#define TFS_ORAPPEND 5




error free_block(disk_id *id, int numblock, int volume);
error use_block(disk_id *id, int numblock, int volume);
error add_free_file(disk_id id,int volume,int file);
error remove_free_file(disk_id id,int volume,int file);
error add_file_block(disk_id* disk,int id_part,int id_f, int id_block);
error remove_file_block(disk_id* disk,int id_part,int id_f);
error test_file(disk_id* disk, int id_part, char* name);
error free_file_blocks(disk_id* disk, int volume, int id_f);
int* get_indirect1(disk_id* disk, int id_part, int id_f);
error set_size_file(disk_id* disk, int id_part, int id_f, int no);
int lastbf(disk_id* disk, int id_part, int id_f);

iter decomposition(char *path);
error readname_rep(block b, char *a, int loc);
int name_in_block(disk_id id, int volume, int num_block, char *name);
error name_in_dir(disk_id id, int volume, int dir, char *name, int *numblock, int *posi);
void file_tableau(int *tab[16], disk_id id, int volume);
error find_name(iter i, disk_id disk, int part, int *place);

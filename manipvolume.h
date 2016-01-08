#include "manip.h"







struct iter{
  char *name;
  struct iter *next;
  struct iter *prec;
};
typedef struct iter* iter;

void printiter(iter i);
void go_end(iter i);
error free_block(disk_id *id, int numblock, int volume);
error use_block(disk_id *id, int numblock, int volume);
error add_free_file(disk_id id,int volume,int file);
error remove_free_file(disk_id id,int volume,int file);
iter  decomposition(char *path);
error add_file_block(disk_id* disk,int id_part,int id_f, int id_block);
error remove_file_block(disk_id* disk,int id_part,int id_f);
error test_file(disk_id* disk, int id_part, char* name);

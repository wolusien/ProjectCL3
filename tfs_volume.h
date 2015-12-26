#include "manip.h"

typedef struct{
  long TTTFS_MAGIC_NUMBER;
  int TTTFS_VOLUME_BLOCK_SIZE = 1024;
  int TTTFS_VOLUME_BLOCK_COUNT;
  int TTTFS_VOLUME_FREE_BLOCK_COUNT;
  int TTTFS_VOLUME_FIRST_FREE_BLOCK;
  int TTTFS_VOLUME_MAX_FILE_COUNT;
  int TTTFS_VOLUME_FREE_FILE_COUNT;
  int TTTFS_VOLUME_FIRST_FREE_FILE;
}tfs_desc_block;

typedef struct{
  uint32_t tfs_size;
  int tfs_type;
  int tfs_subtype;
  int tfs_direct[10];
  int tfs_indirect1;
  int tfs_indirect2;
  int tfs_next_free;
}tfs_file;

typedef struct{
  tfs_desc_block descBlock;
  int deb_fileTab;//octet de debut de la table de fichiers
  tfs_file fileTab[TTTFS_VOLUME_MAX_FILE_COUNT];
  int end_fileTab;//octet de fin de la table de fichiers
  int idPart//indice de la partition ds le tableau taillePart
}tfs_volume

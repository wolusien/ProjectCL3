#include "manipvolume.h"
#include <ctype.h>

#define MAX_FICHIERS 10

typedef struct{
  disk_id *pdisk;
  int partition;
  int pos;
  int flag;
  char *path;
}tfs_fd;

tfs_fd* fichiers_ouverts[MAX_FICHIERS];



//int tfs_mkdir(const char *path, mode_t mode);
//int tfs_rmdir(const char *path);
int tfs_rename(const char *old, const char *new);
int tfs_open(const char *name,int oflag);
//ssize_t tfs_read(int fildes,void *buf,size_t nbytes);
ssize_t tfs_write(int fildes,void *buff,size_t nbytes);
int tfs_close();
//off_t tfs_lseek(int fildes,off_t offset,int whence);
//DIR *opendir(const char *filename);
//struct dirent *readdir(DIR *dirp);
//void rewinddir(DIR *dirp);
//int closedir(DIR *dirp);

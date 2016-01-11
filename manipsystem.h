/**
 * \file manipsystem.h
 * \brief File contains functions and struct for manage file and directory
 * \author DJEBALI LABADENS HUMBERT
 */

#include "manipvolume.h"
#include <ctype.h>

#define MAX_FICHIERS 10
/**
*\struct tfs_fd manipsystem.h "File_descriptor" 
*	
*/
typedef struct{
  disk_id *pdisk;
  int partition;
  int pos;
  int flag;
  char *path;
}tfs_fd;

tfs_fd* fichiers_ouverts[MAX_FICHIERS];




//int tfs_rmdir(const char *path);
int tfs_rename(const char *old, const char *new);
int tfs_open(const char *name,int oflag);
int tfs_mkdir(const char *path, mode_t mode);
//ssize_t tfs_read(int fildes,void *buf,size_t nbytes);
ssize_t tfs_write(int fildes,void *buff,size_t numbytes);
int tfs_close();
//off_t tfs_lseek(int fildes,off_t offset,int whence);
//DIR *opendir(const char *filename);
//struct dirent *readdir(DIR *dirp);
//void rewinddir(DIR *dirp);
//int closedir(DIR *dirp);

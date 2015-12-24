int tfs_mkdir(const char *path, mode_t mode){
  char *chem;
  strcnpy(chem,path,7);
  if(strcmp(chem,"FILE://")!=-1){
  
  }
  else{
    printf("path \'%s\' uncorrect\n",path);
    return -1;
  }
}

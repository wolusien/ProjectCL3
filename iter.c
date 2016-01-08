#include "iter.h"

void free_iter(iter i){
  
  if(i != NULL){
    iter next = i->next;
    iter prec = i->prec;
    i->name=NULL;
    free(i->name);
    i=NULL;
    free(i);
    if(next != NULL)
      free_iter(next);
    if(prec != NULL)
      free_iter(prec);
  }
}
void go_end(iter it){
  iter cur =it;
  if(cur !=NULL){
    while(cur ->next!= NULL){
      //printf("%s\n",cur->name);
      cur=cur->next;
    }
  }
  printf("%s\n",cur->name);
}

void go_start(iter it){
  if(it != NULL){
    while(it->prec != NULL){
      it=it->prec;
    }
  }
}

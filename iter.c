#include "iter.h"

void free_iter(iter i){
  
  if(i != NULL){
    go_start(i);
    iter curr = i;
    iter next;
    while(curr->next != NULL){
        next=curr->next;
        curr=NULL;
        free(curr);
        curr=next;
    }   
  }
}
void go_end(iter it){
    printf("debut");
    if(it !=NULL){
    while(it->next != NULL){
      it=it->next;
    }
  }
    printf("%s\n",it->name);
}

void go_start(iter it){
  if(it != NULL){
    while(it->prec != NULL){
      it=it->prec;
    }
  }
}

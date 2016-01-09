#include "iter.h"

void free_iter(iter i){
  
  if(i != NULL){
    go_start(i);//on ramene au début de l'itération
    iter curr = i;
    iter next;//on recupere le prochain
    while(curr->next != NULL){//on refait l'opération jusqu'a qu'il n'y est plus d'itération
        next=curr->next;
        curr=NULL;
        free(curr);//on free l'itération courante
        curr=next;//et on lui attribut la valeur du prochain
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

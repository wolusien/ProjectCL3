#include "iter.h"

void free_iter(iter i){
  
  if(i != NULL){
    i = go_start(i);//on ramene au début de l'itération
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
iter go_end(iter it){
    iter current = it;
    if(current !=NULL){
    while(current->next != NULL){
        current=go_next(current);
    }
    return current;
  }
    return NULL;
}

iter go_start(iter it){
    iter current= it;
    if(current != NULL){
    while(current->prec != NULL){
        current=go_prec(current);
    }
    return current;
  }
    return NULL;
}
iter go_next(iter it){
    iter current =it;
    if(current != NULL && it->next !=NULL){
        current = it->next;
        return current;
    }
    return NULL;
}

iter go_prec(iter it){
    iter current = it;
    if(current != NULL && it->prec !=NULL){
        current = it->prec;
        return current;
    }
    return NULL;
}
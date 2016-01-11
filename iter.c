/**
 * \file iter.c
 * \brief fichier contenant les fonctions liées au itération
 * \author DJEBALI LABADENS HUMBERT
 */
#include "iter.h"
/**
 * \fn void free_iter(iter i)
 * \brief free toutes les itérations
 * 
 * \param i
 * \return void
 */
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
/**
 * \fn iter go_end(iter it)
 * \brief ramene a la fin du chemin
 * 
 * \param it itération qu'on veut ramener a celle du début
 * \return une itération 
 */
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

/**
 * \fn iter go_start(iter it)
 * \brief ramene au début du chemin
 * 
 * \param it itération qu'on veut ramener a celle de fin
 * \return une itération 
 */
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
/**
 * \fn iter go_next(iter it)
 * \brief avance dans le chemin
 * 
 * \param it 
 * \return it->next
 */
iter go_next(iter it){
    iter current =it;
    if(current != NULL && it->next !=NULL){
        current = it->next;
        return current;
    }
    return NULL;
}
/**
 * \fn iter go_prec(iter it)
 * \brief recul dans le chemin
 * 
 * \param it 
 * \return it->next
 */
iter go_prec(iter it){
    iter current = it;
    if(current != NULL && it->prec !=NULL){
        current = it->prec;
        return current;
    }
    return NULL;
}
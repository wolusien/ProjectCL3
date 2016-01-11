
/**
 * \file iter.h
 * \brief 
 * \author Lucas.l Abel.h Wissam.D
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/**
 *\struct  iter 
 * 
 * une itération est un objet chaine par son précédent et son suivant et représenté par un nom
 */
struct iter{
  char *name;
  struct iter *next;
  struct iter *prec;
};
typedef struct iter* iter;

void free_iter(iter i);
iter go_start(iter i);
iter go_end(iter i);
iter go_next(iter i);
iter go_prec(iter i);
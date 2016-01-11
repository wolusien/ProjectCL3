#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
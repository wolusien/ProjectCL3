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
void go_start(iter i);
void go_end(iter i);

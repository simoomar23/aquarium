#include "liste.h"
#include <stddef.h>


struct link lnk__empty(void) {
  struct link l;
  l.head = NULL;
  return l;
}
struct set * set__empty(){
  struct set *s = malloc(sizeof(struct set));
  s->l = malloc(sizeof(struct link));
  *(s->l) = lnk__empty();
  s->size = 0;
  return s;
}
struct lelement *get_head(struct set *s ){
  return s->l->head;
}
poisson get_poisson(struct lelement*elt){
  return elt->poisson;
}

struct lelement *set__find(struct set *s,poisson Poisson){
  struct lelement *e=s->l->head;
  for(int i=0;i<s->size;i++){
    if(same(get_poisson(e),Poisson)){
      return e;
    }
    e=e->next;
  }
  return NULL;
}
//la direction de add par rapport à la tile 
int set__add_head(struct set *s, poisson poisson){

  struct lelement *e = malloc(sizeof(struct lelement));
  e->poisson = poisson;
  e-> next = get_head(s);
  e->prev = NULL;
  s->l->head = e;
  return 0;
}
int set__remove(struct set *s,poisson poisson) {
  struct lelement * e  = set__find(s,poisson);
  struct lelement * next = e->next;
  struct lelement *prev = e->prev;
  free(e);
  if(prev == NULL){
	s->l->head = next;
  }
  else{
	prev->next = next;
  }
  return 0;
}
void free_set(struct set *s) {
    while (s->l->head != NULL) {
        struct lelement *c = s->l->head;
	      s->l->head = s->l->head->next;
        free(c);
    }
    free(s->l);
    free(s);
}
int set_size(struct set *s){
  return s->size;
}

void set_print(struct set *s,char*prefix){
  printf("%s [",prefix);
  struct lelement *elt=s->l->head;
  for(int i=0;i<s->size;i++){
    print_poisson(elt->poisson);
    elt=elt->next;
  }
  printf("]\n");
}
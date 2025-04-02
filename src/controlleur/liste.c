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

int in(poisson poisson,int x,int y,int length,int width){
	return (x <=poisson.x && poisson.x < x+length) &&  (y <=poisson.y && poisson.y < y+width);
}


struct set * get_fishes_view(struct set * fishes ,int x,int y,int length,int width){
	struct lelement *e = fishes->l->head;
	struct set * fishes_in = set__empty();
	for(int i=0;i<fishes->size;i++){
		if(in(e->poisson,x,y,length,width)){
			poisson poisson = e->poisson;
			poisson.x = poisson.x -x;
			poisson.y = poisson.y -y;
			assert(!set__add_head(fishes_in,poisson));
			e = e->next;
		}
	}
	return fishes_in;
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
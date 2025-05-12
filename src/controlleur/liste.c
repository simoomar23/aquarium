#include "liste.h"
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>



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

struct lelement *set__find(struct set *s,char * name){
  if( s == NULL && s->l == NULL && s->l->head)
    return NULL;

  struct lelement *e=s->l->head;
  for(int i=0;i<s->size;i++){
    if(!strcmp(get_poisson(e).name,name)){
      return e;
    }
    e=e->next;
  }
  return NULL;
}
int set__add_head(struct set *s, poisson poisson){
  if(set__find(s,poisson.name)!=NULL)
    return 1;

  struct lelement *e = malloc(sizeof(struct lelement));
  e->poisson = poisson;
  e-> next = get_head(s);
  e->prev = NULL;
  if(e->next != NULL)
    e->next->prev = e;
  s->l->head = e;
  s->size++;
  return 0;
}
int set__remove(struct set *s,poisson poisson) {
  struct lelement * e  = set__find(s,poisson.name);
  if(e == NULL)
    return 1;
  struct lelement * next = e->next;
  struct lelement *prev = e->prev;
  free(e);
  if(prev == NULL){
	s->l->head = next;
  }
  else{
	prev->next = next;
  }
  s->size--;
  return 0;
}


int in_d(poisson poisson,int x,int y,int length,int width){
	return in_helper(poisson.coord_d.x,poisson.coord_d.y,x,y,length,width) || \
  in_helper(poisson.coord_d.x + poisson.length,poisson.coord_d.y,x,y,length,width) || \
  in_helper(poisson.coord_d.x,poisson.coord_d.y + poisson.width,x,y,length,width) || \
  in_helper(poisson.coord_d.x + poisson.length,poisson.coord_d.y+poisson.width,x,y,length,width) ;

}

int in_f(poisson poisson,int x,int y,int length,int width){
	return in_helper(poisson.coord_f.x,poisson.coord_f.y,x,y,length,width) || \
  in_helper(poisson.coord_f.x + poisson.length,poisson.coord_f.y,x,y,length,width) || \
  in_helper(poisson.coord_f.x,poisson.coord_f.y + poisson.width,x,y,length,width) || \
  in_helper(poisson.coord_f.x + poisson.length,poisson.coord_f.y+poisson.width,x,y,length,width) ;

}

void update_positions(struct set *fishes ){
  struct lelement *e = fishes->l->head;
  printf("fishes size %d\n",fishes->size);
  for (int i=0;i<fishes->size;i++){
    if(e->poisson.status == STARTED){
      e->poisson.coord_d = e->poisson.coord_f;
      e->poisson.coord_f = e->poisson.mobility(e->poisson.coord_f);
    }

    e = e->next;
  }
}

struct set * get_fishes_in_view(struct set * fishes ,int x,int y,int length,int width){
	struct lelement *e = fishes->l->head;
	struct set * fishes_in = set__empty();
	for(int i=0;i<fishes->size;i++){
		if(in_f(e->poisson,x,y,length,width)){
		  //prepare(&(e->poisson));
			poisson poisson = e->poisson;
			poisson.coord_f.x = ((poisson.coord_f.x -x)*HUNDRED)/length;
      		printf("ssss %d \n",poisson.coord_f.x);
			poisson.coord_f.y = ((poisson.coord_f.y -y)*HUNDRED)/width;
      poisson.length = (poisson.length*HUNDRED)/length;
      poisson.width = (poisson.width*HUNDRED)/width;  
			assert(!set__add_head(fishes_in,poisson));
		}
    e = e->next;
	}
	return fishes_in;
}

struct set * get_fishes_in(struct set * fishes ,int x,int y,int length,int width){
	struct lelement *e = fishes->l->head;
	struct set * fishes_in = set__empty();
	for(int i=0;i<fishes->size;i++){
		if(in_f(e->poisson,x,y,length,width)){
			poisson poisson = e->poisson;
			assert(!set__add_head(fishes_in,poisson));
		}
    e = e->next;
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


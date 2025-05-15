#ifndef __LISTE_H__
#define __LISTE_H__

#include "model.h"
#define HUNDRED 100
//linked list

struct lelement {
  poisson poisson; // data 
  struct lelement *next; // next element
  struct lelement *prev; // previous element
};

struct link {
  struct lelement *head;
};

struct set{
  struct link *l;
  int size;
};


/* returns the first element of s*/
struct lelement *get_head(struct set *s );

/* gives the data of elt element*/
poisson get_poisson(struct lelement*elt);

/*returns the id_in_empty_th element of s*/

/*returns an empty link*/
struct link lnk__empty(void) ;

struct set * set__empty();

/*returns the set size*/
int set_size(struct set *s);

struct lelement *set__find(struct set *s,char * name);

/*add add-element at the begining of s*/
int set__add_head(struct set *s,poisson poisson) ;

/*removes c element from s*/
int set__remove(struct set *s,poisson poisson) ;

struct set * get_fishes_in_view(struct set * fishes ,int x,int y,int length,int width,int fd);

struct set * get_fishes_in(struct set * fishes ,int x,int y,int length,int width);

/*frees s set*/
void free_set(struct set *s) ;

/*prints s set*/
void set_print(struct set *s,char*prefix);

void update_positions(struct set *fishes);

#endif
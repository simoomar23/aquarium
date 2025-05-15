#include "liste.h"
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "log_file.h"
#include <errno.h>
#include <sys/socket.h>
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
  //printf("fishes size %d\n",fishes->size);
  for (int i=0;i<fishes->size;i++){
    if(e->poisson.status == STARTED){
	  e->poisson.coord_d = e->poisson.coord_f;
      e->poisson.coord_f = e->poisson.mobility(e->poisson.coord_f,e->poisson.length,e->poisson.width);
    }

    e = e->next;
  }
}

char * fake_get_fishes(struct set * fishes , int count){
    struct lelement *e = fishes->l->head;
    size_t bufferSize = 2048;
    char *result = malloc(bufferSize);
    if (result == NULL) return NULL;
    result[0] = '\0';

    for (int i = 0; i < count; i++) {
        char line[256];
        //printf("this the name :::: %s\n",fishes[i].name);
        snprintf(line, sizeof(line), "[%s at %dx%d,%dx%d,%d] ",
                 e->poisson.name,
                 e->poisson.coord_d.x, e->poisson.coord_d.y,
                 e->poisson.length,e->poisson.width,0);

        if (strlen(result) + strlen(line) + 1 > bufferSize) {
            bufferSize *= 2;
            char *newResult = realloc(result, bufferSize);
            if (newResult == NULL) {
                free(result);
                return NULL;
            }
            result = newResult;
        }

        strcat(result, line);
    }
    size_t result_size = strlen(result);
    result[result_size] = '\n';
    result[result_size + 1] ='\0';
    char header[] ="list ";


    size_t finalSize = strlen(header) + strlen(result) + 1;
    char *final = malloc(finalSize);
    if (final == NULL) {
        free(result);
        return NULL;
    }

    strcpy(final, header);
    strcat(final, result);
    free(result);

    return final;
}

struct set * get_fishes_in_view(struct set * fishes ,int x,int y,int length,int width , int fd){
	struct lelement *e = fishes->l->head;
  int count = 0;
  int quick = 0;
	struct set * fishes_in = set__empty();
	for(int i=0;i<fishes->size;i++){
		int f2 = in_f(e->poisson,x,y,length,width);
		int f1 = in_d(e->poisson,x,y,length,width);
    //printf("%d;%d , %d;%d\n",f1,f2,e->poisson.coord_d.x,e->poisson.coord_d.y);
		if(f1 || f2){
			poisson poisson = e->poisson;
		  	if (!f1){
				quick = 1;
		  	}
			if(f1 && !f2){
				e->poisson.coord_d = e->poisson.coord_f;

			}
        
      	count++;
	
		poisson.coord_f.x = ((poisson.coord_f.x -x)*HUNDRED)/length;
		poisson.coord_f.y = ((poisson.coord_f.y -y)*HUNDRED)/width;
		poisson.coord_d.x = ((poisson.coord_d.x -x)*HUNDRED)/length;
		poisson.coord_d.y = ((poisson.coord_d.y -y)*HUNDRED)/width;
      	poisson.length = (poisson.length*HUNDRED)/length;
      	poisson.width = (poisson.width*HUNDRED)/width;
      	e->poisson.coord_d = e->poisson.coord_f;  
			assert(!set__add_head(fishes_in,poisson));
		
		}
    e = e->next;
	}
  if(quick){
    char * response = fake_get_fishes(fishes_in,count);
    if (send(fd, response, strlen(response), 0) == -1) {
				perror("send failed");
				log_message(LOG_ERROR, "send failed to client %d: %s", fd, strerror(errno));
		}
    else {
							log_message(LOG_DEBUG, "Response sent to client %d", fd);
    }
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


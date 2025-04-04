#ifndef AQUARIUM 
#define AQUARIUM

#include "model.h"
#include "liste.h"
#define MAX_VIEWS 20



typedef struct aquarium {
	int width;
	int length;
	struct set * fishs;
} aquarium;


typedef struct view {
	int id;
	int x;
	int y;
	int width;
	int length;
}view;

typedef struct views {
	view all_views[MAX_VIEWS];
	int size;
}views;


void initialize_aquarium(int length,int width,view all_views[],int size);
 
struct view make_view(int id,int x,int y,int width, int length);

int find_view(int id);

int add_view(view view);

int del_view(int id);

int add_fish(char *name , int x , int y, int length, int width, coord (*mobility)(coord),int view_id);

int views_size();


poisson get_fish_by_name(char * name,int view_id);

int del_fish(char * name , int view_id);

int start_fish(char * name, int view_number);

poisson * getFishes(int view_id);


#endif 

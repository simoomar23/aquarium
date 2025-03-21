#ifndef AQUARIUM 
#define AQUARIUM

#include "model.h"
#include "liste.h"

extern aquarium *aquarium;

typedef struct aquarium {
	int width;
	int length;
	struct set * fishs;
} aquarium;

void initialize_aquarium();
 
struct aquarium make_view(int x,int y,int width, int length);


int add_fish(poisson poisson , int x , int y, void (*mobility)());

int del_fish(type fish_type , int view_number);

int start_fish(type fish_type , int view_number);


#endif 

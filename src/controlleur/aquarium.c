#include "aquarium.h"
#include "model.h"


aquarium * my_aquarium;

void initialize_aquarium(int width,int length){
	my_aquarium = malloc(sizeof(*my_aquarium));
	my_aquarium->length = length;
	my_aquarium->width = width;
	my_aquarium->fishs = set__empty();

}



/*aquarium * make_view(int x,int y,int width, int length){
	aquarium * view = malloc(sizeof(*view));
	view->length = length;
	view->width = width;
	view->fishs = get_fishes_view(my_aquarium->fishs,x,y,width,length);
	return view; 

}*/



int add_fish(enum type fish_type , int x , int y, void (*mobility)()){
	poisson poisson;
	poisson.x= x;
	poisson.y =y;
	
	assert(!set__add_head(my_aquarium->fishs,poisson)); 

}

int del_fish(enum type fish_type , int view_number);

int start_fish(enum type  fish_type , int view_number);

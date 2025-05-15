#include "aquarium.h"
#include "model.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
aquarium * my_aquarium;

views my_views;


void update_positions_continously(void){
	while(1){
		update_positions(my_aquarium->fishs);
		sleep(2);
	}
}

void initialize_aquarium(int length,int width,view all_views[],int size){
	my_aquarium = malloc(sizeof(*my_aquarium));
	my_aquarium->length = length;
	my_aquarium->width = width;
	assert(MAX_VIEWS >= size);
	my_aquarium->fishs = set__empty();
	for(int i=0;i<size;i++){
		my_views.all_views[i] = all_views[i];
	}
	my_views.size = size;
	pthread_t tid;
	pthread_create(&tid,NULL,(void*)update_positions_continously,NULL);
}

struct view make_view(int id,int x,int y,int width, int length , int available){
	struct view view ;
	view.id = id;
	view.x = x;
	view.y = y;
	view.width = width;
	view.length = length;
	view.available = available;
	return view;
}

int find_view(int id){
	int i;
	for(i=0;i<my_views.size;i++){
		if(my_views.all_views[i].id == id)
			return i;
	}
	return -1;
}

int add_view(view view){
	if(find_view(view.id)!= -1)
		return 1;
	if(my_views.size>MAX_VIEWS)
		return 1;
	my_views.all_views[my_views.size] = view;
	my_views.size++;
	return 0;

}



void shift_left(int index){
	for(int i=index;i<my_views.size-1;i++){
		
		my_views.all_views[i] = my_views.all_views[i+1];
	}
}

int del_view(int id){
	int i = find_view(id);
	if(i == -1)
		return 1;
	shift_left(i);
	my_views.size--;
	return 0;
}

int views_size(){
	return my_views.size;
}






int add_fish(char *name , int x , int y, int length, int width, coord (*mobility)(coord,int,int),int view_id){
	int index = find_view(view_id);
	if(index == -1)
		return 1;
	poisson poisson;
	strcpy(poisson.name,name);
	poisson.length =(my_views.all_views[index].length * length )/HUNDRED;
	poisson.width = (my_views.all_views[index].width * width )/HUNDRED;
	poisson.coord_d.x= (my_views.all_views[index].length * x )/HUNDRED + my_views.all_views[index].x;
	poisson.coord_d.y =(my_views.all_views[index].width * y )/HUNDRED + my_views.all_views[index].y;
	poisson.coord_f.x = poisson.coord_d.x;
	poisson.coord_f.y = poisson.coord_d.y;
	poisson.status = NOTSTARTED;
	poisson.mobility = mobility;
	return set__add_head(my_aquarium->fishs,poisson); 

}

poisson get_fish_by_name(char * name,int view_id){
	poisson poisson;
	int index = find_view(view_id);
	if(index != -1){
	struct set * view_fishes = get_fishes_in(my_aquarium->fishs,my_views.all_views[index].x,my_views.all_views[index].y,\
	 									my_views.all_views[index].length,my_views.all_views[index].width);
	struct lelement * elem = set__find(view_fishes,name);
	if (elem ==NULL){
		free_set(view_fishes);
		
		poisson.status = NOT_FOUND;
		return poisson;
	}
	poisson = get_poisson(elem);
	free_set(view_fishes);
	return poisson;
	}

	poisson.status = NOT_FOUND;
	return poisson;
}



int del_fish(char * name , int view_id){
	poisson poisson = get_fish_by_name(name,view_id);
	if(poisson.status == NOT_FOUND)
		return 1;

	return set__remove(my_aquarium->fishs,poisson);
	
}

int start_fish(char * name, int view_number){
	poisson poisson = get_fish_by_name(name,view_number);
	if(poisson.status == NOT_FOUND)
		return 1;
	struct lelement * elem = set__find(my_aquarium->fishs,name);
	elem->poisson.status = STARTED;
	return 0;
}

////////////////////////////////////////////////////// pay attention : it retruns only STARTED fishes
poisson * getFishes(int view_id, int *count,int fd){
	int size;
	int poisson_array_size =0;
	int index = find_view(view_id);
	if(index != -1){
	struct set * view_fishes = get_fishes_in_view(my_aquarium->fishs,my_views.all_views[index].x,my_views.all_views[index].y,\
	 									my_views.all_views[index].length,my_views.all_views[index].width,fd);
	
	if(!(size = set_size(view_fishes))){
		free_set(view_fishes);
		return NULL;
	}
	int size = set_size(view_fishes);
	poisson * all_fish = malloc(sizeof(poisson)*size);

	struct lelement *e=view_fishes->l->head;
  	for(int i=0;i<size;i++){
    	if(get_poisson(e).status == STARTED){
			all_fish[i] = e->poisson; 
			poisson_array_size++;
      
    	}
    	e=e->next;
  	}
	free_set(view_fishes);
	if(poisson_array_size ==0)
		return NULL;
	if(poisson_array_size!=size)
		all_fish =realloc(all_fish,poisson_array_size*sizeof(poisson));
	*count = poisson_array_size;
  	return all_fish ;
	}
	return NULL;
}


int available_view(){
	int size = views_size();
	for (int i = 0 ; i < size ; i++){
		if (my_views.all_views[i].available == -1){
			return my_views.all_views[i].id;
		}
	}
	return -1;
}

void changeavailable(int id,int fd){
	int i = find_view(id);
	if (i ==-1){
		perror("change available error");
	}
	else{
		if(my_views.all_views[i].available == fd)
			my_views.all_views[i].available = -1;
		else 
			my_views.all_views[i].available = fd;
	}
}

views get_views(){
	return my_views;
}

int get_id_of_fd(int fd){
	int size = views_size();
	for(int i=0;i<size;i++){
		if(my_views.all_views[i].available == fd)
			return my_views.all_views[i].id;
	}
	return -1;
}

int is_available(int var){
	int size =views_size();
	for(int i =0;i<size;i++){
		if(my_views.all_views[i].id == var && my_views.all_views[i].available ==-1)
			return 1;
	}
	return 0;
}

view get_view(int id){
	int size = my_views.size;
	for(int i=0;i<size;i++){
		if(my_views.all_views[i].id == id)
			return my_views.all_views[i];
	}
	perror("error while getting view \n ");
	return my_views.all_views[0];
}


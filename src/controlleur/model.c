#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define AQUA_WIDTH   1000
#define AQUA_HEIGHT  1000
#define GRID_COLS      4
#define GRID_ROWS      4

void print_poisson(poisson poisson){
	printf("{%s is %d in (%d;%d) of size (%d;%d)}\n",poisson.name,poisson.status,\
	 poisson.coord_f.x , poisson.coord_f.y , poisson.width , poisson.length );
}

int in_helper(int x1,int y1,int x,int y,int length,int width){ 
  return (x <=x1 && x1 <= x+length) &&  (y <=y1 && y1 <= y+width);
}


coord RandomPathWay(coord coordon,int x,int y){
	int x1 = coordon.x;
	int y1 = coordon.y;
	coord new_coord;
	srand(time(NULL));
	int rand1 = (rand()%3 - 1);
	int rand2 = (rand()%3 - 1);
	new_coord.x = x1 + rand1* MAX_STEP;
	new_coord.y = y1 + rand2*MAX_STEP;
	if((new_coord.x < 0 || new_coord.x + x > 1000) || (new_coord.y < 0 || new_coord.y + y > 1000)){
		new_coord.x = x1 - rand1* MAX_STEP;
		new_coord.y = y1 - rand2*MAX_STEP;
	}

	return new_coord;
}



coord horizenal(coord coord,int x,int y){
	
	coord.x += 70;
	
	return coord;
}
/*
void prepare(poisson * poisson){
	struct timeval tv1;
	gettimeofday(&tv1,NULL);
	int time = tv1.tv_sec - poisson->tv.tv_sec;
	if(time> poisson->temps){
		printf("aaaaa %d\n",poisson->coord_f.x);
		poisson->coord_d = poisson->coord_f;
		poisson->coord_f = poisson->mobility(poisson->coord_f);
		printf("bbbb %d\n",poisson->coord_f.x);
		gettimeofday(&(poisson->tv),NULL);
		poisson->temps = 5;
		
	}
	else {
		poisson->temps = time;
	}
}
*/
#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_poisson(poisson poisson){
	printf("{%s is %d in (%d;%d) of size (%d;%d)}\n",poisson.name,poisson.status,\
	 poisson.coord_f.x , poisson.coord_f.y , poisson.width , poisson.length );
}

int in_helper(int x1,int y1,int x,int y,int length,int width){ 
  return (x <=x1 && x1 <= x+length) &&  (y <=y1 && y1 <= y+width);
}


coord RandomPathWay(coord coordon){
	int x = coordon.x;
	int y = coordon.y;
	coord new_coord;
	srand(time(NULL));

	new_coord.x = x + /*(rand()%3 - 1)**/ MAX_STEP;
	new_coord.y = y + /*(rand()%3 - 1)**/ MAX_STEP;

	return new_coord;
}

int  prepare(poisson * poisson){
	struct timeval tv1;
	gettimeofday(&tv1,NULL);
	if((tv1.tv_sec - poisson->tv.tv_sec)> poisson->temps){
		poisson->coord_d = poisson->coord_f;
		poisson->coord_f = poisson->mobility(poisson->coord_f);

		gettimeofday(&(poisson->tv),NULL);
		return 5;
		
	}
	return tv1.tv_sec - poisson->tv.tv_sec;
}
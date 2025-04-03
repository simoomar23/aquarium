#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_poisson(poisson poisson){
	printf("{%s is %d in (%d;%d) of size (%d;%d)}\n",poisson.name,poisson.status,\
	 poisson.coord.x , poisson.coord.y , poisson.width , poisson.length );
}


coord RandomPathWay(coord coordon){
	int x = coordon.x;
	int y = coordon.y;
	coord new_coord;
	srand(time(NULL));

	new_coord.x = x + (rand()%3 - 1)* MAX_STEP;
	new_coord.y = y + (rand()%3 - 1)* MAX_STEP;

	return new_coord;
}
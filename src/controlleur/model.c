#include "model.h"

void print_poisson(poisson poisson){
	printf("{%d is %d in (%d;%d) of size (%d;%d)}\n",poisson.type,poisson.status,\
	 poisson.x , poisson.y , poisson.width , poisson.length );
}
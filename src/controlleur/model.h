#ifndef MODEL_H 
#define MODEL_H

#include <sys/time.h>
#define MAX_STEP 70



enum status {
	STARTED,
	NOTSTARTED,
	NOT_FOUND
};

typedef struct coord {
	int x;
	int y;
}coord;

typedef struct poisson {
	char name[20];
	int length;
	int width;
	//int temps;
	coord coord_d;
	coord coord_f;
	enum status status;
	coord (*mobility)(coord); 
	//struct timeval tv;
	
} poisson;


void print_poisson(poisson poisson);

coord RandomPathWay(coord corrdon);

int in_helper(int x1,int y1,int x,int y,int length,int width);

//void prepare(poisson * poisson);



#endif
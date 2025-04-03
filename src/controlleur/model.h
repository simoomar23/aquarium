#ifndef MODEL_H 
#define MODEL_H

#define MAX_STEP 5



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
	char * name;
	int length;
	int width;
	coord coord;
	enum status status;
	coord (*mobility)(coord); 
	
} poisson;


void print_poisson(poisson poisson);

coord RandomPathWay(coord corrdon);




#endif
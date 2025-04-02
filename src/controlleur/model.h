#ifndef MODEL_H 
#define MODEL_H

#define POISSONROUGET [0,10,4]
#define POISSONAINT [1,15,8]

enum type {
	POISSONROUGE,
	POISSONAIN

};





enum status {
	STARTED,
	NOTSTARTED
};

typedef struct coord {
	int x;
	int y;
}coord;

typedef struct poisson {
	int length;
	int width;
	int x;
	int y;
	enum status status;
	enum type type;
	coord (*mobility)(coord); 
	
} poisson;


void print_poisson(poisson poisson);




#endif
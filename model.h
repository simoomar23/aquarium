#ifndef MODEL_H 
#define MODEL_H

enum type {
	POISSONROUGE,
	POISSONAIN

};

enum status {
	STARTED,
	NOTSTARTED
};

typedef struct poisson {
	int length;
	int width;
	int x;
	int y;
	status status;
	type type;
} poisson;



#endif
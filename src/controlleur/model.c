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


coord movement_rectangular(coord unused,int x,int y) {
    // Taille d'un sous-rectangle
    const int Wc = AQUA_WIDTH  / GRID_COLS;  // 250
    const int Hc = AQUA_HEIGHT / GRID_ROWS;  // 250
    // Périmètre sans double-coin
    const int P = 2*(Wc + Hc) - 4;           // 2*(250+250)-4 = 996

    // Compteur de pas interne
    static int step = 0;

    // Index de la cellule [0..15]
    int cell = (step / P) % (GRID_COLS * GRID_ROWS);
    int col  = cell % GRID_COLS;
    int row  = cell / GRID_COLS;

    // Position le long du contour de la cellule [0..P-1]
    int s = step % P;

    // Origine (coin haut-gauche) du sous-rectangle
    int ox = col * Wc;
    int oy = row * Hc;

    coord res;
    if (s < Wc) {
        // côté supérieur
        res.x = ox + s;
        res.y = oy;
    }
    else if (s < Wc + Hc - 1) {
        // côté droit
        res.x = ox + (Wc - 1);
        res.y = oy + (s - Wc + 1);
    }
    else if (s < 2*Wc + Hc - 2) {
        // côté inférieur
        res.x = ox + (Wc - 1) - (s - (Wc + Hc - 1));
        res.y = oy + (Hc - 1);
    }
    else {
        // côté gauche
        res.x = ox;
        res.y = oy + (Hc - 1) - (s - (2*Wc + Hc - 2));
    }

    // Incrémente le pas pour le prochain appel
    step++;

    return res;
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
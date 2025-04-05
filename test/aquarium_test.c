#include <stdio.h>
#include "aquarium.h"
#include <assert.h>
#include <stdlib.h>

void aquarium_test(){
    view view1 = {1,0,0,500,500,1};
    view view2 = {2,500,0,500,500,1};
    view view3 = {3,0,500,500,500,1};
    view view4 = {4,500,500,500,500,1};
    view view5 = {5,400,400,400,200,1};
    view all_views[5]={view1,view2,view3,view4,view5};


    initialize_aquarium(1000,1000,all_views,5);
    assert(views_size()==5);
    view view6 = {5,400,400,400,200,1};
    assert(add_view(view6));
    assert(views_size()==5);
    
    assert(!add_fish("POISSONROUGE",90,94,20,12,RandomPathWay,1));
    assert(!start_fish("POISSONROUGE",1));
    poisson * p1 =getFishes(1);
    poisson * p2 =getFishes(2);
    poisson * p3 =getFishes(3);
    poisson * p4 =getFishes(4);
    poisson * p5 =getFishes(5);
    assert(p1!=NULL);
    assert(p2!=NULL);
    assert(p3!=NULL);
    assert(p4!=NULL);
    assert(p5!=NULL);
    free(p1);
    free(p2);
    free(p3);
    free(p4);
    free(p5);


    assert(!del_view(5));
    assert(del_view(5));

}



int main(){
   aquarium_test();

    return 0;
}
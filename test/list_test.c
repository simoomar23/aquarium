#include <stdio.h>
#include "liste.h"
#include "model.h"
#include <assert.h>


void general_list_test(){
    struct set * set = set__empty();
    assert(set_size(set)==0);
    coord cord1 = {25,25};
    poisson fish1 = {"POISSONNAIN",10,5,cord1,NOTSTARTED,RandomPathWay};
    assert(!set__add_head(set,fish1));
    assert(set_size(set)==1);
    poisson fish = get_poisson(set__find(set,"POISSONNAIN"));
    assert((fish1.length == fish.length) &&(fish1.status == fish.status) && (fish1.mobility == fish.mobility) );
    
    coord cord2 = {50,25};
    poisson fish2 = {"POISSONROUGE",16,8,cord2,STARTED,RandomPathWay};
    assert(!set__add_head(set,fish2));
    set_print(set, "two fishes\n");
    assert(set_size(set)==2);
    assert(!set__remove(set,fish1));
    assert(set_size(set) == 1);
    assert(set__find(set,"POISSONROUGE") != NULL);
    poisson fish3;
    fish3.name = "Oops";
    assert(set__remove(set,fish3));
    set_print(set, " just one fish\n");
    assert(!set__remove(set,fish2));
    assert(set_size(set) == 0);
    free_set(set);
}


void fish_list_test(){
    struct set * set = set__empty();
    assert(set_size(set)==0);
    coord cord1 = {25,25};
    poisson fish1 = {"POISSONNAIN1",10,5,cord1,NOTSTARTED,RandomPathWay};
    coord cord2 = {45,10};
    poisson fish2 = {"POISSONNAIN2",10,5,cord2,STARTED,RandomPathWay};
    coord cord3 = {10,48};
    poisson fish3 = {"POISSONNAIN3",10,5,cord3,NOTSTARTED,RandomPathWay};
    coord cord4 = {50,50};
    poisson fish4 = {"POISSONNAIN4",10,5,cord4,STARTED,RandomPathWay};
    coord cord5 = {10,80};
    poisson fish5 = {"POISSONNAIN5",10,5,cord5,NOTSTARTED,RandomPathWay};
    coord cord6 = {25,25};
    poisson fish6 = {"POISSONNAIN6",10,5,cord6,STARTED,RandomPathWay};
    coord cord7 = {80,90};
    poisson fish7 = {"POISSONNAIN7",10,5,cord7,NOTSTARTED,RandomPathWay};
    coord cord8 = {0,0};
    poisson fish8 = {"POISSONNAIN8",10,5,cord8,STARTED,RandomPathWay};
    coord cord9 = {30,50};
    poisson fish9 = {"POISSONNAIN9",10,5,cord9,NOTSTARTED,RandomPathWay};
    coord cord10 = {50,55};
    poisson fish10 = {"POISSONNAIN10",10,5,cord10,STARTED,RandomPathWay};

    assert(!set__add_head(set,fish1));
    assert(!set__add_head(set,fish2));
    assert(!set__add_head(set,fish3));
    assert(!set__add_head(set,fish4));
    assert(!set__add_head(set,fish5));
    assert(!set__add_head(set,fish6));
    assert(!set__add_head(set,fish7));
    assert(!set__add_head(set,fish8));
    assert(!set__add_head(set,fish9));
    assert(!set__add_head(set,fish10));
    assert(set_size(set)==10);

    struct set* set1 =  get_fishes_in_view(set ,0,0,50,50);
    struct set* set2 =  get_fishes_in_view(set ,50,0,50,50);
    struct set* set3 =  get_fishes_in_view(set ,0,50,50,50);
    struct set* set4 =  get_fishes_in_view(set ,50,50,50,50);
    struct set* set5 =  get_fishes_in_view(set ,25,25,50,50);
   
    assert(set_size(set1)==5);
    set_print(set1, "five fishes in view 1\n");
    assert(set_size(set2)==1);
    set_print(set2, "just one in view 2\n");
     assert(set_size(set3)==3);
    set_print(set3, "three fishes in view 3\n");
     assert(set_size(set4)==3);
    set_print(set4, "three fishes in view 4\n");
     assert(set_size(set5)==5);
    set_print(set5, "five fishes in view 5\n");
    free_set(set);
    free_set(set1);
    free_set(set2);
    free_set(set3);
    free_set(set4);
    free_set(set5);




    
}
int main(){
    general_list_test();
    fish_list_test();

    return 0;
}
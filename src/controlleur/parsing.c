#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void tokenize_load() {
    FILE *fichier;
    char ligne[256]; 
    char **lignes = malloc(100 * sizeof(char*)); 
    int i = 0;

    fichier = fopen("aquarium1", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        lignes[i] = malloc(strlen(ligne) + 1); 
        strcpy(lignes[i], ligne);              
        i++;
    }
    int width;
	int length;
    int id;
	int x;
	int y;
	int width1;
	int length1;
    //void initialize_aquarium(length,width,all_views[],i-1);
    sscanf(lignes[0],"%dx%d",&width,&length);

    for (int j = 0;j < i ;j++){
        sscanf(lignes[1],"N%d %dx%d+%d+%d",&id,&x,&y,&width1,&length1);
    //printf("l'id est %d\n",id);
    //printf("x %d\n",x);
    //printf("y %d\n",y);
    //printf("width %d\n",width1);
    //printf("length %d\n",length1);
        addview(make_view(id,x,y,width1,length1));
    }
    fclose(fichier);

    for (int j = 0; j < i; j++) {
        printf("%s", lignes[j]);
        free(lignes[j]); 
    }
    free(lignes); 
}

/*void parsing(char *cmd){
    if (strstr(cmd,"load")){
      tokenize_load(cmd+5);
    }
    else {
        
    }
}
*/

int main() {
    //parsing("load aquarium");  
    //parsing("show aquarium"); 
    tokenize_load();
    return 0;
}
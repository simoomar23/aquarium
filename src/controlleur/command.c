#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include "verification.h"
#include "aquarium.h"
#include "log_file.h"
#include <errno.h>
#include <pthread.h>
#define TIME 2


void cmd_load(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_show(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_add(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_del(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_save(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);


char * cmd_hello(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
char * cmd_addFish(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
char * cmd_status(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
char * cmd_delFish(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
char * cmd_startFish(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
char * cmd_getFishes(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
/*char * cmd_ls(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);*/
void cmd_getFishesContinuously(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
char * cmd_ping(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
char * cmd_log(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);

typedef struct {
    char *name;
    char * (*func)(int,char[MAX_TOKENS][MAX_TOKEN_LENGTH], int);
} ClientCommand;

typedef struct {
    char *name;
    void (*func)(char[MAX_TOKENS][MAX_TOKEN_LENGTH], int);
} PromptCommand;

char loaded_aquarium[MAX_TOKEN_LENGTH] = "";

PromptCommand commandTable[] = {
    {"load", cmd_load},
    {"show", cmd_show},
    {"add", cmd_add},
    {"del", cmd_del},
    {"save", cmd_save}, 
};
ClientCommand clientcommande[] = {
    {"hello", cmd_hello},
    {"addFish", cmd_addFish},
    {"delFish", cmd_delFish},
    {"startFish", cmd_startFish},
    {"getFishes", cmd_getFishes},
    {"log", cmd_log},/*,
    {"ls", cmd_ls},
    {"getFishesContinuously", cmd_getFishesContinuously},
    */
    {"ping", cmd_ping}
};

int command_length;
int command_width;

#define PROMPT_COMMAND_COUNT (sizeof(commandTable) / sizeof(PromptCommand))
#define CLIENT_COMMAND_COUNT (sizeof(clientcommande) / sizeof(ClientCommand))

int tokenize(char *input, char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH]) {
    int tokenCount = 0;
    int size;
    char *token = strtok(input, " \t\n");
    while (token != NULL && tokenCount < MAX_TOKENS) {
        size = strlen(token);
        //printf("%s this is token\n",token);
        //printf("%d size \n",size);
        strncpy(tokens[tokenCount], token, size);
        tokens[tokenCount][size] = '\0';
        //printf("%s play\n",tokens[tokenCount]);
        tokenCount++;
        token = strtok(NULL, " \t\n");
    }


    /*for (long unsigned int i = 0; i < CLIENT_COMMAND_COUNT; i++) {
            int c= strcmp(tokens[0], clientcommande[i].name);
            printf("%d\n",c);
            if (c  == 0) {
                tokens[tokenCount-1][size -1] ='\0';
            }

    }*/
    
    return tokenCount;
}

void handle_command(char *input) {
    char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH];
    int tokenCount = tokenize(input, tokens);

    if (tokenCount == 0) return;
    //printf("tokens %s et %d \n",tokens[0],strlen(tokens[0]));
    //printf("wa motherfucker ha ficher %s",tokens[1]);
    for (long unsigned int i = 0; i < PROMPT_COMMAND_COUNT; i++) {
        int c = strcmp(tokens[0], commandTable[i].name);
       
        //printf("command [%d] = %s\n",i,commandTable[i].name);
        if (c == 0) {
            commandTable[i].func(tokens, tokenCount);
            return;
        }
    }

    printf("commande invalide\n");
}

void tokenize_load(char *c) {
    FILE *fichier;
    char ligne[256]; 
    char **lignes = malloc(100 * sizeof(char*)); 
    int i = 0;

    fichier = fopen(c, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        lignes[i] = malloc(strlen(ligne) + 1); 
        strcpy(lignes[i], ligne);              
        i++;
    }
    int id;
	int x;
	int y;
	int width;
	int length;
    view total_views[i-1];
    
    sscanf(lignes[0],"%dx%d",&command_width,&command_length);

    for (int j = 1;j < i ;j++){
        sscanf(lignes[j],"N%d %dx%d+%d+%d",&id,&x,&y,&width,&length);

       total_views[j-1]=make_view(id,x,y,width,length,-1);
    }
    fclose(fichier);
    initialize_aquarium(command_length,command_width,total_views,i-1);

    for (int j = 0; j < i; j++) {
        free(lignes[j]); 
    }
    free(lignes); 
    printf("aquarium loaded (%s)!\n", c);
}

int help(char * file){
    FILE *dest = fopen(file, "w");
    views v = get_views();
    char * result = malloc(20);
    snprintf(result, 20, "%dx%d\n", command_width,command_length);
    fwrite(result, 1, strlen(result), dest);
    for (int i =0 ; i< v.size;i++){
        snprintf(result, 50, "N%d %dx%d+%d+%d\n", v.all_views[i].id,v.all_views[i].x,v.all_views[i].y,v.all_views[i].width,v.all_views[i].length);
        fwrite(result, 1, strlen(result), dest);
    }
    fclose(dest);
    return v.size;
}


void cmd_load(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    if (tokenCount < 2) {
        printf("Usage: load <filename>\n");
        return;
    }

    strcpy(loaded_aquarium, tokens[1]);
    tokenize_load(loaded_aquarium);

}

void cmd_show(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    if(tokenCount != 2 || strcmp(tokens[1],"aquarium"))
        perror("enter 'aqurium' instead of a file\n");
    else {
    /*FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file: %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    printf("\n");

    fclose(file);*/
    char * show = "show";
    help(show);
    FILE *file = fopen(show, "r");
    if (file == NULL) {
        perror("Erreur à l'ouverture");
    }
    else{
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            printf("%s", buffer);
        }
    }

    fclose(file);
    }
}

void cmd_add(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    if (add_view_verification(tokens,tokenCount,command_length,command_width)){
        printf("error sythax or cordinate are not supported\n");
        return;
    }
    int id;
    int x;
	int y;
	int width;
	int length;
    sscanf(tokens[3],"%dx%d+%d+%d",&x,&y,&width,&length);
    sscanf(tokens[2],"N%d",&id);
    view view = make_view(id,x,y,width,length,-1);
    if (add_view(view) == 1 ){
        printf("caonnot add\n");
        return;
    }
    else {
        add_view(view);
    }
    printf("view added\n");
}

void cmd_del(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    
    if(del_view_verification(tokens,tokenCount)){
        printf("view does not exist or synthax error\n");
    }
    int id ;
    sscanf(tokens[2],"N%d",&id);
   // printf("id : %d\n",id);
    if (!del_view(id)){
        printf("view N%d deleted.\n",id);
    }
    else {
        printf("view does not exist\n");
    }

    /*
    const char *target = tokens[2];
    FILE *file = fopen(loaded_aquarium, "r");
    if (!file) {
        printf("Could not open file to delete view.\n");
        return;
    }

    FILE *temp = fopen("temp_aquarium.txt", "w");
    if (!temp) {
        fclose(file);
        printf("Temporary file error.\n");
        return;
    }

    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, target, strlen(target)) != 0) {
            fputs(line, temp);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp);

    remove(loaded_aquarium);
    rename("temp_aquarium.txt", loaded_aquarium);

    if (found) {
        printf("view %s deleted.\n", target);
    } else {
        printf("view %s not found.\n", target);
    }
    */
}

void cmd_save(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    if (tokenCount < 2) {
        printf("Usage: save <filename>\n");
        return;
    }

    int size = help(tokens[1]);
    printf("Aquarium saved ! (%d display views)\n",size);
    /*
    FILE *src = fopen(loaded_aquarium, "r");
    FILE *dest = fopen(tokens[1], "w");

    if (!src || !dest) {
        printf("Error saving aquarium.\n");
        if (src) fclose(src);
        if (dest) fclose(dest);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), src)) {
        fputs(line, dest);
    }

    fclose(src);
    fclose(dest);

    printf("Aquarium saved! (%s)\n", tokens[1]);*/
}





char * cmd_hello(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    int var = hello_verification(tokens, tokenCount);
    if (var == -1) {
        return strdup("no greeting\n");
    }
    int i = available_view();
    if (i == -1) {
        return strdup("no greeting\n");
    }

    int greeting_num ;
    if (var==0){
        greeting_num = i ;
    }
    else{
        if(find_view(var) != -1 && is_available(var))
            greeting_num = var;
        else 
            greeting_num = i;
    }

    char *result = malloc(40);
    if (result == NULL) {
        return strdup("error allocating memory\n");
    }
    view  v = get_view(greeting_num);
    snprintf(result, 40, "greeting N%d %dx%d+%d+%d\n", greeting_num,v.x,v.y,v.width,v.length);
    changeavailable(greeting_num,fd);
    return result;
}



char * cmd_addFish(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount){
    if(add_fish_verification(tokens,tokenCount)){
        return "NOK : commande introuvable ou paramétres insupportables\n";
    }
	int x;
	int y;
	int width;
	int length;
    int c;
    if((c = strcmp(tokens[5],"RandomWayPoint"))){
       // printf("token is %s ad  is : %d\n",tokens[5],c);
        return "NOK : modèle de mobilité non supporté\n";
    }
    sscanf(tokens[3],"%dx%d,",&x,&y);
    sscanf(tokens[4],"%dx%d,",&length,&width);
    if(add_fish(tokens[1],x,y,length,width,RandomPathWay,get_id_of_fd(fd)))
        return "NOK : poisson existe déja dans l'aquarium\n";
    return "OK\n";
}
/*char *cmd_status(int fd, char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    if(tokenCount ==1 && strcmp(tokens[0],"status")==0){
    int count = 0;
    poisson *fishes = getFishes(get_id_of_fd(fd), &count); 

    if (fishes == NULL || count == 0) {
        return strdup("OK : Connecté au contrôleur, 0 poissons trouvés\n");
    }

    size_t bufferSize = 2048;
    char *result = malloc(bufferSize);
    if (result == NULL) return NULL;
    result[0] = '\0';

    for (int i = 0; i < count; i++) {
        char line[256];
        snprintf(line, sizeof(line), "Fish %s at %dx%d,%dx%d %d\n",
                 fishes[i].name,
                 fishes[i].coord.x, fishes[i].coord.y,
                 fishes[i].width, fishes[i].length,
                 fishes[i].status);

        if (strlen(result) + strlen(line) + 1 > bufferSize) {
            bufferSize *= 2;
            char *newResult = realloc(result, bufferSize);
            if (newResult == NULL) {
                free(result);
                return NULL;
            }
            result = newResult;
        }

        strcat(result, line);
    }
    char header[128];
    snprintf(header, sizeof(header),
             "OK : Connecté au contrôleur, %d poissons trouvés\n", count);

    size_t finalSize = strlen(header) + strlen(result) + 1;
    char *final = malloc(finalSize);
    if (final == NULL) {
        free(result);
        return NULL;
    }

    strcpy(final, header);
    strcat(final, result);
    free(result);

    return final;
    }
    return "NOK : commande introuvable";
}
*/



char * cmd_delFish(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount){
    if(tokenCount == 2){
    if(del_fish(tokens[1],get_id_of_fd(fd)))
        return "NOK : poisson inexistant\n";
    return "OK";
    }
    return "NOK : commande introuvable\n";
}
char * cmd_startFish(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount){
    if(tokenCount == 2){
        if(start_fish(tokens[1],get_id_of_fd(fd)))
            return "NOK : poisson inexistant\n";
        return "OK\n";
    }
     return "NOK : commande introuvable\n";
}
char * cmd_getFishes(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH]__attribute__((unused)), int tokenCount __attribute__((unused))){
    int count = 0;
	printf("lll%d\n",fd);
    poisson *fishes = getFishes(get_id_of_fd(fd), &count); 

    if (fishes == NULL || count == 0) {
		printf("mal zaml\n");
        return strdup("list\n");
    }

    size_t bufferSize = 2048;
    char *result = malloc(bufferSize);
    if (result == NULL) return NULL;
    result[0] = '\0';

    for (int i = 0; i < count; i++) {
        char line[256];
        //printf("this the name :::: %s\n",fishes[i].name);
        snprintf(line, sizeof(line), "[%s at %dx%d,%dx%d,%d] ",
                 fishes[i].name,
                 fishes[i].coord_f.x, fishes[i].coord_f.y,
                 fishes[i].length,fishes[i].width,5);

        if (strlen(result) + strlen(line) + 1 > bufferSize) {
            bufferSize *= 2;
            char *newResult = realloc(result, bufferSize);
            if (newResult == NULL) {
                free(result);
                return NULL;
            }
            result = newResult;
        }

        strcat(result, line);
    }
    size_t result_size = strlen(result);
    result[result_size] = '\n';
    result[result_size + 1] ='\0';
    char header[] ="list ";


    size_t finalSize = strlen(header) + strlen(result) + 1;
    char *final = malloc(finalSize);
    if (final == NULL) {
        free(result);
        return NULL;
    }

    strcpy(final, header);
    strcat(final, result);
    free(result);

    return final;
}
/*char * cmd_ls(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
char * cmd_getFishesContinuously(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);*/

void getFishes_utils(void *fd){
	int sd = (intptr_t)fd;
    while(1){
        char * response = cmd_getFishes(sd,NULL,0);
        if (send(sd, response, strlen(response), 0) == -1) {
							perror("send failed");
							log_message(LOG_ERROR, "send failed to client %d: %s", sd, strerror(errno));
		} else {
        	log_message(LOG_DEBUG, "Response sent to client %d", sd);
        }
        sleep(7);
    }
}

void cmd_getFishesContinuously(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH]__attribute__((unused)), int tokenCount __attribute__((unused))){
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, (void *)getFishes_utils, (void * )(intptr_t )fd);
}

char* cmd_log(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount){
    if (strcmp(tokens[1],"out") || tokenCount != 2){
        return "NOK : commande introuvable\n";
    }
    changeavailable(get_id_of_fd(fd),fd);
    return "bye\n";
}

char * cmd_ping(int fd,char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH] __attribute__((unused)), int tokenCount __attribute__((unused))){
    char *result = malloc(20);
    if (result == NULL) {
        return strdup("error allocating memory\n");
    }
    snprintf(result ,20,"pong %d\n",get_id_of_fd(fd));
    return result;
}

char* handle_client_command(int fd,char *input) {
    char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH];
    int tokenCount = tokenize(input, tokens);
    if (tokenCount == 0) return "\n";
    /*size_t size = strlen(tokens[tokenCount-1]);
    tokens[tokenCount-1][size-1]=0;*/
    //printf("oooos %s\n",tokens[0]);
    for (long unsigned int i = 0; i < CLIENT_COMMAND_COUNT; i++) {
        int c= strcmp(tokens[0], clientcommande[i].name);
        //printf("%d\n",c);
        if (c  == 0) {

            return clientcommande[i].func(fd,tokens, tokenCount);
            
        }

    }
    if(strcmp(tokens[0],"getFishesContinuously") == 0){
        cmd_getFishesContinuously(fd,tokens,tokenCount);
        return "OOPS";
    }
    return "NOK : invalid commande\n";
}


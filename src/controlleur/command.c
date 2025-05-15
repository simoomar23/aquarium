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
#include "model.h"
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

typedef struct {
    char * name ;
    coord (*func)(coord,int,int);
}functions;

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
    */
    {"ping", cmd_ping}
};
functions myfunc[] = {
    {"RandomWayPoint",RandomPathWay},
    {"movement_rectangular",movement_rectangular},
    {"horizenal",horizenal}
};

int command_length;
int command_width;

#define PROMPT_COMMAND_COUNT (sizeof(commandTable) / sizeof(PromptCommand))
#define CLIENT_COMMAND_COUNT (sizeof(clientcommande) / sizeof(ClientCommand))
#define functions_size (sizeof(myfunc))/sizeof(functions)

int tokenize(char *input, char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH]) {
    int tokenCount = 0;
    int size;
    char *token = strtok(input, " \t\n");
    while (token != NULL && tokenCount < MAX_TOKENS) {
        size = strlen(token);
        strncpy(tokens[tokenCount], token, size);
        tokens[tokenCount][size] = '\0';
        tokenCount++;
        token = strtok(NULL, " \t\n");
    }


  
    
    return tokenCount;
}

void handle_command(char *input) {
    char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH];
    int tokenCount = tokenize(input, tokens);

    if (tokenCount == 0) return;
    for (long unsigned int i = 0; i < PROMPT_COMMAND_COUNT; i++) {
        int c = strcmp(tokens[0], commandTable[i].name);
       
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
    if (!del_view(id)){
        printf("view N%d deleted.\n",id);
    }
    else {
        printf("view does not exist\n");
    }

   
}

void cmd_save(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    if (tokenCount < 2) {
        printf("Usage: save <filename>\n");
        return;
    }

    int size = help(tokens[1]);
    printf("Aquarium saved ! (%d display views)\n",size);
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
    if((c = strcmp(tokens[5],"RandomWayPoint")) && (c = strcmp(tokens[5],"movement_rectangular")) &&  (c = strcmp(tokens[5],"horizenal"))){
        return "NOK : modèle de mobilité non supporté\n";
    }
    sscanf(tokens[3],"%dx%d,",&x,&y);
    sscanf(tokens[4],"%dx%d,",&length,&width);
    coord (*func)(coord,int,int);
    for(long unsigned int i=0;i<functions_size;i++){
        if(!strcmp(myfunc[i].name,tokens[5]))
            func = myfunc[i].func;
    }
    if(add_fish(tokens[1],x,y,length,width,func,get_id_of_fd(fd)))
        return "NOK : poisson existe déja dans l'aquarium\n";
    return "OK\n";
}




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
    poisson *fishes = getFishes(get_id_of_fd(fd), &count,fd); 

    if (fishes == NULL || count == 0) {
        return strdup("list\n");
    }

    size_t bufferSize = 2048;
    char *result = malloc(bufferSize);
    if (result == NULL) return NULL;
    result[0] = '\0';

    for (int i = 0; i < count; i++) {
        char line[256];
        snprintf(line, sizeof(line), "[%s at %dx%d,%dx%d,%d] ",
                 fishes[i].name,
                 fishes[i].coord_f.x, fishes[i].coord_f.y,
                 fishes[i].length,fishes[i].width,2);

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
        sleep(2);
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
   
    for (long unsigned int i = 0; i < CLIENT_COMMAND_COUNT; i++) {
        int c= strcmp(tokens[0], clientcommande[i].name);
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


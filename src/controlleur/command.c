#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include "verification.h"
#include "aquarium.h"

#define MAX_TOKENS 10
#define MAX_TOKEN_LENGTH 256

char * cmd_hello(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_load(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_show(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_add(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_del(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_save(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_unknown(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
char * cmd_addFish(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
char * cmd_status(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
char * cmd_delFish(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
char * cmd_startFish(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
char * cmd_getFishes(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
char * cmd_ls(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
char * cmd_getFishesContinuously(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
char * cmd_ping(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
char * cmd_log(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);

typedef struct {
    char *name;
    char * (*func)(char[MAX_TOKENS][MAX_TOKEN_LENGTH], int);
} Command;

char loaded_aquarium[MAX_TOKEN_LENGTH] = "";

Command commandTable[] = {
    {"hello", cmd_hello},
    {"load", cmd_load},
    {"show", cmd_show},
    {"add", cmd_add},
    {"del", cmd_del},
    {"save", cmd_save},
    {"addFish", cmd_addFish},
    {"status", cmd_status},
    {"delFish", cmd_delFish},
    {"startFish", cmd_startFish},
    {"getFishes", cmd_getFishes},
    {"ls", cmd_ls},
    {"getFishesContinuously", cmd_getFishesContinuously},
    {"log", cmd_log},
    {"ping", cmd_ping}
};

int command_length;
int command_width;

#define COMMAND_COUNT (sizeof(commandTable) / sizeof(Command))

int tokenize(char *input, char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH]) {
    int tokenCount = 0;
    char *token = strtok(input, " \t\n");
    while (token != NULL && tokenCount < MAX_TOKENS) {
        strncpy(tokens[tokenCount], token, MAX_TOKEN_LENGTH - 1);
        tokens[tokenCount][MAX_TOKEN_LENGTH - 1] = '\0';
        tokenCount++;
        token = strtok(NULL, " \t\n");
    }
    return tokenCount;
}

void handle_command(char *input) {
    char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH];
    int tokenCount = tokenize(input, tokens);

    if (tokenCount == 0) return;

    for (int i = 0; i < COMMAND_COUNT; i++) {
        if (strcmp(tokens[0], commandTable[i].name) == 0) {
            commandTable[i].func(tokens, tokenCount);
            return;
        }
    }

    cmd_unknown(tokens, tokenCount);
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

       total_views[j-1]=make_view(id,x,y,width,length,1);
    }
    fclose(fichier);
    initialize_aquarium(command_length,command_width,total_views,i-1);

    for (int j = 0; j < i; j++) {
        free(lignes[j]); 
    }
    free(lignes); 
}

char * cmd_hello(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    int var = hello_verification(tokens, tokenCount);
    if (var == -1) {
        return strdup("no greeting");
    }
    int i = available_view();
    if (var == 0 && i == -1) {
        return strdup("no greeting");
    }

    int greeting_num ;
    if (var==0){
        greeting_num = i ;
    }
    else{
        greeting_num = var;
    }

    char *result = malloc(20);
    if (result == NULL) {
        return strdup("error allocating memory");
    }

    snprintf(result, 20, "greeting N%d", greeting_num);
    changeavailable(greeting_num);
    return result;
}

void cmd_load(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    if (tokenCount < 2) {
        printf("Usage: load <filename>\n");
        return;
    }


    strcpy(loaded_aquarium, tokens[1]);
    tokenize_load(loaded_aquarium);

    printf("aquarium loaded (%s)!\n", tokens[1]);
}

void cmd_show(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    const char *filename = (tokenCount >= 2) ? tokens[1] : loaded_aquarium;

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file: %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    printf("\n");

    fclose(file);
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
    view view = make_view(id,x,y,width,length,1);
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

    FILE *dest = fopen(tokens[1], "w");
    views v = get_views();
    char * result;
    snprintf(result, 20, "%dx%d\n", command_width,command_length);
    fwrite(result, 1, strlen(result), dest);
    for (int i =0 ; i< v.size;i++){
        snprintf(result, 50, "N%d %dx%d+%d+%d\n", v.all_views[i].id,v.all_views[i].x,v.all_views[i].y,v.all_views[i].width,v.all_views[i].length);
        fwrite(result, 1, strlen(result), dest);
    }
    fclose(dest);
    printf("Aquarium saved ! (%d display views)\n",v.size);
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

void cmd_unknown(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    printf("Unknown command: %s\n", tokens[0]);
}

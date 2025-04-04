// commands.c
#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


// Command function prototypes
void cmd_hello(int sd, char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_load(int sd, char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_show(int sd, char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_add(int sd, char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_del(int sd, char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_save(int sd, char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);

// Command structure
typedef struct {
    char *name;
    void (*func)(int, char[MAX_TOKENS][MAX_TOKEN_LENGTH], int);
} Command;

// Currently loaded aquarium
char loaded_aquarium[MAX_TOKEN_LENGTH] = "";  // Stores the currently loaded aquarium filename


// Command lookup table
Command commandTable[] = {
    {"HELLO", cmd_hello},
    {"LOAD", cmd_load},
    {"SHOW", cmd_show},
    {"ADD", cmd_add},
    {"DEL", cmd_del},
    {"SAVE", cmd_save}
};
#define COMMAND_COUNT (sizeof(commandTable) / sizeof(Command))

// Tokenizer
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

// Command dispatcher
void handle_command(int sd, char *input) {
    char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH];
    int tokenCount = tokenize(input, tokens);

    if (tokenCount == 0) return;

    for (int i = 0; i < COMMAND_COUNT; i++) {
        if (strcmp(tokens[0], commandTable[i].name) == 0) {
            commandTable[i].func(sd, tokens, tokenCount);
            return;
        }
    }

    cmd_unknown(sd, tokens, tokenCount);
}

// Command implementations
void cmd_hello(int sd, char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    char response[] = "Server: Hello!\n";
    send(sd, response, strlen(response), 0);
}

void cmd_load(int sd, char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    //initialize_aquarium();
    // verify ?
    // read from file info about quarium passed in argument for length/width
    char response[256] = "";  // Empty string to start
    strcat(response, tokens[1]);  
    strcat(response, " loaded");
    send(sd, response, strlen(response), 0);
}
void cmd_show(int sd, char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    FILE *file = fopen(tokens[1], "r");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read and print each line of the file
    char line[256];  // Buffer to store each line
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);  // Print the line to the terminal
    }

    // Close the file after reading
    fclose(file);
}
void cmd_add(int sd, char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    //verify arguments
    // Open the file in write mode ("w")
    FILE *file = fopen(tokens[1], "w"); //tokens[1] which aquarium

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Verify wether it fits iniside the aquarium

    // if it fits

    // Write formatted text to the file
    fprintf(file, tokens[2]);
    fprintf(file, tokens[3]);
    char response[] = "view added\n";

    // if it doesn't fit
    
    send(sd, response, strlen(response), 0);

    

    // Close the file after writing
    fclose(file);
}
void cmd_del(int sd, char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {

}
void cmd_save(int sd, char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {

}



/*void cmd_exit(int sd, char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    char response[] = "Goodbye!\n";
    send(sd, response, strlen(response), 0);
    close(sd);
}*/

void cmd_unknown(int sd, char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    char response[] = "Unknown command\n";
    send(sd, response, strlen(response), 0);
}

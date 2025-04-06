#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>

#define MAX_TOKENS 10
#define MAX_TOKEN_LENGTH 256

void cmd_hello(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_load(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_show(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_add(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_del(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_save(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);
void cmd_unknown(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);

typedef struct {
    char *name;
    void (*func)(char[MAX_TOKENS][MAX_TOKEN_LENGTH], int);
} Command;

char loaded_aquarium[MAX_TOKEN_LENGTH] = "";

Command commandTable[] = {
    {"HELLO", cmd_hello},
    {"LOAD", cmd_load},
    {"SHOW", cmd_show},
    {"ADD", cmd_add},
    {"DEL", cmd_del},
    {"SAVE", cmd_save}
};
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

void cmd_hello(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    printf("Server: Hello!\n");
}

void cmd_load(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    if (tokenCount < 2) {
        printf("Usage: load <filename>\n");
        return;
    }

    FILE *file = fopen(tokens[1], "r");
    if (!file) {
        printf("Error loading file: %s\n", tokens[1]);
        return;
    }

    strcpy(loaded_aquarium, tokens[1]);
    fclose(file);

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
    if (tokenCount < 4 || strcmp(tokens[1], "view") != 0) {
        printf("Usage: add view <ViewName> <Geometry>\n");
        return;
    }

    FILE *file = fopen(loaded_aquarium, "a");
    if (!file) {
        printf("Could not open aquarium file to add view.\n");
        return;
    }

    fprintf(file, "%s %s\n", tokens[2], tokens[3]);
    fclose(file);

    printf("view added\n");
}

void cmd_del(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    if (tokenCount < 3 || strcmp(tokens[1], "view") != 0) {
        printf("Usage: del view <ViewName>\n");
        return;
    }

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
}

void cmd_save(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    if (tokenCount < 2) {
        printf("Usage: save <filename>\n");
        return;
    }

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

    printf("Aquarium saved! (%s)\n", tokens[1]);
}

void cmd_unknown(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount) {
    printf("Unknown command: %s\n", tokens[0]);
}

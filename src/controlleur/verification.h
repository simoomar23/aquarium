#ifndef __VERIFICATION_H__
#define __VERIFICATION_H__

#define MAX_TOKENS 10
#define MAX_TOKEN_LENGTH 100

#include "verification.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int is_id(char * id);

// this function if there is a syntax error returns -1
// otherwise it returns 0 if hello alone or the id of the view

int hello_verification(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);

// this function verifies if one arg is provided and file exists return 0 if exists otherwise 1
int load_show_save_verification(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount, char * file);

//this function doesn't verify the view id , it verifies in command.c file . returns 0 if ok 1 if not
// it checks if there is no synatx error and if coordonates are valid (bonus)
int add_view_verification(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount,int length,int width);

int del_view_verification(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);

//this function verify if there is no syntax error and valid coordinates
int add_fish_verification(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount);




#endif
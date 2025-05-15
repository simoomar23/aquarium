#include "verification.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int is_id(char * id){
    if(id[0] != 'N')
        return 0;
    char * end;
    int number = strtol(&id[1],&end,10);
    //printf("%d\n",number);
    return *end == 0 ? number : 0;
}

// this function if there is a syntax error returns -1
// otherwise it returns 0 if hello alone or the id of the view

int hello_verification(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount){
    if (tokenCount == 1)
        return 0;
    if(tokenCount != 4)
        return -1;
    int id;
    if (!strcmp(tokens[1],"in") && !strcmp(tokens[2],"as") && (id = is_id(tokens[3]))){
         return id;
    }
    return -1; 
}

// this function verifies if one arg is provided and file exists return 0 if exists otherwise 1
int load_show_save_verification(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount, char * file){
    if(tokenCount != 2)
        return 1;
    if(!strcmp(tokens[0],"SAVE"))
        return 0;
    if(!strcmp(tokens[0],"SHOW"))
        return strcmp(tokens[1],file) == 0 ? 0 : 1;
    return access(tokens[1],F_OK) == 0 ? 0 : 1;
}

//this function doesn't verify the view id , it verifies in command.c file . returns 0 if ok 1 if not
// it checks if there is no synatx error and if coordonates are valid (bonus)
int add_view_verification(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount,int length,int width){
    if(tokenCount != 4)
        return 1;
    if (!strcmp(tokens[1],"view") && is_id(tokens[2])){
        char * copy = tokens[3];
        char * end;
        int x = strtol(copy,&end,10);
        if(end[0]!='x')
            return 1;
        copy = &end[1];
        int y = strtol(copy,&end,10);
        if(end[0]!='+')
            return 1;
        copy = &end[1];
        int len = strtol(copy,&end,10);
        if(end[0]!='+')
            return 1;
        copy = &end[1];
        int wid = strtol(copy,&end,10);
        if(*end != 0)
            return 1;
        return !(x<length && y<width && x+len<length && y+wid < width);
        



    }
    return 1;
}

int del_view_verification(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount){
    if (tokenCount == 3 && !strcmp(tokens[1],"view") && is_id(tokens[2]))
        return 0;
    return 1;
}

//this function verify if there is no syntax error and valid coordinates
int add_fish_verification(char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH], int tokenCount){
    if(tokenCount!=6 || strcmp(tokens[2],"at"))
        return 1;
       
    char * copy = tokens[3];
    char * end;
    int x = strtol(copy,&end,10);
    if(end[0]!='x' || x >100)
        return 1;
    copy = &end[1];
    int y = strtol(copy,&end,10);
    if(end[0]!=',' || y>100)
        return 1;
    
    copy = tokens[4];
    int length = strtol(copy,&end,10);
    if(end[0]!='x')
        return 1;
    copy = &end[1];
    length = strtol(copy,&end,10);
    if(end[0]!=','|| length > 1000)
        return 1;    
    return 0;
}
// commands.h
#ifndef COMMANDS_H
#define COMMANDS_H


void handle_command(char *input);
char* handle_client_command(int fd,char *input);
#endif

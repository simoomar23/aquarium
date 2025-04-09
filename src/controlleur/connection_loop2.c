#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include "command.h"

#define MAX_CLIENTS 7


// Function to set the socket to non-blocking mode
int set_non_blocking(int socket) {
	int flags = fcntl(socket, F_GETFL, 0);
	if (flags == -1) {
		perror("fcntl F_GETFL failed");
		return -1;
	}

	flags |= O_NONBLOCK;  // Set the O_NONBLOCK flag
	if (fcntl(socket, F_SETFL, flags) == -1) {
		perror("fcntl F_SETFL failed");
		return -1;
	}

	return 0;
}

int main(int argc,char*argv[]) {
   int serverSocket, clientSocket[MAX_CLIENTS] = {0};
   struct sockaddr_in serverAddr, clientAddr; 
   socklen_t clientLen = sizeof(clientAddr);

   fd_set readfds, writefds, exceptfds;

   char buffer[1024];
  
   // Server Soket create
   /* domain AF_INET = IPv4 - type SOCK_STREAM = TCP - protocol = 0 */
   if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1){
	   perror("Server socket creation failed");
	   exit(EXIT_FAILURE);
   }

   set_non_blocking(serverSocket);

   printf("Server Socket successfully created\n");

   printf("\n");

   // Bind
   memset(&serverAddr, 0, sizeof(serverAddr)); // Initilalize server structure to zeros
   serverAddr.sin_family = AF_INET;
   serverAddr.sin_port = htons(atoi(argv[1])) ; // Auto assign free port
   serverAddr.sin_addr.s_addr = INADDR_ANY;  // Bind to all available interfaces


   if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
	   perror("Bind failed");
	   close(serverSocket);
	   exit(EXIT_FAILURE);
   }

   printf("Socket successfully bound to port %d and  IPV4 adress %s\n", ntohs(serverAddr.sin_port), inet_ntoa(serverAddr.sin_addr));

   printf("\n");

   // Listen --- second argument is backlog
   if (listen(serverSocket, MAX_CLIENTS) == -1){
	   perror("Listening failed");
	   close(serverSocket);
	   exit(EXIT_FAILURE);
   }

   printf("Server listening on port %d\n", serverAddr.sin_port);

   printf("Server waiting for incomming connections..\n");

	printf("\n");


   while(1){
	   // Reset file descriptor sets
	   FD_ZERO(&readfds);
	   FD_ZERO(&writefds);
	   FD_ZERO(&exceptfds);

	   // Add the server socket to sets
	   FD_SET(serverSocket, &readfds);
	   FD_SET(serverSocket, &writefds);
	   FD_SET(serverSocket, &exceptfds);

	   // Add terminal file descriptor
	   FD_SET(STDIN_FILENO, &readfds);
	   FD_SET(STDOUT_FILENO, &writefds);
	   FD_SET(STDERR_FILENO, &exceptfds);
	   int max_sd = serverSocket;



	   // Add active clients to readfds and exceptsfds
	   for (int i=0; i < MAX_CLIENTS; i++) {
		   int sd = clientSocket[i];
		   if (sd > 0) {
			   FD_SET(sd, &readfds);
			   FD_SET(sd, &writefds);
			   FD_SET(sd, &exceptfds);
		   }
		   if (sd > max_sd) max_sd = sd;
	   }

	   // Call select() - monitor for activity
	   int activity = select(max_sd + 1, &readfds, &writefds, &exceptfds, NULL);
	   if (activity < 0 /*&& errno != EINTR*/) { /////////////////
		   perror("select eroor");
		   exit(EXIT_FAILURE);
	   }

		// Check if there's input from terminal
		if (FD_ISSET(STDIN_FILENO, &readfds)) {
			// Print the prompt
			printf("$ ");
			fflush(stdout);
			fgets(buffer, sizeof(buffer), stdin);  // Read terminal input

			// Print the terminal input on the next line
			handle_command(buffer);

			// Move the cursor back to the start of the line (overwrite the current prompt)
			printf("\r$ ");
			fflush(stdout);
		}

	   // Check for new incoming connections
	   if (FD_ISSET(serverSocket, &readfds)) {
		   int newSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
			if (newSocket == -1) {
				if (errno == EWOULDBLOCK || errno == EAGAIN) {
				// No incoming connections, continue with the loop
				// You can also perform other tasks here if needed
					continue;
				} else {
					// Handle other errors
					perror("Accept failed");
					close(serverSocket);
					exit(EXIT_FAILURE);
				}}
		   if (newSocket < 0) {
			   perror("Accept failed");
			   continue;
		   }
		   printf("New client connected: %d\n", newSocket);

			// Set the new client socket to non-blocking mode
			if (set_non_blocking(newSocket) == -1) {
				close(newSocket);
				continue;
			}

		   // Add client to the list
		   for (int i = 0; i < MAX_CLIENTS; i++) {
			   if (clientSocket[i] == 0) {
				   clientSocket[i] = newSocket;
				   break;
			   }
		   }}

			// Check for client messages
			for (int i = 0; i < MAX_CLIENTS; i++) {
				int sd = clientSocket[i];

				if (sd > 0 && FD_ISSET(sd, &readfds)) {
					printf("%d\n", sd);
					char clientBuffer[256];
					memset(clientBuffer, 0, sizeof(clientBuffer));

					int bytesReceived = recv(sd, clientBuffer, sizeof(clientBuffer) - 1, 0);

					if (bytesReceived <= 0) {
						// Connection closed or error
						if (bytesReceived == 0) {
							printf("Client %d disconnected\n", sd);
						} else {
							perror("recv failed");
						}
						close(sd);
						clientSocket[i] = 0;
					} else {
						// Null-terminate and handle message
						clientBuffer[bytesReceived] = '\0';
						
						char * response = handle_client_command(sd,clientBuffer);
						if (send(sd, response, strlen(response), 0) == -1) {
							perror("send failed");
						}
						//free(response);
					}
				}
			}
   }


   printf("server shutting down\n");
   // Close communication sockets
   close(serverSocket);
   printf("Goodbye!\n");
   return 0;
}
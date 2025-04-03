//#include <sys/socket.h>
//#include <string.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <arpa/inet.h> 
//#include <sys/select.h>
//#include <sys/time.h>
//#include <sys/types.h>
//
//#define MAX_CLIENTS 3
//
//
//int main() {
//
//    /*int serverSocket = server_socket();
//    int port = server_bind(serverSocket);
//    
//    listen()
//
//    accept();
//
//
//    server_shut(serverSocket);*/
//
//
//    int serverSocket, clientSocket[MAX_CLIENTS] = {0};
//    struct sockaddr_in serverAddr, clientAddr; 
//    socklen_t clientLen = sizeof(clientAddr);
//
//    fd_set readfds, writefds, exceptfds;
//
//    char buffer[1024];
//   
//    // Server Soket create
//    /* domain AF_INET = IPv4 - type SOCK_STREAM = TCP - protocol = 0 */
//    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1){
//        perror("Server socket creation failed");
//        exit(EXIT_FAILURE);
//    }
//
//    printf("Server Socket successfully created\n");
//
//    printf("\n");
//
//    // Bind
//    memset(&serverAddr, 0, sizeof(serverAddr)); // Initilalize server structure to zeros
//    serverAddr.sin_family = AF_INET;
//    serverAddr.sin_port = htons(9090) ; // Auto assign free port
//    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Bind to all available interfaces
// 
//
//    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
//        perror("Bind failed");
//        close(serverSocket);
//        exit(EXIT_FAILURE);
//    }
//
//    printf("Socket successfully bound to port %d and  IPV4 adress %s\n", ntohs(serverAddr.sin_port), inet_ntoa(serverAddr.sin_addr));
//
//    printf("\n");
//
//    // Listen --- second argument is backlog
//    if (listen(serverSocket, 3) == -1){
//        perror("Listening failed");
//        close(serverSocket);
//        exit(EXIT_FAILURE);
//    }
//
//    printf("Server listening on port %d\n", serverAddr.sin_port);
//
//    // Accept connections --- then use pool of threads
//    //memset(&clientAddr, 0, sizeof(clientAddr)); // Initialize structure to 0s
//
//    // Blocking until accept connections -- for now one client
//    printf("Server waiting for incomming connections..\n");
//
//    printf("\n");
//
//    /*if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen)) == -1) {
//        perror("Accept failed");
//        close(serverSocket);
//        exit(EXIT_FAILURE);
//    }
//
//    printf("Client connection successfully accepted\n");*/
//
//    // Client - Server communication
//    while(1){
//        // Reset file descriptor sets
//        FD_ZERO(&readfds);
//        FD_ZERO(&writefds);
//        FD_ZERO(&exceptfds);
//
//        // Add the server socket to sets
//        FD_SET(serverSocket, &readfds);
//        FD_SET(serverSocket, &writefds);
//        FD_SET(serverSocket, &exceptfds);
//
//        // Add terminal file descriptor
//        FD_SET(fileno(stdin), &readfds);
//        FD_SET(fileno(stdin), &writefds);
//        FD_SET(fileno(stdin), &exceptsfds);
//        int max_sd = serverSocket;
//
//        // Add active clients to readfds and exceptsfds
//        for (int i=0; i < MAX_CLIENTS; i++) {
//            int sd = clientSocket[i];
//            if (sd > 0) {
//                FD_SET(sd, &readfds);
//                FD_SET(sd, &writefds);
//                FD_SET(sd, &exceptfds);
//            }
//            if (sd > max_sd) max_sd = sd;
//        }
//
//        // Call select() - monitor for activity
//        int activity = select(max_sd + 1, &readfds, &writefds, &exceptfds, NULL);
//        if (activity < 0 /*&& errno != EINTR*/) { /////////////////
//            perror("select eroor");
//            exit(EXIT_FAILURE);
//        }
//
//        // Check for new incoming connections
//        if (FD_ISSET(serverSocket, &readfds)) {
//            int newSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
//            if (newSocket < 0) {
//                perror("Accept failed");
//                continue;
//            }
//            printf("New client connected: %d\n", newSocket);
//
//            // Add client to the list
//            for (int i = 0; i < MAX_CLIENTS; i++) {
//                if (clientSocket[i] == 0) {
//                    clientSocket[i] = newSocket;
//                    break;
//                }
//            }
//        }
//
//
//        // Check for read messages - writability - errors
//        for (int i = 0; i < MAX_CLIENTS; i++) {
//        /* Check for errors on client sockets ----------------------------------------------------
//        ------------------------------------------------------------------------------------------ */
//         /* Check for read messages on incomming  */
//        }
//        
//    }
//
//
//    printf("server shutting down\n");
//    // Close communication sockets
//    close(serverSocket);
//    printf("Goodbye!\n");
//    return 0;
//}
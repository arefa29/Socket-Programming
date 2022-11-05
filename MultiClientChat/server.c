/*
 * Author: arefa29
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8081

int main(){

	int sockfd, rv;
	struct sockaddr_in serverAddr;

	int new_sockfd;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	// create socket | domain (AF_INET) => Ipv4, type (SOCK_STREAM) => TCP, protocol (0) => selected automatically
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("Server: Error in connection.\n");
		exit(1);
	}
	printf("Server: Socket created.\n");

	// store address details in struct
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT); // htons => host to network short
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // using local IP

	// bind the socket to port | serverAddr contains the adress info, port and IP
	rv = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(rv < 0){
		printf("Server: Error in binding.\n");
		exit(1);
	}
	printf("Server: Bound to port %d\n", PORT);

	// listen 
	if(listen(sockfd, 10) == 0){
		printf("Server: Listening....\n");
	}else{
		printf("Server: Error in listen().\n");
	}

	while(1){
		// accept connection requests
		new_sockfd = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(new_sockfd < 0){
			exit(1);
		}

        // inet_ntoa (internet network to application) => function converts network adress to dots and dash format string
		// htons => network to host short
		printf("Server: Connection accepted from %s: %d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

        // fork system call creates new processes for concurrently handling multiple child processes, each has its own pcb
		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				// receive message
				bzero(buffer, sizeof(buffer));
				recv(new_sockfd, buffer, 1024, 0);
				strtok(buffer, "\n"); // remove trailing \n
				if(strcmp(buffer, "exit") == 0){
                    //sin_port contains port in network byte order, ntohs converts 16 bit network byte order to host byte order
					send(new_sockfd, "exit\n", strlen("exit\n"), 0);
					printf("Server: Disconnected from %s: %d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				} else {
					printf("Client %d: %s\n", ntohs(newAddr.sin_port), buffer);

					// send message
					bzero(buffer, sizeof(buffer));
					printf("Server (to %d): ", ntohs(newAddr.sin_port));
					fgets(buffer, 1024, stdin);
					send(new_sockfd, buffer, strlen(buffer), 0);
				}
			}
		}
	}

	close(new_sockfd);

	return 0;
}
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
	char buffer[1024];

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("Client: Error in connection.\n");
		exit(1);
	}
	printf("Client: Socket created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if((rv = connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr))) < 0){
		printf("Client: Error in connection.\n");
		exit(1);
	}
	printf("Client: Connected to Server.\n");

	while(1){

		bzero(buffer, sizeof(buffer));
		printf("Client: ");
		fgets(buffer, 1024, stdin);
		send(sockfd, buffer, strlen(buffer), 0);

		// receive message 
		bzero(buffer, sizeof(buffer));
		if(recv(sockfd, buffer, 1024, 0) < 0){
			printf("Client: Error in receiving message.\n");
		} else {
			strtok(buffer, "\n"); // remove trailing \n
			if(strcmp(buffer, "exit") == 0){
				send(sockfd, "exit\n", strlen("exit\n"), 0);
				close(sockfd);
				printf("Client: Disconnected from server.\n");
				exit(1);
			}
			printf("Server: %s\n", buffer);
		}
	}
}
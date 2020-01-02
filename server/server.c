#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "utils.h"


int main(int argc, char *argv[]){
	if (argc != 2)
    {
      printf ("Syntax is: %s <port>\n", argv[0]);
      return -1;
    }

    char payload[100];
    bzero(payload,100);
    struct CommandStruct commands[20];
    int commandsNum = getCommands(commands);
    commandsInfoPayload(commands, payload);
    strcpy(payload, composeResponse(payload));

	int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");
	int port =  atoi (argv[1]);
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons (port);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", port);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
        childpid = fork();
		if(childpid == 0){
			close(sockfd);
            // send commands on connected
            send(newSocket, payload, strlen(payload), 0);
			while(1){
                bzero(payload,100);
                bzero(buffer,1024);
				recv(newSocket, buffer, 1024, 0);
				if(strcmp(buffer, "DISCONNECT") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					//printf("Client: %s\n", buffer);
					printf("Client: %s\n", buffer);
                    strtok(buffer, "|");
                    //printf("Client: %s\n", buffer);
                    int code = atoi(buffer);
                    for(int i = 0; i <= commandsNum;i++) {
                        if(commands[i].code == code) {
                            char execute[100];
                            strcpy(execute, commands[i].command);
                            strcat(execute, " ");
                            strcat(execute, commands[i].commandArgs);

                            printf("EXECUTING %s\n", commands[i].title);
                            system(execute);
                        }
                    }
					//send(newSocket, "test", strlen("test"), 0);
					//bzero(buffer, sizeof(buffer));
				}
			}
		}

	}

	close(newSocket);


	return 0;
}
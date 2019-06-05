// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 50040

int main(int argc, char const *argv[])
{
    char ipAdd[20];
    if (argc < 2){
        printf("Please enter command line arg: IP Address\n");
        return 0;
    }
    else{
        strcpy(ipAdd, argv[1]);
    }
	struct sockaddr_in address;
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char buffer[1024] = {0};
    char inStr[1024] = {0};
    char cmd[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, ipAdd, &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

    /*
    First communication with socket, will return
    Delsys Base Software Version
    */
    send(sock, cmd, strlen(cmd), 0);
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);
    while (1){
        // Clear char arrays
        memset(buffer, 0, sizeof(buffer));
        memset(inStr, 0, sizeof(inStr));
        memset(cmd, 0, sizeof(cmd));

        // Get Next Command
        printf("Enter a command\n");
        std::cin.getline(inStr, sizeof(inStr));
        sprintf(cmd, "%s\r\n\r\n", inStr);
        if (strcmp(inStr, "END") == 0){
            printf("END init\n");
            break;
        }
        send(sock, cmd, strlen(cmd), 0);
        valread = read(sock, buffer, 1024);
        printf("%s\n", buffer);
    }
	return 0;
}

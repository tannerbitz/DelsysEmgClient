// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 50041


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
	char buffer[1024];
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

    printf("Connection To EMG Data Port Established\n");

    // while (1){
        // Clear char arrays
        memset(buffer, 1, sizeof(buffer));
        valread = read(sock, buffer, 64);
        for (int i=0; i<valread + 4; i++){
            std::cout << i << "   " << buffer[i] << std::endl;
        }
        // printf("%i, %.*s\n", valread, valread, buffer);
    // }
	return 0;
}

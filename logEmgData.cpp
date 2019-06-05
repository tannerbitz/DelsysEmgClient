/* Trigno SDK Example.  Copyright (C) 2011-2012 Delsys, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, and distribute the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
*/

/* Compilation and execution instructions:
 * In a shell, execute
 *      [user@workstation]$ gcc LinuxSdkSample.c
 *      [user@workstation]$ ./a.out
 * and follow the prompts on the screen.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <chrono>

#define SZ_INPUT            (256)
#define N_SAMPLES_TO_WRITE  (1)
#define SZ_DATA_EMG         (64 * N_SAMPLES_TO_WRITE)

/* Function prototypes */
char * readReply(int socket);
void GetKeyboardInputToEndProgram(bool * bEndKeyHit);

/* Application entrypoint */
int main()
{
    /* User input */
    char szIpAddress[SZ_INPUT];
    char szTmp[SZ_INPUT];
    char szTmp1[SZ_INPUT];

    /* Network communication and program state */
    in_addr_t ipAddress;
    struct sockaddr_in sinRemote;
    bool bFirstSampleAcquired;
    int commSock, emgSock;
    char *reply;
    int i, j;
    time_t startTime;
    time_t lastTime;

    /* Recieved data */
    char emgData[SZ_DATA_EMG];
    char sensorType[16];
    FILE *emgDataFile;

    /* Read user input */
    printf("Delsys Digital SDK Linux Demo Application\r\n");
    printf("Enter the server IP address: ");
    fgets(szIpAddress, SZ_INPUT, stdin);

    /* Disable buffering */
    setbuf(stdout, NULL);

    printf("\r\nPlease wait...setting up connection.");

    /* Convert input to appropriate data types */
    ipAddress = inet_addr(szIpAddress);

    /* Connect to the server */
    commSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sinRemote.sin_family = AF_INET;
    sinRemote.sin_addr.s_addr = ipAddress;
    sinRemote.sin_port = htons(50040);
    if (connect(commSock, (struct sockaddr*)&sinRemote, sizeof(struct sockaddr_in)) == -1)
    {
        printf("\r\nProblem connecting to server.\r\n");
        return 0;
    }

    reply = readReply(commSock);
    printf("\r\nConnected to %s", reply);
    free(reply);

    emgSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sinRemote.sin_port = htons(50041);
    if (connect(emgSock, (struct sockaddr*)&sinRemote, sizeof(struct sockaddr_in)) == -1)
    {
        printf("\r\nProblem connecting to server.\r\n");
        return 0;
    }


    /* Configure the server */
    printf("Requesting data.");

    //set the endian to little for Linux socket
    //start data collection
    sprintf(szTmp, "ENDIAN LITTLE\r\n\r\n");

    if (send(commSock, szTmp, strlen(szTmp), 0) == -1)
    {
        printf("\r\nProblem starting Delsys data stream.\r\n");
        return 0;
    }

    reply = readReply(commSock);
    free(reply);
    sprintf(szTmp, "ENDIANNESS?\r\n\r\n");

    if (send(commSock, szTmp, strlen(szTmp), 0) == -1)
    {
        printf("\r\nProblem starting Delsys data stream.\r\n");
        return 0;
    }

    reply = readReply(commSock);
    free(reply);

    sprintf(szTmp, "START\r\n\r\n");

    if (send(commSock, szTmp, strlen(szTmp), 0) == -1)
    {
        printf("\r\nProblem starting Delsys data stream.\r\n");
        return 0;
    }

    reply = readReply(commSock);
    free(reply);


    /* get the type of sensors*/
    for(i=0; i<16; i++)
    {
        sprintf(szTmp1, "SENSOR %d TYPE?\r\n\r\n", i+1);
        if (send(commSock, szTmp1, strlen(szTmp1), 0) == -1)
        {
            printf("\r\nProblem starting Delsys data stream.\r\n");
            return 0;
        }

        reply=readReply(commSock);
        sensorType[i]=*reply;
        free(reply);
    }

    /* Open files to store the acquired data */
    /* files for standard sensor*/
    emgDataFile = fopen("emgData.csv", "w+");
    for (i = 0; i < 16; i++)
    {
        if (i < 15){
                fprintf(emgDataFile, "EMG%d, ", i + 1);
        }
        else{
                fprintf(emgDataFile, "EMG%d\r\n", i + 1);
        }
    }

     /* Data collection loop */
    bool bEndKeyHit = false;
    std::thread keyboard_input_listener_thread(GetKeyboardInputToEndProgram, &bEndKeyHit);

    /* Empty tcp/ip buffer */
    while (recv(emgSock, emgData, SZ_DATA_EMG, MSG_PEEK) > SZ_DATA_EMG){
        recv(emgSock, emgData, SZ_DATA_EMG, 0);
    }
    std::cout << "Press 'q' to exit Data Collection Loop" << std::endl;

    // Clock Start
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = std::chrono::high_resolution_clock::now();

    while (!bEndKeyHit){
        /* Read EMG data from the network and write to file */
        if (recv(emgSock, emgData, SZ_DATA_EMG, MSG_PEEK) >= SZ_DATA_EMG)
        {
            recv(emgSock, emgData, SZ_DATA_EMG, 0);
            unsigned int samples = SZ_DATA_EMG / sizeof(float) / 16;
            float emgDataFlt[SZ_DATA_EMG / sizeof(float)];
            memcpy(emgDataFlt, emgData, SZ_DATA_EMG);
            for (i = 0; i < samples; i++){
                for (j = 0; j < 16; j++){
                    if (j < 15){
                        if(sensorType[j]=='D'){
                            fprintf(emgDataFile, "%f, ", emgDataFlt[i * 16 + j]);
                        }
                        else{
                            fprintf(emgDataFile, ", ");
                        }
                    }

                    else{
                        if(sensorType[j]=='D'){
                            fprintf(emgDataFile, "%f", emgDataFlt[i * 16 + j]);
                        }
                        else{
                            fprintf(emgDataFile, ",");
                        }
                    }
                }
                fprintf(emgDataFile, "\r\n");
            }
        }
    }

    high_resolution_clock::time_point t_end = std::chrono::high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t_end - t_start);

    printf("The program lasted %9.8f seconds\n", time_span);

    keyboard_input_listener_thread.join();


    /* Close the EMG and ACC data storage files */
    fclose(emgDataFile);

    /* Close the connection to the server */
    printf("\r\nClosing connections.");

    sprintf(szTmp, "STOP\r\n\r\n");
    if (send(commSock, szTmp, strlen(szTmp), 0) == -1)
    {
            printf("\r\nProblem stopping Delsys data stream.\r\n");
            return 0;
    }
    reply = readReply(commSock);
    free(reply);

    sprintf(szTmp, "QUIT\r\n\r\n");
    if (send(commSock, szTmp, strlen(szTmp), 0) == -1)
    {
            printf("\r\nProblem closing connection.\r\n");
            return 0;
    }

    close(commSock);
    close(emgSock);

    /* Done */
    printf("\r\n");

    return 0;
}

void GetKeyboardInputToEndProgram(bool * bEndKeyHit){
    char keyboard_input[256] = "";
    char * pos;
    char exit_key[10] = "q";
    while (strcmp(keyboard_input, exit_key) != 0){
        fgets(keyboard_input, 256, stdin);
        // remove newline char from fgets
        if ((pos = strchr(keyboard_input, '\n')) != NULL){
            *pos = '\0';
        }
    }
    *bEndKeyHit = true;
}



/* Read and return a reply, waiting until one is present.
 * Memory allocated by this function must be free by the caller.
 */
char * readReply(int socket)
{
        int bytesAvail = 0;
        while (!bytesAvail && ioctl(socket, FIONREAD, &bytesAvail) >= 0)
        {
                usleep(100 * 1000); /* 100 ms */
                printf(".");
        }
        if (bytesAvail > 0)
        {
                char *tmp = (char *) malloc(bytesAvail);
                recv(socket, tmp, bytesAvail, 0);
                tmp[bytesAvail] = '\0';
                return tmp;
        }

        char *tmp = (char *) malloc(1);
        tmp[0] = '\0';
        return tmp;
}

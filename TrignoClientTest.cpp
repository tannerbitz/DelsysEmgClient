#include <stdio.h>
#include "TrignoEmgClient.h"

int main(int argc, char** argv){

    if (argc != 2){
        printf("Use: TrignoEmgClient <EmgServerIP>\n");
        exit(1);
    }

    /* Instantiate Trigno Emg Client */
    std::string ipAddr(argv[1]);
    TrignoEmgClient client(ipAddr);

    /* Connect to comm port */
    client.ConnectDataPort();
    client.ConnectCommPort();

    /* Send start command */
    if (client.IsCommPortConnected()){
        client.SendCommand(1);
        client.ReceiveDataStream();
    }
    else{
        printf("Comm port not connected.  No START command sent.\n");
    }


}

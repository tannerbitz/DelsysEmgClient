#include <stdio.h>
#include <thread>
#include <unistd.h>
#include "TrignoEmgClient.h"
#include "H5Cpp.h"

using namespace H5;

int main(int argc, char** argv){

    if (argc != 2){
        printf("Use: TrignoEmgClient <EmgServerIP>\n");
        exit(1);
    }

    /* Instantiate Trigno Emg Client */
    std::string ipAddr(argv[1]);
    TrignoEmgClient * client = new TrignoEmgClient(ipAddr);

    /* Connect to comm port */
    client->ConnectDataPort();
    client->ConnectCommPort();


    /* Send start command */
    if (client->IsCommPortConnected()){

        H5File * file = 0;
        std::string filename = "myhdf.h5";
        // Exception::dontPrint();
        try{
            /* Open file if it exists */
            file = new H5File(filename.c_str(), H5F_ACC_RDWR);
        }
        catch(const FileIException&){
            /* Create file if it does not exist */
            file = new H5File(filename.c_str(), H5F_ACC_TRUNC);
        }

        client->SendCommand(1);
        int nEmgs = 3;
        int emgList[nEmgs] = {2,3,5};
        client->SetEmgToSave(emgList, nEmgs);
        std::thread t(&TrignoEmgClient::ReceiveDataStream, client);
        sleep(10);
        client->StartWriting(file);
        sleep(20);
        client->StopWriting();
        client->StopReceiveDataStream();
        sleep(1);
        delete client;
        file->close();
        delete file;
        printf("file closed\n");
        t.join();
    }
    else{
        printf("Comm port not connected.  No START command sent.\n");
    }

    printf("I'm out of the loop\n");

}

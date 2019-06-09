#include "TrignoEmgClient.h"

TrignoEmgClient::TrignoEmgClient(std::string ipAddr){
    _ipAddr = ipAddr;
}

void ConnectPort(tcp::socket sock, tcp::resolver res, std::string port, std::string portType){
    try{
        boost::asio::connect(sock, res.resolver(_ipAddr, port));
    }
    catch(std::exception& e){
        printf("Exception occured while connecting to Trigno %s Port\n", portType.c_str());
        printf("Address: %s\n", _ipAddr.c_str());
        printf("Port: %s\n", port.c_str());
        printf("Exception: %s\n", e.what());
    }
}

void TrignoEmgClient::ConnectDataPort(){
    ConnectPort(_sockData, _resData, _portData, "Data");
}

void TrignoEmgClient::ConnectCommPort(){
    ConnectPort(_sockComm, _resComm, _portComm, "Comm");
}

void TrignoEmgClient::ConnectAcclPort(){
    ConnectPort(_sockAccl, _resAccl, _portAccl, "Accl");
}

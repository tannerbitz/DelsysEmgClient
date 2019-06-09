#include "TrignoEmgClient.h"

TrignoEmgClient::TrignoEmgClient(std::string ipAddr){
	/* Set IP address */
    _ipAddr = ipAddr;

	/* Initialize communicaion port commands */
	_cmds[1] = "START\r\n\r\n";
	_cmds[2] = "STOP\r\n\r\n";

}

bool ConnectPort(tcp::socket sock, tcp::resolver res, std::string port, std::string portType){
	/* General function to connect to data or communication port */
    bool connected = True;
    try{
        boost::asio::connect(sock, res.resolver(_ipAddr, port));
    }
    catch(std::exception& e){
        printf("Exception occured while connecting to Trigno %s Port\n", portType.c_str());
        printf("Address: %s\n", _ipAddr.c_str());
        printf("Port: %s\n", port.c_str());
        printf("Exception: %s\n", e.what());
        connected = false;
    }
    return connected;
}

void TrignoEmgClient::ConnectDataPort(){
	/* Calls ConnectPort() to connect data port */
    _connectedData = ConnectPort(_sockData, _resData, _portData, "Data");
}

void TrignoEmgClient::ConnectCommPort(){
 	/* Calls ConnectPort() to connect comm port */
	_connectedComm = ConnectPort(_sockComm, _resComm, _portComm, "Comm");
}

void TrignoEmgClient::GetReplyComm(){
	/* Reset reply string and get new reply */
	memset(_replyComm, 0, std::strlen(_replyComm));
    try{
	   size_t reply_length = boost::asio::read(_sockComm,
                                               bost::asio::buffer(_replyComm, std::strlen(_replyComm)));
    }
    catch (std::exception & e){
      printf("Exception Occured During Comm Read: %s\n", e.what());
    }
	/* Convert to std::string, find \r\n\r\n, and remove */
	std::string temp(_replyComm);
	size_t newlineStart = temp.find("\r\n\r\n");

	if (newlineStart != std::string::npos){
		temp.replace(newlineStart, 4, "");
	}

	/* Print to command line */
	printf("Server Reply: %s\n", temp.c_str());
}

void TrignoEmgClient::SendCommand(int cmdNumber){
	std::string cmd = _cmds[cmdNumber];
	if (cmd.compare("") != 0){	// command found
		printf("Command: %s\n", cmd.c_str());
	try{
      boost::asio::write(_sockComm,
					     boost::asio::buffer(cmd.c_str(), strlen(cmd.c_str())));
    }
    catch (std::exception & e){
      printf("Exception Occured During Write: %s\n", e.what());
    }
    this->GetReplyComm();
	}
	else{	                 // command not found
		printf("Command number %d does not correspond to a command in the command list\n", cmdNumber);
	}
}

void ReceiveDataStream(){
    while (_connectData){
        /* Receive 16 sensors * 4 bytes/sensor worth of data and handle it */
    }
}

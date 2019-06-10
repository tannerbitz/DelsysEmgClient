#include "TrignoEmgClient.h"
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>
#include <thread>

/* Public Functions */
TrignoEmgClient::TrignoEmgClient(std::string ipAddr){
	/* Set IP address */
    _ipAddr = ipAddr;

    /* Create IP address */
    _addr = boost::asio::ip::make_address(_ipAddr);

    /* Set Endpoint Address and Port*/
    _endpointComm.address(_addr);
    _endpointData.address(_addr);
    _endpointComm.port(_portComm);
    _endpointData.port(_portData);


	/* Initialize communicaion port commands */
	_cmds[1] = "START\r\n\r\n";
	_cmds[2] = "STOP\r\n\r\n";

}

void TrignoEmgClient::ConnectDataPort(){
    try{
        _sockData.connect(_endpointData);
        printf("Data port connected\n");
        _connectedDataPort = true;
    }
    catch(std::exception& e){
        printf("Exception occured while connecting to Trigno Data Port\n");
        printf("Address: %s\n", _ipAddr.c_str());
        printf("Port: %d\n", _endpointData.port());
        printf("Exception: %s\n", e.what());
        _connectedDataPort = false;
    }
}

void TrignoEmgClient::ConnectCommPort(){
 	/* Calls ConnectPort() to connect comm port */
	try{
        _sockComm.connect(_endpointComm);
        printf("Comm socket connected\n");
        _connectedCommPort = true;
    }
    catch(std::exception& e){
        printf("Exception occured while connecting to Trigno Comm Port\n");
        printf("Address: %s\n", _ipAddr.c_str());
        printf("Port: %d\n", _endpointComm.port());
        printf("Exception: %s\n", e.what());
        _connectedCommPort = false;
    }
}

void TrignoEmgClient::SendCommand(int cmdNumber){
	std::string cmd = _cmds[cmdNumber];
	if (cmd.compare("") != 0){	// command found
		printf("Command: %s\n", cmd.c_str());
    	try{
          _sockComm.send(boost::asio::buffer(cmd.c_str(), strlen(cmd.c_str())));
        }
        catch (std::exception & e){
          printf("Exception Occured During Write: %s\n", e.what());
        }
	}
	else{	                 // command not found
		printf("Command number %d does not correspond to a command in the command list\n", cmdNumber);
	}
}

bool TrignoEmgClient::IsCommPortConnected(){
    return _connectedCommPort;
}

bool TrignoEmgClient::IsDataPortConnected(){
    return _connectedDataPort;
}

/* Private Functions */
void TrignoEmgClient::GetReplyComm(){
    /* Reset reply string and get new reply */
    memset(_replyComm, 0, std::strlen(_replyComm));
    try{
        boost::asio::socket_base::bytes_readable command(true);
        _sockComm.io_control(command);
        size_t bytes_readable = command.get();
        while (bytes_readable <= 0){
            printf("Bytes available: %d\n", (int) bytes_readable);
            boost::this_thread::sleep_for(boost::chrono::milliseconds(50));
            bytes_readable = command.get();
        }
        _sockComm.receive(boost::asio::buffer(_replyComm, std::strlen(_replyComm)));

        /* Convert to std::string, find \r\n\r\n, and remove */
        std::string temp(_replyComm);
        size_t newlineStart = temp.find("\r\n\r\n");

        if (newlineStart != std::string::npos){
          temp.replace(newlineStart, 4, "");
        }

        /* Print to command line */
        printf("Server Reply: %s\n", temp.c_str());
    }
    catch (std::exception & e){
        printf("Exception Occured During Comm Read: %s\n", e.what());
    }
}



void TrignoEmgClient::ReceiveDataStream(){
    float temp[16];
    while (_connectedDataPort){
        /* Receive 16 sensors * 4 bytes/sensor worth of data and handle it */
        memset(_replyComm, 0, sizeof(_replyComm));
        boost::system::error_code ec;
        size_t n = boost::asio::read(_sockData,
                                     boost::asio::buffer(_replyComm),
                                     boost::asio::transfer_exactly(64),
                                     ec);
        if (ec){
            printf("Error occured while reading\n");
            _connectedDataPort = false; // will exit loop
        }
        else{
            memcpy(temp, _replyComm, _nSensors*_nBytesPerFloat);
            // printf("%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
            //         temp[0],
            //         temp[1],
            //         temp[2],
            //         temp[3],
            //         temp[4],
            //         temp[5],
            //         temp[6],
            //         temp[7],
            //         temp[8],
            //         temp[9],
            //         temp[10],
            //         temp[11],
            //         temp[12],
            //         temp[13],
            //         temp[14],
            //         temp[15]);
            printf("%f\n", temp[5]);
        }
    }
}

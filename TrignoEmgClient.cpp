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
        this->GetReplyComm();
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
		printf("Command: %s\n", RemoveNewlines(cmd).c_str());
    	try{
          _sockComm.send(boost::asio::buffer(cmd.c_str(), strlen(cmd.c_str())));
          this->GetReplyComm();
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
    memset(_replyComm, 0, sizeof(_replyComm));
    try{
        boost::system::error_code ec;
        size_t n = boost::asio::read(_sockComm,
                                  boost::asio::buffer(_replyComm),
                                  boost::asio::transfer_at_least(1),
                                  ec);

        if (ec){    // error
            printf("Error occured while receiving comm reply from server\n");
        }
        else{       // no error
            /* Convert to std::string, find \r\n\r\n, and remove */
            std::string temp(_replyComm);
            temp = RemoveNewlines(temp);

            /* Print to command line */
            printf("Server Reply: %s\n", temp.c_str());
        }
    }
    catch (std::exception & e){
        printf("Exception Occured During Comm Read: %s\n", e.what());
    }
}

std::string TrignoEmgClient::RemoveNewlines(std::string str_in){
    std::string temp;
    size_t newlineStart = str_in.find("\r\n\r\n");
    if (newlineStart != std::string::npos){
        temp = str_in.replace(newlineStart, 4, "");
    }
    else{
        temp = str_in;
    }
    return temp;

}


void TrignoEmgClient::ReceiveDataStream(){
    std::array<float,16> temp;
    while (_connectedDataPort){
        /* Receive 16 sensors * 4 bytes/sensor worth of data and handle it */
        memset(_replyComm, 0, sizeof(_replyComm));
        boost::system::error_code ec;
        size_t n = boost::asio::read(_sockData,
                                     boost::asio::buffer(_replyComm),
                                     boost::asio::transfer_exactly(64),
                                     ec);
        if (ec){    // error
            printf("Error occured while reading\n");
            _connectedDataPort = false; // will exit loop
        }
        else{       // no error
            /* Save to data queue if specified by flag */
            if (_saveDataToQueue){
                memcpy(temp.data(), _replyComm, _nSensors*_nBytesPerFloat);
                _dataQueue.push(temp);
            }
        }
    }
}

void TrignoEmgClient::SetDataQueueSave(bool save){
    _saveDataToQueue = save;
}

#include <boost/asio.hpp>
#include <exception>
#include <stdio.h>
#include <string>
#include <map>
#include <array>
#include <queue>

using boost::asio::ip::tcp;

class TrignoEmgClient{

private:

    /* Trigno IP Address */
    std::string _ipAddr;                    // IPv4 address
    boost::asio::ip::address _addr;

    /* Trigno Ports */
    const unsigned short _portComm = 50040;   // Communication port
    const unsigned short _portData = 50041;   // EMG data port

    /* Trigno Endpoints */
    tcp::endpoint _endpointComm;
    tcp::endpoint _endpointData;

    /* Trigno Sockets */
    boost::asio::io_context io_context_comm;
    boost::asio::io_context io_context_data;
    tcp::socket _sockComm{io_context_comm}; // asynchronous socket
    tcp::socket _sockData{io_context_data}; // synchronous socket

    /* Data Stream constants */
    const unsigned short _nSensors = 16;
    const unsigned short _nBytesPerFloat = 4;

    /* Connection Status */
    bool _connectedDataPort = false;
    bool _connectedCommPort = false;

    /* Data Queue */
    std::queue<std::array<float, 16>> _dataQueue;
    bool _saveDataToQueue = false;

	/* Commands */
	std::map<int, std::string> _cmds;

	/* Reply Variables */
	static const int MAXLENGTH = 1024;
	char _replyComm[MAXLENGTH];
	char _replyData[MAXLENGTH];

	/* Functions */
    void GetReplyComm();
    std::string RemoveNewlines(std::string str_in);

public:
	/* Functions */
    TrignoEmgClient(std::string ipAddr);
    void ConnectDataPort();
    void ConnectCommPort();
	void SendCommand(int cmd);
    bool IsCommPortConnected();
    bool IsDataPortConnected();
    void ReceiveDataStream();
    void SetDataQueueSave(bool save);

};

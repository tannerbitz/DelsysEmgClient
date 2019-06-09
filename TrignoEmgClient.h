#include <boost/asio.hpp>
#include <exception>
#include <stdio.h>
#include <string>
#include <map>

using boost::asio::ip::tcp;

class TrignoEmgClient{

private:

    /* Trigno IP Address */
    std::string _ipAddr;                    // IPv4 address

    /* Trigno Ports */
    const std::string _portComm("50040");   // Communication port
    const std::string _portData("50041");   // EMG data port

    /* Trigno Sockets */
    boost::asio::io_context io_context_comm;
    boost::asio::io_context io_context_data;
    tcp::resolver _resComm(io_context_comm);
    tcp::resolver _resData(io_context_data);
    tcp::socket _sockComm (io_context_comm);
    tcp::socket _sockData(io_context_data);

    /* Connection Status */
    bool _connectedData = false;
    bool _connectedComm = false;

	/* Commands */
	std::map<int, std::string> _cmds;

	/* Reply Variables */
	const int MAXLENGTH = 1024;
	char _replyComm[MAXLENGTH];
	char _replyData[MAXLENGTH];

	/* Functions */
	bool ConnectPort(tcp::socket sock,
                     tcp::resolver res,
                     std::string port,
                     std::string portType);
	void GetReplyComm();
    void ReceiveDataStream();
public:
	/* Functions */
    TrignoEmgClient(std::string ipAddr);
    void ConnectDataPort();
    void ConnectCommPort();
	void SendCommand(int cmd);

}

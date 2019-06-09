#include <boost/asio.hpp>
#include <exception>
#include <stdio.h>
#include <string>

using boost::asio::ip::tcp;

class TrignoEmgClient{

private:

    /* Trigno IP Address */
    std::string _ipAddr;                    // IPv4 address

    /* Trigno Ports */
    const std::string _portComm("50040");   // Communication port
    const std::string _portData("50041");   // EMG data port
    const std::string _portAccl("50042");   // Accelerometer port

    /* Trigno Sockets */
    boost::asio::io_context io_context_comm;
    boost::asio::io_context io_context_data;
    boost::asio::io_context io_context_accl;
    tcp::resolver _resComm(io_context_comm);
    tcp::resolver _resData(io_context_data);
    tcp::resolver _resAccl(io_context_accl);
    tcp::socket _sockComm (io_context_comm);
    tcp::socket _sockData(io_context_data);
    tcp::socket _sockAccl(io_context_accl);

    void ConnectPort(tcp::socket sock,
        tcp::resolver res,
        std::string port,
        std::string portType);
public:
    TrignoEmgClient(std::string ipAddr);
    void ConnectDataPort();
    void ConnectCommPort();
    void ConnectAcclPort();
}

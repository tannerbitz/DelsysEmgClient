#include <boost/asio.hpp>
#include <exception>
#include <stdio.h>
#include <string>
#include <map>
#include <array>
#include <queue>
#include "H5Cpp.h"

using boost::asio::ip::tcp;
using namespace H5;

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
    tcp::socket _sockComm{io_context_comm}; // synchronous socket
    tcp::socket _sockData{io_context_data}; // synchronous socket

    /* Data Stream constants */
    static const unsigned short _nSensors = 16;
    const unsigned short _nBytesPerFloat = 4;

    /* Connection Status */
    bool _connectedDataPort = false;
    bool _connectedCommPort = false;

    /* HDF5 Variables */
    static const int rank = 2;
    static const int chunkRows = 10000;
    const hsize_t chunkDims[rank]   = {(hsize_t) chunkRows, (hsize_t) _nSensors};   // dataset dimensions at creation
    const hsize_t maxDims[rank]     = {H5S_UNLIMITED, H5S_UNLIMITED};
    hsize_t datasetEmgDims[rank]    = {(hsize_t) chunkRows, (hsize_t) _nSensors};
    hsize_t writespaceDims[rank]    = {(hsize_t) chunkRows, (hsize_t) _nSensors};
    hsize_t fspaceOffset[rank]      = {0,0};                                        // offset must be kept track of to append in correct places
    const hsize_t noOffset[rank]    = {0,0};

    DataSpace * mspace      = new DataSpace(rank, chunkDims, maxDims);        // memory space                                 // file space
    DataSet datasetEmg      = DataSet();                                   // emg dataset
    DSetCreatPropList dsPropList;               // Dataset Creation Property List
    bool _firstWrite;

    /* Data Queue */
    float _dataArr[chunkRows][_nSensors];
    bool _writeFlag = false;                    // flag to signal to save to _dataQueue
    int _rowCount = 0;                          // number of emg reads since last write

	/* Commands */
	std::map<int, std::string> _cmds;

	/* Reply Variables */
	static const int MAXLENGTH = 1024;
	char _replyComm[MAXLENGTH];
	char _replyData[MAXLENGTH];

	/* Functions */
    void GetReplyComm();
    void WriteH5Chunk();
    std::string RemoveNewlines(std::string str_in);

public:
	/* PUBLIC FUNCTIONS */
    TrignoEmgClient(std::string ipAddr);            // Constructor
    ~TrignoEmgClient();                             // Destructor
    /* Connect Ports */
    void ConnectDataPort();
    void ConnectCommPort();
    /* Commands */
	void SendCommand(int cmd);
    /* Get Internal Status */
    bool IsCommPortConnected();
    bool IsDataPortConnected();
    bool IsWriting();
    /* Start/Stop Receiving Data */
    void ReceiveDataStream();
    void StopReceiveDataStream();
    /* Start/Stop writing data */
    void StartWriting(H5Location * h5loc);
    void StopWriting();
};

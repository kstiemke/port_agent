/*******************************************************************************
 * Class: InstrumentTCPMultiConnection
 * Filename: instrument_tcp_connection.cxx
 * Author: Bill French (wfrench@ucsd.edu)
 * License: Apache 2.0
 *
 * Manages the socket connection between the observatory and the port agent.
 * This interface consists of a TCP listener on the data port and command port
 * setup in non-blocking mode.
 *
 * Usage:
 *
 * InstrumentTCPMultiConnection connection;
 *
 * connection.setDataPort(4001);
 *
 * // Is the data port configured
 * connection.dataConfigured();
 *
 * // This is a noop for this method.  There is nothing to initialize
 * connection.initialize();
 *
 * // Always true for this connection type.
 * connection.dataInitialized();
 *
 * // Is the data port connected
 * connection.dataConnected();
 *
 * // Always false for this connection type
 * connection.commandConnected();
 *
 * // Get a pointer tcp data listener object
 * TCPCommListener *data = connection.dataConnectionObject();
 *    
 * // Always returns null for this connection type
 * TCPCommListener *command = connection.commandConnectionObject();
 *    
 ******************************************************************************/

#include "instrument_botpt_tcp_connection.h"
#include "common/util.h"
#include "common/logger.h"
#include "common/exception.h"
#include "network/tcp_comm_listener.h"

using namespace std;
using namespace logger;
using namespace network;
using namespace port_agent;
    
/******************************************************************************
 *   PUBLIC METHODS
 ******************************************************************************/
/******************************************************************************
 * Method: Constructor
 * Description: Default constructor.  Is likely never called, but wanted to
 *              define it explicitly.
 ******************************************************************************/
InstrumentTCPMultiConnection::InstrumentTCPMultiConnection() : Connection() {
}

/******************************************************************************
 * Method: Copy Constructor
 * Description: Copy constructor ensuring we do a deep copy of the packet data.
 *
 * Parameters:
 *   copy - rhs object to copy
 ******************************************************************************/
InstrumentTCPMultiConnection::InstrumentTCPMultiConnection(const InstrumentTCPMultiConnection& rhs) {
    copy(rhs);
}

/******************************************************************************
 * Method: Destructor
 * Description: free up our dynamically created packet data.
 ******************************************************************************/
InstrumentTCPMultiConnection::~InstrumentTCPMultiConnection() {
}

/******************************************************************************
 * Method: Assignemnt operator
 * Description: Deep copy
 *
 * Parameters:
 *   copy - rhs object to copy
 ******************************************************************************/
InstrumentTCPMultiConnection & InstrumentTCPMultiConnection::operator=(const InstrumentTCPMultiConnection &rhs) {
    copy(rhs);
    return *this;
}


/******************************************************************************
 * Method: copy
 * Description: Copy data from one packet object into this one.  Ensuring we
 * deep copy when needed.
 *
 * Parameters:
 *   copy - rhs object to copy
 ******************************************************************************/
void InstrumentTCPMultiConnection::copy(const InstrumentTCPMultiConnection &copy) {
    m_oDataTxSocket = copy.m_oDataTxSocket;
    m_oDataRxSocket = copy.m_oDataRxSocket;
}

/******************************************************************************
 * Method: setDataTxPort
 * Description: Set the port.  If we are already connected then we need to
 * disconnect and reconnect to the new port.
 ******************************************************************************/
void InstrumentTCPMultiConnection::setDataTxPort(uint16_t port) {
    uint16_t oldPort = m_oDataTxSocket.port();
    m_oDataTxSocket.setPort(port);
    
    if(m_oDataTxSocket.connected() && m_oDataTxSocket.port() != oldPort) {
	m_oDataTxSocket.initialize();
    }
}

/******************************************************************************
 * Method: setDataRxPort
 * Description: Set the port.  If we are already connected then we need to
 * disconnect and reconnect to the new port.
 ******************************************************************************/
void InstrumentTCPMultiConnection::setDataRxPort(uint16_t port) {
    uint16_t oldPort = m_oDataRxSocket.port();
    m_oDataRxSocket.setPort(port);

    if(m_oDataRxSocket.connected() && m_oDataRxSocket.port() != oldPort) {
    m_oDataRxSocket.initialize();
    }
}

/******************************************************************************
 * Method: setDataHost
 * Description: Set the host.  If we are already connected then we need to
 * disconnect and reconnect to the new port.
 ******************************************************************************/
void InstrumentTCPMultiConnection::setDataHost(const string & host) {
    string oldhost = m_oDataTxSocket.hostname();
    m_oDataTxSocket.setHostname(host);

    if (m_oDataTxSocket.connected() && m_oDataTxSocket.hostname() != oldhost) {
        m_oDataTxSocket.initialize();
    }

    oldhost = m_oDataRxSocket.hostname();
    m_oDataRxSocket.setHostname(host);
    
    if (m_oDataRxSocket.connected() && m_oDataRxSocket.hostname() != oldhost) {
        m_oDataRxSocket.initialize();
    }
}

/******************************************************************************
 * Method: dataConfigured
 * Description: Do we have enough configuration information to initialize the
 * data socket?
 *
 * Return: 
 *   True if we have enough configuration information
 ******************************************************************************/
bool InstrumentTCPMultiConnection::dataConfigured() {
    return m_oDataTxSocket.isConfigured() && m_oDataRxSocket.isConfigured();
}

/******************************************************************************
 * Method: commandConfigured
 * Description: Do we have enough configuration information to initialize the
 * command socket?
 *
 * Return: 
 *   True if we have enough configuration information
 ******************************************************************************/
bool InstrumentTCPMultiConnection::commandConfigured() {
    return false;
}

/******************************************************************************
 * Method: dataInitialized
 * Description: No initialization sequence, so if configure then we are
 * initialiaze
 *
 * Return:
 *   True if configured.
 ******************************************************************************/
bool InstrumentTCPMultiConnection::dataInitialized() {
    return dataConfigured();
}

/******************************************************************************
 * Method: commandInitialized
 * Description: Alwasy false because there is no command interface for this
 * connection type.
 *
 * Return:
 *   False
 ******************************************************************************/
bool InstrumentTCPMultiConnection::commandInitialized() {
    return false;
}

/******************************************************************************
 * Method: dataConnected
 * Description: Is a client connected to the data socket connection
 *
 * Return:
 *   True if the data socket is connected
 ******************************************************************************/
bool InstrumentTCPMultiConnection::dataConnected() {
    return m_oDataTxSocket.connected() && m_oDataRxSocket.connected();
}

/******************************************************************************
 * Method: commandConnected
 * Description: Alwasy false because there is no command interface for this
 * connection type.
 *
 * Return:
 *   False
 ******************************************************************************/
bool InstrumentTCPMultiConnection::commandConnected() {
    return false;
}

/******************************************************************************
 * Method: initializeDataSocket
 * Description: Initialize the data socket
 ******************************************************************************/
void InstrumentTCPMultiConnection::initializeDataSocket() {
    m_oDataTxSocket.initialize();
    m_oDataRxSocket.initialize();
}

/******************************************************************************
 * Method: initializeCommandSocket
 * Description: NOOP
 ******************************************************************************/
void InstrumentTCPMultiConnection::initializeCommandSocket() {
}

/******************************************************************************
 * Method: initialize
 * Description: Initialize any uninitialized sockets if they are configured.
 ******************************************************************************/
void InstrumentTCPMultiConnection::initialize() {
    if(!dataConfigured())
        LOG(DEBUG) << "Data port not configured. Not initializing";
	
    if(dataConfigured() && ! dataConnected()) {
	LOG(DEBUG) << "initialize data socket";
        initializeDataSocket();
    } 
}



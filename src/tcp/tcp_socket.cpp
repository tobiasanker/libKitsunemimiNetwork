/**
 *  @file    tcp_socket.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include <libKitsunemimiNetwork/tcp/tcp_socket.h>
#include <libKitsunemimiCommon/threading/cleanup_thread.h>
#include <libKitsunemimiPersistence/logger/logger.h>

namespace Kitsunemimi
{
namespace Network
{

/**
 * @brief constructor for the socket-side of the tcp-connection
 *
 * @param address ipv4-adress of the server
 * @param port port where the server is listen
 */
TcpSocket::TcpSocket(const std::string &address,
                     const uint16_t port)
    : AbstractSocket()
{
    m_address = address;
    m_port = port;
    m_isClientSide = true;
    m_type = TCP_SOCKET;
}

/**
 * @brief init socket on client-side
 *
 * @return true, if successful, else false
 */
bool
TcpSocket::initClientSide()
{
    if(m_isConnected) {
        return true;
    }

    bool result = initSocket();
    if(result == false)
    {
        LOG_ERROR("Faile to initialized tcp-socket client to targe: " + m_address);
        return false;
    }

    m_isConnected = true;
    LOG_INFO("Successfully initialized tcp-socket client to targe: " + m_address);

    return true;
}

/**
 * @brief constructor for the server-side of the tcp-connection, which is called by the
 *        tcp-server for each incoming connection
 *
 * @param socketFd file-descriptor of the socket-socket
 */
TcpSocket::TcpSocket(const int socketFd)
    : AbstractSocket()
{
    m_socket = socketFd;
    m_isClientSide = false;
    m_type = TCP_SOCKET;
    m_isConnected = true;
}

/**
 * @brief init tcp-socket and connect to the server
 *
 * @return false, if socket-creation or connection to the server failed, else true
 */
bool
TcpSocket::initSocket()
{
    struct sockaddr_in address;
    struct hostent* hostInfo;
    unsigned long addr;

    // create socket
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(m_socket < 0)
    {
        LOG_ERROR("Failed to create a tcp-socket");
        return false;
    }

    // set TCP_NODELAY for sure
    int optval = 1;
    if(setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(int)) < 0)
    {
        LOG_ERROR("Failed to initialize tcp-socket for addresse: " + m_address);
        return false;
    }

    // set server-address
    memset(&address, 0, sizeof(address));
    if((addr = inet_addr(m_address.c_str())) != INADDR_NONE)
    {
        memcpy(reinterpret_cast<char*>(&address.sin_addr), &addr, sizeof(addr));
    }
    else
    {
        // get server-connection via host-name instead of ip-address
        hostInfo = gethostbyname(m_address.c_str());
        if(hostInfo == nullptr)
        {
            LOG_ERROR("Failed to get host by address: " + m_address);
            return false;
        }

        memcpy(reinterpret_cast<char*>(&address.sin_addr),
               hostInfo->h_addr,
               static_cast<size_t>(hostInfo->h_length));
    }

    // set other informations
    address.sin_family = AF_INET;
    address.sin_port = htons(m_port);

    // create connection
    if(connect(m_socket, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) < 0)
    {
        LOG_ERROR("Failed to connect tcp-socket to server with addresse: " + m_address);
        return false;
    }

    m_socketAddr = address;

    return true;
}

/**
 * @brief receive data
 *
 * @return number of read bytes
 */
long
TcpSocket::recvData(int socket,
                    void* bufferPosition,
                    const size_t bufferSize,
                    int flags)
{
    return recv(socket, bufferPosition, bufferSize, flags);
}

/**
 * @brief send data
 *
 * @return number of written bytes
 */
ssize_t
TcpSocket::sendData(int socket,
                    const void* bufferPosition,
                    const size_t bufferSize,
                    int flags)
{
    return send(socket, bufferPosition, bufferSize, flags);
}

} // namespace Network
} // namespace Kitsunemimi

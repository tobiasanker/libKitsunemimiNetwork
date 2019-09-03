/**
 *  @file    abstract_client.cpp
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#include "abstract_client.h"

#include <cleanup_thread.h>
#include <network_trigger.h>

namespace Kitsune
{
namespace Network
{

Kitsune::Network::CleanupThread* AbstractClient::m_cleanup = nullptr;

/**
 * @brief AbstractClient::AbstractClient
 */
AbstractClient::AbstractClient()
{
    if(m_cleanup == nullptr)
    {
        m_cleanup = new Kitsune::Network::CleanupThread();
        m_cleanup->start();
    }
}

/**
 * @brief AbstractClient::~AbstractClient
 */
AbstractClient::~AbstractClient()
{
    closeSocket();
    clearNetworkTrigger();
}

/**
 * add new trigger-object for incoming messages
 *
 * @param trigger new trigger-object
 *
 * @return false, if object was nullptr, else true
 */
bool
AbstractClient::addNetworkTrigger(NetworkTrigger *trigger)
{
    if(trigger == nullptr) {
        return false;
    }

    m_trigger.push_back(trigger);

    return true;
}

/**
 * remove a specific trigger from the client
 *
 * @param index index of the trigger in the list
 *
 * @return false, if index too high, else tru
 */
bool
AbstractClient::removeNetworkTrigger(const uint32_t index)
{
    if(m_trigger.size() <= index) {
        return false;
    }

    m_trigger.erase(m_trigger.begin() + index);

    return true;
}

/**
 * delete all trigger-objects from the client
 */
void
AbstractClient::clearNetworkTrigger()
{
    m_trigger.clear();
}


/**
 * send a text-message over the socket
 *
 * @param message message to send
 *
 * @return false, if send failed or send was incomplete, else true
 */
bool
AbstractClient::sendMessage(const std::string &message)
{
    const uint64_t messageLength = message.length();
    return sendMessage((uint8_t*)message.c_str(), messageLength);
}

/**
 * send a byte-buffer over the tcp-socket
 *
 * @param message byte-buffer to send
 * @param numberOfBytes number of bytes to send
 *
 * @return false, if send failed or send was incomplete, else true
 */
bool
AbstractClient::sendMessage(const uint8_t* message,
                            const uint64_t numberOfBytes)
{
    // precheck if client is connected
    if(m_clientSocket == 0) {
        return false;
    }

    // send message
    const ssize_t successfulSended = send(m_clientSocket,
                                          message,
                                          numberOfBytes,
                                          MSG_NOSIGNAL);

    // check if the message was completely send
    if(successfulSended < -1
            || successfulSended != static_cast<long>(numberOfBytes))
    {
        return false;
    }
    return true;
}

/**
 * wait for new incoming messages
 *
 * @return false, if receive failed or client is aborted, else true
 */
bool
AbstractClient::waitForMessage()
{
    // precheck
    if(m_abort) {
        return false;
    }

    // calulate buffer-part for recv message
    uint64_t writePosition = (m_recvBuffer.readPosition + m_recvBuffer.readWriteDiff)
                             % m_recvBuffer.totalBufferSize;
    if(m_recvBuffer.totalBufferSize == writePosition) {
        writePosition = 0;
    }

    uint64_t spaceToEnd = m_recvBuffer.totalBufferSize - writePosition;
    if(writePosition < m_recvBuffer.readPosition) {
        spaceToEnd = m_recvBuffer.readPosition - writePosition;
    }

    // wait for incoming message
    long recvSize = recv(m_clientSocket,
                         &m_recvBuffer.data[writePosition],
                         spaceToEnd,
                         0);

    // handle error-cases
    if(recvSize <= 0
            || m_abort)
    {
        return false;
    }

    // increase the
    m_recvBuffer.readWriteDiff = (m_recvBuffer.readWriteDiff + static_cast<uint64_t>(recvSize));

    // add all trigger to the new socket
    for(uint64_t i = 0; i < m_trigger.size(); i++)
    {
        const uint64_t readBytes = m_trigger[i]->runTask(m_recvBuffer, this);

        m_recvBuffer.readPosition = (m_recvBuffer.readPosition + readBytes)
                                    % m_recvBuffer.totalBufferSize;
        m_recvBuffer.readWriteDiff -= readBytes;
    }

    return true;
}

/**
 * close the socket and schedule the deletion of the thread
 *
 * @return false, if already closed, else true
 */
bool
AbstractClient::closeSocket()
{
    if(m_abort == true) {
        return false;
    }

    m_abort = true;

    // close socket if connected
    if(m_clientSocket >= 0)
    {
        shutdown(m_clientSocket, SHUT_RDWR);
        close(m_clientSocket);
        m_clientSocket = 0;
    }

    // add socket-thread to the cleanup-thread, because if the client triggers the close by itself,
    // when the other side of the connection triggers the close-process,
    // the thread would try to close itself, which would result into a deadlock.
    // That is the reason, why another thread sould process the delete of the client-thread.
    AbstractClient::m_cleanup->addClientForCleanup(this);

    return true;
}

/**
 * run-method for the thread-class
 */
void
AbstractClient::run()
{
    while(!m_abort)
    {
        waitForMessage();
    }
}

} // namespace Network
} // namespace Kitsune

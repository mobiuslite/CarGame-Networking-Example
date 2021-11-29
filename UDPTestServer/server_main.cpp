#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include "ClientInfo.h"
#include <iostream>

#include <conio.h>
#include <chrono>

#include <BufferLibrary/cBuffer.h>

#include <gen/CarState.pb.h>

#define PORT 27015

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

std::vector<ClientInfo*> ClientArray;
cBuffer buffer = cBuffer(512);

int main()
{

    int iResult = 0;

    WSADATA wsaData;

    SOCKET listenSocket = INVALID_SOCKET;
    SOCKET sendSocket = INVALID_SOCKET;

    struct sockaddr_in RecvAddr;

    char RecvBuf[512];
    int BufLen = 512;

    struct sockaddr_in SenderAddr;
    int SenderAddrSize = sizeof(SenderAddr);

    //-----------------------------------------------
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
    {
        wprintf(L"WSAStartup failed with error %d\n", iResult);
        return 1;
    }
    //-----------------------------------------------
    // Create a receiver socket to receive datagrams
    listenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (listenSocket == INVALID_SOCKET)
    {
        wprintf(L"socket failed with error %d\n", WSAGetLastError());
        return 1;
    }

    sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sendSocket == INVALID_SOCKET)
    {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    //-----------------------------------------------
    // Bind the socket to any address and the specified port.
    RecvAddr.sin_family = AF_INET;
    RecvAddr.sin_port = htons(PORT);
    RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    iResult = bind(listenSocket, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
    if (iResult != 0)
    {
        wprintf(L"bind failed with error %d\n", WSAGetLastError());
        return 1;
    }

    // Change the socket mode on the listening socket from blocking to
    // non-blocking so the application will not block waiting for requests
    DWORD NonBlock = 1;
    iResult = ioctlsocket(listenSocket, FIONBIO, &NonBlock);
    if (iResult == SOCKET_ERROR)
    {
        printf("ioctlsocket() failed with error %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    printf("ioctlsocket() was successful!\n");

    //-----------------------------------------------
    // Call the recvfrom function to receive datagrams
    // on the bound socket.
    wprintf(L"Entering recv loop!\n");

    FD_SET ReadSet;
    int total;

    std::chrono::system_clock::time_point current_time;
    std::chrono::system_clock::time_point previous_update_time;

    current_time = std::chrono::system_clock::now();
    previous_update_time = current_time;

    //60hz (roughly)
    std::chrono::milliseconds tickTime = std::chrono::milliseconds(16);

    while (true)
    {
        if (_kbhit())
        {
            if (_getch() == 27)
            {
                break;
            }
        }

        current_time = std::chrono::system_clock::now();

        //Tick has happened, sent stuff to all clients!
        if (current_time >= previous_update_time + tickTime)
        {
            previous_update_time = current_time;
            
            for (ClientInfo* client : ClientArray)
            {
                sockaddr_in clientAddress;

                clientAddress.sin_family = AF_INET;
                clientAddress.sin_port = client->port;
                clientAddress.sin_addr.s_addr = inet_addr(client->ip.c_str());

                for (ClientInfo* clientToSend : ClientArray)
                {
                    //Don't send the current client their own info (cause why would you do that????)
                    if (client->username != clientToSend->username)
                    {
                         //   Send!
                        buffer.ClearBuffer();

                        std::string messageToSend = clientToSend->carState;

                        buffer.WriteShort((short)messageToSend.size());
                        buffer.WriteString(messageToSend);
                        std::string bufferMsg = buffer.GetBufferMessage();

                        int BufLen = (int)bufferMsg.size();

                        //wprintf(L"Sending a datagram to the receiver...\n");
                        int result = sendto(sendSocket,
                            bufferMsg.c_str(), BufLen, 0, (SOCKADDR*)&clientAddress, sizeof(clientAddress));
                        if (result == SOCKET_ERROR)
                        {
                            wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
                            closesocket(listenSocket);
                            WSACleanup();
                        }
                    }
                }
            }

        }


        timeval tv = { 0 };
        tv.tv_sec = 0;
        // Initialize our read set
        FD_ZERO(&ReadSet);

        // Always look for send packets
        FD_SET(listenSocket, &ReadSet);

        // Call our select function to find the sockets that
        // require our attention
        //printf("Attempting receive...\n");
        total = select(0, &ReadSet, NULL, NULL, &tv);
        if (total == SOCKET_ERROR)
        {
            printf("select() failed with error: %d\n", WSAGetLastError());

            //Leaves loop
            break;
        }

        //If we have received something
        if (total > 0)
        {
            total--;

            iResult = recvfrom(listenSocket,
                RecvBuf, BufLen, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
            if (iResult == SOCKET_ERROR)
            {
                int lastError = WSAGetLastError();

                if (lastError != WSAEWOULDBLOCK)
                {
                    wprintf(L"recvfrom failed with error %d\n", lastError);
                }
            }

            //Received successfully!
            if (iResult > 0)
            {
                //Construct IP string
                std::string senderIp;
                senderIp.append(std::to_string((int)SenderAddr.sin_addr.S_un.S_un_b.s_b1) + ".");
                senderIp.append(std::to_string((int)SenderAddr.sin_addr.S_un.S_un_b.s_b2) + ".");
                senderIp.append(std::to_string((int)SenderAddr.sin_addr.S_un.S_un_b.s_b3) + ".");
                senderIp.append(std::to_string((int)SenderAddr.sin_addr.S_un.S_un_b.s_b4));

                ClientFinder finder = ClientFinder(senderIp, SenderAddr.sin_port);
                std::vector<ClientInfo*>::iterator foundClientIt = std::find_if(ClientArray.begin(), ClientArray.end(), finder);

                ClientInfo* currentClient = nullptr;

                //Didn't find the client
                if (foundClientIt == ClientArray.end())
                {
                    //Creates new client
                    ClientInfo* newClient = new ClientInfo();
                    newClient->ip = senderIp;
                    newClient->port = SenderAddr.sin_port;

                    ClientArray.push_back(newClient);

                    //Sets the current client to the most recent client
                    currentClient = ClientArray[ClientArray.size() - 1];
                }
                //Found the client!
                else
                {
                    currentClient = *foundClientIt;
                }

                std::string recvString;
                for (size_t i = 0; i < BufLen; i++)
                {
                    recvString.push_back(RecvBuf[i]);
                }

                buffer.ClearBuffer();
                buffer.SetBufferMessage(recvString);

                short stringLength = buffer.ReadShort();
                std::string message = buffer.ReadString(stringLength);

                bufferProtos::CarState carState;
                bool success = carState.ParseFromString(message);

                if (success)
                {
                    currentClient->carState = message;
                    currentClient->username = carState.username();
                }

            }
        }
    }

    printf("Shutting down server!...\n\n");
    //-----------------------------------------------
    // Close the socket when finished receiving datagrams
    wprintf(L"Closing socket\n");
    iResult = closesocket(listenSocket);
    if (iResult == SOCKET_ERROR)
    {
        wprintf(L"closesocket failed with error %d\n", WSAGetLastError());
        return 1;
    }

    iResult = closesocket(sendSocket);
    if (iResult == SOCKET_ERROR)
    {
        wprintf(L"closesocket failed with error %d\n", WSAGetLastError());
        return 1;
    }

    //-----------------------------------------------
    // Clean up and exit.
    wprintf(L"Cleaned up WSA.\n");
    WSACleanup();

    //Deletes all client info
    for (ClientInfo* info : ClientArray)
    {
        delete info;
    }

    printf("Deleted all client info..\n\n");
    printf("Goodnight!");

    return 0;
}
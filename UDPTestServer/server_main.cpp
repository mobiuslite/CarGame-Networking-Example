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

#define PORT 27015

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

std::vector<ClientInfo*> ClientArray;
cBuffer buffer = cBuffer(512);

bool gameStarted = false;

void UpdateScreen();

int main()
{

    int iResult = 0;

    WSADATA wsaData;

    SOCKET listenSocket = INVALID_SOCKET;
    //SOCKET sendSocket = INVALID_SOCKET;

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

    /*sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sendSocket == INVALID_SOCKET)
    {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }*/
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

    //24hz (roughly)
    std::chrono::milliseconds tickTime = std::chrono::milliseconds(41);

    while (true)
    {
        if (_kbhit())
        {
            char charHit = _getch();
            if (charHit == 27)
            {
                break;
            }
            else if (charHit == '1')
            {
                tickTime = std::chrono::milliseconds(1000);
                printf("Set server to 1hz\n");
            }
            else if (charHit == '2')
            {
                tickTime = std::chrono::milliseconds(100);
                printf("Set server to 10hz\n");
            }
            else if (charHit == '3')
            {
                tickTime = std::chrono::milliseconds(10);
                printf("Set server to 100hz\n");
            }
            else if (charHit == '4')
            {
                tickTime = std::chrono::milliseconds(16);
                printf("Set server to 60hz\n");
            }
            else if (charHit == '5')
            {
                tickTime = std::chrono::milliseconds(41);
                printf("Set server to 24hz\n");
            }
        }

        current_time = std::chrono::system_clock::now();

        float deltaTime = (std::chrono::duration_cast<std::chrono::milliseconds>(current_time - previous_update_time).count()) / 1000.0f;

        //Erases client after they don't contact the server for a certain amount of time
        for (std::vector<ClientInfo*>::iterator client = ClientArray.begin(); client != ClientArray.end(); client++)
        {
            if ((*client)->carState.username() == "")
            {
                (*client)->timeoutElapsed += deltaTime;
                if ((*client)->timeoutElapsed >= (*client)->timeoutTime)
                {
                    ClientInfo* clientToDelete = *client;
                    ClientArray.erase(client);
                    delete clientToDelete;

                    UpdateScreen();
                    break;
                }
            }
        }

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

                bufferProtos::CarStateArray carArray;

                for (ClientInfo* clientToSend : ClientArray)
                {
                    //Don't send the current client their own info (cause why would you do that????)
                    if (client->username != clientToSend->username && clientToSend->carState.username() != "")
                    {
                        //Adds car to car array
                        bufferProtos::CarStateArray_CarState* addedCar = carArray.add_cararray();
                        *addedCar = clientToSend->carState;
                    }
                }
                //   Send!
                buffer.ClearBuffer();

                //Serialize car array
                std::string messageToSend;
                carArray.SerializeToString(&messageToSend);

                buffer.WriteShort((short)0);
                buffer.WriteShort((short)messageToSend.size());
                buffer.WriteString(messageToSend);
                std::string bufferMsg = buffer.GetBufferMessage();

                int BufLen = (int)bufferMsg.size();

                //Send car array
                int result = sendto(listenSocket,
                    bufferMsg.c_str(), BufLen, 0, (SOCKADDR*)&clientAddress, sizeof(clientAddress));
                if (result == SOCKET_ERROR)
                {
                    wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
                    closesocket(listenSocket);
                    WSACleanup();
                }
            }

            //Sets the car states username to empty
            //Unless the client sends something again, it will not be sent to the client
            //Without this, the server would constantly send a car states position even if they were dissconnected.
            std::for_each(ClientArray.begin(), ClientArray.end(), [](ClientInfo* info) { info->carState.set_username(""); });
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
                //Don't let new players join while game is running.
                if (foundClientIt == ClientArray.end() && !gameStarted)
                {
                    //Creates new client

                    printf("Added new client!\n");

                    ClientInfo* newClient = new ClientInfo();
                    newClient->ip = senderIp;
                    newClient->port = SenderAddr.sin_port;

                    ClientArray.push_back(newClient);

                    //Sets the current client to the most recent client
                    currentClient = ClientArray[ClientArray.size() - 1];

                    UpdateScreen();
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

                short messageType = buffer.ReadShort();

                if (messageType == 0)
                {
                    short stringLength = buffer.ReadShort();
                    std::string message = buffer.ReadString(stringLength);

                    bufferProtos::CarStateArray_CarState carState;
                    bool success = carState.ParseFromString(message);

                    if (success)
                    {
                        currentClient->carState = carState;
                        currentClient->timeoutElapsed = 0.0f;
                        if (currentClient->username == "")
                        {
                            currentClient->username = carState.username();
                            UpdateScreen();
                        }
                    }
                }
                else if (messageType == 1)
                {
                    currentClient->isReady = !currentClient->isReady;
                    UpdateScreen();
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

    /*iResult = closesocket(sendSocket);
    if (iResult == SOCKET_ERROR)
    {
        wprintf(L"closesocket failed with error %d\n", WSAGetLastError());
        return 1;
    }*/

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

void UpdateScreen()
{
    system("cls");

    std::cout << "Number of clients: " << ClientArray.size() << std::endl << std::endl;

    for (ClientInfo* client : ClientArray)
    {
        std::cout << client->username << ": " << ((client->isReady) ? "Ready!" : "Not Ready") << std::endl;
    }
}
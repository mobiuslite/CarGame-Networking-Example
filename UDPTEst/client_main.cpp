#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

// Link with ws2_32.lib
#include <iostream>

#include <BufferLibrary/cBuffer.h>
#include <gen/CarState.pb.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 27015

cBuffer buffer = cBuffer(512);



int main()
{

    int iResult;
    WSADATA wsaData;

    SOCKET SendSocket = INVALID_SOCKET;
    sockaddr_in RecvAddr;

    //----------------------
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
    {
        wprintf(L"WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    //---------------------------------------------
    // Create a socket for sending data
    SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (SendSocket == INVALID_SOCKET)
    {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    //Set non blocking!
    DWORD NonBlock = 1;
    int result = ioctlsocket(SendSocket, FIONBIO, &NonBlock);

    if (result == SOCKET_ERROR)
    {
        printf("ioctlsocket() failed with error %d\n", WSAGetLastError());
        closesocket(SendSocket);
        WSACleanup();
        return 1;
    }

    //---------------------------------------------
    RecvAddr.sin_family = AF_INET;

    //Port and Address
    RecvAddr.sin_port = htons(PORT);
    RecvAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //---------------------------------------------
    // Send a datagram to the receiver

    char dummy = 0;
    while (dummy != 'e')
    {
        bufferProtos::CarState carState;

        bufferProtos::CarState_Vector2 position;
        position.set_x(10.0f);
        position.set_z(-12.7f);
        position.set_z(-12.7f);
        bufferProtos::CarState_Vector2 velocity;
        velocity.set_x(0.0f);
        velocity.set_z(1.0f);

        float test = 2.000001f;

        carState.set_allocated_position(&position);
        carState.set_allocated_velocity(&velocity);
        carState.set_yradiansrotation(test);
        carState.set_username("Ethan");

        std::string messageToSend;
        carState.SerializeToString(&messageToSend);

        buffer.WriteShort((short)messageToSend.size());
        buffer.WriteString(messageToSend);
        std::string bufferMsg = buffer.GetBufferMessage();

        int BufLen = (int)bufferMsg.size();

        wprintf(L"Sending a datagram to the receiver...\n");
        iResult = sendto(SendSocket,
            bufferMsg.c_str(), BufLen, 0, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
        if (iResult == SOCKET_ERROR)
        {
            wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
            closesocket(SendSocket);
            WSACleanup();
            return 1;
        }

        std::cin >> dummy;
    }
    //---------------------------------------------
    // When the application is finished sending, close the socket.
    wprintf(L"Finished sending. Closing socket.\n");
    iResult = closesocket(SendSocket);
    if (iResult == SOCKET_ERROR)
    {
        wprintf(L"closesocket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    //---------------------------------------------
    // Clean up and quit.
    wprintf(L"Exiting.\n");
    WSACleanup();

    printf("Press any key to exit!");
    std::cin >> dummy;

    return 0;
}
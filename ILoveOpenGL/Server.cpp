
#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "Ws2_32.lib")

#include <stdio.h>
#include <winsock2.h>

#include "Server.h"
#include <gen/CarStateArray.pb.h>
cServer::cServer(std::string ip, short port)
{
    int iResult;
    WSADATA wsaData;
    //----------------------
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
    {
        wprintf(L"WSAStartup failed with error: %d\n", iResult);
        wsaStarted = false;
        return;
    }

    //---------------------------------------------
    // Create a socket for sending data
    this->recvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (this->recvSocket == INVALID_SOCKET)
    {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        wsaStarted = false;
        return ;
    }
    sockaddr_in RecvAddr;
    RecvAddr.sin_family = AF_INET;
    RecvAddr.sin_port = htons(0);
    //RecvAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    iResult = bind(this->recvSocket, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
    if (iResult != 0)
    {
        wprintf(L"bind failed with error %d\n", WSAGetLastError());
        this->wsaStarted = false;
        return;
    }

    int length = 100;
    sockaddr_in test;
    getsockname(this->recvSocket, (SOCKADDR*)&test, &length);

    /*this->sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (this->sendSocket == INVALID_SOCKET)
    {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        wsaStarted = false;
        return;
    }*/

    //Set non blocking!
    DWORD NonBlock = 1;
    int result = ioctlsocket(this->recvSocket, FIONBIO, &NonBlock);

    if (result == SOCKET_ERROR)
    {
        printf("ioctlsocket() failed with error %d\n", WSAGetLastError());
        closesocket(this->recvSocket);
        WSACleanup();
        wsaStarted = false;
        return ;
    }

    printf("WSA startup complete!\n");

    serverAddress.sin_family = AF_INET;

    //Port and Address
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());

    this->wsaStarted = true;
}
cServer::~cServer()
{
    for (std::map<std::string, cNetworkCar*>::iterator deleteCarIt = this->networkCars.begin(); deleteCarIt != this->networkCars.end(); deleteCarIt++)
    {
        delete this->networkCars[(*deleteCarIt).first];
    }

    this->networkCars.clear();
}

void cServer::RemoveNetworkCar(cNetworkCar* car)
{
    for (std::map<std::string, cNetworkCar*>::iterator carMapIt = this->networkCars.begin(); carMapIt != this->networkCars.end();)
    {
        if ((*carMapIt).second == car)
        {
            this->networkCars.erase(carMapIt);
            delete car->Mesh();
            delete car;
            break;
        }
        else
        {
            ++carMapIt;
        }
    }
}

bool cServer::SendCarState(std::string username, glm::vec3 pos, glm::vec3 velo, float yRotationRadians)
{
    if (this->wsaStarted)
    {
        buffer.ClearBuffer();

        bufferProtos::CarStateArray_CarState carState;

        bufferProtos::CarStateArray_CarState_Vector2* position = new bufferProtos::CarStateArray_CarState_Vector2();
        position->set_x(pos.x);
        position->set_z(pos.z);
        bufferProtos::CarStateArray_CarState_Vector2* velocity = new bufferProtos::CarStateArray_CarState_Vector2();
        velocity->set_x(velo.x);
        velocity->set_z(velo.z);

        carState.set_allocated_position(position);
        carState.set_allocated_velocity(velocity);
        carState.set_yradiansrotation(yRotationRadians);
        carState.set_username(username);

        std::string messageToSend;
        carState.SerializeToString(&messageToSend);

        buffer.WriteShort((short)0);
        buffer.WriteShort((short)messageToSend.size());
        buffer.WriteString(messageToSend);
        std::string bufferMsg = buffer.GetBufferMessage();

        int BufLen = (int)bufferMsg.size();

        //wprintf(L"Sending a datagram to the receiver...\n");
        int result = sendto(this->recvSocket,
            bufferMsg.c_str(), BufLen, 0, (SOCKADDR*)&this->serverAddress, sizeof(this->serverAddress));
        if (result == SOCKET_ERROR)
        {
            wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
            closesocket(this->recvSocket);
            WSACleanup();
            this->wsaStarted = false;
            return false;
        }

        carState.release_position();
        carState.release_velocity();
    }
    else
    {
        //WSA didn't work, so just say nothing went wrong everytime you try to tell the server something
        return true;
    }
}

bool cServer::SendReady()
{
    if (this->wsaStarted)
    {
        buffer.ClearBuffer();
        buffer.WriteShort(1);

        std::string bufferMsg = buffer.GetBufferMessage();

        int BufLen = (int)bufferMsg.size();

        //wprintf(L"Sending a datagram to the receiver...\n");
        int result = sendto(this->recvSocket,
            bufferMsg.c_str(), BufLen, 0, (SOCKADDR*)&this->serverAddress, sizeof(this->serverAddress));
        if (result == SOCKET_ERROR)
        {
            wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
            closesocket(this->recvSocket);
            WSACleanup();
            this->wsaStarted = false;
            return false;
        }
    }
    else
    {
        //WSA didn't work, so just say nothing went wrong everytime you try to tell the server something
        return true;
    }
}

void cServer::CheckReceive()
{
    FD_SET ReadSet;
    timeval tv = { 0 };
    tv.tv_sec = 0;
    // Initialize our read set
    FD_ZERO(&ReadSet);

    // Always look for send packets
    FD_SET(this->recvSocket, &ReadSet);

    // Call our select function to find the sockets that
    // require our attention
    //printf("Attempting receive...\n");
    int total = select(0, &ReadSet, NULL, NULL, &tv);
    if (total == SOCKET_ERROR)
    {
        printf("select() failed with error: %d\n", WSAGetLastError());

        return;
    }

    if (total > 0)
    {

        const int bufSize = 512;
        char RecvBuf[bufSize];

        int result = recvfrom(this->recvSocket,
            RecvBuf, bufSize, 0, NULL, NULL);
        if (result == SOCKET_ERROR)
        {
            int lastError = WSAGetLastError();

            if (lastError != WSAEWOULDBLOCK)
            {
                wprintf(L"recvfrom failed with error %d\n", lastError);
            }
        }

        //Received successfully!
        if (result > 0)
        {
            std::string recvString;
            for (size_t i = 0; i < bufSize; i++)
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

                bufferProtos::CarStateArray carStateArray;
                bool success = carStateArray.ParseFromString(message);

                if (success)
                {
                    for (size_t i = 0; i < carStateArray.cararray_size(); i++)
                    {

                        bufferProtos::CarStateArray_CarState netCar = carStateArray.cararray(i);
                        std::string netCarName = netCar.username();

                        if (this->networkCars.count(netCarName))
                        {
                            //Car exists
                            //Update old car
                            cNetworkCar* updateCar = this->networkCars[netCarName];

                            glm::vec2 pos = glm::vec2(netCar.position().x(), netCar.position().z());
                            glm::vec2 velo = glm::vec2(netCar.velocity().x(), netCar.velocity().z());
                            updateCar->SetNetworkCar(pos, velo, netCar.yradiansrotation());

                        }
                        else
                        {
                            //Car doesn't exist
                            //Make new Mesh
                            cMesh* carMesh = new cMesh();
                            carMesh->meshName = "car.ply";
                            carMesh->friendlyName = "NetworkedCar";

                            cNetworkCar* newCar = new cNetworkCar(carMesh);
                            glm::vec2 pos = glm::vec2(netCar.position().x(), netCar.position().z());
                            glm::vec2 velo = glm::vec2(netCar.velocity().x(), netCar.velocity().z());
                            newCar->SetNetworkCar(pos, velo, netCar.yradiansrotation());

                            //Adds mesh and car to game
                            this->networkCars.insert(std::pair<std::string, cNetworkCar*>(netCarName, newCar));
                        }
                    }
                }
            }
        }
    }
}
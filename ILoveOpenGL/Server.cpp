
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
    float checkpointRadius = 4.5f;

    //Adds checkpoints to the list of checkpoints
    this->checkpoints.push_back(new cCheckpointTriggerGenerator(glm::vec3(-30.0f, 0.0f, -40.0f), checkpointRadius, false));
    this->checkpoints.push_back(new cCheckpointTriggerGenerator(glm::vec3(-61.0f, 0.0f, -12.0f), checkpointRadius, false));
    this->checkpoints.push_back(new cCheckpointTriggerGenerator(glm::vec3(-30.0f, 0.0f, 8.5f), checkpointRadius, false));
    this->checkpoints.push_back(new cCheckpointTriggerGenerator(glm::vec3(-55.0f, 0.0f, 42.f), checkpointRadius, false));
    this->checkpoints.push_back(new cCheckpointTriggerGenerator(glm::vec3(23.0f, 0.0f, 26.5f), checkpointRadius, false));
    this->checkpoints.push_back(new cCheckpointTriggerGenerator(glm::vec3(58.0f, 0.0f, 36.f), checkpointRadius, false));
    this->checkpoints.push_back(new cCheckpointTriggerGenerator(glm::vec3(45.0f, 0.0f, 5.5f), checkpointRadius, false));
    this->checkpoints.push_back(new cCheckpointTriggerGenerator(glm::vec3(53.0f, 0.0f, -49.f), checkpointRadius, false));
    this->checkpoints.push_back(new cCheckpointTriggerGenerator(glm::vec3(24.0f, 0.0f, -51.f), checkpointRadius, false));
    this->checkpoints.push_back(new cCheckpointTriggerGenerator(glm::vec3(22.0f, 0.0f, -41.7f), checkpointRadius, false));
    this->checkpoints.push_back(new cCheckpointTriggerGenerator(glm::vec3(0.f, 0.0f, -40.0f), checkpointRadius, false));

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

    int length = 100;
    sockaddr_in test;
    getsockname(this->recvSocket, (SOCKADDR*)&test, &length);

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

    this->connectAttemptElapsed = 0.0f;
    this->wsaStarted = true;

    SendConnectionAttempt();
}
cServer::~cServer()
{
    for (cCheckpointTriggerGenerator* checkpoint : this->checkpoints)
    {
        delete checkpoint;
    }
    checkpoints.clear();

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
bool cServer::SendDoneLap()
{
    if (this->wsaStarted)
    {
        buffer.ClearBuffer();
        buffer.WriteShort(2);

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

void cServer::DoneLap()
{
    //this->checkpoints[0]->isActive = false;
    std::cout << "Lap Time: " << lapTime << std::endl;

    this->lapTime = 0.0f;
    
    this->gameStarted = false;
    this->ready = false;

    SendDoneLap();
}

void cServer::CheckReceive(float deltaTime)
{
    if(gameStarted)
        lapTime += deltaTime;

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
                            carMesh->positionXYZ.x = netCar.position().x();
                            carMesh->positionXYZ.z = netCar.position().z();
                            carMesh->orientationXYZ.y = netCar.yradiansrotation();

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

            //Start round
            else if (messageType == 1)
            {
                if (!gameStarted)
                {
                    gameStarted = true;
                    this->checkpoints[0]->isActive = true;
                }
            }
            //Ready was received
            else if (messageType == 10)
            {
                ready = !ready;
            }
            //Server online check
            else if (messageType == 100)
            {
                connected = true;
                std::cout << "Server is online!" << std::endl;
            }
            //Server shutdown
            else if (messageType == -1)
            {
                for (std::map<std::string, cNetworkCar*>::iterator networkIt = this->networkCars.begin(); networkIt != this->networkCars.end(); networkIt++)
                {
                    delete this->networkCars[(*networkIt).first]->Mesh();
                    delete this->networkCars[(*networkIt).first];
                }

                this->networkCars.clear();
                this->connected = false;

                std::cout << "Server has shutdown!" << std::endl;
            }
        }
    }

    if (!this->connected)
    {
        this->connectAttemptElapsed += deltaTime;
        if (this->connectAttemptElapsed >= this->connectAttemptTime)
        {
            this->connectAttemptElapsed = 0.0f;
            SendConnectionAttempt();
        }
    }
}

void cServer::SendConnectionAttempt()
{
    if (this->wsaStarted)
    {
        buffer.ClearBuffer();
        buffer.WriteShort(100);

        std::string bufferMsg = buffer.GetBufferMessage();

        int BufLen = (int)bufferMsg.size();

        printf("Attemping connection to server...\n");
        int result = sendto(this->recvSocket,
            bufferMsg.c_str(), BufLen, 0, (SOCKADDR*)&this->serverAddress, sizeof(this->serverAddress));
        if (result == SOCKET_ERROR)
        {
            printf("Server offline!\n");
            closesocket(this->recvSocket);
            WSACleanup();
            this->wsaStarted = false;
            //return false;
        }
    }
}

bool cServer::isConnected()
{
    return connected;
}
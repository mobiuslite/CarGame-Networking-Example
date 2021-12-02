#pragma once

#include <string>
#include <map>
#include <BufferLibrary/cBuffer.h>
#include <Ws2tcpip.h>

#include <glm/vec3.hpp>

#include "NetworkCar.h"

class cServer
{
public:
	cServer() = delete;
	cServer(std::string ip, short port);

	~cServer();

	bool SendCarState(std::string username, glm::vec3 pos, glm::vec3 velo, float yRotationRadians);
	bool SendReady();

	void CheckReceive(float deltaTime);

	void SendConnectionAttempt();
	bool isConnected();

	void RemoveNetworkCar(cNetworkCar* car);

	std::map<std::string, cNetworkCar*> networkCars;

private:
	bool wsaStarted;
	bool connected = false;

	sockaddr_in serverAddress;
	//SOCKET sendSocket;
	SOCKET recvSocket;

	cBuffer buffer = cBuffer(512);

	float connectAttemptTime = 10.0f;
	float connectAttemptElapsed;
};
#pragma once
// Client structure
#include <WinSock2.h>
#include <string>
#include <vector>
#include "Car.h"

#include <gen/CarStateArray.pb.h>

struct ClientInfo
{
	std::string ip;
	unsigned short port;

	std::string username;

	bool isReady = false;

	float timeoutTime = 4000.0f;
	float timeoutElapsed = 0.0f;

	bufferProtos::CarStateArray_CarState carState;
};

struct ClientFinder
{
	ClientFinder(std::string ip, unsigned short port)
	{
		this->ip = ip;
		this->port = port;
	}

	std::string ip;
	unsigned short port;

	bool operator() (const ClientInfo* other)
	{
		bool ipMatch = this->ip == other->ip;
		bool portMatch = this->port == other->port;
		return (portMatch && ipMatch);
	}
};
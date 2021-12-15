#pragma once
// Client structure
#include <WinSock2.h>
#include <string>
#include <vector>
#include "Car.h"
#include <chrono>

#include <gen/CarStateArray.pb.h>

struct ClientInfo
{
	std::string ip;
	unsigned short port;

	std::string username;

	bool isReady = false;
	bool timingOut = false;

	float timeoutTime = 4000.0f;
	float timeoutElapsed = 0.0f;

	int score = 0;

	std::chrono::system_clock::time_point startTime;
	std::chrono::system_clock::time_point endTime;

	bufferProtos::CarStateArray_CarState carState;
	std::vector<bufferProtos::CarStateArray_CarState> recordingData;
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
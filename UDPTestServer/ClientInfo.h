#pragma once
// Client structure
#include <WinSock2.h>
#include <string>
#include <vector>
#include "Car.h"

struct ClientInfo
{
	std::string ip;
	unsigned short port;

	std::string username;

	std::string carState;
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
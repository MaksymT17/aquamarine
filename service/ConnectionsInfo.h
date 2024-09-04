#pragma once
#include <memory>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cstring>
#include "sh_mem/Message.hpp"

class ConnectionsInfo
{
	enum State : size_t
	{
		UNKNOWN = 0,
		READY,
		CONFIGURED
	};
	struct ClientInfo
	{
		size_t id;
		State state;
		Configuration configuration;
	};

public:
	ConnectionsInfo() = default;

	bool isRequestValid(const Message *message);
	std::vector<ClientInfo>::const_iterator processActionUpdate(const Message *msg);

private:
	std::vector<ClientInfo> clients;
};
#pragma once

#include <memory>
#include <iostream>
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>

class NetworkingController
{
public:
	NetworkingController();
	~NetworkingController();

	void addConnection(std::string uri);
	void broadcastData(const std::vector<uint8_t> data);

private:
	std::vector<std::shared_ptr<ix::WebSocket>> m_connections;
};
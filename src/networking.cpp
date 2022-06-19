#include "networking.h"

#include <memory>

NetworkingController::NetworkingController()
{
    ix::initNetSystem();
}

NetworkingController::~NetworkingController()
{
    for (auto& connection : m_connections)
        connection->close();
}

void NetworkingController::addConnection(std::string uri)
{
    std::cout << "Connecting to " << uri << "..." << std::endl;

    auto ws = std::make_shared<ix::WebSocket>();
    ws->setUrl(uri);
    ws->setOnMessageCallback([=](const ix::WebSocketMessagePtr& msg)
        {
            if (msg->type == ix::WebSocketMessageType::Message)
            {
            }
            else if (msg->type == ix::WebSocketMessageType::Open)
            {
                std::cout << "Connection established with " << uri << std::endl;
            }
            else if (msg->type == ix::WebSocketMessageType::Error)
            {
                std::cout << "Connection error: " << msg->errorInfo.reason << std::endl;
            }
        }
    );
    ws->start();

    m_connections.push_back(ws);
}

void NetworkingController::broadcastData(const std::vector<uint8_t> data)
{
    std::string string_data(data.begin(), data.end());

    for (auto& connection : m_connections)
    {
        connection->sendBinary(string_data);
    }
}
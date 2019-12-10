#include "loginmessagehandler.h"

LoginMessageHandler::LoginMessageHandler(NetManager* netManager, World* world) : netManager(netManager), world(world) { }

void LoginMessageHandler::handle(NetMessage& message) {
    LoginMessage msg = dynamic_cast<LoginMessage&>(message);
    Player player {0.0f, 0.0f, 0.0f};
    if (this->world->players.count(msg.senderId) == 0) {
        this->world->players[msg.senderId] = player;
        std::cout << "> player '" << message.senderId << "' logged in" << std::endl;
        
        auto reply = std::make_shared<LoginMessage>();
        reply->receiverId = msg.senderId;
        reply->senderId = msg.senderId;
        this->netManager->queueOut.push(reply);
    } else {
        std::cout << "> player '" << message.senderId << "' already logged in" << std::endl; 
    }
}

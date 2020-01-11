#include "loginmessagehandler.h"

LoginMessageHandler::LoginMessageHandler(NetManager* netManager, World* world) : netManager(netManager), world(world) { }

void LoginMessageHandler::handle(NetMessage& message) {
    LoginMessage msg = dynamic_cast<LoginMessage&>(message);
    Player player;
    player.position = { rand() % (int)TILE_SIZE - TILE_SIZE/2.0f, 0.4f, rand() % (int)TILE_SIZE - TILE_SIZE/2.0f };
    if (this->world->players.count(msg.senderId) == 0) {
        this->world->players[msg.senderId] = player;
        std::cout << "> player '" << message.senderId << "' logged in" << std::endl;
        
        auto reply = std::make_shared<InitMessage>();
        reply->receiverId = msg.senderId;
        reply->senderId = msg.senderId;
        this->netManager->queueOut.push(reply);
    } else {
        std::cout << "> player '" << message.senderId << "' already logged in" << std::endl; 
    }
}

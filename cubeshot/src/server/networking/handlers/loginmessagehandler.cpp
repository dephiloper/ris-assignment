#include "loginmessagehandler.h"

LoginMessageHandler::LoginMessageHandler(NetManager* netManager, World* world) : netManager(netManager), world(world) { }

void LoginMessageHandler::handle(const NetMessage& message) {
    LoginMessage msg = dynamic_cast<const LoginMessage&>(message);
    Player player;
    player.respawn = true;
    // initialize player at random position in the range of the tile size away from the center of the map
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

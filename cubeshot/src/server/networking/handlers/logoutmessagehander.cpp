#include "logoutmessagehandler.h"

LogoutMessageHandler::LogoutMessageHandler(World* world) : world(world) { }

void LogoutMessageHandler::handle(const NetMessage& message) {
    LogoutMessage msg = dynamic_cast<const LogoutMessage&>(message); 
    this->world->players.erase(msg.senderId);
    std::cout << "> player '" << message.senderId << "' logged out" << std::endl;
}

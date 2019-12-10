#include "logoutmessagehandler.h"

LogoutMessageHandler::LogoutMessageHandler(World* world) : world(world) { }

void LogoutMessageHandler::handle(NetMessage& message) {
    LogoutMessage msg = dynamic_cast<LogoutMessage&>(message); 
    this->world->players.erase(msg.senderId);
    std::cout << "> player " << message.senderId << " logged out" << std::endl;
}

#include "initmessagehandler.h"

InitMessageHandler::InitMessageHandler(std::string *id, World *world) : id(id), world(world) {}

void InitMessageHandler::handle(const NetMessage &message) {
    InitMessage msg = dynamic_cast<const InitMessage &>(message);
    if (msg.receiverId.empty()) std::cout << "Error no valid user id!" << std::endl;
    *id = msg.receiverId;
    *world = msg.world;
    std::cout << "> logged in as " << msg.receiverId << std::endl;
}
